//=============================================================================
//�y ScreenEffect �z
//-----------------------------------------------------------------------------
///**
//  @file       ScreenEffect.h
//  @brief      ScreenEffect
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Animation/FadeValue.h"
#include "../../Animation/FixedLengthScalar.h"
#include "../../Graphics/Interface.h"

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
// �� ScreenEffect
//-----------------------------------------------------------------------------
///**
//  @brief		
//
//  @note
//              ���ԒP�ʂ̓t���[�����B
//*/
//=============================================================================
class ScreenEffect
    : public Base::ReferenceObject
{
public:

    ScreenEffect();

    virtual ~ScreenEffect();

public:

    void initialize( Graphics::IGraphicsDevice* device, const LVector2& view_size );

    void dispose();

    void setTone( const LTone& tone, double duration );

    void freeze();

    void transition( double duration, Graphics::ITexture* mask, int vague );

    /// (center �̓X�N���[�����W��ԂŎw�肷��(-1.0�`1.0) )
    void blur( double duration, lnfloat power, lnfloat scale, const LVector3& center );

    /// �u���[�K�p�����𔻒肷��
    bool isBlurEffect() const { return ( ( mBlarTime == -1.0 ) || ( mBlarTime >= 0.0 && !mBlurPower.isFinished() ) ); }

    /// �G�t�F�N�g�K�p�����𔻒肷��
    bool isEffect() const { return ( isBlurEffect() || mIsFreeze || mTone.isFinished() ); }

    void update();

    void updateDrawContent();

    /// ����̕`��ŏ������ނׂ��^�[�Q�b�g
    Graphics::ITexture* getPrimaryRenderTarget() const { return mPrimaryRenderTarget; }

    /// ����̕`��ŏ������ނׂ��^�[�Q�b�g
    Graphics::ITexture* getPrimaryDepthBuffer() const { return mPrimaryDepthBuffer; }

    void swapBuffers();

    /// ��L�^�[�Q�b�g�֍s���ׂ��`��
    void renderBlurAndTransition( IRenderer* renderer, GeometryRenderer* geometry_renderer );

    /// �o�b�N�o�b�t�@�֍s���ׂ��`��
    void renderScreen( IRenderer* renderer, GeometryRenderer* geometry_renderer );

private:

	LVector2				            mViewSize;

    Graphics::ITexture*                 mPrimaryRenderTarget;
    Graphics::ITexture*                 mPrimaryDepthBuffer;
    Graphics::ITexture*                 mSecondaryRenderTarget;
    Graphics::ITexture*                 mSecondaryDepthBuffer;
    Graphics::ITexture*                 mFreezedScreenTexture;
    Graphics::ITexture*                 mTransitionMaskTexture;

    Animation::FadeValue< LVector4, lnfloat >	mTone;

    lnfloat                             mTransitionVague;
    Animation::FadeValue< lnfloat >     mTransitionFactor;  ///< �g�����W�V�����̉��͈͂̍Œ�l (-mTransitionVague�`1.0)
    bool                                mIsFreeze;
    bool                                mFreezedAtFrame;

    LVector4                            mBlurCenter;
    LVector4                            mBlurColor;
    LMatrix                             mBlurMatrix;
    Animation::FixedLengthScalar        mBlurPower;
    double                              mBlarTime;              ///< �u���[�K�p�J�n����̗݌v���� (-1.0 �̏ꍇ�̓A�j���[�V���������ɌŒ肷��)


    // �����V�F�[�_�̕ύX���ł���悤�ɂ���Ȃ�A�Q�ƃJ�E���g����������
    // PaneContext �ő��삷��K�v������
    struct TransitionShader
    {
        Graphics::IShader*          Shader;
        Graphics::IShaderVariable*  varTransitionTime;
        Graphics::IShaderVariable*  varTexture;
        Graphics::IShaderVariable*  varTransitionFreezedTexture;
        Graphics::IShaderTechnique* techMainDraw;
    } mTransitionShader;

    struct TransitionWithMaskShader
    {
        Graphics::IShader*          Shader;
        Graphics::IShaderVariable*  varTransitionTime;
        Graphics::IShaderVariable*  varTransitionVague;
        Graphics::IShaderVariable*  varTexture;
        Graphics::IShaderVariable*  varTransitionFreezedTexture;
        Graphics::IShaderVariable*  varTransitionMaskTexture;
        Graphics::IShaderTechnique* techMainDraw;
    } mTransitionWithMaskShader;

    struct BufferingBlarShader
    {
        Graphics::IShader*          Shader;
        Graphics::IShaderVariable*  varBlurPower;
        Graphics::IShaderVariable*  varBlurColor;
        Graphics::IShaderVariable*  varBlurMatrix;
        Graphics::IShaderVariable*  varSecondaryTexture;
        Graphics::IShaderTechnique* techMainDraw;
    } mBufferingBlarShader;

	struct PostDrawShader
    {
        Graphics::IShader*          Shader;
        Graphics::IShaderVariable*  varTone;
        Graphics::IShaderVariable*  varPrimaryTexture;
        Graphics::IShaderTechnique* techMainDraw;
    } mPostDrawShader;

    class DrawContent;
    friend class DrawContent;
    DrawContent*    mDrawContent;
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