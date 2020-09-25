//=============================================================================
//【 RingRenderer 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Manager.h"
#include "RingRenderer.h"

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
// ■ RingRenderer
//=============================================================================

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    RingRenderer::RingRenderer( Manager* manager_ )
        : mManager          ( manager_ )
        , mGraphicsDevice   ( mManager->getGraphicsDevice() )
        , mGraphicsAPI      ( mGraphicsDevice->getGraphicsAPI() )
        , mVertexBuffer     ( NULL )
        , mIndexBuffer      ( NULL )
        , mTexture          ( NULL )
        , mLockedBuffer     ( NULL )
        , mUsingVertexCount ( 0 )
    {
        
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    RingRenderer::~RingRenderer()
    {
        LN_SAFE_RELEASE( mVertexBuffer );
        LN_SAFE_RELEASE( mIndexBuffer );
        LN_SAFE_RELEASE( mTexture );
        LN_SAFE_RELEASE( mShaderParam.Shader );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void RingRenderer::initialize( int square_max_count_, bool is_3d_ )
    {
        //-----------------------------------------------------
        // 頂点バッファ

        mGraphicsDevice->createVertexBuffer(
            &mVertexBuffer, Vertex::elements(), square_max_count_ * 4, NULL, true );

        mGraphicsDevice->createIndexBuffer(
            &mIndexBuffer, square_max_count_ * 6, NULL, true, true );

        u16* ib = (lnU16*)mIndexBuffer->lock();
        int idx = 0;
        int i2 = 0;
        for ( int i = 0; i < square_max_count_; ++i )
        {
            i2  = i * 6;
            idx = i * 4;
            ib[ i2 + 5 ] = idx;
            ib[ i2 + 4 ] = idx + 1;
            ib[ i2 + 3 ] = idx + 2;
            ib[ i2 + 2 ] = idx + 2;
            ib[ i2 + 1 ] = idx + 1;
            ib[ i2 + 0 ] = idx + 3;
        }
        mIndexBuffer->unlock();


        //-----------------------------------------------------
        // シェーダ
        
        mGraphicsDevice->createShader(
            &mShaderParam.Shader,
            _T( "LNResource\\Shader\\SpriteRenderer.lnfx" ) );

        mShaderParam.varTexture         = mShaderParam.Shader->getVariableByName( "gMaterialTexture" );
        mShaderParam.varViewProjMatrix  = mShaderParam.Shader->getVariableByName( "gViewProjMatrix" );
        mShaderParam.techMainDraw       = mShaderParam.Shader->getTechniqueByName( "MainDraw" );

        setTexture( NULL );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void RingRenderer::setViewProjMatrix( const LMatrix& view_, const LMatrix& proj_ )
    {
        mViewDirection.set( view_.m02, view_.m12, view_.m22 );
        LMatrix::inverse( &mViewInverseMatrix, view_ );
        mViewPosition = mViewInverseMatrix.getPosition();
        mViewProjMatrix = ( view_ * proj_ );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void RingRenderer::begin()
    {
        mLockedBuffer = (Vertex*)mVertexBuffer->lock();
        mUsingVertexCount = 0;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void RingRenderer::end()
    {
        mVertexBuffer->unlock();
        mLockedBuffer = NULL;

        if ( mUsingVertexCount == 0 ) return;

        //-----------------------------------------------------
        // 描画

        IRenderer* r = mGraphicsDevice->getRenderer();
        r->setVertexBuffer( mVertexBuffer, true );
        r->setIndexBuffer( mIndexBuffer );
        mShaderParam.varViewProjMatrix->setMatrix( mViewProjMatrix );
        mShaderParam.varTexture->setTexture( mTexture );

        IShaderPass* pass = mShaderParam.techMainDraw->getPassByIndex( 0 );
       
        pass->begin();
        r->drawPrimitiveIndexed( LN_PRIM_TRIANGLELIST, 0, mUsingVertexCount / 2 );
        pass->end();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void RingRenderer::setTexture( ITexture* texture_ )
    {
        LN_REFOBJ_SET( mTexture, texture_ );

        if ( !mTexture )
        {
            mTexture = mManager->getDummyTexture();
            LN_SAFE_ADDREF( mTexture );
        }
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void RingRenderer::drawRequest3D( const DrawingParamater& param_ )
    {
        if ( mUsingVertexCount >= mVertexBuffer->getVertexNum() ) return;

        const lnfloat radian      = param_.ViewingAngle / 180.0f * 3.141592f;
        const lnfloat step_angle  = radian / (param_.DivisionCount);
	    const lnfloat begin_angle = -radian / 2.0f;

        const lnfloat center_radius = param_.InnerRadius + (param_.OuterRadius - param_.InnerRadius) * param_.CenterRatio;
        const lnfloat center_height = param_.InnerHeight + (param_.OuterHeight - param_.InnerHeight) * param_.CenterRatio;

        lnU32 outer_color = param_.OuterColor.to32Bit( mGraphicsAPI );
        lnU32 inner_color = param_.InnerColor.to32Bit( mGraphicsAPI );
        lnU32 center_color = param_.CenterColor.to32Bit( mGraphicsAPI );

        const lnfloat step_c = cosf( step_angle );
	    const lnfloat step_s = sinf( step_angle );
	    lnfloat c = cosf( begin_angle );
	    lnfloat s = sinf( begin_angle );

        LVector3 outer_current( s * param_.OuterRadius, param_.OuterHeight, c * param_.OuterRadius );
	    LVector3 inner_current( s * param_.InnerRadius, param_.InnerHeight, c * param_.InnerRadius );
	    LVector3 center_current( s * center_radius, center_height, c * center_radius );

        lnfloat tex_current = param_.UV.x;
	    const lnfloat tex_step = param_.UV.w / param_.DivisionCount;
	    const lnfloat v1 = param_.UV.y;
	    const lnfloat v2 = v1 + param_.UV.h * 0.5f;
	    const lnfloat v3 = v1 + param_.UV.h;
        
        LVector3 outer_next, inner_next, center_next;
	    lnfloat tex_next;

        lnU32 vertex_count = param_.DivisionCount * 8;  // 四角形 2 つ分
        Vertex* verteies = &mLockedBuffer[mUsingVertexCount];
        for( lnU32 i = 0; i < vertex_count; i += 8 )
	    {
            lnfloat t;
		    t = c * step_c - s * step_s;
		    s = s * step_c + c * step_s;
		    c = t;

            outer_next.set( s * param_.OuterRadius, param_.OuterHeight, c * param_.OuterRadius );
            inner_next.set( s * param_.InnerRadius, param_.InnerHeight, c * param_.InnerRadius );
            center_next.set( s * center_radius, center_height, c * center_radius );
		    tex_next = tex_current + tex_step;

            Vertex* v = &verteies[i];
		    v[0].Position = outer_current;
		    v[0].Color = outer_color;
            v[0].TexUV.x = tex_current;
		    v[0].TexUV.y = v1;

		    v[1].Position = center_current;
		    v[1].Color = center_color;
		    v[1].TexUV.x = tex_current;
		    v[1].TexUV.y = v2;

		    v[2].Position = outer_next;
		    v[2].Color = outer_color;
		    v[2].TexUV.x = tex_next;
		    v[2].TexUV.y = v1;
    		
		    v[3].Position = center_next;
		    v[3].Color = center_color;
		    v[3].TexUV.x = tex_next;
		    v[3].TexUV.y = v2;

		    v[4].Position = center_current;
		    v[4].Color = center_color;
		    v[4].TexUV.x = tex_current;
		    v[4].TexUV.y = v2;

		    v[5].Position = inner_current;
		    v[5].Color = inner_color;
		    v[5].TexUV.x = tex_current;
		    v[5].TexUV.y = v3;

		    v[6].Position = center_next;
		    v[6].Color = center_color;
		    v[6].TexUV.x = tex_next;
		    v[6].TexUV.y = v2;

		    v[7].Position = inner_next;
		    v[7].Color = inner_color;
		    v[7].TexUV.x = tex_next;
		    v[7].TexUV.y = v3;

            outer_current = outer_next;
		    inner_current = inner_next;
		    center_current = center_next;
		    tex_current = tex_next;
        }

        LMatrix mat;
        mat.setRotateMatrix( param_.Transform );

        // ビルボード 
        if ( param_.BillboardType == LN_BILLBOARD_FRONT )
        {
            mat.setMul3x3( mViewInverseMatrix );
        }
        // ビルボード・Y 軸のみに適用
        else if ( param_.BillboardType == LN_BILLBOARD_ROT_Y )
        {
            if ( mViewDirection.x > 0.0f )
		    {
                mat.rotationY( -atanf( mViewDirection.z / mViewDirection.x ) + LMath::PI / 2 );
		    }   
		    else if ( mViewDirection.x == 0.0f )
		    {
			    //D3DXMatrixIdentity(&matWorld); // 0除算を防ぐため
		    }
		    else
		    {
                mat.rotationY( -atanf( mViewDirection.z / mViewDirection.x ) - LMath::PI / 2 );
		    }
            
        }
        // ビルボードではない
        else
        {
        }

        mat.translation( param_.Transform.getPosition() );

        // 座標変換
        for( lnU32 i = 0; i < vertex_count; ++i )
	    {
            verteies[i].Position.transform( mat );
        }


        mUsingVertexCount += vertex_count;
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================