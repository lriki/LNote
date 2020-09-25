//=============================================================================
//�y Effect �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Manager.h"
#include "../Core/EffectCore.h"
#include "EffectInstance.h"
#include "EffectNodeInstance.h"
#include "EffectNodeInstanceContainer.h"
#include "EffectNodeInstanceGroup.h"
#include "../Emitter/EmitterBase.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Effect
{

struct RandomizeValue
{
    lnfloat             Value;

    lnU8                RandType;   ///< LNExRandType
    lnU8                RateType;   ///< LNExRandRateType

    union
    {
        struct
        {
            lnfloat             Max;
            lnfloat             Min;
        } MinMax;

        struct
        {
            lnfloat             Center;
            lnfloat             Range;
        } Range;
    };
};

struct Vector3Key
{

};

/*
	�� ���{���̒��������_�Ƃ���


	�����l�����v�f

		����
			�ʒu x,y,z
			��] x,y,z,r	(r �̓I�[�_�[����+�p�x�̏ꍇ)
			�g�k x,y,z
			�F   r,g,b,a
			UV   u,v        �i���ƁA�s�N�Z���P�ʂ̎w�肩�A0.0-1.0�̎w�肩�j

			�e�v�f���Ƃ�
				����    �����x,�����x
				3����� ���ӑ��x,�E�ӑ��x

		�����O
			�O�a d	(���a)
			���a d
			���S�a�� r (0.0�`1.0)
	
		���{��
			(�� w (���_�́u���{���̒��������_�Ƃ���v�ɂ���ĕω��B���[�ɍL���Ȃ邩�A�Е���))



	�V�F�[�_�̎g����
		Core::begin() �ŌŒ�V�F�[�_�Ƀp�����[�^�ݒ���Č`�ɂȂ�Ǝv���B
*/



//=============================================================================
// �� Effect �N���X
//=============================================================================

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    EffectInstance::EffectInstance( Manager* manager_ )
        : mManager              ( manager_ )
        , mEffectCore           ( NULL )
        , mCurrentFramePos      ( 0 )
        , mLastFramePos         ( 0 )
        , mEnableAutoUpdate     ( true )
    {
        LN_SAFE_ADDREF( mManager );
        mManager->addEffectInstance( this );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    EffectInstance::~EffectInstance()
    {
        LN_SAFE_RELEASE( mRootContainer );
        //if ( mManager )
        //{
        //    mManager->removeEffectInstance( this );
        //}
        LN_SAFE_RELEASE( mEffectCore );
        LN_SAFE_RELEASE( mManager );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectInstance::initialize( EffectCore* core_ )
    {
        mEffectCore = core_;
        LN_SAFE_ADDREF( mEffectCore );

        mRootContainer = LN_NEW EffectNodeInstanceContainer( this->mManager );
        mRootContainer->initialize( mEffectCore->getRootNode(), this );

        // ���[�g�m�[�h�͂����ōŏ��̃O���[�v�ƃC���X�^���X�����
        EffectNodeInstanceGroup* group = mRootContainer->createNodeInstanceGroup();
        EffectNodeInstance* instance = group->createNodeInstance();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectInstance::setFrame( lnTime frame_pos_ )
    {
        mCurrentFramePos = frame_pos_;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectInstance::advanceFrame( lnTime delta_frame_ )
    {
        mCurrentFramePos += delta_frame_;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectInstance::update()
    {
        mRootContainer->update( mCurrentFramePos - mLastFramePos );
        mLastFramePos = mCurrentFramePos;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectInstance::updateDrawContents()
    {
        mRootContainer->updateDrawContents();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectInstance::draw()
    {
        mRootContainer->draw();
    }

#if 0
    

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectInstance::draw()
    {
        ln_foreach( EffectNodeInstance* n, mEffectNodeDrawList )
        {
            if( n->getEmitterInstance() )
            {
                n->getEmitterInstance()->draw( n->getWorldMatrix() );
            }
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectInstance::update()
    {
        mRootGroup->update( mCurrentGrobalFramePos, mTransformMatrix );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectInstance::updateDrawContents()
    {
        //mEffectNodeDrawList.clear();
        mRootGroup->updateDrawContents();//( &mEffectNodeDrawList );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void EffectInstance::refresh()
    {
        mRootGroup->refresh();
    }
#endif
    

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Effect
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================