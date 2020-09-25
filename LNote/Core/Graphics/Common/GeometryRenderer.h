//=============================================================================
//�y GeometryRenderer �z
//-----------------------------------------------------------------------------
///**
//  @file       GeometryRenderer.h
//  @brief      GeometryRenderer
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Interface.h"
#include "../../Physics/IDebugRenderer.h"
#include "../../Geometry/Geometry.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
class PointParticle;

//=============================================================================
// �� GeometryRenderer �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		3D�v���~�e�B�u�̕`��N���X
//*/
//=============================================================================
class GeometryRenderer
    : public Base::ReferenceObject
    , public Physics::IDebugRenderer
{
public:

	/// �������f�[�^
	struct InitData
	{
        Base::LogFile*      LogFile;
        IGraphicsDevice*    GraphicsDevice;      
	};

    /// �`��p�X
    enum DrawingPass
    {
        PASS_BASIC = 0,
        PASS_POINT_PARTICLE,
    };

public:

    /// �R���X�g���N�^
    GeometryRenderer();

	/// �f�X�g���N�^
	virtual ~GeometryRenderer();

    LN_REFOBJ_METHODS;

public:

    /// ������
    LNRESULT initialize( const InitData& init_data_ );

    /// �`��p�X�̊J�n
    void begin( DrawingPass pass_ = PASS_BASIC );

    /// �`��p�X�̏I��
    void end();

    /// ���W�ϊ��s��̐ݒ�
    void setMatrix( const LMatrix& matrix_ );

    /// ���W�ϊ��s��̐ݒ�
    void setViewProjMatrix( const LMatrix& matrix_ );

    /// �r���[�T�C�Y�̐ݒ�( 2D �p�̃v���W�F�N�V�����s�������āAsetViewProjMatrix() �ɐݒ肷��)
    //      ���_���W�̓s�N�Z���P�ʂŎw��ł��邪�A�e�N�X�`�����W��0�`1.0�Ȃ̂Œ��ӁB
    void setViewSize( const LVector2& view_size_ );

    /// �e�N�X�`���̐ݒ�
    void setTexture( ITexture* texture_ );


    /// �l�p�`�̕`��
    LNRESULT drawSquare(
        lnfloat x0_, lnfloat y0_, lnU32 c0_, lnfloat u0_, lnfloat v0_, 
	    lnfloat x1_, lnfloat y1_, lnU32 c1_, lnfloat u1_, lnfloat v1_,
	    lnfloat x2_, lnfloat y2_, lnU32 c2_, lnfloat u2_, lnfloat v2_,
	    lnfloat x3_, lnfloat y3_, lnU32 c3_, lnfloat u3_, lnfloat v3_ );

    /// ��`�̕`��     
    LNRESULT drawRect( const Geometry::RectF& dest_rect_, const Geometry::RectF& src_rect_, lnU32 color_ );

    
    /// �����̕`�� (�P�F)
    LNRESULT drawLine( const LVector3& from_, const LVector3& to_, const LColor& color_ );

    /// �����̕`�� (��[�̐F�w��)
    LNRESULT drawLine( const LVector3& from_, const LVector3& to_, const LColor& from_color_, const LColor& to_color_ );

    /// ���̕`��
    LNRESULT drawSphere( const LVector3& pos_, lnfloat radius_, const LColor& color_ );

    /// �O�p�`�̕`��
    LNRESULT drawTriangle( const LVector3& v0_, const LVector3& v1_, const LVector3& v2_, const LColor& color_ );

    /// �ړ_�̕`�� (�ۗ�)
    //LNRESULT drawContactPoint( const LVector3& PointOnB, const LVector3& normalOnB, btScalar distance, int lifeTime, const LVector3& color );

    /// ���W���̕`��
    LNRESULT drawTransformAxis( lnfloat ortho_len_ );

    /// �{�b�N�X�̕`��
    LNRESULT drawBox( const LVector3& bb_min_, const LVector3& bb_max_, const LColor& color_ );
    
    /// �J�v�Z���̕`��
    LNRESULT drawCapsule( lnfloat radius_, lnfloat half_height_, const LColor& color_ );
    
    /// �V�����_�[�̕`�� (�ۗ�)
    LNRESULT drawCylinder( lnfloat radius_, lnfloat half_height_, const LVector3& color_ );
    
    /// �R�[���̕`�� (�ۗ�)
    LNRESULT drawCone( lnfloat radius_, lnfloat half_height_, const LVector3& color_ );
    
    /// ���ʂ̕`�� (�ۗ�)
    LNRESULT drawPlane( const LVector3& normal_, lnfloat plane_const_, const LVector3& color_ );

    /// ������̕`�� (���W�ϊ��ς݂Ƃ��ĕ`�悷��BsetMatrix() �ɂ͒P�ʍs���n���Ă�������)
    void drawViewFrustum( const LViewFrustum& frustum_, const LVector3& view_pos_, lnU32 color_ );

    /// PointParticle �̕`�� (������ PASS_POINT_PARTICLE �ɂ��p�X�J�n���s����)
    //      ���Ăяo�����Ń����_�����O�X�e�[�g��ύX���Ă������ƁI
    void drawPointParticle(
        const LMatrix& world_,
        const LMatrix& porj_,
        const LMatrix& view_porj_,
        const LVector4& camera_pos_,
        PointParticle* particle_ );

private:

    Base::LogFile*          mLogFile;
    IGraphicsDevice*        mGraphicsDevice;
    LNGraphicsAPI           mGraphicsAPI;
    IRenderer*              mRenderer;
    ITexture*               mDummyTexture;
    IShaderPass*            mCurrentShaderPass;

    IVertexBuffer*          mVBPosColor;
    IVertexBuffer*          mVBPosColorUV;
    IIndexBuffer*           mIndexBuffer;

    /// �V�F�[�_�֌W�̕ϐ����܂Ƃ߂��\����
    struct 
    {
        IShader*            Shader;
        IShaderVariable*    varWorldMatrix;
        IShaderVariable*    varViewProjMatrix;
        IShaderVariable*    varTexture;
        IShaderTechnique*   techMainDraw;
        IShaderPass*        passP0;

    } mShaderParam;

    /// PointParticle
    struct
    {
        IShader*            Shader;
        IShaderVariable*    varWorldMatrix;
        IShaderVariable*    varViewProjMatrix;
        IShaderVariable*    varProj_11_Div_00;
        IShaderVariable*    varTime;
        IShaderVariable*    varGlobalAccel;
        IShaderVariable*    varGravityPower;
        IShaderVariable*    varIsSequential;
        IShaderVariable*    varIsParticleLoop;
        IShaderVariable*    varIsDynamic;
        IShaderVariable*    varMaterialTexture;
        IShaderPass*        passP0;
    } mPointParticleShader;

    /*
    /// ���̕`��
    virtual LNRESULT drawAxis( lnfloat length_ );

    /// �l�p�`�̕`��
    virtual LNRESULT drawSquare(
        lnfloat x0_, lnfloat y0_, u32 c0_, lnfloat u0_, lnfloat v0_, 
	    lnfloat x1_, lnfloat y1_, u32 c1_, lnfloat u1_, lnfloat v1_,
	    lnfloat x2_, lnfloat y2_, u32 c2_, lnfloat u2_, lnfloat v2_,
	    lnfloat x3_, lnfloat y3_, u32 c3_, lnfloat u3_, lnfloat v3_ );

    /// ��`�̕`��     
    virtual LNRESULT drawRect( const LRectF& dest_rect_, const LRectF& src_rect_, u32 color_ );

private:

    Kernel::Base::LogFile*  mLogFile;
    IGraphicsDevice*        mGraphicsDevice;

    IVertexBuffer*          mVBPosColorUV;
    IVertexBuffer*          mVBPosColor;
    IRenderer*              mRenderer;
    
    

    
*/
    /*
    Manager*                            mManager;
    Core::Graphics::IGraphicsDevice*    mDevice;
	Core::Graphics::

    Core::Graphics::IVertexBuffer*      mAxisXYZVertexBuffer;   ///< ���p
    Core::Graphics::IVertexBuffer*      mBillboardVertexBuffer;
    Core::Graphics::IVertexBuffer*      mSpriteVertexBuffer;
    Core::Graphics::IVertexBuffer*      mBasic2DVertexBuffer;  //drawSquare �Ŏg���\�肾��������
    Core::Graphics::IVertexBuffer*      mScreenBuffer;          ///< �X�N���[���`��p



    Core::Graphics::IVertexBuffer*      mDebugShapesVertexBuffer;   ///< �f�o�b�O�`��p���_�o�b�t�@
    Core::Graphics::IIndexBuffer*       mDebugShapesIndexBuffer;    ///< �f�o�b�O�`��p�C���f�b�N�X�o�b�t�@


    Core::Graphics::IShader*            mShader;
    Core::Graphics::IShaderVariable*    mMatrixVar;
    Core::Graphics::IShaderVariable*    mTextureSizeVar;
    
    Core::Graphics::IShaderVariable*    mTextureVar;
    Core::Graphics::IShaderTechnique*   mScreenBufferTech;


    Core::Graphics::IShaderTechnique*   mCurrentShaderTech;
    Core::Graphics::IShaderPass*        mCurrentShaderPass;

    struct 
    {
        struct
        {
            Core::Graphics::IShaderVariable*    ViewportSize;

        } Var;

        struct 
        {
            Core::Graphics::IShaderTechnique*   Basic2DTranslated;

        } Tech;

        Core::Graphics::IShaderTechnique*   techDebugShapes;

        Core::Graphics::IShaderVariable*    varViewProjMatrix;


    } mShaderParam;
    */
};




