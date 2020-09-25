//=============================================================================
//【 Effect 】
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
	レ リボンの中央を原点とする


	乱数値を持つ要素

		共通
			位置 x,y,z
			回転 x,y,z,r	(r はオーダーが軸+角度の場合)
			拡縮 x,y,z
			色   r,g,b,a
			UV   u,v        （あと、ピクセル単位の指定か、0.0-1.0の指定か）

			各要素ごとに
				物理    初速度,加速度
				3次補間 左辺速度,右辺速度

		リング
			外径 d	(直径)
			内径 d
			中心径比 r (0.0～1.0)
	
		リボン
			(幅 w (原点は「リボンの中央を原点とする」によって変化。両端に広くなるか、片方か))



	シェーダの使い方
		Core::begin() で固定シェーダにパラメータ設定って形になると思う。
*/



//=============================================================================
// ■ Effect クラス
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

        // ルートノードはここで最初のグループとインスタンスを作る
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