//=============================================================================
//�y ScreenEffect �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "GeometryRenderer.h"
#include "ScreenEffect.h"

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
// �� ScreenEffect::DrawContent
//=============================================================================
class ScreenEffect::DrawContent
{
public:

    DrawContent()
        : mTransitionMaskTexture    ( NULL )
    {
    }

    ~DrawContent()
    {
        LN_SAFE_RELEASE( mTransitionMaskTexture );
    }
    
public:

    void updateDrawContent( ScreenEffect* obj )
    {
        mViewSize = obj->mViewSize;

        // mPrimaryRenderTarget�AmFreezedScreenTexture �͊O������Z�b�g����邱�Ƃ͂Ȃ��̂ł��̂܂܂�OK
        mPrimaryRenderTarget    = obj->mPrimaryRenderTarget; 
        mFreezedScreenTexture   = obj->mFreezedScreenTexture;
        LN_REFOBJ_SET( mTransitionMaskTexture, obj->mTransitionMaskTexture );

        mTone                   = obj->mTone.getValue();
        mTransitionVague        = obj->mTransitionVague;
        mTransitionFactor       = obj->mTransitionFactor.getValue();
        mIsFreeze               = obj->mIsFreeze;

        mTransitionShader           = &obj->mTransitionShader;
        mTransitionWithMaskShader   = &obj->mTransitionWithMaskShader;
        mPostDrawShader             = &obj->mPostDrawShader;

        if ( obj->isBlurEffect() )
        {
            mBlurColor = obj->mBlurColor;
            mBlurMatrix = obj->mBlurMatrix;
            mBlurPower = obj->mBlurPower.getValue();
            mSecondaryRenderTarget = obj->mSecondaryRenderTarget;
            mBufferingBlarShader = &obj->mBufferingBlarShader;
        }
        else
        {
            mBufferingBlarShader = NULL;
        }
    }

    void renderForInternalTarget(
        Graphics::IRenderer* renderer,
        Graphics::GeometryRenderer* geometry_renderer )
    {
        // �g�����W�V��������
        if ( mIsFreeze )
	    {
            // �}�X�N�e�N�X�`���g�p
            if ( mTransitionMaskTexture )
            {
                mTransitionWithMaskShader->varTexture->setTexture( mPrimaryRenderTarget );
                mTransitionWithMaskShader->varTransitionTime->setFloat( mTransitionFactor );
                mTransitionWithMaskShader->varTransitionFreezedTexture->setTexture( mFreezedScreenTexture );
                mTransitionWithMaskShader->varTransitionVague->setFloat( mTransitionVague );
                mTransitionWithMaskShader->varTransitionMaskTexture->setTexture( mTransitionMaskTexture );
            }
            else
            {
                mTransitionShader->varTexture->setTexture( mPrimaryRenderTarget );
                mTransitionShader->varTransitionTime->setFloat( mTransitionFactor );
                mTransitionShader->varTransitionFreezedTexture->setTexture( mFreezedScreenTexture );
            }
        }
        else
        {
            mTransitionShader->varTransitionTime->setFloat( 1.0f );
        }

        // �u���[����
        if ( mBufferingBlarShader )
        {
            mBufferingBlarShader->varBlurPower->setFloat( mBlurPower );
            mBufferingBlarShader->varBlurColor->setVector( mBlurColor );
            mBufferingBlarShader->varBlurMatrix->setMatrix( mBlurMatrix );
            mBufferingBlarShader->varSecondaryTexture->setTexture( mSecondaryRenderTarget );
        }

        // �`�揀��
        lnfloat ou = 0.5f / mViewSize.x;
        lnfloat ov = 0.5f / mViewSize.y;

        // �u���\�`��
        if ( mBufferingBlarShader )
        {
            LRenderState state;
            state.DepthTest = false;
            state.DepthWrite = false;
            state.AlphaTest = false;
            state.BlendMode = LN_BLEND_ALPHA;
            mBufferingBlarShader->techMainDraw->getPassByIndex( 0 )->begin();
            geometry_renderer->drawSquare(
                -1.0f,  1.0f, 0xffffffff, 0.0f + ou, 0.0f + ov,   // ����
                 1.0f,  1.0f, 0xffffffff, 1.0f + ou, 0.0f + ov,   // �E��
                -1.0f, -1.0f, 0xffffffff, 0.0f + ou, 1.0f + ov,
                 1.0f, -1.0f, 0xffffffff, 1.0f + ou, 1.0f + ov );
            mBufferingBlarShader->techMainDraw->getPassByIndex( 0 )->end();
        }

        // �g�����W�V�����`��
        if ( mIsFreeze )
        {
            Graphics::IShaderTechnique* tech;

            // �}�X�N�e�N�X�`������
            if ( mTransitionMaskTexture )
            {
                tech = mTransitionWithMaskShader->techMainDraw;
            }
            // �}�X�N�e�N�X�`���Ȃ�
            else
            {
                tech = mTransitionShader->techMainDraw;
            }

            renderer->setRenderState( LRenderState::BUFFER_COPY_STATE );
            tech->getPassByIndex( 0 )->begin();
            geometry_renderer->drawSquare(
                -1.0f,  1.0f, 0xffffffff, 0.0f + ou, 0.0f + ov,   // ����
                 1.0f,  1.0f, 0xffffffff, 1.0f + ou, 0.0f + ov,   // �E��
                -1.0f, -1.0f, 0xffffffff, 0.0f + ou, 1.0f + ov,
                 1.0f, -1.0f, 0xffffffff, 1.0f + ou, 1.0f + ov );
            tech->getPassByIndex( 0 )->end();
        }
    }

