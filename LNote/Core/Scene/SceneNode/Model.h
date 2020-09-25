//=============================================================================
//�y Model �z
//-----------------------------------------------------------------------------
///**
//  @file       Model.h
//  @brief      Model
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Graphics/Common/Model/Model.h"
#include "SceneNode.h"

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
// �� Model �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class Model
    : public SceneNode
{
public:

    /// �R���X�g���N�^
    Model( SceneGraph* scene_ );

    /// �f�X�g���N�^
    virtual ~Model();

public:

    /// ������ (�~���t�H���_�ɓ�����.fx�t�@�C��������ꍇ�͂����ǂݍ���Őݒ肷��)
	//		flags_ �� LNModelFlag �̑g�ݍ��킹
    LNRESULT initialize( const lnChar* filename_, lnU32 flags_ = 0 );

    Graphics::Model* getModel() { return mModel; }

public:

    /// �m�[�h�̎�ނ̎擾
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_MODEL; }

    /// �t���[���X�V
    virtual void update( const Game::GameTime& time_ );

    /// �T�u�Z�b�g��`�悷��
    virtual LNRESULT drawSubset( lnU32 index_ );

private:

    Graphics::Model*    mModel;
    Graphics::ITexture* mSkinningBoneTexture;
    
    class DrawingContext;
    friend class DrawingContext;
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