//=============================================================================
//【 GeometryRenderer 】
//-----------------------------------------------------------------------------
///**
//  @file       GeometryRenderer.h
//  @brief      GeometryRenderer
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Interface.h"
#include "../../Physics/IDebugRenderer.h"
#include "../../Geometry/Geometry.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
class PointParticle;

//=============================================================================
// ■ GeometryRenderer クラス
//-----------------------------------------------------------------------------
///**
//  @brief		3Dプリミティブの描画クラス
//*/
//=============================================================================
class GeometryRenderer
    : public Base::ReferenceObject
    , public Physics::IDebugRenderer
{
public:

	/// 初期化データ
	struct InitData
	{
        Base::LogFile*      LogFile;
        IGraphicsDevice*    GraphicsDevice;      
	};

    /// 描画パス
    enum DrawingPass
    {
        PASS_BASIC = 0,
        PASS_POINT_PARTICLE,
    };

public:

    /// コンストラクタ
    GeometryRenderer();

	/// デストラクタ
	virtual ~GeometryRenderer();

    LN_REFOBJ_METHODS;

public:

    /// 初期化
    LNRESULT initialize( const InitData& init_data_ );

    /// 描画パスの開始
    void begin( DrawingPass pass_ = PASS_BASIC );

    /// 描画パスの終了
    void end();

    /// 座標変換行列の設定
    void setMatrix( const LMatrix& matrix_ );

    /// 座標変換行列の設定
    void setViewProjMatrix( const LMatrix& matrix_ );

    /// ビューサイズの設定( 2D 用のプロジェクション行列を作って、setViewProjMatrix() に設定する)
    //      頂点座標はピクセル単位で指定できるが、テクスチャ座標は0～1.0なので注意。
    void setViewSize( const LVector2& view_size_ );

    /// テクスチャの設定
    void setTexture( ITexture* texture_ );


    /// 四角形の描画
    LNRESULT drawSquare(
        lnfloat x0_, lnfloat y0_, lnU32 c0_, lnfloat u0_, lnfloat v0_, 
	    lnfloat x1_, lnfloat y1_, lnU32 c1_, lnfloat u1_, lnfloat v1_,
	    lnfloat x2_, lnfloat y2_, lnU32 c2_, lnfloat u2_, lnfloat v2_,
	    lnfloat x3_, lnfloat y3_, lnU32 c3_, lnfloat u3_, lnfloat v3_ );

    /// 矩形の描画     
    LNRESULT drawRect( const Geometry::RectF& dest_rect_, const Geometry::RectF& src_rect_, lnU32 color_ );

    
    /// 線分の描画 (単色)
    LNRESULT drawLine( const LVector3& from_, const LVector3& to_, const LColor& color_ );

    /// 線分の描画 (先端の色指定)
    LNRESULT drawLine( const LVector3& from_, const LVector3& to_, const LColor& from_color_, const LColor& to_color_ );

    /// 球の描画
    LNRESULT drawSphere( const LVector3& pos_, lnfloat radius_, const LColor& color_ );

    /// 三角形の描画
    LNRESULT drawTriangle( const LVector3& v0_, const LVector3& v1_, const LVector3& v2_, const LColor& color_ );

    /// 接点の描画 (保留)
    //LNRESULT drawContactPoint( const LVector3& PointOnB, const LVector3& normalOnB, btScalar distance, int lifeTime, const LVector3& color );

    /// 座標軸の描画
    LNRESULT drawTransformAxis( lnfloat ortho_len_ );

    /// ボックスの描画
    LNRESULT drawBox( const LVector3& bb_min_, const LVector3& bb_max_, const LColor& color_ );
    
    /// カプセルの描画
    LNRESULT drawCapsule( lnfloat radius_, lnfloat half_height_, const LColor& color_ );
    
    /// シリンダーの描画 (保留)
    LNRESULT drawCylinder( lnfloat radius_, lnfloat half_height_, const LVector3& color_ );
    
    /// コーンの描画 (保留)
    LNRESULT drawCone( lnfloat radius_, lnfloat half_height_, const LVector3& color_ );
    
    /// 平面の描画 (保留)
    LNRESULT drawPlane( const LVector3& normal_, lnfloat plane_const_, const LVector3& color_ );

    /// 視錐台の描画 (座標変換済みとして描画する。setMatrix() には単位行列を渡しておくこと)
    void drawViewFrustum( const LViewFrustum& frustum_, const LVector3& view_pos_, lnU32 color_ );

    /// PointParticle の描画 (内部で PASS_POINT_PARTICLE によるパス開始が行われる)
    //      ※呼び出し側でレンダリングステートを変更しておくこと！
    void drawPointParticle(
        const LMatrix& world_,
        const LMatrix& porj_,
        const LMatrix& view_porj_,
        const LVector4& camera_pos_,
        PointParticle* particle_ );

private:

    Base::LogFile*          mLogFile;
    IGraphicsDevice*        mGraphicsDevice;
    LNGraphicsAPI           mGraphicsAPI;
    IRenderer*              mRenderer;
    ITexture*               mDummyTexture;
    IShaderPass*            mCurrentShaderPass;

    IVertexBuffer*          mVBPosColor;
    IVertexBuffer*          mVBPosColorUV;
    IIndexBuffer*           mIndexBuffer;

    /// シェーダ関係の変数をまとめた構造体
    struct 
    {
        IShader*            Shader;
        IShaderVariable*    varWorldMatrix;
        IShaderVariable*    varViewProjMatrix;
        IShaderVariable*    varTexture;
        IShaderTechnique*   techMainDraw;
        IShaderPass*        passP0;

    } mShaderParam;

    /// PointParticle
    struct
    {
        IShader*            Shader;
        IShaderVariable*    varWorldMatrix;
        IShaderVariable*    varViewProjMatrix;
        IShaderVariable*    varProj_11_Div_00;
        IShaderVariable*    varTime;
        IShaderVariable*    varGlobalAccel;
        IShaderVariable*    varGravityPower;
        IShaderVariable*    varIsSequential;
        IShaderVariable*    varIsParticleLoop;
        IShaderVariable*    varIsDynamic;
        IShaderVariable*    varMaterialTexture;
        IShaderPass*        passP0;
    } mPointParticleShader;

    /*
    /// 軸の描画
    virtual LNRESULT drawAxis( lnfloat length_ );

    /// 四角形の描画
    virtual LNRESULT drawSquare(
        lnfloat x0_, lnfloat y0_, u32 c0_, lnfloat u0_, lnfloat v0_, 
	    lnfloat x1_, lnfloat y1_, u32 c1_, lnfloat u1_, lnfloat v1_,
	    lnfloat x2_, lnfloat y2_, u32 c2_, lnfloat u2_, lnfloat v2_,
	    lnfloat x3_, lnfloat y3_, u32 c3_, lnfloat u3_, lnfloat v3_ );

    /// 矩形の描画     
    virtual LNRESULT drawRect( const LRectF& dest_rect_, const LRectF& src_rect_, u32 color_ );

private:

    Kernel::Base::LogFile*  mLogFile;
    IGraphicsDevice*        mGraphicsDevice;

    IVertexBuffer*          mVBPosColorUV;
    IVertexBuffer*          mVBPosColor;
    IRenderer*              mRenderer;
    
    

    
*/
    /*
    Manager*                            mManager;
    Core::Graphics::IGraphicsDevice*    mDevice;
	Core::Graphics::

    Core::Graphics::IVertexBuffer*      mAxisXYZVertexBuffer;   ///< 軸用
    Core::Graphics::IVertexBuffer*      mBillboardVertexBuffer;
    Core::Graphics::IVertexBuffer*      mSpriteVertexBuffer;
    Core::Graphics::IVertexBuffer*      mBasic2DVertexBuffer;  //drawSquare で使う予定だったもの
    Core::Graphics::IVertexBuffer*      mScreenBuffer;          ///< スクリーン描画用



    Core::Graphics::IVertexBuffer*      mDebugShapesVertexBuffer;   ///< デバッグ形状用頂点バッファ
    Core::Graphics::IIndexBuffer*       mDebugShapesIndexBuffer;    ///< デバッグ形状用インデックスバッファ


    Core::Graphics::IShader*            mShader;
    Core::Graphics::IShaderVariable*    mMatrixVar;
    Core::Graphics::IShaderVariable*    mTextureSizeVar;
    
    Core::Graphics::IShaderVariable*    mTextureVar;
    Core::Graphics::IShaderTechnique*   mScreenBufferTech;


    Core::Graphics::IShaderTechnique*   mCurrentShaderTech;
    Core::Graphics::IShaderPass*        mCurrentShaderPass;

    struct 
    {
        struct
        {
            Core::Graphics::IShaderVariable*    ViewportSize;

        } Var;

        struct 
        {
            Core::Graphics::IShaderTechnique*   Basic2DTranslated;

        } Tech;

        Core::Graphics::IShaderTechnique*   techDebugShapes;

        Core::Graphics::IShaderVariable*    varViewProjMatrix;


    } mShaderParam;
    */
};




