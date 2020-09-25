//=============================================================================
//�y LGraphics �z
//-----------------------------------------------------------------------------
///**
//  @file       LGraphics.h
//  @brief      LGraphics
//  @version    1.0
//  @date       2010/11/10
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Core/Scene/Manager.h"
#include "../Base/LRefClass.h"
#include "../Math/LMath.h"
#include "Types.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

//=============================================================================
// �� LGraphics �N���X
//-----------------------------------------------------------------------------
///**
//	@brief      �O���t�B�b�N�֌W�̊Ǘ��N���X
//*/
//=============================================================================
class LGraphics
    : public LSingletonRefClass< LGraphics, Core::Graphics::Manager >
{
public:

    /// �C���X�^���X�̎擾
    static LGraphics getInstance();

public:

    //---------------------------------------------------------------------
	///**
    //  @brief      �Q�[���X�N���[���̃T�C�Y���擾����
    //
    //  @par
    //              �E�B���h�E���[�h�A�t���X�N���[�����[�h�ɂ�����炸�A
    //              ���݂̃X�N���[���T�C�Y���s�N�Z���P�ʂŕԂ��܂��B<br>
    //              (�t���X�N���[�����[�h�ŉ�ʂ��g�傳��Ă���ꍇ�ł�
    //               ��ɃE�B���h�E���[�h�̂Ƃ��Ɠ����T�C�Y��Ԃ��܂�)
    //*/
    //---------------------------------------------------------------------
    LVector2 getScreenSize() const;

    //---------------------------------------------------------------------
	///**
    //  @brief      �w�i�F��ݒ肷��
    //
    //  @param[in]  color_ : �w�i�F�Ƃ��Đݒ肷��F
    //
    //  @par
    //              �A���t�@�l�͖�������܂��B
    //*/
    //---------------------------------------------------------------------
	void setBGColor( const LColor& color_ );

    //---------------------------------------------------------------------
	///**
    //  @brief      �w�i�F��ݒ肷��
    //
    //  @param[in]  r_ : R (��) ���� (0.0 �` 1.0)
    //  @param[in]  g_ : G (��) ���� (0.0 �` 1.0)
    //  @param[in]  b_ : B (��) ���� (0.0 �` 1.0)
    //*/
    //---------------------------------------------------------------------
	void setBGColor( lnfloat r_, lnfloat g_,lnfloat b_ );



};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================