//=============================================================================
//【 Common 】
//-----------------------------------------------------------------------------
///**
//  @file       Common.h
//  @brief      Common
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include <list>
#include <map>
#include "../Base/RefString.h"
#include "../Graphics/Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{
    
    class SceneGraph;
    class SceneNode;
    class Camera;
    class Light;
    class Viewport;


    class Manager;
    class DrawCommandContainer;
    class SceneShaderManager;
    class SceneShader;
    class DummyNode;
    class Camera;
    class Light;
    class EffectBatchProxy;
    class BatchSpriteProxy;
    class PhysicsDebugDrawProxy;
    class Pane;

    class SceneGraphContext;
    class SceneNodeContext;
    class PaneContext;
    class CameraContext;
    class LightContext;


    typedef std::vector< SceneNode* >               SceneNodeArray;
    typedef std::list< SceneNode* >                 SceneNodeList;
    typedef std::map< LRefTString, SceneNode* >     SceneNodeNameMap;
    typedef std::pair< LRefTString, SceneNode* >    SceneNodeNamePair;
    typedef std::vector< lnU32 >                    SubsetIndexArray;
    typedef std::vector< Light* >                   LightArray;
    typedef std::list< Viewport* >                  ViewportList;
    typedef std::vector< Pane* >    PaneArray;
    typedef std::vector< Camera* >  CameraList;
    typedef std::vector< Light* >   LightList;

    typedef std::vector< SceneNodeContext* >    SceneNodeContextArray;
    typedef std::vector< CameraContext* >       CameraContextArray;
    typedef std::vector< LightContext* >        LightContextArray;
    

// (描画コマンドのサイズ肥大化対策のために用意)
class DrawingContext
    : public Base::ReferenceObject
{
public:

    /// (引数は後で Graphics::Manager に変えるかも…)
    virtual void draw(
        Graphics::IRenderer* renderer_,
        Graphics::GeometryRenderer* geometry_renderer_ ) = 0;
};

} // namespace Scene
} // namespace Core

//-------------------------------------------------------------

/// デフォルトライトの優先度
static const int LN_DEFAULT_LIGHT_PRIORITY = 1000;

/// デフォルトでセットされるノードの優先度
enum LNDefaultSceneNodePriority
{
    LN_SCENENODEPRIORITY_EFFECT_BATCH_SPRITE = 1000000,   ///< Effect 用バッチスプライト (2D/3D)
    LN_SCENENODEPRIORITY_PHYSICS_DEBUG = 1000000,           ///< 物理演算のデバッグ描画
    LN_SCENENODEPRIORITY_PROXY = 1000000,
};

/// 座標空間
enum LNDrawCoordSpace
{
    LN_DRAWCOORD_UNKNOWN = 0,   ///< NULL ノード、汎用ノード用、または未設定
    LN_DRAWCOORD_2D,
    LN_DRAWCOORD_3D
};

/// ノードの種類
enum LNSceneNodeType
{
	LN_SCENENODE_UNKNOWN = 0,
    //LN_SCENENODE_PANE,
	LN_SCENENODE_VIEWPORT,          ///< ビューポート (OffscreenScene::addSceneNode() で登録をはじいている)
    LN_SCENENODE_GRID,
    LN_SCENENODE_LIGHT,
    LN_SCENENODE_SPRITE3D,
    LN_SCENENODE_BATCHSPRITE,
    LN_SCENENODE_BATCHSPRITE3D,
    LN_SCENENODE_BATCHSPRITEPROXY,
    LN_SCENENODE_MODEL,
    LN_SCENENODE_PARTICLE,
    LN_SCENENODE_TILEMAP,
    LN_SCENENODE_EFFECT,
    LN_SCENENODE_EFFECTSPRITEPROXY,
    LN_SCENENODE_PHYSICSDEBUGDRAWPROXY,
};



/// ライトの種類
enum LNLightType
{
    LN_LIGHTTYPE_DIRECTIONAL = 1,   ///< ディレクショナルライト
	LN_LIGHTTYPE_POINT,             ///< ポイントライト
	LN_LIGHTTYPE_SPOT,              ///< スポットライト
};

/// Z ソートのための距離の計算方法
enum LNZSortFunc
{
    LN_ZSORTF_IMM_Z = 0,        ///< Z 値を直接使う (2D 用)
    LN_ZSORTF_CAMERA_DIST,      ///< カメラからの直線距離
    LN_ZSORTF_CAMERA_SCREEN,    ///< カメラから見たスクリーン平面からの距離
};

/// 反転方法
enum LNFlipFlag
{
    LN_FLIP_NONE    = 0x00,     ///< 反転無し
    LN_FLIP_V       = 0x01,     ///< 上下反転
    LN_FLIP_H       = 0x02,     ///< 左右反転
};

/// デバッグ描画フラグ
enum LNSceneDebugDrawFlag
{
    LN_SCENEDEBUGDRAW_NONE              = 0,
    LN_SCENEDEBUGDRAW_CAMERA            = 0x01,     ///< カメラのイメージ
    LN_SCENEDEBUGDRAW_CAMERA_FRUSTUM    = 0x02,     ///< カメラの視錐台
    LN_SCENEDEBUGDRAW_LIGHT             = 0x04,     ///< ライトのイメージ
    LN_SCENEDEBUGDRAW_LIGHT_FRUSTUM     = 0x08,     ///< ライトの視錐台

    LN_SCENEDEBUGDRAW_ALL = 0xffffffff,
};

