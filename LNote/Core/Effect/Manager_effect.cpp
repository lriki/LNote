//=============================================================================
//�y Manager �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Base/STLUtil.h"
#include "Core/EffectCore.h"
#include "Manager.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Effect
{

//=============================================================================
// �� Manager �N���X
//=============================================================================
    
    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    Manager::Manager()
        : mLogFile          ( NULL )
        , mFileManager      ( NULL )
        , mGraphicsManager  ( NULL )
        , mSprite3DRenderer ( NULL )
        , mSprite2DRenderer ( NULL )
        //, mASyncBarrier     ( NULL )
        //, mEndRequested     ( NULL )
        , mTimeTick         ( 60.0f )
    {
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    Manager::~Manager()
    {

    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Manager::initialize( const InitData& init_data_ )
    {
        mFileManager        = init_data_.FileManager;
        mGraphicsManager    = init_data_.GraphicsManager;

        mSprite3DRenderer   = LN_NEW Graphics::SpriteRenderer( mGraphicsManager );
        mSprite3DRenderer->initialize( mGraphicsManager->getGraphicsDevice(), init_data_.MaxSpriteNum, true );

        mSprite2DRenderer   = LN_NEW Graphics::SpriteRenderer( mGraphicsManager );
        mSprite2DRenderer->initialize( mGraphicsManager->getGraphicsDevice(), init_data_.MaxSpriteNum, false );

        mRingRenderer       = LN_NEW Graphics::RingRenderer( mGraphicsManager );
        mRingRenderer->initialize( 4096, true );

        EffectCoreCache::initialize( init_data_.EffectCacheSize );

        //mASyncUpdating = LN_NEW Thread::EventFlag( false );
        //mEndRequested = LN_NEW Thread::EventFlag( false );
        //mASyncBarrier = LN_NEW Thread::Barrier( 2 );
        //mUpdateThread = LN_NEW UpdateThread( this );
        //mUpdateThread->start();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Manager::finalize()
    {
        //if ( mUpdateThread )
        //{
        //    mEndRequested->setTrue();               // �I���t���O�𗧂Ă�
        //    mASyncBarrier->setNotifyFlag( true );   // �o���A�������s��Ȃ��悤�ɂ���
        //    mASyncUpdating->setTrue();              // �X�V�X���b�h�𓮂�����
        //    mUpdateThread->wait();                  // �X�V�X���b�h�̏I����҂�

        //    SAFE_DELETE( mUpdateThread );
        //    LN_SAFE_RELEASE( mASyncBarrier );
        //    LN_SAFE_RELEASE( mEndRequested );
        //    LN_SAFE_RELEASE( mASyncUpdating );
        //}

        //ln_foreach( EffectInstance* e, mDrawingInstanceArray )
        //{
        //    e->release();
        //}
        //mDrawingInstanceArray.clear();

        ln_foreach( EffectInstance* e, mEffectInstanceArray )
        {
            e->release();
        }
        mEffectInstanceArray.clear();

        ln_foreach( EffectInstance* e, mAddedEffectInstanceList )
        {
            e->release();
        }
        mAddedEffectInstanceList.clear();

        EffectCoreCache::finalize();
        LN_SAFE_RELEASE( mSprite3DRenderer );
        LN_SAFE_RELEASE( mSprite2DRenderer );
        LN_SAFE_RELEASE( mRingRenderer );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Manager::setViewProjection( const LMatrix& view_, const LMatrix& proj_, bool is_3d_pahse_ )
    {
        if ( is_3d_pahse_ )
        {
            mSprite3DRenderer->setViewProjMatrix( view_, proj_ );
            mRingRenderer->setViewProjMatrix( view_, proj_ );
        }
        else
        {
            mSprite2DRenderer->setViewProjMatrix( view_, proj_ );
        }
    }

 //   //---------------------------------------------------------------------
	////
 //   //---------------------------------------------------------------------
 //   Effect* Manager::createEffect()
 //   {
 //       Effect* ef = LN_NEW Effect( this );
 //       ef->initialize( createEmptyEffectCore() );
 //       return ef;
 //   }

 //   //---------------------------------------------------------------------
	////
 //   //---------------------------------------------------------------------
 //   EffectCore* Manager::createEmptyEffectCore()
 //   {
 //       EffectCore* core = LN_NEW EffectCore( this );
 //       core->initialize();
 //       return core;
 //   }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    //void Manager::advanceTime( lnTime delta_time_ )
    //{
    //    lnTime delta_frame = delta_time_ * mTimeTick;

    //    ln_foreach( EffectInstance* e, mEffectInstanceArray )
    //    {
    //        if ( e->isEnableAutoUpdate() )
    //            e->advanceFrame( delta_frame );
    //    }
    //    ln_foreach( EffectInstance* e, mAddedEffectInstanceList )
    //    {
    //        if ( e->isEnableAutoUpdate() )
    //            e->advanceFrame( delta_frame );
    //    }
    //}

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Manager::setGameTime( const Game::GameTime& game_time_ )
    {
        mGameTime = game_time_;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Manager::update()
    {
        lnTime delta_frame = mGameTime.getElapsedGameTime() * mTimeTick;

        ln_foreach( EffectInstance* e, mEffectInstanceArray )
        {
            if ( e->isEnableAutoUpdate() )
            {
                e->advanceFrame( delta_frame );
                e->update();
            }
        }
        //ln_foreach( EffectInstance* e, mAddedEffectInstanceList )
        //{
        //    if ( e->isEnableAutoUpdate() )
        //    {
        //        e->advanceFrame( delta_frame );
        //        e->update();
        //    }
        //}


        //if ( mUpdateThread )
        //{
        //    // �X�V�X���b�h������ wait() �𓥂ނ܂ő҂�
        //    mASyncBarrier->wait();

        //    // ���X�g�X�V
        //    ln_foreach( EffectInstance* e, mAddedEffectInstanceList )
        //    {
        //        mEffectInstanceArray.push_back( e );
        //    }
        //    mAddedEffectInstanceList.clear();

        //    // �X�V�X���b�h�J�n
        //    mASyncUpdating->setTrue();
        //}
        //else

        //{
        //    _update();
        //}
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Manager::updateDrawContents()
    {
        // release
        //ln_foreach( EffectInstance* e, mDrawingInstanceArray )
        //{
        //    e->release();
        //}
        //mDrawingInstanceArray.clear();
        
        // �ǉ����X�g����̈ړ�
        ln_foreach( EffectInstance* e, mAddedEffectInstanceList )
        {
            mEffectInstanceArray.push_back( e );
        }
        mAddedEffectInstanceList.clear();

        // Manager ���炵���Q�Ƃ���Ă��Ȃ��ꍇ�͍폜
        EffectInstanceArray::iterator itr = mEffectInstanceArray.begin();
        while ( itr != mEffectInstanceArray.end() )
        {
            if ( (*itr)->getRefCount() == 1 )
            {
                (*itr)->release();
                itr = mEffectInstanceArray.erase( itr );
            }
            else
            {
                ++itr;
            }
        }

        // �`�揀��
        ln_foreach( EffectInstance* e, mEffectInstanceArray )
        {
            e->updateDrawContents();
            //mDrawingInstanceArray.push_back( e );
        }

        // addref
        //ln_foreach( EffectInstance* e, mDrawingInstanceArray )
        //{
        //    e->addRef();
        //}
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Manager::drawBatch3D()
    {
        LRenderState s;
        s.BlendMode = LN_BLEND_ADD;
        s.Culling = LN_CULL_DOUBLE;
        s.AlphaTest = false;
        s.DepthTest = true;
        s.DepthWrite = false;
        mGraphicsManager->getGraphicsDevice()->getRenderer()->setRenderState( s );

        mSprite3DRenderer->flash();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Manager::drawBatch2D()
    {
        LRenderState s;
        s.BlendMode = LN_BLEND_ADD;
        s.Culling = LN_CULL_DOUBLE;
        s.AlphaTest = false;
        s.DepthTest = false;
        s.DepthWrite = false;
        mGraphicsManager->getGraphicsDevice()->getRenderer()->setRenderState( s );

        mSprite2DRenderer->flash();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Manager::addEffectInstance( EffectInstance* effect_ )
    {
        mAddedEffectInstanceList.push_back( effect_ );
        effect_->addRef();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Manager::removeEffectInstance( EffectInstance* effect_ )
    {
        //Base::STLUtil::remove( mEffectInstanceArray, effect_ );
        //mAddedEffectInstanceList.remove( effect_ );
        //effect_->release();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    //void Manager::setAllModifiedFlag( bool flag_ )
    //{
    //    
    //}

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    //void Manager::_update()
    //{
    //    ln_foreach( EffectInstance* e, mEffectInstanceArray )
    //    {
    //        e->update();
    //    }
    //    
    //}

//=============================================================================
// �� Manager �N���X
//=============================================================================

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    //void Manager::UpdateThread::operator()()
    //{
    //    while ( true )
    //    {
    //        mManager->mASyncUpdating->setFalse();
    //        mManager->mASyncBarrier->wait();
    //        mManager->mASyncUpdating->wait();   // Manager ����t���O�� true �ɂȂ�܂őҋ@

    //        if ( mManager->mEndRequested->isTrue() ) return;

    //        mManager->_update();
    //    }
    //}


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Effect
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================