//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

/*




class LocusData
{



    // 以下はこのデータがグループの先頭にあるデータの場合に設定される

    bool    mGroupFirst;    ///< グループの先頭にあるデータの場合 true

    u32     mVertexIndex;   ///< グループの開始頂点番号

    u32     mPrimitiveNum;  ///< グループ全体のプリミティブ数
};


class BatchLocus
    : public ILocus
{
public:

    virtual bool isEnable() { return mLocusData.mGroupFirst; }

private:

    LocusData   mLocusData;
};





以前までの







	

LEffectEmitter
	getType
	setRenderState
	setFillMode
	refresh
	





class Line3D
{
	Vector3		mPoint1;	///< 線分の開始位置 
	Vector3		mPoint2;	///< 線分の終了位置
};


/// LocusRenderer に渡すような感じのデータまとめ
struct LocusDrawData
{
	Line3D*		LineArray;		///< 
	u32			LineArraySize;
	u32			LineNum;
};





軌跡エミッタ
	
	
	LocusDrawData


１．作成されているラインの数によって、透明度を先頭から減算していく( 前の)
		末尾のラインは常に透明度が0になる。
		
２．作成されているライン毎に、経過フレーム数によって透明度を下げておく
		末尾のラインの透明度が 0 とは限らない。
		隣接するラインとの透明度の差の絶対値が常に等しい。
		
		問題は、透明度の差が 1.0 / MaxLine 以上でないと、バッファが1周したときに
		軌跡がプツリと切れたような状態になること。
		
		
		
軌跡は、ふつーに描画したいときはラインと同じ感じになる。
Win32 形式をちょっといじった感じにすると、

line_renderer->begin( 始点 );		// MoveToEx
line_renderer->drawLineTo( 点 );	// LineTo
line_renderer->drawLineTo( 点 );	// LineTo
line_renderer->drawLineTo( 点 );	// LineTo
//...
line_renderer->end();

軌跡もこれと同じ感じ。
winapi とかのと違うのは、テクスチャ座標や色情報も含む頂点であること。

もうひとつ、頂点配列を渡して描画するのもある。C# には DrawLines みたいなのがあった。

locus_renderer->drawLines( 線分の配列 );

線分の配列とかあるけど、実際は頂点2つをセットにする頂点バッファ。
エフェクトではこっちをメインに使っていくことになると思う。


となると、Locus なるクラスの中から頂点バッファを取り出して drawLines に渡せるものがあればそれなりにOK。




ただ、Locus は基本半透明で、3D空間に配置されるもの。
１．バッファとか関係なく、普通にメッシュと同じように onDrawSubset() で描画する。
２．不透明描画と半透明描画のステップに分ける
	(ステップ分けは Priority で設定すればいいかも。)
	
一応 SceneObject から描画する場合、リクエストを送信するタイミングでZソートは済んでるので
リクエストリストには正しい順番で追加される。
ステップ分けと、半透明描画時に ZWrite を切るなどの対応をしっかりとしていれば２．でもそこそこ問題なく動作する。

問題は、シェーダの適用。
テクスチャ何かと同じく、シェーダ単位でグループ化が必須。
ただ、描画コマンドの作成をどうするのか。

Z単位のソートはともかく、Priority でのソートはまた問題が出てくる。
上記に加えて、(Z)、Priprity もグループ化の対象にする場合は
もうなんか細分化されすぎてリクエストとかバッファとか使う意味がなくなってきてる気がする。

仮にこのへんの利点があるとすれば、同一エフェクト内で軌跡エミッタをコピペして複数作った場合。

実装するとしたら、スプライトと同じく Locus と BatchLocs みたいに二つのクラスを作る感じになるとおもう。

BatchLocs は BatchSprite と同じく、エフェクトでの使用を想定する。
SceneObject 単位で Z ソートはするけど、同一バッチ内でソートはしない。
また、グループ化によるソートは以下のタイミングでフラグを ONにし、描画の直前でまとめて行うようにする。

	新しいオブジェクトが作成された
	テクスチャがセットされた
	シェーダがセットされた
	
これらの操作が1フレーム内で実行されなかった場合、グループ化、ソートは行われない。
Zソートは関係ないので、リアルタイムでグループ化ってことはあんまりないと思う。

グループ化後は、SceneObject に
	・全体の頂点バッファの書き込み位置
を返しておく必要がある。

グループ化は、単純に Priprity でソートした後、連続しているテクスチャ、シェーダでまとめるだけ。


あとは、描画コマンドの作成。

SceneObject に LocusRenderer を持たせて、onDrawSubset() で描画する。引数はグループ番号。
ただ、テクニック開始とかの関係で問題があるため、新しく onCreateDrawCommand() を作って
これをオーバーライドするようにしておく必要がある。

…って言っても、結局このままだと SceneObject ひとつ当たりの処理になってくるため、
他のと Priiority 単位のソートができない。

用はコマンド作成のとき、LocusRenderer を他の SceneObject のループの中に埋め込めればいいけど、
このループは単純なリストのイテレートだし、そもそも CameraScene とかに渡してるのは
SceneObject 単位なのでちょっとキツイ。

最終的な描画は DrawSubset で行うようにすれば他のと統一できるから、
やっぱり SceneObject 内に持たせる方向でいいと思う。
っていうのも、コマンドの DRAW_SUBSET で描画したいから。


さらに問題になってくるのは、シェーダに送る値。変換行列やマテリアル。
行列は SEND_OBJ_PARAM、マテリアルは DRAW_SUBSET で設定される。
っていうか、これを使った方が設定しやすい。
変換行列の方は単位行列設定しておけばいいだけだからそんなに難しくないと思うけど、
どのみち最終的には SceneObject 単位の描画。


問題点とかをまとめると…

１．描画は SceneObject::onDrawSubset() で、グループ番号を受け取って行う。
２．グループのプライオリティで、他の SceneObject とソートする。

２．は、以前カットという方向にした「サブセット単位のZソート」とまったく同じもの。
シェーダとの兼ね合いもあって、SceneObject 単位で実装するのは無しになった。

でも、グループはひとつのシェーダを持っているというあたり、
SceneObject ひとつ分に相当する。


最終的にやりたいことは、
「同じステート(マテリアルやテクスチャ、シェーダ等)で描画するべき頂点バッファを一度の drawPrimitive() にまとめる」こと。

それなら、グループの数だけ SceneObject を作っておくのが一番適当なのかも。

あるいは、軌跡を作った ILocus の onDrawSubset で、
この ILocus がグループの先頭だった場合、後続をすべて描画し、
それ以外のは onDrawSubset でも何もしないというか、むしろ isEnable() がfalse 返しておいて、
シェーダへの設定もなにも行わないようにすればよいのではないか。

この方法の場合、一番重要なことは
「設定される頂点位置はワールド変換済み」であること。
または、スプライトと同じように、頂点に行列データを含めてしまう。
これは、先頭の SceneObject で描画を一括して行ってしまうため、
後続のワールド行列が反映されないという現象が出てくるため。





…もうなんかかなり複雑になってきてるし、
シェーダ実装する側もいろいろ覚えておかないとならないけど、
結局大量のエフェクトを出すには一役買ってくれる…と思う。

バッチ系の Renderer はエフェクト専用の特殊なものって方向で。


















LocusRenderer 内での処理…
	







*/

//=============================================================================
//								end of file
//=============================================================================