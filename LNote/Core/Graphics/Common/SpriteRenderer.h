//=============================================================================
//【 SpriteRenderer 】
//-----------------------------------------------------------------------------
///**
//  @file       SpriteRenderer.h
//  @brief      SpriteRenderer
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <list>
#include "../Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

/// バッチ処理用スプライト頂点
struct BatchSpriteVertex
{
    LVector3	Position;   ///< 座標
    u32         Color;      ///< 頂点カラー
    LVector2	TexUV;      ///< テクスチャ座標
 
    
    /// 頂点レイアウト
    static LNVertexElemenst* elements()
    {
	    static LNVertexElemenst elements[] = {
		    { 0, LN_DECL_FLOAT3, LN_USAGE_POSITION, 0 },
            { 0, LN_DECL_COLOR4, LN_USAGE_COLOR,    0 },
            { 0, LN_DECL_FLOAT2, LN_USAGE_TEXCOORD, 0 },
		      LN_DECL_END() };
	    return elements;
    }
};

struct BatchSpriteData
{
    BatchSpriteVertex   Vertices[ 4 ];  ///< 座標変換済み頂点 ([0].Position.z がソートの基準になる)
    ITexture*           Texture;
    int                 Priority;       ///< 優先度 (大きい方が後から描画される =手前)
    lnfloat             Depth;          ///< ソートに使われる Z 値 (大きいほど遠い)
    bool                Visible;
    bool                Assigned;       ///< SpriteRenderer::getBatchSpriteData() で取得された場合に true になる

};


//=============================================================================
// ■ SpriteRenderer クラス
//-----------------------------------------------------------------------------
///**
//  @brief		スプライト描画クラス (2D、3D兼用)
//*/
//=============================================================================
class SpriteRenderer
    : public Base::ReferenceObject
{
public:

    SpriteRenderer( Manager* manager_ );

    virtual ~SpriteRenderer();

    LN_REFOBJ_METHODS;

public:

    /// 初期化
    LNRESULT initialize( IGraphicsDevice* device_, int max_sprites_, bool is_3d_ );

    /// 座標変換行列の設定
    virtual void setTransform( const LMatrix& matrix_ );

    /// ビュー、プロジェクション行列の設定 (view はビルボードの計算で必要になるので、view proj で分ける)
    virtual void setViewProjMatrix( const LMatrix& view_, const LMatrix& proj_ );

    /// ソート方法の設定(LNSpriteSortMode  enable_view_pos_depth_:カメラからの距離をソート用Z値にする場合 true)
    virtual void setSortMode( u32 flags_, bool enable_view_pos_depth_ );

    /// 描画リクエスト
    virtual LNRESULT drawRequest2D(
        const LVector3& position_,
        const LVector3& center_,
        ITexture* texture_,
        const Geometry::Rect& src_rect_,
        u32 color_ );

    /// 描画リクエスト
    virtual LNRESULT drawRequest3D(
        const LVector3& position_,
        const LVector3& center_,
        const LVector2& size_,
        ITexture* texture_,
        const Geometry::Rect& src_rect_,
        const u32* color_table_,    // 4 頂点分。NULL の場合は白
        LNAxisAlignedDir front_ );

    /// バッチ処理されているスプライトの描画
    virtual LNRESULT flash();

private:

    /// Z 値の大きい方から小さい方へソートする比較関数
    static bool _cmpDepthBackToFront( const u16& l_, const u16& r_ );

    /// Z 値の小さい方から大きい方へソートする比較関数
    static bool _cmpDepthFrontToBack( const u16& l_, const u16& r_ );

    /// Z 値の大きい方から小さい方へソートする比較関数 (テクスチャ優先)
    static bool _cmpTexDepthBackToFront( const u16& l_, const u16& r_ );

    /// Z 値の小さい方から大きい方へソートする比較関数 (テクスチャ優先)
    static bool _cmpTexDepthFrontToBack( const u16& l_, const u16& r_ );
    

private:

    struct Attribute
    {
        int StartIndex;
        int PrimitiveNum;
        ITexture*   Texture;
    };

private:

    Manager*            mManager;
    IGraphicsDevice*    mGraphicsDevice;
    IVertexBuffer*      mVertexBuffer;
    IIndexBuffer*       mIndexBuffer;
    int                                 mMaxSprites;
    BatchSpriteData*                    mSpriteRequests;
    //Core::Base::Stack< int >            mIndexStack;
    int                                 mNextIndex;         ///< mSpriteRequests の空いているインデックスを示す
    int                                 mLastSpriteNum;
    LMatrix                             mTransformMatrix;
    LMatrix                             mViewInverseMatrix;
    LMatrix                             mViewProjMatrix;
    LVector3                            mViewPosition;
    LVector3                            mViewDirection;

    u16*                                mSpriteIndexArray;
    u16*                                mSpriteIndexArraySource;
    std::list< Attribute >              mAttributeList;

    bool ( *mComFunc )( const u16& l_, const u16& r_ );
    u32                                 mSpriteSortMode;        ///< ソート方法 (LNSpriteSortMode)
    bool                                mEnableViewPosDepth;
    bool                                mIs3DSystem;        ///< true の場合、スプライトの中心は四角形の中心で、Y+ 方向が上になる

    
   
    /// シェーダ関係の変数をまとめた構造体
    struct 
    {
        IShader*            Shader;

        IShaderVariable*    varViewProjMatrix;
        IShaderVariable*    varTexture;

        IShaderTechnique*   techMainDraw;

    } mShaderParam;


    static BatchSpriteData* sSpriteRequests;
};


