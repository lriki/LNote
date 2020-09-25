//=============================================================================
//�y Sprite �z
//-----------------------------------------------------------------------------
///**
//  @file       Sprite.h
//  @brief      Sprite
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
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
// �� Sprite �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�l�p�`�I�u�W�F�N�g
//*/
//=============================================================================
class Sprite
    : public SceneNode
    //, public ISprite3D
{
public:

    /// �R���X�g���N�^
    Sprite( SceneGraph* scene_ );

    /// �f�X�g���N�^
    virtual ~Sprite();

public:

    /// ������
    LNRESULT initialize();

    ///// �傫���̐ݒ�
    //void setSize( const LVector2& size_ );

    ///// �傫���̎擾
    //const LVector2& getSize() const { return mSize; }

    /// �e�N�X�`���̐ݒ�
    void setTexture( Graphics::ITexture* texture_ );

    /// �e�N�X�`���̎擾
    Graphics::ITexture* getTexture();

    /// �e�N�X�`���̓]����`�̐ݒ�
    void setSrcRect( const Geometry::Rect& rect_ );

    /// �e�N�X�`���̓]����`�̎擾
    const Geometry::Rect& getSrcRect() const { return mSrcRect; }

    /// ���]���@�̐ݒ�
    void setFlipFlag( lnU32 flags_ ) { mFlipFlags = flags_; _updateTexUV(); }

    /// ���]���@�̎擾
    lnU32 getFlipFlag() const { return mFlipFlags; }

public:

    /// �m�[�h�̎�ނ̎擾
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_SPRITE3D; }

    /// �T�u�Z�b�g��`�悷��
    virtual LNRESULT drawSubset( lnU32 index_ );

private:

    /// �e�N�X�`�����W�̍X�V
    void _updateTexUV();

private:

    Graphics::GeometryRenderer* mRenderer;
    //LNAxisAlignedDir            mAADir;
    //LVector2                    mSize;
    Geometry::Rect				mSrcRect;
    lnU32                       mFlipFlags;

    LVector3                    mUpperLeft;
    LVector3                    mLowerRight;
    LVector2                    mUVUpperLeft;
    LVector2                    mUVLowerRight;

    bool                        mCalledSetSrcRect;
    
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