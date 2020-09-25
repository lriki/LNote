//=============================================================================
//【 Model 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "ModelCore.h"
#include "ModelRigidBody.h"
#include "ModelConstraint.h"
#include "Model.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace Lumine
{
namespace Graphics
{

//=============================================================================
// ■ Model クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    Model::Model()
        : mModelCore        ( NULL )
        , mConstraintNum    ( 0 )
        , mConstraints      ( NULL )
        , mMotionController ( NULL )
        , mRigidResetCount  ( 0 )
    {
    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    Model::~Model()
    {
        SAFE_DELETE_ARRAY( mConstraints );
        LN_SAFE_RELEASE( mModelCore );

        SAFE_DELETE( mMotionController );
        
    }

    //---------------------------------------------------------------------
	// ● フレームを検索する
    //---------------------------------------------------------------------
    ModelFrame* Model::findFrame( const lnChar* name_ )
    {
        lnU32 idx = mModelCore->getFrameIndexByName( name_ );
        if ( idx == 0xffffffff ) { 
	return NULL; }
        return &mModelFrames[ idx ];
    }

    //---------------------------------------------------------------------
	// ● すべてのフレーム行列を階層的に更新する
    //---------------------------------------------------------------------
    void Model::updateFrameMatrix()
    {
        mRootFrame->updateTransform( LMatrix() );

        // 親のないフレームの更新
        const NoParentFrameIndexArray& ary = mModelCore->getNoParentFrameIndexArray();
        if ( !ary.empty() )
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
	// ● すべての剛体を強制的にボーンの位置に移動させる
    //---------------------------------------------------------------------
    void Model::resetRigidBody()
    {
        mRigidResetCount = 3;
        
    }

    //---------------------------------------------------------------------
	// ● 
    //---------------------------------------------------------------------
    void Model::postUpdate()
    {
        // ボーン位置合わせ
        //      ボーンのワールド行列が必要。
        //      この関数の前に ModelFrame::mWorldMatrix が確定している必要がある。

        // IK 更新
        //      この中では ModelFrame::mRotation を書き換え、
        //      ModelFrame::updateMatrixInverse() を呼んで mWorldMatrix を更新している。
    }

    //---------------------------------------------------------------------
	// ● スキニング行列の更新
    //---------------------------------------------------------------------
    void Model::updateSkinningMatrix()
    {
        // 物理演算反映
	    for ( lnU32 i = 0; i < mRigidBodyNum; ++i )
  	    {
		    mModelRigidBodys[ i ].updateBoneTransform();
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
        mModelCore->skinning( mModelFrames );
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Lumine

//=============================================================================
//								end of file
//=============================================================================