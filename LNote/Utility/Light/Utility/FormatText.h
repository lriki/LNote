//=============================================================================
//【 FormatText 】
//-----------------------------------------------------------------------------
///**
//  @file       FormatText.h
//  @brief      FormatText
//  @version    1.0
//  @date       2012/2/1
//*
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <queue>
#include "../../../Core/Graphics/Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

/// FormatText クラスから出力される文字描画データ
struct LNFormatTextDrawData
{
	const lnChar*			Text;   ///< 描画する文字列 ('\0'終端であることは保障されない)
    int						Length; ///< 描画する文字数 (マルチバイト文字の分も考慮済み。"あ"の場合は2となる)
	LRect					Rect;   ///< 文字を描画する領域
    Core::Graphics::IFont*	Font;   ///< 描画に使うフォント

	//LString control_code;
	//LString control_param;
};

namespace Utility
{

/*
    \R[本文,ルビ] : 本分の上にルビを挿入
    \F[数値]      : 文字の大きさ変更
*/

//=============================================================================
// ■ FormatText クラス
//-----------------------------------------------------------------------------
///**
//  @brief		メッセージ制御文字列
//
//  @note
//              制御文字の引数[]内では改行できない
//*/
//=============================================================================
class FormatText
{
public:

    /// コンストラクタ
    FormatText();

    /// デストラクタ
	virtual ~FormatText();

public:

    //---------------------------------------------------------------------
	///**
    //  @brief      文字列を設定する
    //*/
    //---------------------------------------------------------------------
    void setText( const lnChar* text_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      フォントを設定する
    //*/
    //---------------------------------------------------------------------
    void setFont( Core::Graphics::IFont* font_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      タイピングスピードの設定
    //*/
    //---------------------------------------------------------------------
    void setTypingSpeed( int speed_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      描画先矩形の設定
    //*/
    //---------------------------------------------------------------------
    void setDrawRect( const LRect& rect_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      設定されているデータをもとに描画情報を構築する
    //*/
    //---------------------------------------------------------------------
    LNRESULT build();

    //---------------------------------------------------------------------
	///**
    //  @brief      フレーム更新
    //*/
    //---------------------------------------------------------------------
    LNRESULT update();

    //---------------------------------------------------------------------
	///**
    //  @brief      描画情報の取得
    //*/
    //---------------------------------------------------------------------
    bool getNextDrawData( LNFormatTextDrawData* data_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      全ての処理が完了しているかを判定する
    //*/
    //---------------------------------------------------------------------
    bool isFinished() const;

private:

     static const int MAX_CTRL_ARGMENTS = 4;

    struct ArgData
    {
        const lnChar*    BeginChar;          ///< 引数の先頭文字を指す
        int             Length;             ///< 文字列の長さ
        bool            IsOptional;         ///< 引数が省略されている場合 true
    };

    struct CtrlArgsConteiner
    {
        int             CommandLength;      ///< '[' ']' を含む制御文字引数の長さ ("[32]" の場合は 4)
        int             ArgsNum;
        ArgData         Args[ MAX_CTRL_ARGMENTS ];
    };

private:

    /// 1行の幅と高さを求める
    LNRESULT _getLineSize( const lnChar* str_, int* width_, int* height_ );
 
    

    /// str_ 以降に続く 3 つの文字が数字かをチェックして、数値化した値を返す (無効な場合は 0)
    int _checkCtrlCharNumber( const lnChar* str_ );

    /// 制御文字かどうかをチェックして処理をする (進めたインデックス数が返る。0の場合はなにもしていない)
	bool _checkControlChar( const lnChar* str_, int* adv_index_, int* unknown_code_ );

    /// [] 付きの制御文字列を解析する (str_ は制御文字の次(普通は'[')を指していること。'[' 出ない場合は false が返る。[]の引数は最大 4 つまで)
    bool _parsControlArgs( const lnChar* str_, CtrlArgsConteiner* out_ );

    /// 12 桁までの文字列→数値変換
    int _toInt( const lnChar* str_, int len_ );

    /// ArgsData から数値を取得
    int _getIntArg( const ArgData& arg_ );

    /// ルビの描画情報の作成 (update() から呼ばれる)
    void _makeRubyData();

    /// 引数文字列の前後の空白を取り除く (_parsControlArgs() から呼ばれる)
    void _trimArgStrSpace( ArgData* arg_data_ );

    

    //virtual inline volatile const unsigned long long int const* operator()() const { return 0; }

private:

   

    struct DrawDataEntry
    {
        LNFormatTextDrawData    DrawData;
        //int                     Duration;   ///< 次の出力までのフレーム数 (0 の場合は同じフレーム中に続けて出力)
    };

    typedef std::deque< DrawDataEntry >   DrawDataEntryQueue;

private:

    static const lnChar CONTROL_CHAR = _T( '\\' );

    //Core::Base::RefTString  mSrcText;
    ln_std_tstring          mSrcText;
    Core::Graphics::IFont*  mFont;
    int                     mDefaultFontSize;
    int                     mTypingSpeed;
    LRect                   mDrawRect;

    int                     mIndex;
    int                     mWaitCount;
    int                     mX;             ///< 次に作る描画データの X 座標 (座標は全てローカル)
    int                     mY;             ///< 行の上側の Y 座標
    int                     mCurrentLine;   ///< 処理中の行番号
    int                     mLineHeight;    ///< 処理中の行の高さ

    ln_std_tstring          mRubyText;      ///< 描画するべきルビ文字
    int                     mRubyLength;    ///< 残りの描画するべきルビ文字数 (マルチバイト文字の場合は1つで 2)
    int                     mRubySize;
    const lnChar*            mRubyMainStr;       ///< ルビをふる文字列
    int                     mRubyMainStrLen;    ///< mRubyMainStr の文字数
    float                   mRubyX;             ///< ルビを描きだす X 座標
    float                   mRubyOffsetX;       ///< ルビの X 座標加算量
    Core::Graphics::IFont*  mRubyFont;


    DrawDataEntryQueue      mDrawDataEntryQueue;
    CtrlArgsConteiner       mCtrlArgs;

};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Utility
} // namespace LNote

/*
    _buildLine() は最初 \n の次の文字を指すポインタを返してたけど、
    そうすると "\n\n\n" とかみたいな改行や終端が連続してるときに
    想定通りに出力されなくなると思ったので変更。


    描画する全体の領域によってウィンドウサイズを変更する場合等に備えて、
    全体の矩形を示す getMaxDrawArea() みたいな関数が必要。

    右揃えなんかをする場合はあらかじめその行の幅をチェックしておく必要がある。
*/

//=============================================================================
//								end of file
//=============================================================================