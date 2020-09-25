//=============================================================================
//【 FixedLengthScalar 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <cmath>
#include <algorithm>
#include "Scalar.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Animation
{

//=============================================================================
// ■ Scalar クラス
//=============================================================================

    Scalar::Key::Key()
        : FramePos      ( 0.0f )
        , Value         ( 0.0f )
        , Velocity      ( 0.0f )
        , Accel         ( 0.0f )
        , LeftSlope     ( 0.0f )
        , RightSlope    ( 0.0f )
        , InterMode     ( LN_ANIM_INTER_LINEAR )
    {
    }

    /// キーフレームソート用比較関数
    static bool scalarCompareFunc( const Scalar::Key& l_, const Scalar::Key& r_ )
    {
	    return l_.FramePos < r_.FramePos;
    }

    /// あるフレーム位置直前のキーフレームを検索するための関数オブジェクト
    class scalarGreaterEqual
    {
        double frame;
    public:
        scalarGreaterEqual( double frame_ ) : frame( frame_ ){}
        bool operator()( const Scalar::Key& key_ ) const
        {
            return key_.FramePos <= frame;
        }
    };

    //---------------------------------------------------------------------
    // ● デフォルトコンストラクタ
    //---------------------------------------------------------------------
    Scalar::Scalar()
        : mTime             ( 0.0 )
        , mTimeTick         ( 60.0f )
        , mCurrentFramePos  ( 0.0f )
        , mLoopBeginPos     ( 0.0f )
        , mLoopEndPos       ( 0.0f )
        , mDefaultValue     ( 0.0f )
        , mValue            ( 0.0f )
        , mEnableLoop       ( false )
    {
    }

 
    //---------------------------------------------------------------------
    // ● キーの追加
    //---------------------------------------------------------------------
    void Scalar::addKeyFrame( double frame_pos_, lnfloat value_ )
    {
        Key key;
        key.FramePos = frame_pos_;
        key.Value    = value_;
        mKeyFrameList.push_back( key );
    }

    //---------------------------------------------------------------------
    // ● キーの追加
    //---------------------------------------------------------------------
    void Scalar::addKeyFrame( const Key& key_ )
    {
        mKeyFrameList.push_back( key_ );
    }

    //---------------------------------------------------------------------
    // ● キーフレームのソート
    //---------------------------------------------------------------------
    void Scalar::sortKeyFrame()
    {
        std::sort( mKeyFrameList.begin(), mKeyFrameList.end(), scalarCompareFunc );
    }

    //---------------------------------------------------------------------
    // ● ループ再生の有効設定
    //---------------------------------------------------------------------
    void Scalar::setEnableLoop( bool flag_ )
    {
        mEnableLoop = flag_;
    }

    //---------------------------------------------------------------------
    // ● ループ時間の設定
    //---------------------------------------------------------------------
    void Scalar::setLoopFrame( lnfloat begin_frame_, lnfloat end_frame_ )
    {
        mLoopBeginPos = begin_frame_;
        mLoopEndPos   = end_frame_;
    }

    

    //---------------------------------------------------------------------
    // ● 時間の設定
    //---------------------------------------------------------------------
    void Scalar::setTime( double time_ )
    {
        mTime = time_;

        time_ *= mTimeTick;

        
        if ( mEnableLoop )
        {
            // ループ位置未設定
            if ( mLoopEndPos == 0.0f && mLoopBeginPos == 0.0f )
            {
                // 終端のキーフレームの時間を使ってループ
                if ( mKeyFrameList.back().FramePos < time_ )
                {
                    time_ = fmod( time_, static_cast< double >( mKeyFrameList.back().FramePos ) );
                }
            }
            // ループ位置設定済み
            else 
            {
                // time_ がループ領域を超えていた場合は開始時間からループ
                if ( mLoopEndPos < time_ )
                {
                    time_ -= mLoopBeginPos;
                    time_ = fmod( time_, ( mLoopEndPos - mLoopBeginPos ) );
                    time_ = mLoopBeginPos + time_;
                }
            }
        }

        mCurrentFramePos = time_;

        if ( getKeyNum() )
        {
            // time_ が最初のフレーム位置より前の場合はデフォルト値
            if ( time_ < mKeyFrameList.front().FramePos )
            {
                mValue = mDefaultValue;
            }
            // キーがひとつだけの場合はそのキーの値
            else if ( getKeyNum() == 1 )
            {
                mValue = mKeyFrameList.front().Value;
            }
            // time_ が終端以降の場合は終端の値
            else if ( time_ >= mKeyFrameList.back().FramePos )
            {
                mValue = mKeyFrameList.back().Value;
            }
            // 以上以外の場合は補間する
            else
            {
                // time_ の型と Key::FramePos の型は同じのにしておかないと正常に検索できない
                //key = (Key*)bsearch( &time_, mKeyFrames, mSize, sizeof( Key ), _cmpKey );

                // キー検索
                /*
                KeyFrameList::iterator itr = std::find_if( mKeyFrameList.begin(), mKeyFrameList.end(), scalarGreaterEqual( mCurrentFramePos ) );
                const Key& key0 = *itr;
                const Key& key1 = *(itr + 1);
                */

                const Key* key0 = (Key*)bsearch( &mCurrentFramePos, &(mKeyFrameList[0]), mKeyFrameList.size(), sizeof( Key ), _cmpKey );
                const Key* key1 = key0 + 1;

                lnfloat p0 = key0->Value;
		        lnfloat p1 = key1->Value;
                lnfloat t0 = static_cast< lnfloat >( key0->FramePos );
		        lnfloat t1 = static_cast< lnfloat >( key1->FramePos );
                lnfloat t  = static_cast< lnfloat >( mCurrentFramePos - t0 ) / ( t1 - t0 );

                switch ( key0->InterMode )
		        {
			        // 補間無し
			        case LN_ANIM_INTER_NONE:
                    {
				        mValue = p0;
                        break;
                    }
			        // 線形
			        case LN_ANIM_INTER_LINEAR:
                    {
				        mValue = p0 + ( p1 - p0 ) * t;
                        break;
                    }
			        // 等加速度
			        case LN_ANIM_INTER_ACCEL:
			        {
                        mValue = LMath::accel( p0, key0->Velocity, key0->Accel, static_cast< lnfloat >( mCurrentFramePos - key0->FramePos )  );
                        break;
			        }
			        // 三次補間
			        case LN_ANIM_INTER_CUBIC:
			        {
				        mValue = LMath::cubic(
					        p0, p1, 
                            key0->RightSlope,
                            key1->LeftSlope,
					        t );
                        break;
			        }
			        // Catmull-Rom
			        case LN_ANIM_INTER_CATMULLROM:
			        {
                        // ループ再生で time が終端を超えている場合、
                        // この時点でkey の値は ループ開始位置のひとつ前のキーを指している

                        const Key& begin = mKeyFrameList.front();
                        const Key& end   = mKeyFrameList.back();
                        
				        // この補間には、begin のひとつ前と end のひとつ後の値が必要。
				        // それぞれが始点、終点の場合はループするように補間する
				        mValue = LMath::catmullRom(
                            ( ( key0->FramePos == begin.FramePos ) ? end.Value : (key0 - 1)->Value ),
					        p0,
					        p1,
					        ( ( key1->FramePos == end.FramePos ) ? begin.Value : (key0 + 2)->Value ),
					        t );
                        break;
			        }
		        }

#if 0
                lnfloat p0 = key0.Value;
		        lnfloat p1 = key1.Value;
                lnfloat t0 = static_cast< lnfloat >( key0.FramePos );
		        lnfloat t1 = static_cast< lnfloat >( key1.FramePos );
                lnfloat t  = static_cast< lnfloat >( time_ - t0 ) / ( t1 - t0 );

                switch ( key0.InterMode )
		        {
			        // 補間無し
			        case LN_ANIM_INTER_NONE:
                    {
				        mValue = p0;
                        break;
                    }
			        // 線形
			        case LN_ANIM_INTER_LINEAR:
                    {
				        mValue = p0 + ( p1 - p0 ) * t;
                        lnprintf("a %f %f %f %f\n", mValue, p0, p1, t);
                        break;
                    }
			        // 等加速度
			        case LN_ANIM_INTER_ACCEL:
			        {
                        mValue = LMath::accel( p0, key0.Velocity, key0.Accel, static_cast< lnfloat >( mCurrentFramePos - key0.FramePos )  );
                        break;
			        }
			        // 三次補間
			        case LN_ANIM_INTER_CUBIC:
			        {
				        mValue = LMath::cubic(
					        p0, p1, 
                            key0.RightSlope,
                            key1.LeftSlope,
					        t );
                        break;
			        }
			        // Catmull-Rom
			        case LN_ANIM_INTER_CATMULLROM:
			        {
                        // ループ再生で time が終端を超えている場合、
                        // この時点でkey の値は ループ開始位置のひとつ前のキーを指している

                        const Key& begin = mKeyFrameList.front();
                        const Key& end   = mKeyFrameList.back();
                        
				        // この補間には、begin のひとつ前と end のひとつ後の値が必要。
				        // それぞれが始点、終点の場合はループするように補間する
				        mValue = LMath::catmullRom(
                            ( ( key0.FramePos == begin.FramePos ) ? end.Value : (itr - 1)->Value ),
					        p0,
					        p1,
					        ( ( key1.FramePos == end.FramePos ) ? begin.Value : (itr + 2)->Value ),
					        t );
                        break;
			        }
		        }
#endif
            }

        }
        else
        {
            mValue = mDefaultValue;
        }
    }

    //---------------------------------------------------------------------
    // ● 時間を進める
    //---------------------------------------------------------------------
    void Scalar::advanceTime( lnfloat delta_time_ )
    {
        setTime( mTime + static_cast< double >( delta_time_ ) );
    }

    //---------------------------------------------------------------------
    // ● 再生が終了しているかを判定する (ループ再生時は常に false)
    //---------------------------------------------------------------------
    bool Scalar::isFinished() const
    {
        if ( mEnableLoop ) return false;

        if ( getKeyNum() == 0 ) { 
        	return true; }
        
        if ( mCurrentFramePos < mKeyFrameList.back().FramePos ) { 
        	return false; }

        return true;
    }

    //---------------------------------------------------------------------
    // ● キーを全て削除する
    //---------------------------------------------------------------------
    void Scalar::clear()
    {
        mKeyFrameList.clear();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
     Scalar::Key* Scalar::getKey( int idx_ )
    {
        if ( idx_ >= getKeyNum() )
        {
            // 警告とか
            //printf( __FUNCTION__" index error[%d]\n", idx_ );
            *reinterpret_cast< int* >( 0 ) = 0;
        }
        return &mKeyFrameList[ idx_ ];
    }

    //---------------------------------------------------------------------
    // ● キーにアクセスする
    //---------------------------------------------------------------------
    Scalar::Key& Scalar::operator [] ( int idx_ )
    {
        if ( idx_ >= getKeyNum() )
        {
            // 警告とか
            //printf( __FUNCTION__" index error[%d]\n", idx_ );
            *reinterpret_cast< int* >( 0 ) = 0;
        }
        return mKeyFrameList[ idx_ ];
    }

    //---------------------------------------------------------------------
    // ● 比較関数
    //---------------------------------------------------------------------
    int Scalar::_cmpKey( const void* a_, const void* b_ )
    {
        if ( ((Key*)a_)->FramePos < ((Key*)b_)->FramePos )
            return -1;
        else if ( ((Key*)a_)->FramePos >= ((Key*)b_)->FramePos && ((Key*)a_)->FramePos < (((Key*)b_+1))->FramePos )
            return 0;
        else
            return 1;
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Animation
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================