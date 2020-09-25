//=============================================================================
//【 Tilemap 】
//-----------------------------------------------------------------------------
///**
//  @file       Tilemap.h
//  @brief      Tilemap
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "SceneNode.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{

//=============================================================================
// ■ Tilemap
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class Tilemap
    : public SceneNode
{
public:

    Tilemap( SceneGraph* scene_ );

    virtual ~Tilemap();

public:

    /// 初期化
    void initialize( u32 w_size_, u32 h_size_, LNTilemapDirection dir_ );

    /// 表示位置の設定
    //virtual void setPosition( int x_, int y_ );

    /// タイルセットテクスチャの設定
    virtual void setTilesetTexture( Graphics::ITexture* texture_ );

    /// マップデータ配列の設定
    virtual void setMapData( int* mapdata_ ) { mMapData = mapdata_; }


public:

    /// ノードの種類の取得
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_SPRITE3D; }

    /// フレーム更新
    virtual void update( const Game::GameTime& time_ );

    /// サブセットを描画する
    virtual LNRESULT drawSubset( lnU32 index_ );

private:

    /// 現在の状態を元に頂点バッファを作る
	LNRESULT _makeVertexBuffer( LNTilemapDirection dir_ );

    /// テクスチャ座標を書き込む
	void _writeVertexUV();

private:

    u32                         mVertexNum;     ///< 頂点の総数 (頂点バッファに入ってるからいらないかも)
    Graphics::IVertexBuffer*    mVertexBuffer;  ///< 頂点バッファ
    Graphics::IIndexBuffer*     mIndexBuffer;   ///< インデックスバッファ
    Graphics::ITexture*         mTilesetTexture;

    u32         mXSize;         ///< X 方向のタイルの数
    u32         mYSize;         ///< Y 方向のタイルの数
    u32         mFaceNum;       ///< 面数

    lnfloat     mTileWidth;     ///< タイルひとつ分の幅
    lnfloat     mTileHeight;    ///< タイルひとつ分の高さ

    lnfloat     mUVWidth;       ///< マップチップひとつ分のピクセルサイズをテクスチャ座標系に変換したときの幅
    lnfloat     mUVHeight;      ///< マップチップひとつ分のピクセルサイズをテクスチャ座標系に変換したときの幅

    int*        mMapData;

    u32         mSrcXNum;       ///< タイルセット画像の横方向のチップ数 (RGSS の場合は 8)
    
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================