#if 0


/// 各 ISprite が持つデータ
struct SpriteDrawRequest
{
	SpriteVertex	            Vertices[ 4 ];	///< 四角形ひとつ分の頂点配列
    Core::Graphics::ITexture*   Texture;
	
    u32     Index;      ///< requestDraw() に渡した時に割り当てられるデータ番号 (サブセット番号)
};

//=============================================================================
// ■ SpriteRenderer クラス
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class SpriteRenderer
    : public ISpriteRenderer
{
public:

    //---------------------------------------------------------------------
	///**
    //  @brief      描画リクエストを追加する
    //*/
    //---------------------------------------------------------------------
    LNRESULT requestDraw( SpriteDrawRequest* data_ );
   
};





class Sprite
{

    void onPreDraw()
};

#endif


/*

◆ 設計とか

・Sprite クラスの drawSubset() は、サブセット番号 0 のみが渡されてくる。

・SceneObject を継承することでユーザー任意の描画をできるようにする
	・SpriteRenderer を使う限り、drawPrimitive() 使うわけじゃないのでこれまで通りっていうわけにはいかない。

・SceneOjbect には onRequestBatchDraw() を持たせておく。
  この関数は、SpriteRenderer や BillboardRenderer のように、データをバッファに溜めてから
  後で一度に描画するものの描画リクエスト送信を実装する。

・ISprite には２つの種類がある。
  ひとつは、単一の汎用性のあるスプライトっていうイメージ。
  これは以前までと同様、onDrawSubset() で setVertexBuffer()、drawPrimitive() の順に実行して描画する。
  バッチ処理の対象ではない。
  もうひとつは、バッチ処理されるもの。ひとつめのよりも高速に描画できる
  (描画中、setVertexBuffer() で頂点バッファの設定を変えない等)が、
  スプライト以外のオブジェクトとのZソートがうまくいかない。
  （2Dであるスプライトなら頑張ればいけると思うけど、3D空間に配置されるビルボードはかなりキツイ）
  そのため、バッチ処理は以下のものを対象とする。
    ・半透明要素の無いオブジェクト(アルファテストでカットされるものはOK(アルファ値が完全に0))
    ・半透明要素があり、かつ、半透明要素の描画パスで描画するもの(通常のオブジェクトの後に描画するべきもの)
  一応、バッチ処理の想定している使い方は主にエフェクト関係なので、この方向で問題はないかも。
  作成に関しては createSprite() createBatchSprite() の関数を使い、
  返るインターフェイスは両方とも ISprite。





Selene ではスプライトひとつ毎に drawPrimitive 読んでたけど…？

検証
    頂点 60000
    トライアングルリスト
    視点の近くで描画

    １．四角形ひとつにつき 回、DrawPrimitive() を呼ぶ
        奇数フレーム目が平均 1000ms
        偶数フレーム目が平均 500ms

    ２．全体を1度の DrawPrimitive() で描画
        １．とほとんど同じ。

    DrawPrimitive() のところだけ比べてみると、
    １．では 0～2ms、２．では 0ms。


    速度的にはほとんど変わらなかった。ちなみに速度が落ちてるのは Present()。
    誤差は関数呼び出しの分の時間って感じで、描画にはほとんど関係ないかも。




◆ 複数の四角形を格納した頂点バッファに対して、
   四角形ひとつにつき一度 drawPrimitive() を呼んで描画する


検証するコード

	u32 n = (VERTEX_NUM / 3) / 2;
	for ( u32 i = 0; i < n; ++i )
	{
	    renderer_->setVertexBuffer( mVertexBuffer, true );
	    renderer_->drawPrimitive( LN_PRIMITIVE_TRIANGLELIST, i * 3, 2 );
	}


頂点数 60000
トライアングルリスト
一度の drawPrimitive() で2つのプリミティブ (四角形)を描画する


● 動的な頂点バッファとして作成した場合
	80ms～90ms
	
● 静的な頂点バッファとして作成した場合
	40ms～50ms
	倍くらい早い。

● lock() unlock() の重さ
	静的な頂点バッファとして作成後、
	上記コードの直前で
		mVertexBuffer->lock();
		mVertexBuffer->unlock();
	の２行を呼ぶ。
	結果、80ms～90ms。
	動的な頂点バッファとほぼ同じ。
	
● lock() unlock() の重さ  100回
	静的な頂点バッファとして作成後、
	上記コードの直前で
	for ( u32 i = 0; i < 100; ++i )
    {
        mVertexBuffer->lock();
        mVertexBuffer->unlock();
    }
    を実行する。
    結果、80ms～90ms。
    ほとんど変わらなかった。
    
● renderer_->setVertexBuffer( mVertexBuffer, true ); を
	ループの外に出して、１度だけ実行するようにする
	
	・動的の場合
		80ms～90ms。
		あまり変わらない。
		
	・静的の場合
		40ms～50ms
		あまり変わらない。
		
	・静的で１００回lock()
		80ms～90ms。
		あまり変わらない。
		
		
◆ もうひとつ頂点バッファを作って、以下のコードを実行

	u32 n = (VERTEX_NUM / 3) / 2;
	n /= 2;
    for ( u32 i = 0; i < n; ++i )
    {
        renderer_->setVertexBuffer( mVertexBuffer, true );
        renderer_->drawPrimitive( LN_PRIMITIVE_TRIANGLELIST, i * 3, 2 );

        renderer_->setVertexBuffer( mVertexBuffer2, true );
        renderer_->drawPrimitive( LN_PRIMITIVE_TRIANGLELIST, i * 3, 2 );
    }
    
    ・動的の場合
		200ms～220ms。
		
		
    ・静的の場合
    	90～110ms
    	
    ・静的・100 lock() の場合
    	90～110ms
    	
	・静的・100 lock() 両方の場合
    	90～110ms






struct SpriteVertex
{
	Core::Math::Vector3		Position;		///< 座標
	u32						Color;			///< 乗算色 (+アルファ値)
	u32						BlendColor;		///< ブレンド色 (塗りつぶし色)
	u32						Tone;			///< 色調
	Core::Math::Vector2		TexUV;			///< テクスチャ座標
	
	Core::Math::Vector3		WorldMatrix00_03;
	Core::Math::Vector3		WorldMatrix10_13;
	Core::Math::Vector3		WorldMatrix20_23;
};








class ISprite
{
};


class Sprite
	: public SceneObject
{
	Sprite()
	{
	}
};






class SpriteRenderer
{

	LNRESULT drawRequest();
};
*/





//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================