//=============================================================================
//�y IDebugRenderer �z
//-----------------------------------------------------------------------------
///**
//  @file       IDebugRenderer.h
//  @brief      IDebugRenderer
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Geometry/Vector3.h"
#include "../Graphics/Common/Common.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Physics
{

//=============================================================================
// �� IDebugRenderer �N���X
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class IDebugRenderer
    : public Base::Interface
{
public:

    /// ���W�ϊ��s��̐ݒ� (���[���h�s��)
    virtual void setMatrix( const LMatrix& matrix_ ) = 0;

    /// ���̕`��
    virtual LNRESULT drawSphere( const LVector3& pos_, lnfloat radius_, const LColor& color_ ) = 0;

    /// �{�b�N�X�̕`��
    virtual LNRESULT drawBox( const LVector3& bb_min_, const LVector3& bb_max_, const LColor& color_ ) = 0;
    
    /// �J�v�Z���̕`��
    virtual LNRESULT drawCapsule( lnfloat radius_, lnfloat half_height_, const LColor& color_ ) = 0;

protected:

    virtual ~IDebugRenderer() {}
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Physics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================