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
#include "SceneNode.h"
#include "../../Graphics/Common/PointParticleParameter.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{

//=============================================================================
// �� PointParticle
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class PointParticle
    : public SceneNode
{
public:

    PointParticle( SceneGraph* scene_ );

    virtual ~PointParticle();

public:

    /// ������
    void initialize( const Graphics::PointParticleParameter& param_ );

    /// �e�N�X�`���̐ݒ�
    void setTexture( Graphics::ITexture* texture_ );

    /// �e�N�X�`���̎擾
    //Graphics::ITexture* getTexture();

public:

    /// �m�[�h�̎�ނ̎擾
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_PARTICLE; }

    /// �t���[���X�V
    virtual void update( const Game::GameTime& time_ );

    /// �T�u�Z�b�g��`�悷��
    virtual LNRESULT drawSubset( lnU32 index_ );

private:
    
    Graphics::PointParticle*    mPointParticle;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core

} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================