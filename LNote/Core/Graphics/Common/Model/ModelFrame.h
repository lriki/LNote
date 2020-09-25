//=============================================================================
//【 ModelFrame 】
//-----------------------------------------------------------------------------
///**
//  @file       ModelFrame.h
//  @brief      ModelFrame
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include "../../../Base/RefString.h"
#include "../../../Geometry/SQTTransform.h"
#include "PMDTypes.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
class ModelCore;
class ModelIK;


//=============================================================================
// ■ ModelFrame クラス
//-----------------------------------------------------------------------------
///**
//  @brief
//
//  @note
//              PMD のボーンにあたる。どうしても名前で混乱しそうなら変えるかも。
//
//              ModelCore 共有時、コピーするべきデータとそうでないデータが混在していて
//              少しメモリ効率が悪いけど、わかりやすさっていうのも大事。
//              そんなわけで、フレーム配列ごと全部コピーする。
//*/
//=============================================================================
class ModelFrame
{
public:

    ModelFrame();

    virtual ~ModelFrame();

public:

    /// 親フレームを取得する
    ModelFrame* getParent() { return mParent; }

    /// ワールド行列を階層的に更新する (IK 等、親のないボーンはルートの updateTransform() を呼んでも更新されないので注意)
    void updateTransform( const LMatrix& parent_mat_ );

    /// フレームのワールド行列を設定する
    void setWorldMatrix( const LMatrix& matrix_, lnU32 update_depth_ );

    /// スキニング用行列の更新
    void updateSkinningMatrix();

public:

    // 以下は内部用

    /// 初期化 (frames_ は単純に親へのポインタを取得するのに使う。中身はできてなくてOK)
    void initializePMD( ModelCore* model_core_, const PMD_Bone* bone_data_, lnU32 index_, ModelFrame* frames_ );

    /// PMD初期化後処理 (子リスト作成、オフセット再計算)
    void postInitializePMD();

    /// 各変数の初期値を設定
    void reset();

    /// 名前の取得
    const LRefTString& getName() const { return mName; }

    /// IK 角度制限の有効設定 (ModelIK::initializePMD() から呼ばれる)
    void setIKLimitEnable( bool f_ ) { mIKLimitAngle = f_; }

    /// IK 角度制限の有効判定
    bool isIKLimit() const { return mIKLimitAngle; }


    LSQTTransform* getLocalTransformPtr() { return &mLocalTransform; }

    LMatrix* getWorldMatrixPtr() { return &mWorldMatrix; }


    const LMatrix& getWorldMatrix() const { return mWorldMatrix; }

    const LMatrix& getSkinningMatrix() const { return mSkinningMatrix; }

    /// mLocalTransform と mParent->mWorldMatrix から mWorldMatrix を作る
    void updateWorldMatrixFromLocalAndParent();

    lnU32 getIndex() const { return mIndex; }

private:

    typedef std::vector< ModelFrame* >  ModelFrameList;

private:

    ModelCore*          mModelCore;

    lnU32               mIndex;
    LRefTString         mName;
    ModelFrame*         mParent;    ///< 親
    ModelFrameList	    mChildList; ///< 子一覧

    LVector3            mOrgPosition;   ///< モデル原点からの位置
    LVector3		    mOffset;        ///< 親mOrgPosition - mOrgPosition (initializePMD、postInitializePMD で再計算)
    LMatrix			    mInvTransform;	///< 初期値のボーンを原点に移動させるような行列 (移動成分のみ)
    bool			    mIKLimitAngle;	///< IK時に角度制限をするかどうか

    // 本来は以下だけをコピーして使う
 //   LVector3			mPosition;          // ARTK_MMD ではモーションはこれと mRotation に書かれていた
	//LQuaternion			mRotation;          // ModelIK からも直接書き換えられる (ARTK_MMD ではワールドかは考慮してないみたい…)
    LSQTTransform       mLocalTransform;   // モーションを書き込むのはここがいいと思うけど…。Selene ではここに書き込んでる。
    LMatrix             mWorldMatrix;       ///< 結合済み行列 (setWorldMatrix() で更新されたワールド(グローバル)行列。ModelRigidBody::updateBoneTransform() からも書き換えられる)
    LMatrix             mSkinningMatrix;    ///< この行列でスキニングする (updateSkinningMatrix() で作成。mInvTransform * mWorldMatrix)

    friend class ModelIK;
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