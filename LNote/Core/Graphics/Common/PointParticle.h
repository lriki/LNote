//=============================================================================
//�y PointParticle �z
//-----------------------------------------------------------------------------
///**
//  @file       PointParticle.h
//  @brief      PointParticle
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Interface.h"
#include "PointParticleParameter.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

//=============================================================================
// �� PointParticle �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�|�C���g�X�v���C�g�ɂ��p�[�e�B�N���̃N���X
//
//  @note
//              �������ɕ��ׂ�������������\��������B���̕��`��͍����B
//              �t�B�[���h�w�i�̊X���̉��ȂǁA�t�B�[���h�������ɂ܂Ƃ߂č��A
//              ������x�ÓI�ȃG�t�F�N�g�p�ɂȂ邩���B
//*/
//=============================================================================
class PointParticle
    : public Base::ReferenceObject
{
public:

    /// �R���X�g���N�^
    PointParticle();

    /// �f�X�g���N�^
    ~PointParticle();

public:

    /// ������
    void initialize( IGraphicsDevice* device_, const PointParticleParameter& data_ );

    /// ���Ԃ̃X�P�[���W���̐ݒ� (�ʏ�̓t���[�����[�g�̒l�����̂܂ܓn���Ă����B�f�t�H���g�l�� 60�B�X�V���A���Ԃ̒l�ɏ�Z�����)
    void setTimeTickPerSec( lnfloat tick_ ) { mTimeTick = tick_; }

    /// �e�N�X�`���̐ݒ�
    void setTexture( ITexture* texture_ );

    /// ���I�Ȓ��_�o�b�t�@�Ƃ��č쐬����Ă��邩�𔻒肷��
    bool isDynamic() { return mIsDynamic; }

    /// ���Ԃ�i�߂�
    void advanceTime( lnfloat delta_time_ );

    /// �Đ����Ԃ̐ݒ� (���Ԃ𒼐ڎw�肷��B�c�[���ȂǂŎg��)
    void setTime( lnfloat time_ );

    /// ���݂̎��Ԃ��擾����
    lnfloat getTime();

    /// ���_���X�V���� (advanceTime()�AsetTime() �̌�ɂ�����Ă�)
    LNRESULT update( const LMatrix& world_matrix_ );

    /// �`�� (GeometryRenderer::drawPointParticle() ������Ă΂��)
    LNRESULT draw();

private:

    /// ���_�o�b�t�@�𖄂߂�
    void _createVertexData( const PointParticleParameter& data_ );

private:

    IRenderer*      mRenderer;
    IVertexBuffer*  mVertexBuffer;		///< ���_�o�b�t�@
    LVector3*       mOriginalPositions;
    LVector3*       mCreatedPositions;

    lnU32           mParticleNum;       ///< ���̂̃p�[�e�B�N���� (�c���ȊO)
    lnU32           mAfterImageLevel;

    lnU32           mOneLoopFrameNum;

    lnfloat         mTimeTick;
    lnfloat         mTime;
    lnfloat         mLastTime;

    ITexture*       mTexture;
    LVector4        mGlobalAccel;
    lnfloat         mGravityPower;
    bool            mIsSequential;
    bool            mIsParticleLoop;
    bool            mIsDynamic;

    friend class GeometryRenderer;
};


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================