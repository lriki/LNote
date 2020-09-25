//=============================================================================
//【 ModelCore 】
//-----------------------------------------------------------------------------
///**
//  @file       ModelCore.h
//  @brief      ModelCore
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <map>
#include "../../../Base/RefString.h"
#include "../../../Math/Vector2.h"
#include "../../../Math/Vector3.h"
#include "../../../Math/Vector4.h"
#include "../../Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace Lumine
{
namespace Graphics
{



class Model;
class ModelManager;
class ModelFrame;
class ModelBone;
class ModelFace;
class ModelIK;
class ModelRigidBody;
class ModelConstraint;

typedef std::vector< lnU32 > NoParentFrameIndexArray;

//=============================================================================
// ■ ModelCore クラス
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class ModelCore
    : public Base::ReferenceObject
{
public:
    ModelCore( ModelManager* manager_ );
    LN_REFOBJ_METHODS;

protected:
    virtual ~ModelCore();

public:

    /// PMD モデル読み込み (新しく作成して model_ にポインタを返す)
    LNRESULT loadPMD( const void* data_, lnU32 size_, bool sw_skinning_, Model** model_ );

    /// マテリアル配列の取得
    LMaterial* getMaterials() { return mMaterials; };

    /// 属性グループ数の取得 (サブセット数)
    lnU32 getAttrGroupNum() const { return mAttrNum; }  // 今はテスト用ってことで mAttrNum で。

    LNMeshAttributeRange* getAttributes() { return mAttributeRanges; }

    /// サブセットの描画
    LNRESULT drawSubset( lnU32 idx_ );

    /// フレーム配列の取得 (ModelFrames から使う)
    ModelFrame* getModelFrames() { return mModelFrames; }
    //ModelFrame* getModelFrame( lnU32 idx_ ) { return &mModelFrames[ idx_ ]; } // サイズが不明ですと言われるのでとりあえず

    /// 剛体の位置修正、IK 更新 (モーション更新後に呼ぶ)
    void correction( lnfloat elapsed_time_ );



    /// 名前からフレームを取得する (Model クラスに持たせるべき)
    //ModelFrame* getFrameByName( const lnChar* name_ );

    /// 名前からフレーム番号を取得する (見つからない場合は 0xffffffff)
    lnU32 getFrameIndexByName( const lnChar* name_ );

    /// ソフトウェアスキニング
    void skinning( ModelFrame* frames_ );

    const NoParentFrameIndexArray& getNoParentFrameIndexArray() const { return mNoParentFrameIndexArray; }

private:

    /// モデルデータ解放
    void _release();

    /// スフィアマップ用のテクスチャ名かを判定する
    bool _isSphereMapTexName( const char* tex_name_ ) const;

private:

    typedef std::map< LRefTString, lnU32 >   FrameIndexMap;
    typedef std::pair< LRefTString, lnU32 >  FrameIndexPair;
    

private:

    ModelManager*           mManager;
    IGraphicsDevice*        mGraphicsDevice;
    LRefTString             mModelName;

    IVertexBuffer*          mVertexBuffer;
    IIndexBuffer*           mIndexBuffer;
    LVector3*               mOrgVertexPositions;    ///< (ソフトウェアスキニング用)
    LVector3*               mOrgVertexNormals;      ///< (ソフトウェアスキニング用)
    

    lnU32                   mAttrNum;
    LMaterial*              mMaterials;
    LNMeshAttributeRange*   mAttributeRanges;

    lnU32                   mBoneNum;
    ModelFrame*             mModelFrames;   // Model へ
    FrameIndexMap           mFrameIndexMap; ///< (これは ModelCore に持たせておいてOK)

    lnU32                   mIKNum;
    ModelIK*                mIKArray;

    lnU32                   mFaceNum;
    ModelFace*              mModelFaces;

    lnU32                   mRigidBodyNum;
    ModelRigidBody*         mModelRigidBodys;

    NoParentFrameIndexArray mNoParentFrameIndexArray;   ///< ルートフレーム以外の親のないフレームのインデックスリスト

};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Lumine

//=============================================================================
//								end of file
//=============================================================================