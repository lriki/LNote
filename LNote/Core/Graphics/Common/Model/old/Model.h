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

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../../Math/Vector4.h"
#include "ModelFrame.h"
#include "MotionController.h"
#include "ModelCore.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace Lumine
{
namespace Graphics
{
/*
    ▼ ユーザー側 HowTo
        
    ■ 頭や腕を相手の方向に向ける
        
        ノードツリーにモーションを適用
            ↓
        head = model.getFrame("頭");
        mat = head.getWorldMatrix();
        - mat を相手に向ける -
        head.setWorldMatrix( mat, 0 );  // 第2引数は更新する深さ


    ▼ 実装側 HowTo

    ■ HLSL でスキニングできる形にする
        
        HLSL の最大ボーン数は 26 くらい。
        他にもレジストリ使うとしたらもっと減る。
        ID3DXSkinInfo::ConvertToIndexedBlendedMesh を使って変換。
        最大ボーン数もこの関数に渡すことで、メッシュが適切に変換される。

    ■ 複数メッシュコンテナの対応

        頂点フォーマットが違う場合に、複数のメッシュコンテナが使われることがある。
        頂点フォーマットは強制的に変換するし、滅多にないことだから対応しなくてもいいと思うけど…。
        とりあえず今のところは無しで。

    ■ 特定のフレーム以下のみにモーションを適用・ブレンドする


    ▼ その他注意

        DirectXサンプルやDXLib では、フレームの中にメッシュ(コンテナ)が格納されていて、
        メッシュを持っていないフレームも存在する。
        しかし、このライブラリではModelMesh内にすべてを配列で持つことにする。

        DirectXサンプルではメッシュコンテナのNumInflというメンバに
        1面あたりの影響ボーン数 (最大4) を格納して HLSL 側で乗算回数を減らす工夫してたけど、
        このライブラリでは4つで固定する。
        (ユーザーがシェーダをカスタマイズするとき、余計なこと考えなくてもいいように)

*/

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
    Model();
    LN_REFOBJ_METHODS;

protected:
    virtual ~Model();

public:

    /// フレーム数の取得
    lnU32 getFrameNum() const { return mFrameNum; }

    /// インデックスを指定してフレームを取得する
    ModelFrame* getFrame( lnU32 index_ ) { return &mModelFrames[ index_ ]; }

    /// フレームを検索する
    ModelFrame* findFrame( const lnChar* name_ );

    /// モーションコントローラの取得
    MotionController* getMotionController() { return mMotionController; }

    /// すべてのフレーム行列を階層的に更新する
    void updateFrameMatrix();

    /// すべての剛体を強制的にボーンの位置に移動させる
    void resetRigidBody();


    void postUpdate();

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

    // 剛体の位置を現在のボーンの位置にあわせる
    //void resetRigidBodyPos();

    /// ソフトウェアスキニング
    void skinning();




public:

    ModelCore* mModelCore;     ///< (ModelCore から代入され、その時参照カウント+1)

private:

    lnU32               mFrameNum;
    ModelFrame*         mModelFrames;
    ModelFrame*         mRootFrame;     ///< (ModelCore から代入される)

    lnU32               mRigidBodyNum;
    ModelRigidBody*     mModelRigidBodys;

    lnU32               mConstraintNum;
    ModelConstraint*    mConstraints;

    MotionController*   mMotionController;

    lnU8                mRigidResetCount;             

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
} // namespace Lumine

//=============================================================================
//								end of file
//=============================================================================