    void renderForScreen(
        Graphics::IRenderer* renderer,
        Graphics::GeometryRenderer* geometry_renderer )
	{
		mPostDrawShader->varTone->setVector( mTone );
		mPostDrawShader->varPrimaryTexture->setTexture( mPrimaryRenderTarget );

        lnfloat ou = 0.5f / mViewSize.x;
        lnfloat ov = 0.5f / mViewSize.y;

        renderer->setRenderState( LRenderState::BUFFER_COPY_STATE );
        //renderer_->clear(true,true,0xff0000ff,1.0f);
        mPostDrawShader->techMainDraw->getPassByIndex( 0 )->begin();
        geometry_renderer->drawSquare(
            -1.0f,  1.0f, 0xffffffff, 0.0f + ou, 0.0f + ov,
			1.0f,  1.0f, 0xffffffff, 1.0f + ou, 0.0f + ov,
            -1.0f, -1.0f, 0xffffffff, 0.0f + ou, 1.0f + ov,
			1.0f, -1.0f, 0xffffffff, 1.0f + ou, 1.0f + ov );
        mPostDrawShader->techMainDraw->getPassByIndex( 0 )->end();
        
	}

private:

    LVector2            mViewSize;

    Graphics::ITexture*                 mPrimaryRenderTarget;   ///< draw() ���_�ł��̉�̃V�[�����`�����܂�Ă��邱��
    //Graphics::ITexture*                 mPrimaryDepthBuffer;
    
    //Graphics::ITexture*                 mSecondaryDepthBuffer;
    Graphics::ITexture*                 mFreezedScreenTexture;
    Graphics::ITexture*                 mTransitionMaskTexture;

	LVector4				mTone;

    lnfloat                             mTransitionVague;
    lnfloat      mTransitionFactor;
    bool        mIsFreeze;

    LVector4                            mBlurColor;
    LMatrix                             mBlurMatrix;
    lnfloat                             mBlurPower;
    Graphics::ITexture*                 mSecondaryRenderTarget;

    ScreenEffect::BufferingBlarShader*      mBufferingBlarShader;
    ScreenEffect::TransitionShader*         mTransitionShader;
    ScreenEffect::TransitionWithMaskShader* mTransitionWithMaskShader;
	ScreenEffect::PostDrawShader*	        mPostDrawShader;
};