/// ビューポートエフェクトの種類
enum LNScreenEffectMode
{
    LN_SCREENEFFECTMODE_NONE = 0,         ///< 特殊効果を使用しない
    LN_SCREENEFFECTMODE_BUFFERINGEFFECT,  ///< ブラ―、トランジションをサポートする
};

/// タイルマップ作成時の向き
enum LNTilemapDirection
{
    LN_TILEMAPDIR_XYR = 0,     ///< XY 平面に対して Z+ 向きに作成する (2D の場合はこれを指定する)
    //LN_TILEMAPDIR_XYM          ///< XY 平面の第 4 象限に、Z- 向きに作成する (Z- 方向から見たときに LN_TILEMAPDIR_XYR と同じ配置になる http://www.suriken.com/knowledge/glossary/fourth-quadrant.html)

    LN_TILEMAPDIR_XY            ///< XY 平面に対して、マップの左下が原点になるように作成する (第一象限に作成する)
};

//-------------------------------------------------------------

/// デフォルトで作成されるシェーダプログラムの番号
enum LNDefaultSceneShader
{
    LN_DEFSHADER_BASIC_2D = 0,
    LN_DEFSHADER_BASIC_3D,

    LN_DEFSHADER_NOLIGHTING,

	//LN_DEFSHADER_DEBUGSHAPES_3D = 0,

    //
 //   LN_DEFSHADER_PARTICLE_3D,

    LN_MAX_DEFAULT_SHADERS
};

/// デフォルトで作成される SceneNode
enum LNDefaultSceneNode
{
    LN_DEFAULT_SCENENODE_ROOT = 0,  ///< ルートノード
    LN_DEFAULT_SCENENODE_3D,        ///< 3D 空間のルート
    LN_DEFAULT_SCENENODE_2D,        ///< 2D 空間のルート
    LN_MAX_DEFAULT_NODES
};

//-------------------------------------------------------------

/// カメラ単位の設定項目 (1フレームに1度だけ設定すればいいパラメータ。Camera::getCameraSceneParam() で取得)
struct LNCameraSceneParam
{
    const LMatrix*   Mat_CameraView;
    const LMatrix*   Mat_CameraProj;
    const LMatrix*   Mat_CameraViewProj;    ///< カメラのビューとプロジェクション行列の積

    LVector4	     CameraPosition;	    ///< カメラの位置
    LVector4	     CameraDirection;	    ///< カメラの向き

    // 以下は calcMatrices() で設定する
    LMatrix  Mat_CameraView_Inverse;
    LMatrix  Mat_CameraProj_Inverse;
    LMatrix  Mat_CameraViewProj_Inverse;
    LMatrix  Mat_CameraView_Transpose;
    LMatrix  Mat_CameraProj_Transpose;
    LMatrix  Mat_CameraViewProj_Transpose;
    LMatrix  Mat_CameraView_InverseTranspose;
    LMatrix  Mat_CameraProj_InverseTranspose;
    LMatrix  Mat_CameraViewProj_InverseTranspose;

    void calcMatrices()
    {
        LMatrix::inverse( &Mat_CameraView_Inverse, *Mat_CameraView );
        LMatrix::inverse( &Mat_CameraProj_Inverse, *Mat_CameraProj );
        LMatrix::inverse( &Mat_CameraViewProj_Inverse, *Mat_CameraViewProj );
        LMatrix::transpose( &Mat_CameraView_Transpose, *Mat_CameraView );
        LMatrix::transpose( &Mat_CameraProj_Transpose, *Mat_CameraProj );
        LMatrix::transpose( &Mat_CameraViewProj_Transpose, *Mat_CameraViewProj );
        Mat_CameraView_InverseTranspose = Mat_CameraView_Inverse * Mat_CameraView_Transpose;
        Mat_CameraProj_InverseTranspose = Mat_CameraProj_Inverse * Mat_CameraProj_Transpose;
        Mat_CameraViewProj_InverseTranspose = Mat_CameraViewProj_Inverse * Mat_CameraViewProj_Transpose;
    }
};

/// サブセット単位の描画パラメータ
struct LNSubsetRenderParam
{
    LMaterial                   Material;       ///< 描画オブジェクトのマテリアル (Node ひとつにつき Shader ひとつが原則なので、この中の Shader は使わない)
    LVector4		            ToonColor;      ///< 描画オブジェクトのトゥーンカラー
    LVector4		            EdgeColor;      ///< 描画オブジェクトの輪郭色

    lnfloat                     Opacity;        ///< 不透明度 (シェーダに乗算色を送るとき、そのα値に乗算される)
    LColor	                    ColorScale;     ///< 乗算する色
    LColor	                    BlendColor;     ///< ブレンドする色
    LTone	                    Tone;           ///< 色調
    LMatrix		                UVTransform;    ///< テクスチャ座標変換行列

    /// 初期値
    LNSubsetRenderParam()
        : Opacity           ( 1.0f )
        , ToonColor         ( 1.0f, 1.0f, 1.0f, 1.0f ) 
        , EdgeColor         ( 0.0f, 0.0f, 0.0f, 1.0f )
        , ColorScale        ( 1.0f, 1.0f, 1.0f, 1.0f ) 
        , BlendColor        ( 0.0f, 0.0f, 0.0f, 0.0f )
        , Tone              ( 0.0f, 0.0f, 0.0f, 0.0f )
    {}
};



//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================