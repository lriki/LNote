//=============================================================================
//【 GeometryRenderer 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Resource/LNResource.h"
#include "PointParticle.h"
#include "GeometryRenderer.h"

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
// ■ GeometryRenderer クラス
//=============================================================================

    /// 描画用の頂点
    struct VertexPosColorUV
    {
        LVector3	Position;   ///< 座標
        lnU32       Color;      ///< 頂点カラー
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

  /*  /// 描画用の頂点
    struct VertexPosColor
    {
        LVector3	Position;   ///< 座標
        lnU32       Color;      ///< 頂点カラー
        
        /// 頂点レイアウト
        static LNVertexElemenst* elements()
        {
	        static LNVertexElemenst elements[] = {
		        { 0, LN_DECL_FLOAT3, LN_USAGE_POSITION, 0 },
                { 0, LN_DECL_COLOR4, LN_USAGE_COLOR,    0 },
		          LN_DECL_END() };
	        return elements;
        }
    };*/

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    GeometryRenderer::GeometryRenderer()
        : mLogFile              ( NULL )
        , mGraphicsDevice       ( NULL )
        , mRenderer             ( NULL )
        , mDummyTexture         ( NULL )
        , mCurrentShaderPass    ( NULL )
        , mVBPosColor           ( NULL )
        , mVBPosColorUV         ( NULL )
        , mIndexBuffer          ( NULL )
    {
        mShaderParam.Shader = NULL;
        mPointParticleShader.Shader = NULL;
    }

	//---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    GeometryRenderer::~GeometryRenderer()
    {
        LN_SAFE_RELEASE( mDummyTexture );
        LN_SAFE_RELEASE( mShaderParam.Shader );
        LN_SAFE_RELEASE( mPointParticleShader.Shader );
        LN_SAFE_RELEASE( mIndexBuffer );
        LN_SAFE_RELEASE( mVBPosColor );
        LN_SAFE_RELEASE( mVBPosColorUV );
        LN_SAFE_RELEASE( mGraphicsDevice );
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    LNRESULT GeometryRenderer::initialize( const InitData& init_data_ )
    {
        LNRESULT lr;

        mGraphicsDevice = init_data_.GraphicsDevice;
        mRenderer       = mGraphicsDevice->getRenderer();
        mGraphicsAPI    = mGraphicsDevice->getGraphicsAPI();
        LN_SAFE_ADDREF( mGraphicsDevice );

        //-----------------------------------------------------
        // 各描画オブジェクト用の頂点バッファを作る
        //      デバッグ用の物はいろいろな形状に対応できるように、頂点数を多めに取っておく

        LN_CALL_R( mGraphicsDevice->createVertexBuffer( &mVBPosColorUV, Vertex::PosColorUV::elements(), 40, NULL, true ) );

        LN_CALL_R( mGraphicsDevice->createVertexBuffer( &mVBPosColor, Vertex::PosColor::elements(), 100, NULL, true ) );

        LN_CALL_R( mGraphicsDevice->createIndexBuffer( &mIndexBuffer, 200, NULL, true, true ) );

        //-----------------------------------------------------
        // シェーダの作成

        mGraphicsDevice->createShader( &mShaderParam.Shader, _T( "LNResource\\Shader\\GeometryRenderer.lnfx" ) );
        
        mShaderParam.varWorldMatrix     = mShaderParam.Shader->getVariableByName( "gWorldMatrix" );
        mShaderParam.varViewProjMatrix  = mShaderParam.Shader->getVariableByName( "gViewProjMatrix" );
        mShaderParam.varTexture         = mShaderParam.Shader->getVariableByName( "gTexture" );
        mShaderParam.techMainDraw       = mShaderParam.Shader->getTechniqueByIndex( 0 );
        mShaderParam.passP0             = mShaderParam.techMainDraw->getPassByIndex( 0 );


        //-----------------------------------------------------
        // PointSprite シェーダ

        mGraphicsDevice->createShader( &mPointParticleShader.Shader, _T( "LNResource\\Shader\\PointParticle.lnfx" ) );

        mPointParticleShader.varWorldMatrix = mPointParticleShader.Shader->getVariableByName( "gWorldMatrix" );
        mPointParticleShader.varViewProjMatrix = mPointParticleShader.Shader->getVariableByName( "gViewProjMatrix" );
        mPointParticleShader.varProj_11_Div_00 = mPointParticleShader.Shader->getVariableByName( "gProj_11_Div_00" );
        mPointParticleShader.varTime = mPointParticleShader.Shader->getVariableByName( "gTime" );
        mPointParticleShader.varGlobalAccel = mPointParticleShader.Shader->getVariableByName( "gGlobalAccel" );
        mPointParticleShader.varGravityPower = mPointParticleShader.Shader->getVariableByName( "gGravityPower" );
        mPointParticleShader.varIsSequential = mPointParticleShader.Shader->getVariableByName( "gIsSequential" );
        mPointParticleShader.varIsParticleLoop = mPointParticleShader.Shader->getVariableByName( "gIsParticleLoop" );
        mPointParticleShader.varIsDynamic = mPointParticleShader.Shader->getVariableByName( "gIsDynamic" );
        mPointParticleShader.varMaterialTexture = mPointParticleShader.Shader->getVariableByName( "gMaterialTexture" );
        mPointParticleShader.passP0 = mPointParticleShader.Shader->getTechniqueByIndex( 0 )->getPassByIndex( 0 );
       
        //-----------------------------------------------------
        // ダミーテクスチャ

        LN_CALL_R( mGraphicsDevice->createTexture( &mDummyTexture, 32, 32, 1, LN_FMT_A8R8G8B8 ) );
        mDummyTexture->clear( LColor( 1, 1, 1, 1 ) );
        setTexture( mDummyTexture );

        // 単位行列をセットしておく
        setMatrix( LMatrix::IDENTITY );
        setViewProjMatrix( LMatrix::IDENTITY );
        
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 描画パスの開始
    //---------------------------------------------------------------------
    void GeometryRenderer::begin( DrawingPass pass_ )
    {
        switch ( pass_ )
        {
            case PASS_BASIC:
                mCurrentShaderPass = mShaderParam.passP0; 
                break;
            case PASS_POINT_PARTICLE:
                mCurrentShaderPass = mPointParticleShader.passP0;
                break;
        }
        
        mCurrentShaderPass->begin();
    }

    //---------------------------------------------------------------------
    // ● 描画パスの終了
    //---------------------------------------------------------------------
    void GeometryRenderer::end()
    {
        if ( mCurrentShaderPass )
        {
            mCurrentShaderPass->end();
            mCurrentShaderPass = NULL;
        }
    }

    //---------------------------------------------------------------------
    // ● 座標変換行列の設定
    //---------------------------------------------------------------------
    void GeometryRenderer::setMatrix( const LMatrix& matrix_ )
    {
        mShaderParam.varWorldMatrix->setMatrix( matrix_ );
        if ( mCurrentShaderPass )
        {
            mCurrentShaderPass->commit();
        }
    }

    //---------------------------------------------------------------------
    // ● 座標変換行列の設定
    //---------------------------------------------------------------------
    void GeometryRenderer::setViewProjMatrix( const LMatrix& matrix_ )
    {
        mShaderParam.varViewProjMatrix->setMatrix( matrix_ );
        if ( mCurrentShaderPass )
        {
            mCurrentShaderPass->commit();
        }
    }

    //---------------------------------------------------------------------
    // ● ビューサイズの設定
    //---------------------------------------------------------------------
    void GeometryRenderer::setViewSize( const LVector2& view_size_ )
    {
        LMatrix mat;
        LMatrix::perspective2DLH(
            &mat, 
            view_size_.x,
            view_size_.y,
            0.0f,
            10000.0f );
        setViewProjMatrix( mat );
    }

    //---------------------------------------------------------------------
    // ● テクスチャの設定
    //---------------------------------------------------------------------
    void GeometryRenderer::setTexture( ITexture* texture_ )
    {
        if ( texture_ )
        {
            mShaderParam.varTexture->setTexture( texture_ );
        }
        else
        {
            mShaderParam.varTexture->setTexture( mDummyTexture );
        }
    }


    //---------------------------------------------------------------------
    // ● 四角形の描画
    //---------------------------------------------------------------------
    LNRESULT GeometryRenderer::drawSquare(
        lnfloat x0_, lnfloat y0_, lnU32 c0_, lnfloat u0_, lnfloat v0_, 
	    lnfloat x1_, lnfloat y1_, lnU32 c1_, lnfloat u1_, lnfloat v1_,
	    lnfloat x2_, lnfloat y2_, lnU32 c2_, lnfloat u2_, lnfloat v2_,
	    lnfloat x3_, lnfloat y3_, lnU32 c3_, lnfloat u3_, lnfloat v3_ )
    {
        LNRESULT lr;

        Vertex::PosColorUV* v = (Vertex::PosColorUV*)mVBPosColorUV->lock();
        v[ 0 ].Position.set( x0_, y0_, 0.0f );
        v[ 0 ].Color = c0_;
        v[ 0 ].TexUV.set( u0_, v0_ );
        v[ 1 ].Position.set( x1_, y1_, 0.0f );
        v[ 1 ].Color = c1_;
        v[ 1 ].TexUV.set( u1_, v1_ );
        v[ 2 ].Position.set( x2_, y2_, 0.0f );
        v[ 2 ].Color = c2_;
        v[ 2 ].TexUV.set( u2_, v2_ );
        v[ 3 ].Position.set( x3_, y3_, 0.0f );
        v[ 3 ].Color = c3_;
        v[ 3 ].TexUV.set( u3_, v3_ );
        mVBPosColorUV->unlock();

        LN_CALL_R( mRenderer->setVertexBuffer( mVBPosColorUV, true ) );
        LN_CALL_R( mRenderer->drawPrimitive( LN_PRIM_TRIANGLESTRIP, 0, 2 ) );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 矩形の描画     
    //---------------------------------------------------------------------
    LNRESULT GeometryRenderer::drawRect( const Geometry::RectF& dest_rect_, const Geometry::RectF& src_rect_, lnU32 color_ )
    {
        return drawSquare(
            (lnfloat)dest_rect_.x               , (lnfloat)dest_rect_.y,                color_, (lnfloat)src_rect_.x,               (lnfloat)src_rect_.y,
            (lnfloat)dest_rect_.x + dest_rect_.w, (lnfloat)dest_rect_.y,                color_, (lnfloat)src_rect_.x + src_rect_.w, (lnfloat)src_rect_.y,
            (lnfloat)dest_rect_.x               , (lnfloat)dest_rect_.y + dest_rect_.h, color_, (lnfloat)src_rect_.x,               (lnfloat)src_rect_.y + src_rect_.h,
            (lnfloat)dest_rect_.x + dest_rect_.w, (lnfloat)dest_rect_.y + dest_rect_.h, color_, (lnfloat)src_rect_.x + src_rect_.w, (lnfloat)src_rect_.y + src_rect_.h );
    }


    //---------------------------------------------------------------------
    // ● 線分の描画( 単色)
    //---------------------------------------------------------------------
    LNRESULT GeometryRenderer::drawLine( const LVector3& from_, const LVector3& to_, const LColor& color_ )
    {
        return drawLine( from_, to_, color_, color_ );
    }

    //---------------------------------------------------------------------
    // ● 線分の描画( 先端の色指定)
    //---------------------------------------------------------------------
    LNRESULT GeometryRenderer::drawLine( const LVector3& from_, const LVector3& to_, const LColor& from_color_, const LColor& to_color_ )
    {
        LNRESULT lr;

        Vertex::PosColor* v = (Vertex::PosColor*)mVBPosColor->lock();
        v[ 0 ].Position = from_;
        v[ 0 ].Color = from_color_.to32Bit( mGraphicsAPI );
        v[ 1 ].Position = to_;
        v[ 1 ].Color = to_color_.to32Bit( mGraphicsAPI );
        mVBPosColor->unlock();
        
        LN_CALL_R( mRenderer->setVertexBuffer( mVBPosColor, true ) );
        LN_CALL_R( mRenderer->drawPrimitive( LN_PRIM_LINELIST, 0, 1 ) );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 球の描画
    //---------------------------------------------------------------------
    LNRESULT GeometryRenderer::drawSphere( const LVector3& pos_, lnfloat radius_, const LColor& color_ )
    {
        LNRESULT lr;
        lnfloat r  = radius_;
        lnfloat r1 = r * 1.41421356f / 2;
        lnfloat r2 = r / 2;					// X Z 平面の
        u32     c  = color_.to32Bit( mGraphicsAPI );

        // ※ 右手用
        Vertex::PosColor vertices[] = 
        {
            { LVector3( 0.0f, r, 0.0f ), c },
            { LVector3( r2, r1, r2 ), c },
            { LVector3( 0.0f, r1, r1 ), c },
            { LVector3( 0.0f, r, 0.0f ), c },
            { LVector3( r1, r1, 0.0f ), c },
            { LVector3( 0.0f, r, 0.0f ), c },
            { LVector3( r2, r1, -r2 ), c },
            { LVector3( 0.0f, r, 0.0f ), c },
            { LVector3( 0.0f, r1, -r1 ), c },
            { LVector3( 0.0f, r, 0.0f ), c },
            { LVector3( -r2, r1, -r2 ), c },
            { LVector3( 0.0f, r, 0.0f ), c },
            { LVector3( -r1, r1, 0.0f ), c },
            { LVector3( 0.0f, r, 0.0f ), c },
            { LVector3( -r2, r1, r2 ), c },
            { LVector3( 0.0f, r, 0.0f ), c },
            { LVector3( 0.0f, r1, r1 ), c },
            { LVector3( 0.0f, -r1, r1 ), c },
            { LVector3( r2, -r1, r2 ), c },
            { LVector3( 0.0f, -r, 0.0f ), c },
            { LVector3( r1, -r1, 0.0f ), c },
            { LVector3( 0.0f, -r, 0.0f ), c },
            { LVector3( r2, -r1, -r2 ), c },
            { LVector3( 0.0f, -r, 0.0f ), c },
            { LVector3( 0.0f, -r1, -r1 ), c },
            { LVector3( 0.0f, -r, 0.0f ), c },
            { LVector3( -r2, -r1, -r2 ), c },
            { LVector3( 0.0f, -r, 0.0f ), c },
            { LVector3( -r1, -r1, 0.0f ), c },
            { LVector3( 0.0f, -r, 0.0f ), c },
            { LVector3( -r2, -r1, r2 ), c },
            { LVector3( 0.0f, -r, 0.0f ), c },
            { LVector3( 0.0f, -r1, r1 ), c },
            { LVector3( 0.0f, -r, 0.0f ), c },
            { LVector3( r1, 0.0f, r1 ), c },
            { LVector3( 0.0f, 0.0f, r ), c },
            { LVector3( r, 0.0f, 0.0f ), c },
            { LVector3( r1, 0.0f, -r1 ), c },
            { LVector3( 0.0f, 0.0f, -r ), c },
            { LVector3( -r1, 0.0f, -r1 ), c },
            { LVector3( -r, 0.0f, 0.0f ), c },
            { LVector3( -r1, 0.0f, r1 ), c },
            { LVector3( 0.0f, 0.0f, r ), c }
        };

        // ※ 右手用
        lnU16 indices[] =  
        {
            2,1,0,
            1,4,3,
            4,6,5,
            6,8,7,
            8,10,9,
            10,12,11,
            12,14,13,
            14,16,15,
            19,18,17,
            21,20,18,
            23,22,20,
            25,24,22,
            27,26,24,
            29,28,26,
            31,30,28,
            33,32,30,
            35,34,1,
            2,35,1,
            34,36,4,
            1,34,4,
            36,37,6,
            4,36,6,
            37,38,8,
            6,37,8,
            38,39,10,
            8,38,10,
            39,40,12,
            10,39,12,
            40,41,14,
            12,40,14,
            41,42,16,
            14,41,16,
            17,18,34,
            35,17,34,
            18,20,36,
            34,18,36,
            20,22,37,
            36,20,37,
            22,24,38,
            37,22,38,
            24,26,39,
            38,24,39,
            26,28,40,
            39,26,40,
            28,30,41,
            40,28,41,
            30,32,42,
            41,30,42
        };

        Vertex::PosColor* v = (Vertex::PosColor*)mVBPosColor->lock();
        memcpy( v, vertices, sizeof( vertices ) );
        mVBPosColor->unlock();

        lnU16* i = (lnU16*)mIndexBuffer->lock();
        memcpy( i, indices, sizeof( indices ) );
        mIndexBuffer->unlock();
 
        LN_CALL_R( mRenderer->setVertexBuffer( mVBPosColor, true ) );
        LN_CALL_R( mRenderer->setIndexBuffer( mIndexBuffer, true ) );
        LN_CALL_R( mRenderer->drawPrimitiveIndexed( LN_PRIM_TRIANGLELIST, 0, 48 ) );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 三角形の描画
    //---------------------------------------------------------------------
    LNRESULT GeometryRenderer::drawTriangle( const LVector3& v0_, const LVector3& v1_, const LVector3& v2_, const LColor& color_ )
    {
        LN_PRINT_NOT_IMPL_FUNCTION;
        return LN_OK;
    }

   
    //---------------------------------------------------------------------
    // ● 座標軸の描画
    //---------------------------------------------------------------------
    LNRESULT GeometryRenderer::drawTransformAxis( lnfloat ortho_len_ )
    {
        LNRESULT lr;
        LN_CALL_R( drawLine( LVector3::ZERO, LVector3( ortho_len_, 0, 0 ), LColor::RED ) );
        LN_CALL_R( drawLine( LVector3::ZERO, LVector3( 0, ortho_len_, 0 ), LColor::GREEN ) );
        LN_CALL_R( drawLine( LVector3::ZERO, LVector3( 0, 0, ortho_len_ ), LColor::BLUE ) );
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● ボックスの描画
    //---------------------------------------------------------------------
    LNRESULT GeometryRenderer::drawBox( const LVector3& bb_min_, const LVector3& bb_max_, const LColor& color_ )
    {
        LNRESULT lr;
        lnU32 c = color_.to32Bit( mGraphicsAPI );

        // ※ 右手用
        Vertex::PosColor vertices[] = 
        {
            { LVector3( bb_max_.x, bb_max_.y, bb_max_.z ), c }, // [0] 
            { LVector3( bb_max_.x, bb_min_.y, bb_max_.z ), c },
            { LVector3( bb_min_.x, bb_min_.y, bb_max_.z ), c },
            { LVector3( bb_min_.x, bb_max_.y, bb_max_.z ), c },
            { LVector3( bb_max_.x, bb_max_.y, bb_min_.z ), c },
            { LVector3( bb_max_.x, bb_min_.y, bb_min_.z ), c },
            { LVector3( bb_max_.x, bb_min_.y, bb_max_.z ), c },
            { LVector3( bb_max_.x, bb_max_.y, bb_max_.z ), c },
            { LVector3( bb_min_.x, bb_max_.y, bb_min_.z ), c },
            { LVector3( bb_min_.x, bb_min_.y, bb_min_.z ), c },
            { LVector3( bb_max_.x, bb_min_.y, bb_min_.z ), c },
            { LVector3( bb_max_.x, bb_max_.y, bb_min_.z ), c },
            { LVector3( bb_min_.x, bb_max_.y, bb_max_.z ), c },
            { LVector3( bb_min_.x, bb_min_.y, bb_max_.z ), c },
            { LVector3( bb_min_.x, bb_min_.y, bb_min_.z ), c },
            { LVector3( bb_min_.x, bb_max_.y, bb_min_.z ), c },
            { LVector3( bb_max_.x, bb_max_.y, bb_max_.z ), c },
            { LVector3( bb_min_.x, bb_max_.y, bb_max_.z ), c },
            { LVector3( bb_max_.x, bb_min_.y, bb_max_.z ), c },
            { LVector3( bb_min_.x, bb_min_.y, bb_max_.z ), c }, // [19]
        };  

        // ※ 右手用
        lnU16 indices[] =  
        {
            2,1,0,
            3,2,0,
            6,5,4,
            7,6,4,
            10,9,8,
            11,10,8,
            14,13,12,
            15,14,12,
            17,16,4,
            15,17,4,
            14,5,18,
            19,14,18    // 36
        };

        Vertex::PosColor* v = (Vertex::PosColor*)mVBPosColor->lock();
        memcpy( v, vertices, sizeof( vertices ) );
        mVBPosColor->unlock();

        lnU16* i = (lnU16*)mIndexBuffer->lock();
        memcpy( i, indices, sizeof( indices ) );
        mIndexBuffer->unlock();
 
        LN_CALL_R( mRenderer->setVertexBuffer( mVBPosColor, true ) );
        LN_CALL_R( mRenderer->setIndexBuffer( mIndexBuffer, true ) );
        LN_CALL_R( mRenderer->drawPrimitiveIndexed( LN_PRIM_TRIANGLELIST, 0, 12 ) );

        return LN_OK;
    }
    
    //---------------------------------------------------------------------
    // ● カプセルの描画
    //---------------------------------------------------------------------
    LNRESULT GeometryRenderer::drawCapsule( lnfloat radius_, lnfloat half_height_, const LColor& color_ )
    {
        LNRESULT lr;
        lnU32 c = color_.to32Bit( mGraphicsAPI );
        lnfloat hh  = half_height_ + radius_;   // 全体 half height
        lnfloat hsh = hh * 0.853555f;	        // 半球の中央
        lnfloat chh = hh / 2;			        // Cylinder half height
        lnfloat r   = radius_;
        lnfloat hr  = r / 2;
        lnfloat hr2 = r * 0.70712f;

        // ※ 右手用
        Vertex::PosColor vertices[] = 
        {
            { LVector3( 0.0f, hh, 0.0f ), c },  // [0]
            { LVector3( hr, hsh, hr ), c },
            { LVector3( 0.0f, hsh, hr2 ), c },
            { LVector3( 0.0f, hh, 0.0f ), c },
            { LVector3( hr2, hsh, 0.0f ), c },
            { LVector3( 0.0f, hh, 0.0f ), c },
            { LVector3( hr, hsh, -hr ), c },
            { LVector3( 0.0f, hh, 0.0f ), c },
            { LVector3( 0.0f, hsh, -hr2 ), c },
            { LVector3( 0.0f, hh, 0.0f ), c },
            { LVector3( -hr, hsh, -hr ), c },
            { LVector3( 0.0f, hh, 0.0f ), c },
            { LVector3( -hr2, hsh, 0.0f ), c },
            { LVector3( 0.0f, hh, 0.0f ), c },
            { LVector3( -hr, hsh, hr ), c },
            { LVector3( 0.0f, hh, 0.0f ), c },
            { LVector3( 0.0f, hsh, hr2 ), c },
            { LVector3( 0.0f, -hsh, hr2 ), c },
            { LVector3( hr, -hsh, hr ), c },
            { LVector3( 0.0f, -hh, 0.0f ), c },
            { LVector3( hr2, -hsh, 0.0f ), c },
            { LVector3( 0.0f, -hh, 0.0f ), c },
            { LVector3( hr, -hsh, -hr ), c },
            { LVector3( 0.0f, -hh, 0.0f ), c },
            { LVector3( 0.0f, -hsh, -hr2 ), c },
            { LVector3( 0.0f, -hh, 0.0f ), c },
            { LVector3( -hr, -hsh, -hr ), c },
            { LVector3( 0.0f, -hh, 0.0f ), c },
            { LVector3( -hr2, -hsh, 0.0f ), c },
            { LVector3( .0f, -hh, 0.0f ), c },
            { LVector3( -hr, -hsh, hr ), c },
            { LVector3( 0.0f, -hh, 0.0f ), c },
            { LVector3( 0.0f, -hsh, hr2 ), c },
            { LVector3( 0.0f, -hh, 0.0f ), c },
            { LVector3( hr2, chh, hr2 ), c },
            { LVector3( hr2, -chh, hr2 ), c },
            { LVector3( 0.0f, -chh, r ), c },
            { LVector3( 0.0f, chh, r ), c },
            { LVector3( r, chh, 0.0f ), c },
            { LVector3( r, -chh, 0.0f ), c },
            { LVector3( hr2, chh, -hr2 ), c },
            { LVector3( hr2, -chh, -hr2 ), c },
            { LVector3( 0.0f, chh, -r ), c },
            { LVector3( 0.0f, -chh, -r ), c },
            { LVector3( -hr2, chh, -hr2 ), c },
            { LVector3( -hr2, -chh, -hr2 ), c },
            { LVector3( -r, chh, 0.0f ), c },
            { LVector3( -r, -chh, 0.0f ), c },
            { LVector3( -hr2, chh, hr2 ), c },
            { LVector3( -hr2, -chh, hr2 ), c },
            { LVector3( 0.0f, chh, r ), c },
            { LVector3( 0.0f, -chh, r ), c },
            { LVector3( hr2, chh, hr2 ), c },
            { LVector3( 0.0f, chh, r ), c },
            { LVector3( r, chh, 0.0f ), c },
            { LVector3( hr2, chh, -hr2 ), c },
            { LVector3( 0.0f, chh, -r ), c },
            { LVector3( -hr2, chh, -hr2 ), c },
            { LVector3( -r, chh, 0.0f ), c },
            { LVector3( -hr2, chh, hr2 ), c },
            { LVector3( 0.0f, chh, r ), c },
            { LVector3( hr2, -chh, hr2 ), c },
            { LVector3( 0.0f, -chh, r ), c },
            { LVector3( r, -chh, 0.0f ), c },
            { LVector3( hr2, -chh, -hr2 ), c },
            { LVector3( 0.0f, -chh, -r ), c },
            { LVector3( -hr2, -chh, -hr2 ), c },
            { LVector3( -r, -chh, 0.0f ), c },
            { LVector3( -hr2, -chh, hr2 ), c },
            { LVector3( 0.0f, -chh, r ), c }        // [69]
        };

        // ※ 右手用
        lnU16 indices[] =  
        {
            2,1,0,
            1,4,3,
            4,6,5,
            6,8,7,
            8,10,9,
            10,12,11,
            12,14,13,
            14,16,15,
            19,18,17,
            21,20,18,
            23,22,20,
            25,24,22,
            27,26,24,
            29,28,26,
            31,30,28,
            33,32,30,
            36,35,34,
            37,36,34,
            35,39,38,
            34,35,38,
            39,41,40,
            38,39,40,
            41,43,42,
            40,41,42,
            43,45,44,
            42,43,44,
            45,47,46,
            44,45,46,
            47,49,48,
            46,47,48,
            49,51,50,
            48,49,50,
            53,52,1,
            2,53,1,
            52,54,4,
            1,52,4,
            54,55,6,
            4,54,6,
            55,56,8,
            6,55,8,
            56,57,10,
            8,56,10,
            57,58,12,
            10,57,12,
            58,59,14,
            12,58,14,
            59,60,16,
            14,59,16,
            17,18,61,
            62,17,61,
            18,20,63,
            61,18,63,
            20,22,64,
            63,20,64,
            22,24,65,
            64,22,65,
            24,26,66,
            65,24,66,
            26,28,67,
            66,26,67,
            28,30,68,
            67,28,68,
            30,32,69,
            68,30,69
        };

        Vertex::PosColor* v = (Vertex::PosColor*)mVBPosColor->lock();
        memcpy( v, vertices, sizeof( vertices ) );
        mVBPosColor->unlock();

        lnU16* i = (lnU16*)mIndexBuffer->lock();
        memcpy( i, indices, sizeof( indices ) );
        mIndexBuffer->unlock();
 
        LN_CALL_R( mRenderer->setVertexBuffer( mVBPosColor, true ) );
        LN_CALL_R( mRenderer->setIndexBuffer( mIndexBuffer, true ) );
        LN_CALL_R( mRenderer->drawPrimitiveIndexed( LN_PRIM_TRIANGLELIST, 0, 64 ) );

        return LN_OK;
    }
    
    //---------------------------------------------------------------------
    // ● シリンダーの描画
    //---------------------------------------------------------------------
    LNRESULT GeometryRenderer::drawCylinder( lnfloat radius_, lnfloat half_height__, const LVector3& color_ )
    {
        return LN_OK;
    }
    
    //---------------------------------------------------------------------
    // ● コーンの描画
    //---------------------------------------------------------------------
    LNRESULT GeometryRenderer::drawCone( lnfloat radius_, lnfloat half_height_, const LVector3& color_ )
    {
        return LN_OK;
    }
    
    //---------------------------------------------------------------------
    // ● 平面の描画
    //---------------------------------------------------------------------
    LNRESULT GeometryRenderer::drawPlane( const LVector3& normal_, lnfloat plane_const_, const LVector3& color_ )
    {
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 視錐台の描画 (座標変換済みとして描画する。setMatrix() には単位行列を渡しておくこと)
    //---------------------------------------------------------------------
    void GeometryRenderer::drawViewFrustum( const LViewFrustum& frustum_, const LVector3& vp_, lnU32 color_ )
    {
        LVector3* np = (LVector3*)frustum_.NearClip;
        LVector3* fp = (LVector3*)frustum_.FarClip;
        u32 c = color_;

        //using namespace Math;

        // ライン用
        Vertex::PosColor vertices[] =
        {
            // 近い方の面
            { LVector3( np[ 0 ].x, np[ 0 ].y, np[ 0 ].z ), c },	// 0
            { LVector3( np[ 1 ].x, np[ 1 ].y, np[ 1 ].z ), c },
            { LVector3( np[ 2 ].x, np[ 2 ].y, np[ 2 ].z ), c },
            { LVector3( np[ 3 ].x, np[ 3 ].y, np[ 3 ].z ), c },
            { LVector3( np[ 0 ].x, np[ 0 ].y, np[ 0 ].z ), c },

            // 遠い方の面
            { LVector3( fp[ 0 ].x, fp[ 0 ].y, fp[ 0 ].z ), c },	// 5
            { LVector3( fp[ 1 ].x, fp[ 1 ].y, fp[ 1 ].z ), c },
            { LVector3( fp[ 2 ].x, fp[ 2 ].y, fp[ 2 ].z ), c },
            { LVector3( fp[ 3 ].x, fp[ 3 ].y, fp[ 3 ].z ), c },
            { LVector3( fp[ 0 ].x, fp[ 0 ].y, fp[ 0 ].z ), c },

            // 視点から遠い方の面までの線
            { LVector3( fp[ 0 ].x, fp[ 0 ].y, fp[ 0 ].z ), c },	// 10
            { LVector3(     vp_.x,     vp_.y,     vp_.z ), c },
            { LVector3( fp[ 1 ].x, fp[ 1 ].y, fp[ 1 ].z ), c },

            { LVector3( fp[ 2 ].x, fp[ 2 ].y, fp[ 2 ].z ), c },  // 13
            { LVector3(     vp_.x,     vp_.y,     vp_.z ), c },
            { LVector3( fp[ 3 ].x, fp[ 3 ].y, fp[ 3 ].z ), c },
        };

        Vertex::PosColor* v = (Vertex::PosColor*)mVBPosColor->lock();
        memcpy( v, vertices, sizeof( vertices ) );
        mVBPosColor->unlock();

        mRenderer->setVertexBuffer( mVBPosColor );
        mRenderer->drawPrimitive( LN_PRIM_LINESTRIP, 0, 4 );
        mRenderer->drawPrimitive( LN_PRIM_LINESTRIP, 5, 4 );
        mRenderer->drawPrimitive( LN_PRIM_LINESTRIP, 10, 2 );
        mRenderer->drawPrimitive( LN_PRIM_LINESTRIP, 13, 2 );

    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void GeometryRenderer::drawPointParticle(
        const LMatrix& world_,
        const LMatrix& porj_,
        const LMatrix& view_porj_,
        const LVector4& camera_pos_,
        PointParticle* particle_ )
    {
        if ( particle_->mTexture )
        {
            mPointParticleShader.varWorldMatrix->setMatrix( world_ );
            mPointParticleShader.varViewProjMatrix->setMatrix( view_porj_ );
            mPointParticleShader.varProj_11_Div_00->setFloat( porj_.m11 / porj_.m00 );
            mPointParticleShader.Shader->getVariableByName("gCameraPosition")->setVector(camera_pos_);

            mPointParticleShader.varTime->setFloat( particle_->mTime * particle_->mTimeTick );
            mPointParticleShader.varGlobalAccel->setVector( particle_->mGlobalAccel );
            mPointParticleShader.varGravityPower->setFloat( particle_->mGravityPower );
            mPointParticleShader.varIsSequential->setBool( particle_->mIsSequential );
            mPointParticleShader.varIsParticleLoop->setBool( particle_->mIsParticleLoop );
            mPointParticleShader.varIsDynamic->setBool( particle_->mIsDynamic );
            mPointParticleShader.varMaterialTexture->setTexture( particle_->mTexture );



            begin(PASS_POINT_PARTICLE);

            particle_->draw();

            end();
        }
    }

/*
    //---------------------------------------------------------------------
    // ● 軸の描画
    //---------------------------------------------------------------------
    LNRESULT GeometryRenderer::drawAxis( lnfloat length_ )
    {
        LNRESULT lr;

        VertexPosColor* v = (VertexPosColor*)mVBPosColor->lock();
        v[ 0 ].Position.set( 0.0f, 0.0f, 0.0f );
        v[ 0 ].Color = 0xffff0000;
        v[ 1 ].Position.set( length_, 0.0f, 0.0f );
        v[ 1 ].Color = 0xffff0000;

        v[ 2 ].Position.set( 0.0f, 0.0f, 0.0f );
        v[ 2 ].Color = 0xff00ff00;
        v[ 3 ].Position.set( 0.0f, length_, 0.0f );
        v[ 3 ].Color = 0xff00ff00;

        v[ 4 ].Position.set( 0.0f, 0.0f, 0.0f );
        v[ 4 ].Color = 0xff0000ff;
        v[ 5 ].Position.set( 0.0f, 0.0f, length_ );
        v[ 5 ].Color = 0xff0000ff;
        mVBPosColor->unlock();
        
        LN_CALL_R( mRenderer->setVertexBuffer( mVBPosColor, true ) );
        LN_CALL_R( mRenderer->drawPrimitive( LN_PRIM_LINELIST, 0, 3 ) );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 四角形の描画
    //---------------------------------------------------------------------
    LNRESULT GeometryRenderer::drawSquare(
        lnfloat x0_, lnfloat y0_, u32 c0_, lnfloat u0_, lnfloat v0_, 
	    lnfloat x1_, lnfloat y1_, u32 c1_, lnfloat u1_, lnfloat v1_,
	    lnfloat x2_, lnfloat y2_, u32 c2_, lnfloat u2_, lnfloat v2_,
	    lnfloat x3_, lnfloat y3_, u32 c3_, lnfloat u3_, lnfloat v3_ )
    {
        LNRESULT lr;

        VertexPosColorUV* v = (VertexPosColorUV*)mVBPosColorUV->lock();
        v[ 0 ].Position.set( x0_, y0_, 0.0f );
        v[ 0 ].Color = c0_;
        v[ 0 ].TexUV.set( u0_, v0_ );
        v[ 1 ].Position.set( x1_, y1_, 0.0f );
        v[ 1 ].Color = c1_;
        v[ 1 ].TexUV.set( u1_, v1_ );
        v[ 2 ].Position.set( x2_, y2_, 0.0f );
        v[ 2 ].Color = c2_;
        v[ 2 ].TexUV.set( u2_, v2_ );
        v[ 3 ].Position.set( x3_, y3_, 0.0f );
        v[ 3 ].Color = c3_;
        v[ 3 ].TexUV.set( u3_, v3_ );
        mVBPosColorUV->unlock();

        LN_CALL_R( mRenderer->setVertexBuffer( mVBPosColorUV, true ) );
        LN_CALL_R( mRenderer->drawPrimitive( LN_PRIM_TRIANGLESTRIP, 0, 2 ) );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 矩形の描画     
    //---------------------------------------------------------------------
    LNRESULT GeometryRenderer::drawRect( const LRectF& dest_rect_, const LRectF& src_rect_, u32 color_ )
    {
        return drawSquare(
            dest_rect_.x               , dest_rect_.y,                color_, src_rect_.x,               src_rect_.y,
            dest_rect_.x + dest_rect_.w, dest_rect_.y,                color_, src_rect_.x + src_rect_.w, src_rect_.y,
            dest_rect_.x               , dest_rect_.y + dest_rect_.h, color_, src_rect_.x,               src_rect_.y + src_rect_.h,
            dest_rect_.x + dest_rect_.w, dest_rect_.y + dest_rect_.h, color_, src_rect_.x + src_rect_.w, src_rect_.y + src_rect_.h );
    }
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