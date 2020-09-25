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
#include "../../../Geometry/Vector2.h"
#include "../../../Geometry/Vector3.h"
#include "../../../Geometry/Vector4.h"
#include "../../Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{



class Model;
class ModelManager;
class ModelFrame;
class ModelBone;
class ModelFace;
class ModelIK;
class ModelIKLimitter;
class ModelRigidBody;
class ModelConstraint;


struct CollisionMesh
{
    lnU32       VertexCount;
    LVector3*   CollisionVertexBuffer;
    lnU32       IndexCount;
    lnU32       IndexStride;            ///< 2 or 4
    void*       CollisionIndexBuffer;
};



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

    /// スキンメッシュかを判定
    bool isSkinMesh() const { return mIsSkinMesh; }

    /// PMD モデル読み込み (新しく作成して model_ にポインタを返す)
    /// flags_ は LNModelFlag の組み合わせ
    LNRESULT loadPMD( const void* data_, lnU32 size_, lnU32 flags_, Model** model_ );

    /// X ファイル読み込み
    void loadXFile( const void* data_top_, lnU32 size_, lnU32 flags_, Model** model_ );

    /// 名前からフレーム番号を取得する (見つからない場合は LN_INVALID_INDEX)
    lnU32 getFrameIndexByName( const lnChar* name_ );

    /// ソフトウェアスキニング
    void skinning( ModelFrame* frames_ );

    /// IK リミッタの取得
    ModelIKLimitter* getIKLimitter() { return mIKLimitter; }

    /// コリジョンメッシュの取得
    const CollisionMesh& getCollisionMesh() { return *mCollisionMesh; }

private:

    /// モデルデータ解放
    void _release();

    /// スフィアマップ用のテクスチャ名かを判定する
    bool _isSphereMapTexName( const char* tex_name_ ) const;

    void _createCollisionBuffers();

private:

    typedef std::map< LRefTString, lnU32 >   FrameIndexMap;
    typedef std::pair< LRefTString, lnU32 >  FrameIndexPair;
    

private:

    ModelManager*           mManager;
    IGraphicsDevice*        mGraphicsDevice;    // 使うの load でだけだからメンバに持たなくていいかも
    LRefTString             mModelName;

    // 共有する
    IVertexBuffer*          mVertexBuffer;
    IIndexBuffer*           mIndexBuffer;
    LVector3*               mOrgVertexPositions;    ///< (ソフトウェアスキニング用)
    LVector3*               mOrgVertexNormals;      ///< (ソフトウェアスキニング用)
    CollisionMesh*          mCollisionMesh;
    

    FrameIndexMap           mFrameIndexMap;     ///< フレーム名に対応するフレーム番号表

    // 共有する
    lnU32                   mIKNum;
    ModelIK*                mIKArray;
    ModelIKLimitter*        mIKLimitter;

    // 共有する
    lnU32                   mFaceNum;
    ModelFace*              mModelFaces;

    bool                    mIsSkinMesh;

    friend class Model;
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