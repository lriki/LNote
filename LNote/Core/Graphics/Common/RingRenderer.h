//=============================================================================
//�y RingRenderer �z
//-----------------------------------------------------------------------------
///**
//  @file       RingRenderer.h
//  @brief      RingRenderer
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

//=============================================================================
// �� RingRenderer �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�X�v���C�g�`��N���X (2D�A3D���p)
//*/
//=============================================================================
class RingRenderer
    : public Base::ReferenceObject
{
public:

    /// ���_�f�[�^
    struct Vertex
    {
        LVector3	Position;
        lnU32       Color;
        LVector2	TexUV;
     
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

    /// �`�����
    struct DrawingParamater
    {
        LMatrix         Transform;
        lnU32           DivisionCount;
        lnfloat		    ViewingAngle;
        lnfloat         OuterRadius;
        lnfloat         OuterHeight;
        lnfloat         InnerRadius;
        lnfloat         InnerHeight;
		lnfloat		    CenterRatio;
		LColor		    OuterColor;
		LColor		    CenterColor;
		LColor		    InnerColor;
		LRectF	        UV;
        LNBillboardType BillboardType;
    };

public:

    RingRenderer( Manager* manager_ );

    virtual ~RingRenderer();

public:

    /// ������
    void initialize( int square_max_count_, bool is_3d_ );

    /// ���W�ϊ��s��̐ݒ�
    //virtual void setTransform( const LMatrix& matrix_ );

    /// �r���[�A�v���W�F�N�V�����s��̐ݒ� (view �̓r���{�[�h�̌v�Z�ŕK�v�ɂȂ�̂ŁAview proj �ŕ�����)
    void setViewProjMatrix( const LMatrix& view_, const LMatrix& proj_ );

    /// �e�N�X�`���̐ݒ�
    void setTexture( ITexture* texture_ );

    /// �\�[�g���@�̐ݒ�(LNSpriteSortMode  enable_view_pos_depth_:�J��������̋������\�[�g�pZ�l�ɂ���ꍇ true)
    //virtual void setSortMode( u32 flags_, bool enable_view_pos_depth_ );

    /// �`��J�n
    void begin();

    /// �`��I��
    void end();

    /// �`�惊�N�G�X�g
    //virtual LNRESULT drawRequest2D(

    /// �`�惊�N�G�X�g
    void drawRequest3D( const DrawingParamater& param_ );


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
    LNGraphicsAPI       mGraphicsAPI;
    IVertexBuffer*      mVertexBuffer;
    IIndexBuffer*       mIndexBuffer;
    ITexture*           mTexture;
    LMatrix             mViewInverseMatrix;
    LMatrix             mViewProjMatrix;
    LVector3            mViewPosition;
    LVector3            mViewDirection;
    Vertex*             mLockedBuffer;
    lnU32               mUsingVertexCount;


    struct 
    {
        IShader*            Shader;
        IShaderVariable*    varViewProjMatrix;
        IShaderVariable*    varTexture;
        IShaderTechnique*   techMainDraw;

    } mShaderParam;

    //IIndexBuffer*       mIndexBuffer;
#if 0
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

#endif
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