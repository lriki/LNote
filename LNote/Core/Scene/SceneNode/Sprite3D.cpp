//=============================================================================
//【 Sprite3D 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Graphics/Manager.h"
#include "../SceneGraph.h"
#include "Util.h"
#include "Sprite3D.h"

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
// ■ Sprite3D クラス
//=============================================================================

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    Sprite3D::Sprite3D( SceneGraph* scene_ )
        : SceneNode         ( scene_ )
        , mRenderer         ( NULL )
        , mAADir            ( LN_AADIR_Z )
        , mFlipFlags        ( LN_FLIP_NONE )
        , mCalledSetSrcRect ( false )
    {
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    Sprite3D::~Sprite3D()
    {
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    LNRESULT Sprite3D::initialize( lnfloat w_size_, lnfloat h_size_, LNAxisAlignedDir dir_ )
    {
        SceneNode::initialize( 1, LN_DRAWCOORD_3D );

        mRenderer   = this->mSceneGraph->getGraphicsManager()->getGeometryRenderer();
        mAADir      = dir_;
        setSize( LVector2( w_size_, h_size_ ) );

        setShader( this->mSceneGraph->getDefaultShader( LN_DEFSHADER_NOLIGHTING ) );
    
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 大きさの設定
    //---------------------------------------------------------------------
    void Sprite3D::setSize( const LVector2& size_ )
    {
        mSize = size_;

        lnfloat r = size_.x * 0.5f;     // +
        lnfloat b = -size_.y * 0.5f;    // -
        lnfloat l = -r;                 // -
        lnfloat t = -b;                 // +

        switch ( mAADir )
        {
            case LN_AADIR_X:
                mUpperLeft.set(  0, t, r );
                mLowerRight.set( 0, b, l );
                break;
            case LN_AADIR_Y:
                mUpperLeft.set(  l, 0, b );
                mLowerRight.set( r, 0, t );
                break;
            case LN_AADIR_RZ:
                mUpperLeft.set(  l, t, 0 );
                mLowerRight.set( r, b, 0 );
                break;
            case LN_AADIR_RX:
                mUpperLeft.set(  0, t, l );
                mLowerRight.set( 0, b, r );
                break;
	        case LN_AADIR_RY:
                mUpperLeft.set(  r, 0, b );
                mLowerRight.set( l, 0, t );
                break;
            case LN_AADIR_Z:
                mUpperLeft.set(  r, t, 0 );
                mLowerRight.set( l, b, 0 );
                break;
        }
    }

    //---------------------------------------------------------------------
    // ● テクスチャの設定
    //---------------------------------------------------------------------
    void Sprite3D::setTexture( Graphics::ITexture* texture_ )
    {
        this->mNodeRenderParam.Subset[ 0 ].Material.setTexture( texture_ );

        if ( texture_ )
        {
            // 一度も setSrcRect() が呼ばれていなければ、テクスチャ全体を転送するようにする
            if ( !mCalledSetSrcRect )
            {
                mSrcRect.set(
                    0, 0,
                    static_cast< int >( texture_->getSize().x ),
                    static_cast< int >( texture_->getSize().y ) );
            }
        }
        else
        {
            mCalledSetSrcRect = false;
        }

        _updateTexUV();
    }

    //---------------------------------------------------------------------
    // ● テクスチャの取得
    //---------------------------------------------------------------------
    Graphics::ITexture* Sprite3D::getTexture()
    {
        return this->mNodeRenderParam.Subset[ 0 ].Material.getTexture();
    }

    //---------------------------------------------------------------------
    // ● テクスチャの転送矩形の設定
    //---------------------------------------------------------------------
    void Sprite3D::setSrcRect( const Geometry::Rect& rect_ )
    {
        mSrcRect = rect_;
        mCalledSetSrcRect = true;
        _updateTexUV(); 
    }

    //---------------------------------------------------------------------
    // ● サブセットを描画する
    //---------------------------------------------------------------------
    LNRESULT Sprite3D::drawSubset( lnU32 index_ )
    {
        //return mRenderer->drawTransformAxis(1);

        return mRenderer->drawSquare(
            mUpperLeft.x,   mUpperLeft.y,  0xffffffff, mUVUpperLeft.x,  mUVUpperLeft.y,
            mLowerRight.x,  mUpperLeft.y,  0xffffffff, mUVLowerRight.x, mUVUpperLeft.y,
            mUpperLeft.x,   mLowerRight.y, 0xffffffff, mUVUpperLeft.x,  mUVLowerRight.y,
            mLowerRight.x,  mLowerRight.y, 0xffffffff, mUVLowerRight.x, mUVLowerRight.y );
/*
        return mRenderer->drawSquare(
            mUpperLeft.x,   mUpperLeft.y,  0xffffffff, mUVUpperLeft.x,  mUVUpperLeft.y,
            mUpperLeft.x,   mLowerRight.y, 0xffffffff, mUVUpperLeft.x,  mUVLowerRight.y,
            mLowerRight.x,  mUpperLeft.y,  0xffffffff, mUVLowerRight.x, mUVUpperLeft.y,
            mLowerRight.x,  mLowerRight.y, 0xffffffff, mUVLowerRight.x, mUVLowerRight.y );
            */
    }

    //---------------------------------------------------------------------
    // ● テクスチャ座標の更新
    //---------------------------------------------------------------------
    void Sprite3D::_updateTexUV()
    {
        Graphics::ITexture* tex = getTexture();

        if ( tex )
        {
            lnfloat l, t, r, b;

            Util::normalizeSrcRect( &l, &t, &r, &b, mSrcRect, tex->getRealSize() );

            // 垂直反転
            if ( mFlipFlags & LN_FLIP_V )
            {
                mUVUpperLeft.y = b;
                mUVLowerRight.y = t;
            }
            else
            {
                mUVUpperLeft.y = t;
                mUVLowerRight.y = b;
            }
            // 水平反転
            if ( mFlipFlags & LN_FLIP_H )
            {
                mUVUpperLeft.x = r;
                mUVLowerRight.x = l;
            }
            else
            {
                mUVUpperLeft.x = l;
                mUVLowerRight.x = r;
            }
        }
        else
        {
        }
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================