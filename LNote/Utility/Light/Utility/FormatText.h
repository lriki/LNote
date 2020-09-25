//=============================================================================
//�y FormatText �z
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

/// FormatText �N���X����o�͂���镶���`��f�[�^
struct LNFormatTextDrawData
{
	const lnChar*			Text;   ///< �`�悷�镶���� ('\0'�I�[�ł��邱�Ƃ͕ۏႳ��Ȃ�)
    int						Length; ///< �`�悷�镶���� (�}���`�o�C�g�����̕����l���ς݁B"��"�̏ꍇ��2�ƂȂ�)
	LRect					Rect;   ///< ������`�悷��̈�
    Core::Graphics::IFont*	Font;   ///< �`��Ɏg���t�H���g

	//LString control_code;
	//LString control_param;
};

namespace Utility
{

/*
    \R[�{��,���r] : �{���̏�Ƀ��r��}��
    \F[���l]      : �����̑傫���ύX
*/

//=============================================================================
// �� FormatText �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		���b�Z�[�W���䕶����
//
//  @note
//              ���䕶���̈���[]���ł͉��s�ł��Ȃ�
//*/
//=============================================================================
class FormatText
{
public:

    /// �R���X�g���N�^
    FormatText();

    /// �f�X�g���N�^
	virtual ~FormatText();

public:

    //---------------------------------------------------------------------
	///**
    //  @brief      �������ݒ肷��
    //*/
    //---------------------------------------------------------------------
    void setText( const lnChar* text_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      �t�H���g��ݒ肷��
    //*/
    //---------------------------------------------------------------------
    void setFont( Core::Graphics::IFont* font_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      �^�C�s���O�X�s�[�h�̐ݒ�
    //*/
    //---------------------------------------------------------------------
    void setTypingSpeed( int speed_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      �`����`�̐ݒ�
    //*/
    //---------------------------------------------------------------------
    void setDrawRect( const LRect& rect_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      �ݒ肳��Ă���f�[�^�����Ƃɕ`������\�z����
    //*/
    //---------------------------------------------------------------------
    LNRESULT build();

    //---------------------------------------------------------------------
	///**
    //  @brief      �t���[���X�V
    //*/
    //---------------------------------------------------------------------
    LNRESULT update();

    //---------------------------------------------------------------------
	///**
    //  @brief      �`����̎擾
    //*/
    //---------------------------------------------------------------------
    bool getNextDrawData( LNFormatTextDrawData* data_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      �S�Ă̏������������Ă��邩�𔻒肷��
    //*/
    //---------------------------------------------------------------------
    bool isFinished() const;

private:

     static const int MAX_CTRL_ARGMENTS = 4;

    struct ArgData
    {
        const lnChar*    BeginChar;          ///< �����̐擪�������w��
        int             Length;             ///< ������̒���
        bool            IsOptional;         ///< �������ȗ�����Ă���ꍇ true
    };

    struct CtrlArgsConteiner
    {
        int             CommandLength;      ///< '[' ']' ���܂ސ��䕶�������̒��� ("[32]" �̏ꍇ�� 4)
        int             ArgsNum;
        ArgData         Args[ MAX_CTRL_ARGMENTS ];
    };

private:

    /// 1�s�̕��ƍ��������߂�
    LNRESULT _getLineSize( const lnChar* str_, int* width_, int* height_ );
 
    

    /// str_ �ȍ~�ɑ��� 3 �̕��������������`�F�b�N���āA���l�������l��Ԃ� (�����ȏꍇ�� 0)
    int _checkCtrlCharNumber( const lnChar* str_ );

    /// ���䕶�����ǂ������`�F�b�N���ď��������� (�i�߂��C���f�b�N�X�����Ԃ�B0�̏ꍇ�͂Ȃɂ����Ă��Ȃ�)
	bool _checkControlChar( const lnChar* str_, int* adv_index_, int* unknown_code_ );

    /// [] �t���̐��䕶�������͂��� (str_ �͐��䕶���̎�(���ʂ�'[')���w���Ă��邱�ƁB'[' �o�Ȃ��ꍇ�� false ���Ԃ�B[]�̈����͍ő� 4 �܂�)
    bool _parsControlArgs( const lnChar* str_, CtrlArgsConteiner* out_ );

    /// 12 ���܂ł̕����񁨐��l�ϊ�
    int _toInt( const lnChar* str_, int len_ );

    /// ArgsData ���琔�l���擾
    int _getIntArg( const ArgData& arg_ );

    /// ���r�̕`����̍쐬 (update() ����Ă΂��)
    void _makeRubyData();

    /// ����������̑O��̋󔒂���菜�� (_parsControlArgs() ����Ă΂��)
    void _trimArgStrSpace( ArgData* arg_data_ );

    

    //virtual inline volatile const unsigned long long int const* operator()() const { return 0; }

private:

   

    struct DrawDataEntry
    {
        LNFormatTextDrawData    DrawData;
        //int                     Duration;   ///< ���̏o�͂܂ł̃t���[���� (0 �̏ꍇ�͓����t���[�����ɑ����ďo��)
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
    int                     mX;             ///< ���ɍ��`��f�[�^�� X ���W (���W�͑S�ă��[�J��)
    int                     mY;             ///< �s�̏㑤�� Y ���W
    int                     mCurrentLine;   ///< �������̍s�ԍ�
    int                     mLineHeight;    ///< �������̍s�̍���

    ln_std_tstring          mRubyText;      ///< �`�悷��ׂ����r����
    int                     mRubyLength;    ///< �c��̕`�悷��ׂ����r������ (�}���`�o�C�g�����̏ꍇ��1�� 2)
    int                     mRubySize;
    const lnChar*            mRubyMainStr;       ///< ���r���ӂ镶����
    int                     mRubyMainStrLen;    ///< mRubyMainStr �̕�����
    float                   mRubyX;             ///< ���r��`������ X ���W
    float                   mRubyOffsetX;       ///< ���r�� X ���W���Z��
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
    _buildLine() �͍ŏ� \n �̎��̕������w���|�C���^��Ԃ��Ă����ǁA
    ��������� "\n\n\n" �Ƃ��݂����ȉ��s��I�[���A�����Ă�Ƃ���
    �z��ʂ�ɏo�͂���Ȃ��Ȃ�Ǝv�����̂ŕύX�B


    �`�悷��S�̗̂̈�ɂ���ăE�B���h�E�T�C�Y��ύX����ꍇ���ɔ����āA
    �S�̂̋�`������ getMaxDrawArea() �݂����Ȋ֐����K�v�B

    �E�����Ȃ񂩂�����ꍇ�͂��炩���߂��̍s�̕����`�F�b�N���Ă����K�v������B
*/

//=============================================================================
//								end of file
//=============================================================================