//=============================================================================
//�y Grid �z
//-----------------------------------------------------------------------------
///**
//  @file       Grid.h
//  @brief      Grid
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Interface.h"
#include "VertexTypes.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
class GeometryRenderer;

//=============================================================================
// �� Grid �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class Grid
    : public Base::ReferenceObject
{
public:
    Grid();

protected:
    virtual ~Grid();

public:

    /// ������
    LNRESULT initialize( IGraphicsDevice* device_ );

    /// �i�q���̐ݒ�
    LNRESULT setLatticeNum( u32 num_ );

    /// �i�q���̎擾
    u32 getLatticeNum() { return mSquareNum; }

    /// �Ԋu�̐ݒ�
    LNRESULT setInterval( lnfloat interval_ );

    /// �Ԋu�̎擾
    lnfloat getInterval() { return mInterval; }
    
    /// �e���ʂ̕\���͈͂̐ݒ�
    LNRESULT setPlaceRange( LNGridPlaceRange x_, LNGridPlaceRange y_, LNGridPlaceRange z_ );

    /// �e���ʂ̕\���͈͂̎擾
    void getPlaceRange( LNGridPlaceRange* x_, LNGridPlaceRange* y_, LNGridPlaceRange* z_ );

    /// �e�v�f�̕\���t���O�̐ݒ� ( LNGridVisible �̑g�ݍ��킹 )
    virtual void setVisibleFlags( u32 flags_ ) { mVisibleFlags = flags_; }

    /// �e�v�f�̕\���t���O�̎擾
    virtual u32 getVisibleFlags() { return mVisibleFlags; }
    
    /// �`�� (���C�����X�g�ŕ`�悷�邾��)
    LNRESULT draw();

private:

    // ���_�o�b�t�@����蒼��
    LNRESULT _updateGridVertexBuffer();

private:

    IGraphicsDevice*    mGraphicsDevice;
    IRenderer*          mRenderer;
    IVertexBuffer*      mGridVertexBuffer;

    u32                 mSquareNum;     ///< �ЂƂ̏ی����̎l�p�`�̐�
    lnfloat             mInterval;

    u32                 mVisibleFlags;     ///< LNGridVisible �̃t���O�̑g�ݍ��킹

    LNGridPlaceRange    mXPlace;
    LNGridPlaceRange    mYPlace;
    LNGridPlaceRange    mZPlace;

    u32                 mXZGridIndex;   ///< ���_�o�b�t�@���� XZ ���ʃO���b�h�̊J�n�C���f�b�N�X
    u32                 mXYGridIndex;   ///< ���_�o�b�t�@���� XY ���ʃO���b�h�̊J�n�C���f�b�N�X
    u32                 mYZGridIndex;   ///< ���_�o�b�t�@���� ZY ���ʃO���b�h�̊J�n�C���f�b�N�X
    u32                 mAxisIndex;     ///< ���_�o�b�t�@���̎��̊J�n�C���f�b�N�X

    bool                mAxisVisible;

};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================