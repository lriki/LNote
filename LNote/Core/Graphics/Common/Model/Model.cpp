//=============================================================================
//【 Model 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "ModelCore.h"
#include "ModelIK.h"
#include "ModelRigidBody.h"
#include "ModelConstraint.h"
#include "Model.h"

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
// ■ Model クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    Model::Model( ModelManager* manager_ )
        : mManager          ( manager_ )
        , mGraphicsDevice   ( manager_->getGraphicsDevice() )
        , mModelCore        ( NULL )
        , mAttrNum          ( 0 )
        , mMaterials        ( NULL )
        , mAttributes       ( NULL )
        , mFrameNum         ( 0 )
        , mModelFrames      ( NULL )
        , mRootFrame        ( NULL )
        , mRigidBodyNum     ( 0 )
        , mModelRigidBodys  ( NULL )
        , mConstraintNum    ( 0 )
        , mConstraints      ( NULL )
        , mMotionController ( NULL )
        , mRigidResetCount  ( 0 )
		, mEnablePhysics	( true )
    {
    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    Model::~Model()
    {
        for ( lnU32 i = 0; i < mConstraintNum; ++i )
        {
            LN_SAFE_RELEASE( mConstraints[ i ] );
        }
        SAFE_DELETE_ARRAY( mConstraints );

        SAFE_DELETE_ARRAY( mMaterials );
        SAFE_DELETE_ARRAY( mAttributes );
        SAFE_DELETE_ARRAY( mModelFrames );
        SAFE_DELETE_ARRAY( mModelRigidBodys );

        
        LN_SAFE_RELEASE( mModelCore );

        
 
        SAFE_DELETE( mMotionController );
        
    }

    //---------------------------------------------------------------------
	// ● フレームを検索する
    //---------------------------------------------------------------------
    ModelFrame* Model::findFrame( const lnChar* name_ )
    {
        lnU32 idx = mModelCore->getFrameIndexByName( name_ );
        if ( idx == 0xffffffff ) return NULL;
        return &mModelFrames[ idx ];
    }

	//---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
	void Model::setEnablePhysics( bool flag )
	{
		mEnablePhysics = flag;
		//updateSkinningMatrix();
	}

    //---------------------------------------------------------------------
	// ● すべての剛体を強制的にボーンの位置に移動させる
    //---------------------------------------------------------------------
    void Model::resetRigidBody()
    {
        mRigidResetCount = 3;
    }

    //---------------------------------------------------------------------
	// ● すべてのフレーム行列を階層的に更新する
    //---------------------------------------------------------------------
    void Model::updateFrameMatrix(bool ik_)
    {
        if ( mRootFrame )
        {
            mRootFrame->updateTransform( LMatrix() );
        }

        // 親のないフレームの更新
        const NoParentFrameIndexArray& ary = mNoParentFrameIndexArray;
        if ( !ary.empty() && ik_ )
        {
            NoParentFrameIndexArray::const_iterator itr = ary.begin();
            NoParentFrameIndexArray::const_iterator end = ary.end();
            for ( ; itr != end; ++itr )
            {
                mModelFrames[ (*itr) ].updateTransform( LMatrix() );
            }
        }


        if ( mRigidResetCount > 0 )
        {
            for ( lnU32 i = 0; i < mRigidBodyNum; ++i )
	        {
		        mModelRigidBodys[ i ].moveToBonePos();
	        }
            --mRigidResetCount;
        }
    }
    
    //---------------------------------------------------------------------
	// ● 
    //---------------------------------------------------------------------
    void Model::postUpdate( lnfloat elapsed_time_ )
    {
        // ボーン位置合わせ (剛体を移動させる)
        //      ボーンのワールド行列が必要。
        //      この関数の前に ModelFrame::mWorldMatrix が確定している必要がある。
        for ( lnU32 i = 0 ; i < mRigidBodyNum ; ++i )
	    {
		    mModelRigidBodys[ i ].fixPosition( elapsed_time_ ); // もともとは elapsed_frame_
	    }

        // IK 更新
        //      この中では ModelFrame::mRotation を書き換え、
        //      ModelFrame::updateMatrixInverse() を呼んで mWorldMatrix を更新している。
        for ( lnU32 i = 0 ; i < mModelCore->mIKNum ; ++i )
	    {
		    mModelCore->mIKArray[ i ].update( mModelFrames ); // mModelFramesは仮
	    }
    }

    //---------------------------------------------------------------------
	// ● スキニング行列の更新
    //---------------------------------------------------------------------
    void Model::updateSkinningMatrix()
    {
		if ( mEnablePhysics )
		{
			// 物理演算反映 (ボーンの位置を剛体に合わせる)
			for ( lnU32 i = 0; i < mRigidBodyNum; ++i )
  			{
				mModelRigidBodys[ i ].updateBoneTransform();
			}
		}

        // スキニング用行列の更新
	    for ( lnU32 i = 0; i < mFrameNum; ++i )
	    {
		    mModelFrames[ i ].updateSkinningMatrix();
	    }
    }

    //---------------------------------------------------------------------
	// ● 剛体の位置を現在のボーンの位置にあわせる
    //---------------------------------------------------------------------
    //void Model::resetRigidBodyPos()
    //{
	   // for ( lnU32 i = 0; i < mRigidBodyNum; ++i )
	   // {
		  //  mModelRigidBodys[ i ].moveToBonePos();
	   // }
    //}

    //---------------------------------------------------------------------
	// ● ソフトウェアスキニング
    //---------------------------------------------------------------------
    void Model::skinning()
    {
        if ( mModelFrames )
        {
            mModelCore->skinning( mModelFrames );
        }
    }

    //---------------------------------------------------------------------
	// ● サブセットの描画
    //---------------------------------------------------------------------
    LNRESULT Model::drawSubset( lnU32 idx_ )
    {
        IRenderer* r = mGraphicsDevice->getRenderer();
        r->setVertexBuffer( mModelCore->mVertexBuffer );
        r->setIndexBuffer( mModelCore->mIndexBuffer );
        r->drawPrimitiveIndexed(
            LN_PRIM_TRIANGLELIST, mAttributes[ idx_ ].StartIndex, mAttributes[ idx_ ].PrimitiveNum );
        return LN_OK;
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