//=============================================================================
// �� ScreenEffect �N���X
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    ScreenEffect::ScreenEffect()
        : mPrimaryRenderTarget      ( NULL )
        , mPrimaryDepthBuffer       ( NULL )
        , mSecondaryRenderTarget    ( NULL )
        , mSecondaryDepthBuffer     ( NULL )
        , mFreezedScreenTexture     ( NULL )
        , mTransitionMaskTexture    ( NULL )
        , mTransitionVague          ( 0.0f )
        , mIsFreeze                 ( false )
        , mFreezedAtFrame           ( false )
        , mBlurCenter               ( 0, 0, 0, 0 )
        , mBlurColor                ( 1, 1, 1, 1 )
        , mBlarTime                 ( 0.0 )
        , mDrawContent              ( NULL )
    {
        mTransitionShader.Shader = NULL;
        mTransitionWithMaskShader.Shader = NULL;
        mBufferingBlarShader.Shader = NULL;
		mPostDrawShader.Shader = NULL;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    ScreenEffect::~ScreenEffect()
    {
        dispose();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void ScreenEffect::initialize( Graphics::IGraphicsDevice* device, const LVector2& view_size )
	{
        dispose();

        mViewSize = view_size;
        lnU32 w = static_cast< lnU32 >( view_size.x );
        lnU32 h = static_cast< lnU32 >( view_size.y );

        //-----------------------------------------------------
        // ���\�[�X�쐬

        device->createRenderTarget(
            &mPrimaryRenderTarget, 
            w, h, 1, LN_FMT_X8R8G8B8 );

        device->createDepthBuffer( 
            &mPrimaryDepthBuffer,
            w, h, LN_FMT_D24S8 );

        device->createRenderTarget(
            &mSecondaryRenderTarget, 
            w, h, 1, LN_FMT_X8R8G8B8 );

        device->createDepthBuffer( 
            &mSecondaryDepthBuffer,
            w, h, LN_FMT_D24S8 );

        device->createRenderTarget(
            &mFreezedScreenTexture, 
            w, h, 1, LN_FMT_X8R8G8B8 );

        device->createShader(
            &mTransitionShader.Shader,
            _T( "LNResource\\Shader\\BasicPane_Transition.lnfx" ),
            LSharingKey( _T( "LNResource\\Shader\\BasicPane_Transition.lnfx" ) ) );
        mTransitionShader.varTransitionTime = mTransitionShader.Shader->getVariableByName( _T( "gTransitionTime" ) );
        mTransitionShader.varTexture = mTransitionShader.Shader->getVariableByName( _T( "gTexture" ) );
        mTransitionShader.varTransitionFreezedTexture = mTransitionShader.Shader->getVariableByName( _T( "gTransitionFreezedTexture" ) );
        mTransitionShader.techMainDraw = mTransitionShader.Shader->getTechniqueByIndex( 0 );

        device->createShader(
            &mTransitionWithMaskShader.Shader,
            _T( "LNResource\\Shader\\BasicPane_TransitionWithMask.lnfx" ),
            LSharingKey( _T( "LNResource\\Shader\\BasicPane_TransitionWithMask.lnfx" ) ) );
        mTransitionWithMaskShader.varTransitionTime = mTransitionWithMaskShader.Shader->getVariableByName( _T( "gTransitionTime" ) );
        mTransitionWithMaskShader.varTransitionVague = mTransitionWithMaskShader.Shader->getVariableByName( _T( "gTransitionVague" ) );
        mTransitionWithMaskShader.varTexture = mTransitionWithMaskShader.Shader->getVariableByName( _T( "gTexture" ) );
        mTransitionWithMaskShader.varTransitionFreezedTexture = mTransitionWithMaskShader.Shader->getVariableByName( _T( "gTransitionFreezedTexture" ) );
        mTransitionWithMaskShader.varTransitionMaskTexture = mTransitionWithMaskShader.Shader->getVariableByName( _T( "gTransitionMaskTexture" ) );
        mTransitionWithMaskShader.techMainDraw = mTransitionWithMaskShader.Shader->getTechniqueByIndex( 0 );
                
        device->createShader(
            &mBufferingBlarShader.Shader,
            _T( "LNResource\\Shader\\BasicPane_BufferingBlar.lnfx" ),
            LSharingKey( _T( "LNResource\\Shader\\BasicPane_BufferingBlar.lnfx" ) ) );
        mBufferingBlarShader.varBlurPower = mBufferingBlarShader.Shader->getVariableByName( _T( "gBlurPower" ) );
        mBufferingBlarShader.varBlurColor = mBufferingBlarShader.Shader->getVariableByName( _T( "gBlurColor" ) );
        mBufferingBlarShader.varBlurMatrix = mBufferingBlarShader.Shader->getVariableByName( _T( "gBlurMatrix" ) );
        mBufferingBlarShader.varSecondaryTexture = mBufferingBlarShader.Shader->getVariableByName( _T( "gSecondaryTexture" ) );
        mBufferingBlarShader.techMainDraw = mBufferingBlarShader.Shader->getTechniqueByIndex( 0 );
            
		device->createShader(
            &mPostDrawShader.Shader,
            _T( "LNResource\\Shader\\BasicPane_PostDraw.lnfx" ),
            LSharingKey( _T( "LNResource\\Shader\\BasicPane_PostDraw.lnfx" ) ) );
		mPostDrawShader.varTone = mPostDrawShader.Shader->getVariableByName( _T( "gTone" ) );
		mPostDrawShader.varPrimaryTexture = mPostDrawShader.Shader->getVariableByName( _T( "gPrimaryTexture" ) );
		mPostDrawShader.techMainDraw = mPostDrawShader.Shader->getTechniqueByIndex( 0 );

        //-----------------------------------------------------
        // 

        // �t���[���P�ʂŃA�j���[�V����
		mBlurPower.setTimeTickPerSec( 1.0f );
	    mBlurPower.setCapacity( 5 );

        mDrawContent = LN_NEW DrawContent();
	}

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void ScreenEffect::dispose()
    {
        LN_SAFE_RELEASE( mPrimaryRenderTarget );
        LN_SAFE_RELEASE( mPrimaryDepthBuffer );
        LN_SAFE_RELEASE( mSecondaryRenderTarget );
        LN_SAFE_RELEASE( mSecondaryDepthBuffer );
        LN_SAFE_RELEASE( mFreezedScreenTexture );
        LN_SAFE_RELEASE( mTransitionMaskTexture );
        LN_SAFE_RELEASE( mTransitionShader.Shader );
        LN_SAFE_RELEASE( mTransitionWithMaskShader.Shader );
        LN_SAFE_RELEASE( mBufferingBlarShader.Shader );
		LN_SAFE_RELEASE( mPostDrawShader.Shader );
        SAFE_DELETE( mDrawContent );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void ScreenEffect::setTone( const LTone& tone, double duration )
    {
        mTone.start( mTone.getValue(), (const LVector4&)tone, (lnfloat)duration );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
	void ScreenEffect::freeze()
    {
        if ( !mFreezedAtFrame )
        {
            // ���݂̉�ʂ� mFreezedScreenTexture �Ƃ��Ď����Ă���
            Graphics::ITexture* primary = mPrimaryRenderTarget;
            mPrimaryRenderTarget = mFreezedScreenTexture;
		    mFreezedScreenTexture = primary;
            
            mTransitionFactor.start( 0.0f, 0.0f, 0.0 );
            mTransitionFactor.setTime( 0.0 );
		    mIsFreeze = true;
            mFreezedAtFrame = true;
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void ScreenEffect::transition( double duration, Graphics::ITexture* mask, int vague )
    {
        if ( mIsFreeze )
		{
            if ( mask )
            {
                LN_SAFE_ADDREF( mask );
                LN_SAFE_RELEASE( mTransitionMaskTexture );
			    mTransitionMaskTexture = mask;
                
                // �����܂����͈̔͂� 0.0�`1.0 �ɕϊ�����
                int v = LMath::limit( vague, 1, 255 );
			    mTransitionVague = static_cast< lnfloat >( v ) / 255.f;

                mTransitionFactor.start( -mTransitionVague, 1.0f, duration );
                mTransitionFactor.setTime( 0.0 );

                /*
			    // �g�����W�V�����J�n���Atime = 0.0 �̏�Ԃ���
			    // �����܂����͈̔͂ɓ����Ă���F���ŏ����瓧�߂���Ă��܂��B
			    // �J�n���Ԃ͂����܂����͈̔͂������̒l�ɃZ�b�g���Ă����B
			    mTransitionTime = -mTransitionVague;
			    mTransitionElapsedTime = 1.0 / duration_;
                mTransitionElapsedTime += mTransitionVague / duration_;

                // �Ȃ��A�g�����W�V������ mTransitionTime �� 1.0 �ȏ�ɂȂ�ƏI��
                */
            }
            else
            {
                mTransitionFactor.start( 0.0f, 1.0f, duration );
                mTransitionFactor.setTime( 0.0 );
            }
		}
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void ScreenEffect::blur(
        double duration,
        lnfloat power,
        lnfloat scale,
        const LVector3& center )
    {
        mBlurMatrix.identity();

        // �u���[�̓K�p�𖳂��ɂ���ꍇ
        if ( power == 0.0 )
        {
            mBlurPower.clear();
            mBlarTime = 0.0;
            mBlurPower.setTime( 0.0 );
            return;
        }

        if ( scale != 1.0f )
        {
            mBlurCenter.set( center.x, center.y, center.z, 1.0f );

            // mBlurCenter �𒆐S�Ɋg�傷��
            mBlurMatrix.translation( -mBlurCenter.x, -mBlurCenter.y, 0 );
            mBlurMatrix.scaling( scale );
            mBlurMatrix.translation( mBlurCenter.x, mBlurCenter.y, 0 );
        }

        mBlurPower.clear();

        // �����J�ڂ��Ȃ��ꍇ
        if ( duration == 0 )
        {
            mBlurPower.addKeyFrame( 0, power );

            mBlarTime = -1.0;
            mBlurPower.setTime( 0.0 );
        }
        // �����J�ڂ���ꍇ
        else
        {
            mBlurPower.addKeyFrame( 0, power );
            mBlurPower.addKeyFrame( static_cast< lnfloat >( duration ), 0 );

            mBlarTime = 0.0;
            mBlurPower.setTime( 0.0 );
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void ScreenEffect::update()
    {
        const float elapsed_time = 1.0; //game_time_.getElapsedGameTime()

        // �F��
		mTone.advanceTime( elapsed_time );

        // �g�����W�V����
        if ( !mTransitionFactor.isFinished() )
        {
            mTransitionFactor.advanceTime( elapsed_time );
        }

        // �u���[
        if ( mBlarTime >= 0.0 && !mBlurPower.isFinished() )
        {
            mBlarTime += elapsed_time;
            mBlurPower.setTime( mBlarTime );
        }
        else if ( mBlarTime == -1.0 )
        {
            mBlurPower.setTime( 0.0 );
        }
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void ScreenEffect::updateDrawContent()
    {
        mDrawContent->updateDrawContent( this );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void ScreenEffect::swapBuffers()
    {
        if ( mSecondaryRenderTarget )
        {
            Graphics::ITexture* tmp = mPrimaryRenderTarget;
            mPrimaryRenderTarget = mSecondaryRenderTarget;
            mSecondaryRenderTarget = tmp;
        }

        if ( mSecondaryDepthBuffer )
        {
            Graphics::ITexture* tmp = mPrimaryDepthBuffer;
            mPrimaryDepthBuffer = mSecondaryDepthBuffer;
            mSecondaryDepthBuffer = tmp;
        }

        mFreezedAtFrame = false;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void ScreenEffect::renderBlurAndTransition( IRenderer* renderer, GeometryRenderer* geometry_renderer )
    {
        mDrawContent->renderForInternalTarget( renderer, geometry_renderer );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void ScreenEffect::renderScreen( IRenderer* renderer, GeometryRenderer* geometry_renderer )
    {
        mDrawContent->renderForScreen( renderer, geometry_renderer );
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================