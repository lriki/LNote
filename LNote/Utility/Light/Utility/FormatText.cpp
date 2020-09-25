//=============================================================================
//【 FormatText 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "FormatText.h"
#include <math.h>

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Utility
{
    


//=============================================================================
// ■ FormatText クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    FormatText::FormatText()
        : mFont             ( NULL )
        , mDefaultFontSize  ( 20 )
        , mTypingSpeed      ( 2 )
        , mIndex            ( 0 )
        , mWaitCount        ( 0 )
        , mX                ( 0 )
        , mY                ( 0 )
        , mCurrentLine      ( 0 )
        , mLineHeight       ( 0 )
        //, mRubyChar     ( NULL )
        , mRubyLength       ( 0 )
        , mRubyX            ( 0 )
        , mRubyFont         ( NULL )
    {
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    FormatText::~FormatText()
    {
        LN_SAFE_RELEASE( mFont );
        LN_SAFE_RELEASE( mRubyFont );
    }


    //---------------------------------------------------------------------
	// ● 文字列を設定する
    //---------------------------------------------------------------------
    void FormatText::setText( const lnChar* text_ )
    {
        mSrcText = text_;
    }

    //---------------------------------------------------------------------
	// ● フォントを設定する
    //---------------------------------------------------------------------
    void FormatText::setFont( Core::Graphics::IFont* font_ )
    {
        LN_SAFE_RELEASE( mFont );
        mFont = font_;
        LN_SAFE_ADDREF( mFont );

        LN_SAFE_RELEASE( mRubyFont );
        if ( mFont )
        {
            mDefaultFontSize = mFont->getSize();
            mFont->deepCopy( &mRubyFont );
            mRubyFont->setSize( mFont->getSize() / 2 );
        }
    }

    //---------------------------------------------------------------------
	// ● タイピングスピードの設定
    //---------------------------------------------------------------------
    void FormatText::setTypingSpeed( int speed_ )
    {
        mTypingSpeed = LMath::max( 0, speed_ );
    }

    //---------------------------------------------------------------------
	// ● 描画先矩形の設定
    //---------------------------------------------------------------------
    void FormatText::setDrawRect( const LRect& rect_ )
    {
        mDrawRect = rect_;
    }

    //---------------------------------------------------------------------
	// ● 設定された情報をもとに描画情報を構築する
    //---------------------------------------------------------------------
    LNRESULT FormatText::build()
    {
        

    
        Core::Base::StringUtil::replaceString( &mSrcText, _T( "\\f" ), _T( "\\001" ) );
        Core::Base::StringUtil::replaceString( &mSrcText, _T( "\\F" ), _T( "\\001" ) );
        Core::Base::StringUtil::replaceString( &mSrcText, _T( "\\r" ), _T( "\\027" ) );
        Core::Base::StringUtil::replaceString( &mSrcText, _T( "\\R" ), _T( "\\027" ) );



        int w, h;
        _getLineSize( mSrcText.c_str(), &w, &h );

        mDrawDataEntryQueue.clear();
        mIndex = 0;
        mWaitCount = 0;
        mX = 0;
        mY = 0;
        mCurrentLine = 0;
        mLineHeight = h;

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● フレーム更新
    //---------------------------------------------------------------------
    LNRESULT FormatText::update()
    {
        if ( mSrcText.size() <= 0 || isFinished() ) { return LN_OK; }

        const lnChar* text = mSrcText.c_str();
        LRect rc;

        while ( true )
        {
            // 待機時間がある場合は減らしてから終了
            if ( mWaitCount > 0 )
            {
                --mWaitCount;
                return LN_OK;
            }

            // 終了
            if ( text[ mIndex ] == _T( '\0' ) )
            {
                break;
            }

            // 改行
            if ( text[ mIndex ] == _T( '\n' ) )
            {
                ++mCurrentLine;
                ++mIndex;
                mX = 0;

                int w, h;
                _getLineSize( &text[ mIndex ], &w, &h );
                mY = mY + mLineHeight;
                mLineHeight = h;
                continue;
            }

            // 制御文字の処理
            int adv_idx;
            int unknown_code;
            bool is_continue = _checkControlChar( &text[ mIndex ], &adv_idx, &unknown_code );
            mIndex += adv_idx;
            if ( is_continue )
            {
                continue;
            }

            // 制御文字の処理後に終端・改行を指しているか、制御文字の場合は continue
            if ( text[ mIndex ] == _T( '\0' ) || text[ mIndex ] == _T( '\n' ) || text[ mIndex ] == CONTROL_CHAR )
            {
                continue;
            }

            
            /// ルビの描画情報の作成
            _makeRubyData();

            



            // マルチバイト文字かどうかでチェックする文字列長さを決める unicode の場合は常に 1
            int len = Core::Base::StringUtil::checkMultiByteChar( &text[ mIndex ] ) ? 2 : 1;

            mFont->getTextSize( &text[ mIndex ], len, &rc );


            DrawDataEntry entry;
            entry.DrawData.Text = &text[ mIndex ];
            entry.DrawData.Length = len;
            entry.DrawData.Rect.set( mX, mY + ( mLineHeight - rc.h ), rc.w, rc.h );
            entry.DrawData.Font = mFont;
            mDrawDataEntryQueue.push_back( entry );

            mX += rc.w;

            mIndex += len;

            mWaitCount = mTypingSpeed;
        }


        //mDrawDataEntryList

        /*
        const lnChar* line_head = mSrcText.c_str();
        int line_width = 0;

        while ( true )
        {
            line_head = _buildLine( line_head, &line_width );
            if ( *line_head == _T( '\0' ) )
            {
                break;
            }
        }
        */

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● 描画情報の取得
    //---------------------------------------------------------------------
    bool FormatText::getNextDrawData( LNFormatTextDrawData* data_ )
    {
        if ( mDrawDataEntryQueue.empty() ) { return false; }

		*data_ = mDrawDataEntryQueue.front().DrawData;
        data_->Rect.x += mDrawRect.x;
        data_->Rect.y += mDrawRect.y;
		mDrawDataEntryQueue.pop_front();

		return true;
    }

    //---------------------------------------------------------------------
	// ● 全ての処理が完了しているかを判定する
    //---------------------------------------------------------------------
    bool FormatText::isFinished() const
    {
        return ( mSrcText.size() == 0 || mSrcText.c_str()[ mIndex ] == _T( '\0' ) );
    }

    //---------------------------------------------------------------------
	// ● 1行の幅と高さを求める
    //---------------------------------------------------------------------
    LNRESULT FormatText::_getLineSize( const lnChar* str_, int* width_, int* height_ )
    {
        *width_  = 0;
        *height_ = 0;

        int org_fontsize = mFont->getSize();
        int i = 0;
        int w = 0;
        int h = org_fontsize;
        int ruby_size = 0;

        int start_normal_char_idx = 0;
        int last_normal_char_idx = 0;

        bool font_changed = false;
        int new_font_size = 0;
        bool new_bold = false;

        while ( true )
        {
            // 改行か終端まで
            if ( str_[ i ] == _T( '\n' ) || str_[ i ] == _T( '\0' ) )
            {
                LRect rc;
                mFont->getTextSize(
                    &str_[ start_normal_char_idx ],
                    ( last_normal_char_idx - start_normal_char_idx ),
                    &rc );
                //printf("1:%d w:%d\n",last_normal_char_idx - start_normal_char_idx, rc.width);
                *width_ += rc.w;
                break;
            }

            // 制御文字列の処理
            if ( str_[ i ] == CONTROL_CHAR )
            {
                ++i;
                int code = _checkCtrlCharNumber( &str_[ i ] );
                if ( code )
                {
                    i += 3;
                }

                switch ( code )
                {
                    /////////// フォントサイズ変更
                    case 1:
                    {
                        if ( _parsControlArgs( &str_[ i ], &mCtrlArgs ) )
                        {
                            i += mCtrlArgs.CommandLength;       // ] の次を指す

                            // フォントサイズが省略されている場合はデフォルトのサイズにする
                            int size;
                            if ( mCtrlArgs.Args[ 0 ].IsOptional )
                            {
                                size = mDefaultFontSize;
                            }
                            else
                            {
                                size = _getIntArg( mCtrlArgs.Args[ 0 ] );
                            }

                            new_font_size = size;
                            h = LMath::max( h, size );
                            font_changed = true;
                        }
                        break;
                    }
                    /////////// ルビ
                    case 27:
                    {
                        if ( _parsControlArgs( &str_[ i ], &mCtrlArgs ) )
                        {
                            i += mCtrlArgs.CommandLength;       // ] の次を指す

                            // ひとつでもあれば行の高さを増やす
                            ruby_size = mFont->getSize() / 2;
                        }
                        break;
                    }
                }
                continue;
            }

            // フォントサイズ変更などで、last_normal_char_idx までの文字列幅を計算する必要がある場合
            if ( font_changed )
            {
                LRect rc;
                mFont->getTextSize(
                    &str_[ start_normal_char_idx ],
                    ( last_normal_char_idx - start_normal_char_idx ),
                    &rc );
                //printf("2:%d w:%d\n",last_normal_char_idx - start_normal_char_idx, rc.width);
                *width_ += rc.w;
                start_normal_char_idx = i;
                

                mFont->setSize( new_font_size );
                font_changed = false;
            }

            // ここまで来たら普通の文字
            
            if ( Core::Base::StringUtil::checkMultiByteChar( &str_[ i ] ) )
            {
                i += 2;
            }
            else
            {
                ++i;
            }

            last_normal_char_idx = i;
        }

        mFont->setSize( org_fontsize );
        *height_ = h + ruby_size;
        return LN_OK;
    }

    

    //---------------------------------------------------------------------
	// ● str_ 以降に続く 3 つの文字が数字かをチェックして、数値化した値を返す (無効な場合は 0)
    //---------------------------------------------------------------------
    int FormatText::_checkCtrlCharNumber( const lnChar* str_ )
    {
        if ( isdigit( *str_ ) && isdigit( *(str_ + 1) ) && isdigit( *(str_ + 2) ) )
        {
            return
                (*str_       - _T( '0' )) * 100 +
                (*(str_ + 1) - _T( '0' )) * 10  +
                (*(str_ + 2) - _T( '0' ));
        }
        return 0;
    }

    //---------------------------------------------------------------------
	// ● 制御文字かどうかをチェックして処理をする
	//---------------------------------------------------------------------
	bool FormatText::_checkControlChar( const lnChar* str_, int* adv_index_, int* unknown_code_ )
    {
        *unknown_code_ = 0;
        *adv_index_ = 0;
        int i = 0;

        // 制御文字列の処理
        if ( str_[ i ] == CONTROL_CHAR )
        {
            ++i;
            int code = _checkCtrlCharNumber( &str_[ i ] );
            if ( code )
            {
                i += 3;

                _parsControlArgs( &str_[ i ], &mCtrlArgs );
                i += mCtrlArgs.CommandLength;

                switch ( code )
                {
                    /////////// フォントサイズ変更
                    case 1:
                    {
                        // フォントサイズが省略されている場合はデフォルトのサイズにする
                        int size;
                        if ( mCtrlArgs.Args[ 0 ].IsOptional )
                        {
                            mFont->setSize( mDefaultFontSize );
                        }
                        else
                        {
                            mFont->setSize( _getIntArg( mCtrlArgs.Args[ 0 ] ) );
                        }
                    
                        break;
                    }
                    /////////// ルビ
                    case 27:
                    {
                        //Base::StringUtil::trimHFSpace( &mRubyText, ln_std_tstring( mCtrlArgs.Args[ 1 ].BeginChar, mCtrlArgs.Args[ 1 ].Length ) );
                        mRubyText   = ln_std_tstring( mCtrlArgs.Args[ 1 ].BeginChar, mCtrlArgs.Args[ 1 ].Length );

                        mRubyLength = mRubyText.size();
                        //mRubySize   = mFont->getSize() / 2;

                        ln_std_tstring tmp( mCtrlArgs.Args[ 0 ].BeginChar, mCtrlArgs.Args[ 0 ].Length );
                         
                        // mIndex は '\\' を指している。
                        //  + 4 は \ と 3桁の数字
                        mSrcText.replace( mIndex, mCtrlArgs.CommandLength + 4, tmp.c_str() );

                        mRubyMainStr    = &mSrcText.c_str()[ mIndex ];
                        mRubyMainStrLen = Core::Base::StringUtil::strlenMB( tmp.c_str() );//mCtrlArgs.Args[ 0 ].Length;


                        // ルビとメイン文字列の幅を取得
                        LRect rc;
                        int ruby_w, main_w;
                        mFont->getTextSize( tmp.c_str(), -1, &rc );
                        main_w = rc.w;
                        mRubyFont->getTextSize( mRubyText.c_str(), -1, &rc );
                        ruby_w = rc.w;

                        // マルチバイトの場合はそれを1文字とした文字数を計算
                        int true_len = Core::Base::StringUtil::strlenMB( mRubyText.c_str() );

                        mRubySize = mRubyFont->getSize();
                        //_p(mRubySize);
                        
                        // ルビの描画を始める X 座標と加算量の計算
                        if ( main_w >= ruby_w || true_len == 1 )
                        {
                            if ( true_len == 1 )
                            {
                                mRubyX = static_cast< float >( main_w ) / ( true_len * 2 ) - mRubySize / 2;
                            }
                            else
                            {
                                mRubyX = static_cast< float >( main_w ) / ( true_len * 2 ) - mRubySize / 3;
                            }
                            
                            
                            mRubyOffsetX = static_cast< float >( main_w ) / true_len;
                            //int ox = mX + ( main_w - ruby_w ) / 2;
                        }
                        else
                        {
                            //mRubyX       = static_cast< float >( ruby_w - main_w - mRubySize ) / 2;
                            //mRubyX       = ( main_w / 2 ) - ( ruby_w / 2 );//static_cast< float >( ruby_w - main_w ) / 2;
                            mRubyX       = static_cast< float >( main_w - ruby_w ) / 2;
                            mRubyOffsetX = static_cast< float >( ruby_w ) / true_len;
                        }
                        
                        mRubyX = mX + mRubyX;

                        i = 0;
                        break;
                    }
                    /////////// 登録されていないコード
                    default:
                    {
                        *unknown_code_ = code;
                        break;
                    }
                }
            }
        }
        *adv_index_ = i;
        return false;
    }

    //---------------------------------------------------------------------
	// ● [] 付きの制御文字列を解析する
    //---------------------------------------------------------------------
    bool FormatText::_parsControlArgs( const lnChar* str_, CtrlArgsConteiner* out_ )
    {
        if ( *str_ != _T( '[' ) )
        {
            out_->CommandLength = 0;
            return false;
        }

        int state = 0;
        int i = 1;
        int arg_idx = 0;
        int begin = 1;

        for ( int i = 0; i < MAX_CTRL_ARGMENTS; ++i )
        {
            out_->Args[ i ].BeginChar = NULL;
            out_->Args[ i ].Length = 0;
            out_->Args[ i ].IsOptional = true;
        }


        while ( true )
        {
            // 改行、終端が見つかった場合はエラー
            if ( str_[ i ] == _T( '\n' ) || str_[ i ] == _T( '\0' ) )
            {
                out_->CommandLength = 0;
                return false;
            }
            // 区切り文字
            else if ( str_[ i ] == _T( ',' ) )
            {
                // 最大数に達していなければ追加する
                if ( arg_idx < MAX_CTRL_ARGMENTS )
                {
                    out_->Args[ arg_idx ].BeginChar  = &str_[ begin ];
                    out_->Args[ arg_idx ].Length     = i - begin;

                    // 前後のスペースをカット
                    _trimArgStrSpace( &out_->Args[ arg_idx ] );

                    // 成形後、文字数が 0 以下の場合、引数は省略されている
                    out_->Args[ arg_idx ].IsOptional = ( out_->Args[ arg_idx ].Length <= 0 );
                    
                    ++arg_idx;
                    ++i;
                    begin = i;
                    out_->ArgsNum = arg_idx;
                }
            }
            // 終了
            if ( str_[ i ] == _T( ']' ) )
            {
                if ( arg_idx < MAX_CTRL_ARGMENTS )
                {
                    out_->Args[ arg_idx ].BeginChar = &str_[ begin ];
                    out_->Args[ arg_idx ].Length    = i - begin;

                    // 前後のスペースをカット
                    _trimArgStrSpace( &out_->Args[ arg_idx ] );

                    // 成形後、文字数が 0 以下の場合、引数は省略されている
                    out_->Args[ arg_idx ].IsOptional = ( out_->Args[ arg_idx ].Length <= 0 );
                }
                out_->CommandLength = i + 1;
                return true;
            }

            ++i;
        }

       
        return true;
    }

    //---------------------------------------------------------------------
	// ● 12 桁までの文字列→数値変換
    //---------------------------------------------------------------------
    int FormatText::_toInt( const lnChar* str_, int len_ )
    {
        lnChar buf[ 13 ] = { 0 };
        _tcsncpy( buf, str_, LMath::max( len_, 12 ) );
        return _tstoi( buf );
    }

    //---------------------------------------------------------------------
	// ● ArgsData から数値を取得
    //---------------------------------------------------------------------
    int FormatText::_getIntArg( const ArgData& arg_ )
    {
        if ( arg_.BeginChar )
        {
            return _toInt( arg_.BeginChar, arg_.Length );
        }
        return 0;
    }


    //---------------------------------------------------------------------
	// ● ルビの描画情報の作成 (update() から呼ばれる)
    //---------------------------------------------------------------------
    void FormatText::_makeRubyData()
    {
        if ( mRubyLength > 0 )
        {
            // フォントサイズのバックアップを取って、ルビ用のサイズをセット
            //int org_fontsize = mFont->getSize();
            //mRubyFont->setSize( mRubySize );

            int ruby_text_true_len = Core::Base::StringUtil::strlenMB( mRubyText.c_str() );

            // ルビの文字数 / メイン文字列の文字数
            float num =
                static_cast< float >( ruby_text_true_len ) / mRubyMainStrLen;
            
            // 一回の描画でどれだけルビを振るか計算
            float c = ceil( num );
            int cnt = c - floor( (c - num) + 0.5f );

            
            LRect rc;
           

            // 回数分だけルビを描画
            for ( int i = 0; i < cnt; ++i )
            {
                //if ( mRubyLength > 0 )  // <= で i を回してるので、1回多くなることもあるため、その保険
                {

                    int idx = mRubyText.size() - mRubyLength;
                    const lnChar* str = mRubyText.c_str();
                    int len = Core::Base::StringUtil::checkMultiByteChar( &str[ idx ] ) ? 2 : 1;
                    mRubyLength -= len;


                    mRubyFont->getTextSize( &str[ idx ], len, &rc );

                    DrawDataEntry entry;
                    entry.DrawData.Text = &str[ idx ];
                    entry.DrawData.Length = len;
                    entry.DrawData.Rect.set( mRubyX, mY + ( mLineHeight - mFont->getSize() - mRubySize ), rc.w, rc.h );
                    entry.DrawData.Font = mRubyFont;
                    mDrawDataEntryQueue.push_back( entry );

                    mRubyX += mRubyOffsetX;
                }

            }
        }
    }

    //---------------------------------------------------------------------
	// ● 引数文字列の前後の空白を取り除く (_parsControlArgs() から呼ばれる)
    //---------------------------------------------------------------------
    void FormatText::_trimArgStrSpace( ArgData* arg_data_ )
    {
        int b = 0;
        int e = arg_data_->Length;
        const lnChar* c = arg_data_->BeginChar;
        for ( ; c[ b ] != _T( '\0' ) && c[ b ] == _T( ' ' ); ++b ) {}

        // e は ']' ',' を指していることもあるので、それの対応
        /*
        for ( ;
            e >= b &&
            ( c[ e ] == _T( ',' ) || 
              c[ e ] == _T( ']' ) || 
              c[ e ] == _T( ' ' ) ) ; --e ) {}
              */
        // マルチバイト文字列の対応
        while ( true )
        {
            if ( e > b )
            {
                // 「今」なんかは2バイト目が ' ' と同じなので、ちゃんとチェックしておく必要がある
                if ( Core::Base::StringUtil::checkMultiByteChar( &c[ e - 2 ] ) )
                {
                    break;
                }
            }
            if ( e >= b && ( c[ e ] == _T( ',' ) || c[ e ] == _T( ']' ) || c[ e ] == _T( ' ' ) ) )
            {
                --e;
            }
            else
            {
                break;
            }
        }

        arg_data_->BeginChar = &c[ b ];
        arg_data_->Length    = e - b;
    }









//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Utility
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================