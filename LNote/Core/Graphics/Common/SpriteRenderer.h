//=============================================================================
//�y SpriteRenderer �z
//-----------------------------------------------------------------------------
///**
//  @file       SpriteRenderer.h
//  @brief      SpriteRenderer
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <list>
#include "../Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

/// �o�b�`�����p�X�v���C�g���_
struct BatchSpriteVertex
{
    LVector3	Position;   ///< ���W
    u32         Color;      ///< ���_�J���[
    LVector2	TexUV;      ///< �e�N�X�`�����W
 
    
    /// ���_���C�A�E�g
    static LNVertexElemenst* elements()
    {
	    static LNVertexElemenst elements[] = {
		    { 0, LN_DECL_FLOAT3, LN_USAGE_POSITION, 0 },
            { 0, LN_DECL_COLOR4, LN_USAGE_COLOR,    0 },
            { 0, LN_DECL_FLOAT2, LN_USAGE_TEXCOORD, 0 },
		      LN_DECL_END() };
	    return elements;
    }
};

struct BatchSpriteData
{
    BatchSpriteVertex   Vertices[ 4 ];  ///< ���W�ϊ��ςݒ��_ ([0].Position.z ���\�[�g�̊�ɂȂ�)
    ITexture*           Texture;
    int                 Priority;       ///< �D��x (�傫�������ォ��`�悳��� =��O)
    lnfloat             Depth;          ///< �\�[�g�Ɏg���� Z �l (�傫���قǉ���)
    bool                Visible;
    bool                Assigned;       ///< SpriteRenderer::getBatchSpriteData() �Ŏ擾���ꂽ�ꍇ�� true �ɂȂ�

};


//=============================================================================
// �� SpriteRenderer �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�X�v���C�g�`��N���X (2D�A3D���p)
//*/
//=============================================================================
class SpriteRenderer
    : public Base::ReferenceObject
{
public:

    SpriteRenderer( Manager* manager_ );

    virtual ~SpriteRenderer();

    LN_REFOBJ_METHODS;

public:

    /// ������
    LNRESULT initialize( IGraphicsDevice* device_, int max_sprites_, bool is_3d_ );

    /// ���W�ϊ��s��̐ݒ�
    virtual void setTransform( const LMatrix& matrix_ );

    /// �r���[�A�v���W�F�N�V�����s��̐ݒ� (view �̓r���{�[�h�̌v�Z�ŕK�v�ɂȂ�̂ŁAview proj �ŕ�����)
    virtual void setViewProjMatrix( const LMatrix& view_, const LMatrix& proj_ );

    /// �\�[�g���@�̐ݒ�(LNSpriteSortMode  enable_view_pos_depth_:�J��������̋������\�[�g�pZ�l�ɂ���ꍇ true)
    virtual void setSortMode( u32 flags_, bool enable_view_pos_depth_ );

    /// �`�惊�N�G�X�g
    virtual LNRESULT drawRequest2D(
        const LVector3& position_,
        const LVector3& center_,
        ITexture* texture_,
        const Geometry::Rect& src_rect_,
        u32 color_ );

    /// �`�惊�N�G�X�g
    virtual LNRESULT drawRequest3D(
        const LVector3& position_,
        const LVector3& center_,
        const LVector2& size_,
        ITexture* texture_,
        const Geometry::Rect& src_rect_,
        const u32* color_table_,    // 4 ���_���BNULL �̏ꍇ�͔�
        LNAxisAlignedDir front_ );

    /// �o�b�`��������Ă���X�v���C�g�̕`��
    virtual LNRESULT flash();

private:

    /// Z �l�̑傫�������珬�������փ\�[�g�����r�֐�
    static bool _cmpDepthBackToFront( const u16& l_, const u16& r_ );

    /// Z �l�̏�����������傫�����փ\�[�g�����r�֐�
    static bool _cmpDepthFrontToBack( const u16& l_, const u16& r_ );

    /// Z �l�̑傫�������珬�������փ\�[�g�����r�֐� (�e�N�X�`���D��)
    static bool _cmpTexDepthBackToFront( const u16& l_, const u16& r_ );

    /// Z �l�̏�����������傫�����փ\�[�g�����r�֐� (�e�N�X�`���D��)
    static bool _cmpTexDepthFrontToBack( const u16& l_, const u16& r_ );
    

private:

    struct Attribute
    {
        int StartIndex;
        int PrimitiveNum;
        ITexture*   Texture;
    };

private:

    Manager*            mManager;
    IGraphicsDevice*    mGraphicsDevice;
    IVertexBuffer*      mVertexBuffer;
    IIndexBuffer*       mIndexBuffer;
    int                                 mMaxSprites;
    BatchSpriteData*                    mSpriteRequests;
    //Core::Base::Stack< int >            mIndexStack;
    int                                 mNextIndex;         ///< mSpriteRequests �̋󂢂Ă���C���f�b�N�X������
    int                                 mLastSpriteNum;
    LMatrix                             mTransformMatrix;
    LMatrix                             mViewInverseMatrix;
    LMatrix                             mViewProjMatrix;
    LVector3                            mViewPosition;
    LVector3                            mViewDirection;

    u16*                                mSpriteIndexArray;
    u16*                                mSpriteIndexArraySource;
    std::list< Attribute >              mAttributeList;

    bool ( *mComFunc )( const u16& l_, const u16& r_ );
    u32                                 mSpriteSortMode;        ///< �\�[�g���@ (LNSpriteSortMode)
    bool                                mEnableViewPosDepth;
    bool                                mIs3DSystem;        ///< true �̏ꍇ�A�X�v���C�g�̒��S�͎l�p�`�̒��S�ŁAY+ ��������ɂȂ�

    
   
    /// �V�F�[�_�֌W�̕ϐ����܂Ƃ߂��\����
    struct 
    {
        IShader*            Shader;

        IShaderVariable*    varViewProjMatrix;
        IShaderVariable*    varTexture;

        IShaderTechnique*   techMainDraw;

    } mShaderParam;


    static BatchSpriteData* sSpriteRequests;
};


