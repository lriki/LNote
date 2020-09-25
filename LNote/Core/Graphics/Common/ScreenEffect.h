//=============================================================================
//【 ScreenEffect 】
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
// ■ ScreenEffect
//-----------------------------------------------------------------------------
///**
//  @brief		
//
//  @note
//              時間単位はフレーム数。
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

    /// (center はスクリーン座標空間で指定する(-1.0～1.0) )
    void blur( double duration, lnfloat power, lnfloat scale, const LVector3& center );

    /// ブラー適用中かを判定する
    bool isBlurEffect() const { return ( ( mBlarTime == -1.0 ) || ( mBlarTime >= 0.0 && !mBlurPower.isFinished() ) ); }

    /// エフェクト適用中かを判定する
    bool isEffect() const { return ( isBlurEffect() || mIsFreeze || mTone.isFinished() ); }

    void update();

    void updateDrawContent();

    /// 今回の描画で書き込むべきターゲット
    Graphics::ITexture* getPrimaryRenderTarget() const { return mPrimaryRenderTarget; }

    /// 今回の描画で書き込むべきターゲット
    Graphics::ITexture* getPrimaryDepthBuffer() const { return mPrimaryDepthBuffer; }

    void swapBuffers();

    /// 上記ターゲットへ行うべき描画
    void renderBlurAndTransition( IRenderer* renderer, GeometryRenderer* geometry_renderer );

    /// バックバッファへ行うべき描画
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
    Animation::FadeValue< lnfloat >     mTransitionFactor;  ///< トランジションの可視範囲の最低値 (-mTransitionVague～1.0)
    bool                                mIsFreeze;
    bool                                mFreezedAtFrame;

    LVector4                            mBlurCenter;
    LVector4                            mBlurColor;
    LMatrix                             mBlurMatrix;
    Animation::FixedLengthScalar        mBlurPower;
    double                              mBlarTime;              ///< ブラー適用開始からの累計時間 (-1.0 の場合はアニメーションせずに固定する)


    // もしシェーダの変更をできるようにするなら、参照カウントを実装して
    // PaneContext で操作する必要がある
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