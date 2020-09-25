//=============================================================================
//【 RingRenderer 】
//-----------------------------------------------------------------------------
///**
//  @file       RingRenderer.h
//  @brief      RingRenderer
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

//=============================================================================
// ■ RingRenderer クラス
//-----------------------------------------------------------------------------
///**
//  @brief		スプライト描画クラス (2D、3D兼用)
//*/
//=============================================================================
class RingRenderer
    : public Base::ReferenceObject
{
public:

    /// 頂点データ
    struct Vertex
    {
        LVector3	Position;
        lnU32       Color;
        LVector2	TexUV;
     
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

    /// 描画引数
    struct DrawingParamater
    {
        LMatrix         Transform;
        lnU32           DivisionCount;
        lnfloat		    ViewingAngle;
        lnfloat         OuterRadius;
        lnfloat         OuterHeight;
        lnfloat         InnerRadius;
        lnfloat         InnerHeight;
		lnfloat		    CenterRatio;
		LColor		    OuterColor;
		LColor		    CenterColor;
		LColor		    InnerColor;
		LRectF	        UV;
        LNBillboardType BillboardType;
    };

public:

    RingRenderer( Manager* manager_ );

    virtual ~RingRenderer();

public:

    /// 初期化
    void initialize( int square_max_count_, bool is_3d_ );

    /// 座標変換行列の設定
    //virtual void setTransform( const LMatrix& matrix_ );

    /// ビュー、プロジェクション行列の設定 (view はビルボードの計算で必要になるので、view proj で分ける)
    void setViewProjMatrix( const LMatrix& view_, const LMatrix& proj_ );

    /// テクスチャの設定
    void setTexture( ITexture* texture_ );

    /// ソート方法の設定(LNSpriteSortMode  enable_view_pos_depth_:カメラからの距離をソート用Z値にする場合 true)
    //virtual void setSortMode( u32 flags_, bool enable_view_pos_depth_ );

    /// 描画開始
    void begin();

    /// 描画終了
    void end();

    /// 描画リクエスト
    //virtual LNRESULT drawRequest2D(

    /// 描画リクエスト
    void drawRequest3D( const DrawingParamater& param_ );


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
    LNGraphicsAPI       mGraphicsAPI;
    IVertexBuffer*      mVertexBuffer;
    IIndexBuffer*       mIndexBuffer;
    ITexture*           mTexture;
    LMatrix             mViewInverseMatrix;
    LMatrix             mViewProjMatrix;
    LVector3            mViewPosition;
    LVector3            mViewDirection;
    Vertex*             mLockedBuffer;
    lnU32               mUsingVertexCount;


    struct 
    {
        IShader*            Shader;
        IShaderVariable*    varViewProjMatrix;
        IShaderVariable*    varTexture;
        IShaderTechnique*   techMainDraw;

    } mShaderParam;

    //IIndexBuffer*       mIndexBuffer;
#if 0
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

#endif
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