#if 0


/// �e ISprite �����f�[�^
struct SpriteDrawRequest
{
	SpriteVertex	            Vertices[ 4 ];	///< �l�p�`�ЂƂ��̒��_�z��
    Core::Graphics::ITexture*   Texture;
	
    u32     Index;      ///< requestDraw() �ɓn�������Ɋ��蓖�Ă���f�[�^�ԍ� (�T�u�Z�b�g�ԍ�)
};

//=============================================================================
// �� SpriteRenderer �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class SpriteRenderer
    : public ISpriteRenderer
{
public:

    //---------------------------------------------------------------------
	///**
    //  @brief      �`�惊�N�G�X�g��ǉ�����
    //*/
    //---------------------------------------------------------------------
    LNRESULT requestDraw( SpriteDrawRequest* data_ );
   
};





class Sprite
{

    void onPreDraw()
};

#endif


/*

�� �݌v�Ƃ�

�ESprite �N���X�� drawSubset() �́A�T�u�Z�b�g�ԍ� 0 �݂̂��n����Ă���B

�ESceneObject ���p�����邱�ƂŃ��[�U�[�C�ӂ̕`����ł���悤�ɂ���
	�ESpriteRenderer ���g������AdrawPrimitive() �g���킯����Ȃ��̂ł���܂Œʂ���Ă����킯�ɂ͂����Ȃ��B

�ESceneOjbect �ɂ� onRequestBatchDraw() ���������Ă����B
  ���̊֐��́ASpriteRenderer �� BillboardRenderer �̂悤�ɁA�f�[�^���o�b�t�@�ɗ��߂Ă���
  ��ň�x�ɕ`�悷����̂̕`�惊�N�G�X�g���M����������B

�EISprite �ɂ͂Q�̎�ނ�����B
  �ЂƂ́A�P��̔ėp���̂���X�v���C�g���Ă����C���[�W�B
  ����͈ȑO�܂łƓ��l�AonDrawSubset() �� setVertexBuffer()�AdrawPrimitive() �̏��Ɏ��s���ĕ`�悷��B
  �o�b�`�����̑Ώۂł͂Ȃ��B
  �����ЂƂ́A�o�b�`�����������́B�ЂƂ߂̂��������ɕ`��ł���
  (�`�撆�AsetVertexBuffer() �Œ��_�o�b�t�@�̐ݒ��ς��Ȃ���)���A
  �X�v���C�g�ȊO�̃I�u�W�F�N�g�Ƃ�Z�\�[�g�����܂������Ȃ��B
  �i2D�ł���X�v���C�g�Ȃ�撣��΂�����Ǝv�����ǁA3D��Ԃɔz�u�����r���{�[�h�͂��Ȃ�L�c�C�j
  ���̂��߁A�o�b�`�����͈ȉ��̂��̂�ΏۂƂ���B
    �E�������v�f�̖����I�u�W�F�N�g(�A���t�@�e�X�g�ŃJ�b�g�������̂�OK(�A���t�@�l�����S��0))
    �E�������v�f������A���A�������v�f�̕`��p�X�ŕ`�悷�����(�ʏ�̃I�u�W�F�N�g�̌�ɕ`�悷��ׂ�����)
  �ꉞ�A�o�b�`�����̑z�肵�Ă���g�����͎�ɃG�t�F�N�g�֌W�Ȃ̂ŁA���̕����Ŗ��͂Ȃ������B
  �쐬�Ɋւ��Ă� createSprite() createBatchSprite() �̊֐����g���A
  �Ԃ�C���^�[�t�F�C�X�͗����Ƃ� ISprite�B





Selene �ł̓X�v���C�g�ЂƂ��� drawPrimitive �ǂ�ł����ǁc�H

����
    ���_ 60000
    �g���C�A���O�����X�g
    ���_�̋߂��ŕ`��

    �P�D�l�p�`�ЂƂɂ� ��ADrawPrimitive() ���Ă�
        ��t���[���ڂ����� 1000ms
        �����t���[���ڂ����� 500ms

    �Q�D�S�̂�1�x�� DrawPrimitive() �ŕ`��
        �P�D�ƂقƂ�Ǔ����B

    DrawPrimitive() �̂Ƃ��낾����ׂĂ݂�ƁA
    �P�D�ł� 0�`2ms�A�Q�D�ł� 0ms�B


    ���x�I�ɂ͂قƂ�Ǖς��Ȃ������B���Ȃ݂ɑ��x�������Ă�̂� Present()�B
    �덷�͊֐��Ăяo���̕��̎��Ԃ��Ċ����ŁA�`��ɂ͂قƂ�Ǌ֌W�Ȃ������B




�� �����̎l�p�`���i�[�������_�o�b�t�@�ɑ΂��āA
   �l�p�`�ЂƂɂ���x drawPrimitive() ���Ă�ŕ`�悷��


���؂���R�[�h

	u32 n = (VERTEX_NUM / 3) / 2;
	for ( u32 i = 0; i < n; ++i )
	{
	    renderer_->setVertexBuffer( mVertexBuffer, true );
	    renderer_->drawPrimitive( LN_PRIMITIVE_TRIANGLELIST, i * 3, 2 );
	}


���_�� 60000
�g���C�A���O�����X�g
��x�� drawPrimitive() ��2�̃v���~�e�B�u (�l�p�`)��`�悷��


�� ���I�Ȓ��_�o�b�t�@�Ƃ��č쐬�����ꍇ
	80ms�`90ms
	
�� �ÓI�Ȓ��_�o�b�t�@�Ƃ��č쐬�����ꍇ
	40ms�`50ms
	�{���炢�����B

�� lock() unlock() �̏d��
	�ÓI�Ȓ��_�o�b�t�@�Ƃ��č쐬��A
	��L�R�[�h�̒��O��
		mVertexBuffer->lock();
		mVertexBuffer->unlock();
	�̂Q�s���ĂԁB
	���ʁA80ms�`90ms�B
	���I�Ȓ��_�o�b�t�@�Ƃقړ����B
	
�� lock() unlock() �̏d��  100��
	�ÓI�Ȓ��_�o�b�t�@�Ƃ��č쐬��A
	��L�R�[�h�̒��O��
	for ( u32 i = 0; i < 100; ++i )
    {
        mVertexBuffer->lock();
        mVertexBuffer->unlock();
    }
    �����s����B
    ���ʁA80ms�`90ms�B
    �قƂ�Ǖς��Ȃ������B
    
�� renderer_->setVertexBuffer( mVertexBuffer, true ); ��
	���[�v�̊O�ɏo���āA�P�x�������s����悤�ɂ���
	
	�E���I�̏ꍇ
		80ms�`90ms�B
		���܂�ς��Ȃ��B
		
	�E�ÓI�̏ꍇ
		40ms�`50ms
		���܂�ς��Ȃ��B
		
	�E�ÓI�łP�O�O��lock()
		80ms�`90ms�B
		���܂�ς��Ȃ��B
		
		
�� �����ЂƂ��_�o�b�t�@������āA�ȉ��̃R�[�h�����s

	u32 n = (VERTEX_NUM / 3) / 2;
	n /= 2;
    for ( u32 i = 0; i < n; ++i )
    {
        renderer_->setVertexBuffer( mVertexBuffer, true );
        renderer_->drawPrimitive( LN_PRIMITIVE_TRIANGLELIST, i * 3, 2 );

        renderer_->setVertexBuffer( mVertexBuffer2, true );
        renderer_->drawPrimitive( LN_PRIMITIVE_TRIANGLELIST, i * 3, 2 );
    }
    
    �E���I�̏ꍇ
		200ms�`220ms�B
		
		
    �E�ÓI�̏ꍇ
    	90�`110ms
    	
    �E�ÓI�E100 lock() �̏ꍇ
    	90�`110ms
    	
	�E�ÓI�E100 lock() �����̏ꍇ
    	90�`110ms






struct SpriteVertex
{
	Core::Math::Vector3		Position;		///< ���W
	u32						Color;			///< ��Z�F (+�A���t�@�l)
	u32						BlendColor;		///< �u�����h�F (�h��Ԃ��F)
	u32						Tone;			///< �F��
	Core::Math::Vector2		TexUV;			///< �e�N�X�`�����W
	
	Core::Math::Vector3		WorldMatrix00_03;
	Core::Math::Vector3		WorldMatrix10_13;
	Core::Math::Vector3		WorldMatrix20_23;
};








class ISprite
{
};


class Sprite
	: public SceneObject
{
	Sprite()
	{
	}
};






class SpriteRenderer
{

	LNRESULT drawRequest();
};
*/





//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================