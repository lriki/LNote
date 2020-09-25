//=============================================================================
//【 Model 】
//-----------------------------------------------------------------------------
///**
//  @file       Model.h
//  @brief      Model
//  @author     Riki
//*/
//=============================================================================

#pragma once

/*
[作業メモ 12/7/31]
    Model～ 系のクラスはほぼ完成。
    モデルの共有についてはチェックが必要。

    アニメーションはとりあえず再生できるってだけの状態。

*/

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../../Geometry/Vector4.h"
#include "ModelFrame.h"
#include "MotionController.h"
#include "ModelCore.h"

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
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class Model
    : public Base::ReferenceObject
{
public:
    Model( ModelManager* manager_ );
    LN_REFOBJ_METHODS;

protected:
    virtual ~Model();

public:

    /// スキンメッシュかを判定
    bool isSkinMesh() const { return mModelCore->isSkinMesh(); }

    /// フレーム数の取得
    lnU32 getFrameNum() const { return mFrameNum; }

    /// インデックスを指定してフレームを取得する
    ModelFrame* getFrame( lnU32 index_ ) { return &mModelFrames[ index_ ]; }

    /// フレームを検索する
    ModelFrame* findFrame( const lnChar* name_ );

    /// マテリアルの取得
    LMaterial* getMaterial( lnU32 idx_ ) { return &mMaterials[ idx_ ]; }

    /// サブセット数の取得 (属性数)
    lnU32 getSubsetNum() const { return mAttrNum; }

    /// 属性の取得
    LNMeshAttribute* getAttribute( lnU32 idx_ ) { return &mAttributes[ idx_ ]; }

    /// モーションコントローラの取得
    MotionController* getMotionController() { return mMotionController; }

    /// コリジョンメッシュの取得
    const CollisionMesh& getCollisionMesh() { return mModelCore->getCollisionMesh(); }

	/// 物理演算の有効設定 (設定後、次の updateSkinningMatrix() から有効)
	void setEnablePhysics( bool flag );

	/// 物理演算の有効判定
	bool isEnablePhysics() const { return mEnablePhysics; }


    /// すべての剛体を強制的にボーンの位置に移動させる
    void resetRigidBody();


    /// すべてのフレーム行列を階層的に更新する
    void updateFrameMatrix(bool ik_ = true);

    /// 剛体の位置修正、IK 更新
    void postUpdate( lnfloat elapsed_time_ );

    /// スキニング行列の更新
    void updateSkinningMatrix();

    // アニメーション更新
    // updateFrameMatrix()
    // 任意のボーン更新
    // postUpdate()
    // 物理更新
    // updateSkinningMatrix()
    // 描画
    //      の順で処理を行う

    /// ソフトウェアスキニング
    void skinning();


    /// サブセットの描画
    LNRESULT drawSubset( lnU32 idx_ );


private:

    typedef std::vector< lnU32 > NoParentFrameIndexArray;

private:

    ModelManager*           mManager;
    IGraphicsDevice*        mGraphicsDevice;    ///< これよりも Renderer?
    ModelCore*              mModelCore;         ///< (ModelCore から代入され、その時参照カウント+1)

    lnU32                   mAttrNum;
    LMaterial*              mMaterials;
    LNMeshAttribute*        mAttributes;

    lnU32                   mFrameNum;
    ModelFrame*             mModelFrames;
    ModelFrame*             mRootFrame;     ///< (ModelCore から代入される)
    NoParentFrameIndexArray mNoParentFrameIndexArray;   ///< ルートフレーム以外の親のないフレームのインデックスリスト

    lnU32                   mRigidBodyNum;
    ModelRigidBody*         mModelRigidBodys;

    lnU32                   mConstraintNum;
    ModelConstraint**       mConstraints;

    MotionController*       mMotionController;

    lnU8                    mRigidResetCount;
	bool					mEnablePhysics;

    friend class ModelCore;
};

#if 0

/// メッシュの属性値
struct LNMeshAttributeRange
{
    u32     AttribID;           ///< 属性テーブルの識別子
    u32     StartIndex;         ///< 開始インデックス
    u32     PrimitiveNum;       ///< 描画プリミティブ数
};


struct LNBoneCombination
{
    lnU32   MaterialAttribId;   ///< mMaterials、mTextures の番号 (HLSL スキニングではサブセット番号ではないので注意！)
    lnU32*  BoneIds;            ///< 最大 26 くらいの。UINT_MAX は必要ないことを表す。
};

class Model
{

private:

    Graphics::Material*     mMaterials;       ///< 
    Graphics::ITexture**    mTextures;

    lnU32                   mBoneNum;
    LMatrix*                mBoneMatrices;
};

class ModelCore
{
private:

    std::map< ln_std_tstring, lnU32 >   mFrameNameMap;  ///< 
};

// DirectX のメッシュコンテナにあたる。
class ModelMesh
{
private:

    Graphics::IVertexBuffer*    mVertexBuffer;
    Graphics::IIndexBuffer*     mIndexBuffer;

    lnU32                       mAttrGroupNum;          ///< (HLSL スキニングでは、描画サブセット番号も兼ねる)
    LNMeshAttributeRange*       mMeshAttrs;             ///< メッシュサブセット
    LNBoneCombination*          mBoneCombinations;

    lnU32                       mAttrNum;
    Graphics::Material*         mOrginalMaterials;       ///< オリジナルのマテリアル (共有モデルを作成するとき、コピーして使う)
    Graphics::ITexture**        mOrginalTextures;

    lnU32                       mBoneNum;
    LMatrix*                    mOrginalBoneMatrices;
};

class ModelFrame
{
private:

    
};




/// モーションひとつ分 (MMD なら VMD ファイルひとつ分)
class Motion
{
private:

    // ボーン対象のキーフレームデータ
    struct KeyFrame
    {
	    lnU32	    FrameNo;		// フレーム番号

	    LVector3	Position;	    // 位置
	    LQuaternion	Rotation;	    // 回転(クォータニオン)

	    cVMDBezier	clPosXInterBezier;	// X軸移動補間
	    cVMDBezier	clPosYInterBezier;	// Y軸移動補間
	    cVMDBezier	clPosZInterBezier;	// Z軸移動補間
	    cVMDBezier	clRotInterBezier;	// 回転補間
    };

    // ボーンごとのキーフレームデータのリスト
    struct MotionDataList
    {
	    char	szBoneName[16];			// ボーン名

	    unsigned long	ulNumKeyFrames;	// キーフレーム数
	    BoneKeyFrame	*pKeyFrames;	// キーフレームデータ配列

	    MotionDataList	*pNext;
    };
};


class MotionController
{
public:

    /// モーションを検索する
    MotionTrack* findMotionTrack( const lnChar* name_ );

private:
};


// ボーン、カメラ?などのひとつ分のモーション
class MotionTrack
{
public:

    /// ブレンド済み行列の取得
    const LMatrix& getTransformMatrix();

private:
};
#endif


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================