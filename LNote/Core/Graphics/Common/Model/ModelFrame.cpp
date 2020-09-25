//=============================================================================
//【 ModelFrame 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "ModelCore.h"
#include "ModelFrame.h"

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
// ■ ModelFrame クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    ModelFrame::ModelFrame()
        : mModelCore        ( NULL )
        , mIndex            ( 0 )
        , mParent           ( NULL )
        , mIKLimitAngle     ( false )
    {
    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    ModelFrame::~ModelFrame()
    {
    }

    //---------------------------------------------------------------------
	// ● ワールド行列を階層的に更新する
    //---------------------------------------------------------------------
    void ModelFrame::updateTransform( const LMatrix& parent_mat_ )
    {
        // ローカルな変換にワールドを適用
	    mWorldMatrix = mLocalTransform;
        mWorldMatrix.translation( mOffset );
        mWorldMatrix *= parent_mat_;

	    // 子供の計算
#if 1
        ModelFrameList::iterator itr = mChildList.begin();
	    ModelFrameList::iterator end = mChildList.end();
	    for ( ; itr != end; ++itr )
        {
            (*itr)->updateTransform( mWorldMatrix );
        }
#else
	    //ModelFrameList::iterator itr = mChildList.begin();
	    //ModelFrameList::iterator end = mChildList.end();
	    //for ( ; itr != end; ++itr )
        int size = mChildList.size();
        if (size)
        {
            ModelFrame** fs = &mChildList[0];
            for (int i = 0; i < size; ++i, ++fs)
	        {
                (*fs)->updateTransform( mWorldMatrix );

		        //(*itr)->updateTransform( mWorldMatrix );
	        }
        }
#endif
    }

    //---------------------------------------------------------------------
	// ● フレームのワールド行列を設定する
    //---------------------------------------------------------------------
    void ModelFrame::setWorldMatrix( const LMatrix& matrix_, lnU32 update_depth_ )
    {
        mWorldMatrix = matrix_;

	    if ( update_depth_ > 0 )
	    {
		    --update_depth_;

		    // 子供の計算
		    ModelFrameList::iterator itr = mChildList.begin();
		    ModelFrameList::iterator end = mChildList.end();
		    for ( ; itr != end; ++itr )
		    {
                LMatrix mat = (*itr)->mLocalTransform;
                mat.translation( mOffset ); // TODO ここでいいのかチェックがいるかも
                mat *= mWorldMatrix;
			    (*itr)->setWorldMatrix( mat, update_depth_ );
		    }
	    }
    }

    //---------------------------------------------------------------------
	// ● スキニング用行列の更新
    //---------------------------------------------------------------------
    void ModelFrame::updateSkinningMatrix()
    {
        mSkinningMatrix =  mInvTransform;
        mSkinningMatrix *= mWorldMatrix;
    }



    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
    void ModelFrame::initializePMD( ModelCore* model_core_, const PMD_Bone* bone_data_, lnU32 index_, ModelFrame* frames_ )
    {
        mModelCore = model_core_;
        mIndex = index_;

        // ボーン名のコピー (ボーン名が 20 で、終端がなかった場合の対処)
        char name[ 21 ];
        strncpy( name, bone_data_->szName, 20 );
	    name[ 20 ] = '\0';
        mName = name;

	    // 位置のコピー
	    mOrgPosition = bone_data_->vec3Position;

        
	    // 親ボーンの設定
	    if ( bone_data_->nParentNo != -1 )
	    {
            mParent = &frames_[ bone_data_->nParentNo ];
            mOffset = mOrgPosition - mParent->mOrgPosition;
	    }
        // 親なし
	    else
	    {		    
		    mOffset = mOrgPosition;
	    }

	    // 子ボーンの設定
	    //if( pPMDBoneData->nChildNo != -1 )
	    //{
		   // m_pChildBone = (cPMDBone *)&(pBoneArray[pPMDBoneData->nChildNo]);
	    //}

        /*
            初期姿勢 = スキニングしなくても同じ姿勢。
            ボーンは最初からオフセットが入ってるけど、
            それをスキニングに適用すると姿勢が崩れてしまう。
            そのため、初期オフセットを打ち消す処理が必要。それがこの行列。

            いままで意識してなかったけど、ID3DXSkinInfo::GetBoneOffsetMatrix() で
            取得できる行列 (SkinnedMeshサンプルの D3DXMESHCONTAINER_DERIVED::pBoneOffsetMatrices) が
            これにあたるものっぽい。
            サンプルでも描画の直前に対象ボーン行列にこの行列を乗算している。
        */
        LMatrix::translation(
            &mInvTransform,
            -mOrgPosition.x,
            -mOrgPosition.y,
            -mOrgPosition.z );	 

	    mIKLimitAngle = false;

	    // 各変数の初期値を設定
	    reset();
    }

    //---------------------------------------------------------------------
	// ● PMD初期化後処理 (子リスト作成、オフセット再計算)
    //---------------------------------------------------------------------
    void ModelFrame::postInitializePMD()
    {
        if ( mParent )
        {
            mOffset = mOrgPosition - mParent->mOrgPosition;
            mParent->mChildList.push_back( this );
        }
    }

    //---------------------------------------------------------------------
	// ● 各変数の初期値を設定
    //---------------------------------------------------------------------
    void ModelFrame::reset()
    {
        mLocalTransform = LSQTTransform::IDENTITY;
        mLocalTransform.Translation = LVector3::ZERO;//mOffset;///mOrgPosition;//
        //mWorldMatrix = mLocalTransform;
        updateWorldMatrixFromLocalAndParent();

    }

    //---------------------------------------------------------------------
	// ● mLocalTransform と mParent->mWorldMatrix から mWorldMatrix を作る
    //---------------------------------------------------------------------
    void ModelFrame::updateWorldMatrixFromLocalAndParent()
    {
        mWorldMatrix = mLocalTransform;
        mWorldMatrix.translation( mOffset );
        if ( mParent ) mWorldMatrix *= mParent->mWorldMatrix;
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