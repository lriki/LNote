//=============================================================================
//�y BatchSprite �z
//-----------------------------------------------------------------------------
///**
//  @file       BatchSprite.h
//  @brief      BatchSprite
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
// �� BatchSprite3D
//-----------------------------------------------------------------------------
///**
//  @brief		�o�b�`�`��3D�X�v���C�g (���V�F�[�_�g�p�s��)
//*/
//=============================================================================
class BatchSprite3D
    : public SceneNode
{
public:

    /// �R���X�g���N�^
    BatchSprite3D( SceneGraph* scene_ );

    /// �f�X�g���N�^
    virtual ~BatchSprite3D();

public:

    /// ������
    void initialize( lnfloat w_size_, lnfloat h_size_, LNAxisAlignedDir dir_ = LN_AADIR_RZ );

    /// �傫���̐ݒ�
    void setSize( const LVector2& size_ ) { mSize = size_; }

    /// �傫���̎擾
    const LVector2& getSize() const { return mSize; }

    /// �e�N�X�`���̐ݒ�
    void setTexture( Graphics::ITexture* texture_ );

    /// �e�N�X�`���̎擾
    Graphics::ITexture* getTexture() { return mTexture; }

    /// �e�N�X�`���̓]����`�̐ݒ�
    void setSrcRect( const Geometry::Rect& rect_ );

    /// �e�N�X�`���̓]����`�̎擾
    const Geometry::Rect& getSrcRect() const { return mSrcRect; }

    /// ���]���@�̐ݒ�
    void setFlipFlag( lnU8 flags_ ) { mFlipFlags = flags_; }

    /// ���]���@�̎擾
    lnU8 getFlipFlag() const { return mFlipFlags; }

public:

    /// �m�[�h�̎�ނ̎擾
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_BATCHSPRITE3D; }

    /// �T�u�Z�b�g��`�悷��
    virtual LNRESULT drawSubset( lnU32 index_ );

private:

    BatchSpriteProxy*   mBatchSpriteProxy;
    LNAxisAlignedDir    mAADir;
    LVector2            mSize;
    Graphics::ITexture* mTexture;
    Geometry::Rect		mSrcRect;
    lnU8                mFlipFlags;
    bool                mCalledSetSrcRect;

    class BatchSprite3DContext;
    friend class BatchSprite3DContext;
};

//=============================================================================
// �� BatchSprite
//-----------------------------------------------------------------------------
///**
//  @brief		�o�b�`�`��2D�X�v���C�g (���V�F�[�_�g�p�s��)
//*/
//=============================================================================
class BatchSprite
    : public SceneNode
{
public:

    /// �R���X�g���N�^
    BatchSprite( SceneGraph* scene_ );

    /// �f�X�g���N�^
    virtual ~BatchSprite();

public:

    /// ������
    void initialize();

    /// �e�N�X�`���̐ݒ�
    void setTexture( Graphics::ITexture* texture_ );

    /// �e�N�X�`���̎擾
    Graphics::ITexture* getTexture() { return mTexture; }

    /// �e�N�X�`���̓]����`�̐ݒ�
    void setSrcRect( const Geometry::Rect& rect_ );

    /// �e�N�X�`���̓]����`�̎擾
    const Geometry::Rect& getSrcRect() const { return mSrcRect; }

    /// ���]���@�̐ݒ�
    void setFlipFlag( lnU8 flags_ ) { mFlipFlags = flags_; }

    /// ���]���@�̎擾
    lnU8 getFlipFlag() const { return mFlipFlags; }

public:

    /// �m�[�h�̎�ނ̎擾
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_BATCHSPRITE; }

    /// �T�u�Z�b�g��`�悷��
    virtual LNRESULT drawSubset( lnU32 index_ );

private:

    BatchSpriteProxy*   mBatchSpriteProxy;
    Graphics::ITexture* mTexture;
    Geometry::Rect		mSrcRect;
    lnU8                mFlipFlags;
    bool                mCalledSetSrcRect;

    class BatchSpriteContext;
    friend class BatchSpriteContext;
};

#if 0
//=============================================================================
// �� BatchSprite
//-----------------------------------------------------------------------------
///**
//  @brief		�o�b�`�`��X�v���C�g (���V�F�[�_�g�p�s��)
//*/
//=============================================================================
class BatchSprite
    : public SceneNode
{
public:

    BatchSprite( SceneGraph* scene_ );

    virtual ~BatchSprite();

public:

    /// ������
    LNRESULT initialize();

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
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_BATCHSPRITE; }

    /// �T�u�Z�b�g��`�悷��
    virtual LNRESULT drawSubset( lnU32 index_ );

private:

    /// �e�N�X�`�����W�̍X�V
    void _updateTexUV();

private:

    Engine::Graphics::ISpriteRenderer*  mSpriteRenderer;
    LN3DFront                           mPanelDir;
    LVector2                            mSize;
    Core::Graphics::ITexture*           mTexture;
    LRect                               mSrcRect;
    u32                                 mFlipFlags;
    bool                                mCalledSetSrcRect;  ///< ��x�ł� setSrcRect() ���Ă΂ꂽ�t���O


    Graphics::GeometryRenderer* mRenderer;
    //LNAxisAlignedDir            mAADir;
    //LVector2                    mSize;
    LRect                       mSrcRect;
    lnU32                       mFlipFlags;

    LVector3                    mUpperLeft;
    LVector3                    mLowerRight;
    LVector2                    mUVUpperLeft;
    LVector2                    mUVLowerRight;

    bool                        mCalledSetSrcRect;
    
};
#endif
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================