//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

/*




class LocusData
{



    // �ȉ��͂��̃f�[�^���O���[�v�̐擪�ɂ���f�[�^�̏ꍇ�ɐݒ肳���

    bool    mGroupFirst;    ///< �O���[�v�̐擪�ɂ���f�[�^�̏ꍇ true

    u32     mVertexIndex;   ///< �O���[�v�̊J�n���_�ԍ�

    u32     mPrimitiveNum;  ///< �O���[�v�S�̂̃v���~�e�B�u��
};


class BatchLocus
    : public ILocus
{
public:

    virtual bool isEnable() { return mLocusData.mGroupFirst; }

private:

    LocusData   mLocusData;
};





�ȑO�܂ł�







	

LEffectEmitter
	getType
	setRenderState
	setFillMode
	refresh
	





class Line3D
{
	Vector3		mPoint1;	///< �����̊J�n�ʒu 
	Vector3		mPoint2;	///< �����̏I���ʒu
};


/// LocusRenderer �ɓn���悤�Ȋ����̃f�[�^�܂Ƃ�
struct LocusDrawData
{
	Line3D*		LineArray;		///< 
	u32			LineArraySize;
	u32			LineNum;
};





�O�ՃG�~�b�^
	
	
	LocusDrawData


�P�D�쐬����Ă��郉�C���̐��ɂ���āA�����x��擪���猸�Z���Ă���( �O��)
		�����̃��C���͏�ɓ����x��0�ɂȂ�B
		
�Q�D�쐬����Ă��郉�C�����ɁA�o�߃t���[�����ɂ���ē����x�������Ă���
		�����̃��C���̓����x�� 0 �Ƃ͌���Ȃ��B
		�אڂ��郉�C���Ƃ̓����x�̍��̐�Βl����ɓ������B
		
		���́A�����x�̍��� 1.0 / MaxLine �ȏ�łȂ��ƁA�o�b�t�@��1�������Ƃ���
		�O�Ղ��v�c���Ɛ؂ꂽ�悤�ȏ�ԂɂȂ邱�ƁB
		
		
		
�O�Ղ́A�ӂ[�ɕ`�悵�����Ƃ��̓��C���Ɠ��������ɂȂ�B
Win32 �`����������Ƃ������������ɂ���ƁA

line_renderer->begin( �n�_ );		// MoveToEx
line_renderer->drawLineTo( �_ );	// LineTo
line_renderer->drawLineTo( �_ );	// LineTo
line_renderer->drawLineTo( �_ );	// LineTo
//...
line_renderer->end();

�O�Ղ�����Ɠ��������B
winapi �Ƃ��̂ƈႤ�̂́A�e�N�X�`�����W��F�����܂ޒ��_�ł��邱�ƁB

�����ЂƂA���_�z���n���ĕ`�悷��̂�����BC# �ɂ� DrawLines �݂����Ȃ̂��������B

locus_renderer->drawLines( �����̔z�� );

�����̔z��Ƃ����邯�ǁA���ۂ͒��_2���Z�b�g�ɂ��钸�_�o�b�t�@�B
�G�t�F�N�g�ł͂����������C���Ɏg���Ă������ƂɂȂ�Ǝv���B


�ƂȂ�ƁALocus �Ȃ�N���X�̒����璸�_�o�b�t�@�����o���� drawLines �ɓn������̂�����΂���Ȃ��OK�B




�����ALocus �͊�{�������ŁA3D��Ԃɔz�u�������́B
�P�D�o�b�t�@�Ƃ��֌W�Ȃ��A���ʂɃ��b�V���Ɠ����悤�� onDrawSubset() �ŕ`�悷��B
�Q�D�s�����`��Ɣ������`��̃X�e�b�v�ɕ�����
	(�X�e�b�v������ Priority �Őݒ肷��΂��������B)
	
�ꉞ SceneObject ����`�悷��ꍇ�A���N�G�X�g�𑗐M����^�C�~���O��Z�\�[�g�͍ς�ł�̂�
���N�G�X�g���X�g�ɂ͐��������ԂŒǉ������B
�X�e�b�v�����ƁA�������`�掞�� ZWrite ��؂�Ȃǂ̑Ή�����������Ƃ��Ă���΂Q�D�ł������������Ȃ����삷��B

���́A�V�F�[�_�̓K�p�B
�e�N�X�`�������Ɠ������A�V�F�[�_�P�ʂŃO���[�v�����K�{�B
�����A�`��R�}���h�̍쐬���ǂ�����̂��B

Z�P�ʂ̃\�[�g�͂Ƃ������APriority �ł̃\�[�g�͂܂���肪�o�Ă���B
��L�ɉ����āA(Z)�APriprity ���O���[�v���̑Ώۂɂ���ꍇ��
�����Ȃ񂩍ו������ꂷ���ă��N�G�X�g�Ƃ��o�b�t�@�Ƃ��g���Ӗ����Ȃ��Ȃ��Ă��Ă�C������B

���ɂ��̂ւ�̗��_������Ƃ���΁A����G�t�F�N�g���ŋO�ՃG�~�b�^���R�s�y���ĕ���������ꍇ�B

��������Ƃ�����A�X�v���C�g�Ɠ����� Locus �� BatchLocs �݂����ɓ�̃N���X����銴���ɂȂ�Ƃ������B

BatchLocs �� BatchSprite �Ɠ������A�G�t�F�N�g�ł̎g�p��z�肷��B
SceneObject �P�ʂ� Z �\�[�g�͂��邯�ǁA����o�b�`���Ń\�[�g�͂��Ȃ��B
�܂��A�O���[�v���ɂ��\�[�g�͈ȉ��̃^�C�~���O�Ńt���O�� ON�ɂ��A�`��̒��O�ł܂Ƃ߂čs���悤�ɂ���B

	�V�����I�u�W�F�N�g���쐬���ꂽ
	�e�N�X�`�����Z�b�g���ꂽ
	�V�F�[�_���Z�b�g���ꂽ
	
�����̑��삪1�t���[�����Ŏ��s����Ȃ������ꍇ�A�O���[�v���A�\�[�g�͍s���Ȃ��B
Z�\�[�g�͊֌W�Ȃ��̂ŁA���A���^�C���ŃO���[�v�����Ă��Ƃ͂���܂�Ȃ��Ǝv���B

�O���[�v����́ASceneObject ��
	�E�S�̂̒��_�o�b�t�@�̏������݈ʒu
��Ԃ��Ă����K�v������B

�O���[�v���́A�P���� Priprity �Ń\�[�g������A�A�����Ă���e�N�X�`���A�V�F�[�_�ł܂Ƃ߂邾���B


���Ƃ́A�`��R�}���h�̍쐬�B

SceneObject �� LocusRenderer ���������āAonDrawSubset() �ŕ`�悷��B�����̓O���[�v�ԍ��B
�����A�e�N�j�b�N�J�n�Ƃ��̊֌W�Ŗ�肪���邽�߁A�V���� onCreateDrawCommand() �������
������I�[�o�[���C�h����悤�ɂ��Ă����K�v������B

�c���Č����Ă��A���ǂ��̂܂܂��� SceneObject �ЂƂ�����̏����ɂȂ��Ă��邽�߁A
���̂� Priiority �P�ʂ̃\�[�g���ł��Ȃ��B

�p�̓R�}���h�쐬�̂Ƃ��ALocusRenderer �𑼂� SceneObject �̃��[�v�̒��ɖ��ߍ��߂�΂������ǁA
���̃��[�v�͒P���ȃ��X�g�̃C�e���[�g�����A�������� CameraScene �Ƃ��ɓn���Ă�̂�
SceneObject �P�ʂȂ̂ł�����ƃL�c�C�B

�ŏI�I�ȕ`��� DrawSubset �ōs���悤�ɂ���Α��̂Ɠ���ł��邩��A
����ς� SceneObject ���Ɏ�����������ł����Ǝv���B
���Ă����̂��A�R�}���h�� DRAW_SUBSET �ŕ`�悵��������B


����ɖ��ɂȂ��Ă���̂́A�V�F�[�_�ɑ���l�B�ϊ��s���}�e���A���B
�s��� SEND_OBJ_PARAM�A�}�e���A���� DRAW_SUBSET �Őݒ肳���B
���Ă������A������g���������ݒ肵�₷���B
�ϊ��s��̕��͒P�ʍs��ݒ肵�Ă����΂������������炻��Ȃɓ���Ȃ��Ǝv�����ǁA
�ǂ݂̂��ŏI�I�ɂ� SceneObject �P�ʂ̕`��B


���_�Ƃ����܂Ƃ߂�Ɓc

�P�D�`��� SceneObject::onDrawSubset() �ŁA�O���[�v�ԍ����󂯎���čs���B
�Q�D�O���[�v�̃v���C�I���e�B�ŁA���� SceneObject �ƃ\�[�g����B

�Q�D�́A�ȑO�J�b�g�Ƃ��������ɂ����u�T�u�Z�b�g�P�ʂ�Z�\�[�g�v�Ƃ܂������������́B
�V�F�[�_�Ƃ̌��ˍ����������āASceneObject �P�ʂŎ�������͖̂����ɂȂ����B

�ł��A�O���[�v�͂ЂƂ̃V�F�[�_�������Ă���Ƃ���������A
SceneObject �ЂƂ��ɑ�������B


�ŏI�I�ɂ�肽�����Ƃ́A
�u�����X�e�[�g(�}�e���A����e�N�X�`���A�V�F�[�_��)�ŕ`�悷��ׂ����_�o�b�t�@����x�� drawPrimitive() �ɂ܂Ƃ߂�v���ƁB

����Ȃ�A�O���[�v�̐����� SceneObject ������Ă����̂���ԓK���Ȃ̂����B

���邢�́A�O�Ղ������ ILocus �� onDrawSubset �ŁA
���� ILocus ���O���[�v�̐擪�������ꍇ�A�㑱�����ׂĕ`�悵�A
����ȊO�̂� onDrawSubset �ł��������Ȃ��Ƃ������A�ނ��� isEnable() ��false �Ԃ��Ă����āA
�V�F�[�_�ւ̐ݒ���Ȃɂ��s��Ȃ��悤�ɂ���΂悢�̂ł͂Ȃ����B

���̕��@�̏ꍇ�A��ԏd�v�Ȃ��Ƃ�
�u�ݒ肳��钸�_�ʒu�̓��[���h�ϊ��ς݁v�ł��邱�ƁB
�܂��́A�X�v���C�g�Ɠ����悤�ɁA���_�ɍs��f�[�^���܂߂Ă��܂��B
����́A�擪�� SceneObject �ŕ`����ꊇ���čs���Ă��܂����߁A
�㑱�̃��[���h�s�񂪔��f����Ȃ��Ƃ������ۂ��o�Ă��邽�߁B





�c�����Ȃ񂩂��Ȃ蕡�G�ɂȂ��Ă��Ă邵�A
�V�F�[�_�������鑤�����낢��o���Ă����Ȃ��ƂȂ�Ȃ����ǁA
���Ǒ�ʂ̃G�t�F�N�g���o���ɂ͈�𔃂��Ă����c�Ǝv���B

�o�b�`�n�� Renderer �̓G�t�F�N�g��p�̓���Ȃ��̂��ĕ����ŁB


















LocusRenderer ���ł̏����c
	







*/

//=============================================================================
//								end of file
//=============================================================================