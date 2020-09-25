//=============================================================================
//【 Geometry 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Math/Math.h"
#include "Geometry.h"


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{


//=============================================================================
// ■ Square クラス
//=============================================================================


//=============================================================================
// ■ LBox クラス
//=============================================================================

    // 任意の2点からボックスを作る
    void LBox::create( const LVector3& p0_, const LVector3& p1_ )
    {
        if ( p0_.x < p1_.x )
        {
            Min.x = p0_.x;
            Max.x = p1_.x;
        }
        else
        {
            Min.x = p1_.x;
            Max.x = p0_.x;
        }

        if ( p0_.y < p1_.y )
        {
            Min.y = p0_.y;
            Max.y = p1_.y;
        }
        else
        {
            Min.y = p1_.y;
            Max.y = p0_.y;
        }

        if ( p0_.z < p1_.z )
        {
            Min.z = p0_.z;
            Max.z = p1_.z;
        }
        else
        {
            Min.z = p1_.z;
            Max.z = p0_.z;
        }
    }



//=============================================================================
// ■ LPlane クラス
//=============================================================================

    // デフォルトコンストラクタ
    LPlane::LPlane()
        : a ( 0.0f )
        , b ( 0.0f )
        , c ( 0.0f )
        , d ( 0.0f )
    {
    }

    // コンストラクタ
    LPlane::LPlane( lnfloat a_, lnfloat b_, lnfloat c_, lnfloat d_ )
        : a ( a_ )
        , b ( b_ )
        , c ( c_ )
        , d ( d_ )
    {
    }

    // コンストラクタ (3点から作成する)
    LPlane::LPlane( const LVector3& v0_, const LVector3& v1_, const LVector3& v2_ )
    {
        create( v0_, v1_, v2_ );
    }

    // 3点から作成する
    void LPlane::create( const LVector3& p0_, const LVector3& p1_, const LVector3& p2_ )
    {
        LVector3 n;
        LVector3::cross( &n, ( p1_ - p0_ ), ( p2_ - p0_ ) );
        n.normalize();

        a = n.x;
        b = n.y;
        c = n.z;
        d = -LVector3::dot( p0_, n ); 
    }

    // 平面と 3D ベクトルの内積を計算する (w = 1 として計算する)
    lnfloat LPlane::dotCoord( const LVector3& v0_ ) const
    {
        return ( a * v0_.x ) + ( b * v0_.y ) + ( c * v0_.z ) + d;//*1
    }

    // 点が平面の内側にあるかを判定する
    bool LPlane::checkInside( const LVector3& point_ ) const
    {
	    return ( dotCoord( point_ ) >= 0.0f );
    }

    // 球が平面の内側にあるかを判定する
    bool LPlane::checkInside( const LVector3& point_, lnfloat radius_ ) const
    {
	    return ( dotCoord( point_ ) >= -radius_ );
    }

    // コンソールに出力する
	void LPlane::dump( const char* str_ ) const
	{
		if ( str_ )
		{
			printf( "%s", str_ );
		}
		else
		{
			printf( "◆ Plane " );
		}
		printf( "( %f, %f, %f, %f )\n", a, b, c, d );
	}

//=============================================================================
// ■ LViewFrustum クラス
//=============================================================================

    // デフォルトコンストラクタ
    LViewFrustum::LViewFrustum()
        : Right     ( 1.0f, 0.0f, 0.0f )
        , Up        ( 0.0f, 1.0f, 0.0f )
        , Forward   ( 0.0f, 0.0f, 1.0f )
    {
        Fov    = LMath::PI / 4.0f;
	    Aspect = 1.0f;
	    Near   = 1.0f;
	    Far    = 1000.0f;
    }

    // コンストラクタ
    LViewFrustum::LViewFrustum( lnfloat fov_, lnfloat aspect_, lnfloat near_clip_, lnfloat far_clip_ )
    {
        create( fov_, aspect_, near_clip_, far_clip_ );
    }

    // 作成
    void LViewFrustum::create( lnfloat fov_, lnfloat aspect_, lnfloat near_clip_, lnfloat far_clip_ )
    {
        Fov    = fov_;
	    Aspect = aspect_;
	    Near   = near_clip_;
	    Far    = far_clip_;

        lnfloat fov_over2 = ::tan( Fov / 2.0f );
	    LVector3 near_right = Right * Aspect * ( Near * fov_over2 ); // 手前右
	    LVector3 far_right  = Right * Aspect * ( Far  * fov_over2 ); // 奥右
	    LVector3 near_up    = Up * ( Near * fov_over2 );             // 手前上
	    LVector3 far_up     = Up * ( Far  * fov_over2 );             // 奥上

        // 右上から時計回りに手前、奥の四角形を作っていく
	    NearClip[ 0 ] = ( Forward * Near ) - near_right + near_up;
	    NearClip[ 1 ] = ( Forward * Near ) + near_right + near_up;
	    NearClip[ 2 ] = ( Forward * Near ) + near_right - near_up;
	    NearClip[ 3 ] = ( Forward * Near ) - near_right - near_up;

	    FarClip[ 0 ] = ( Forward * Far ) - far_right + far_up;
	    FarClip[ 1 ] = ( Forward * Far ) + far_right + far_up;
	    FarClip[ 2 ] = ( Forward * Far ) + far_right - far_up;
	    FarClip[ 3 ] = ( Forward * Far ) - far_right - far_up;

        // ※もし反時計回りに定義する場合、法線は自分から離れる方向を指している

	    LVector3 origin( 0.0f, 0.0f, 0.0f );
	    Planes[ LN_FRUSTUM_NEAR ].create(   NearClip[ 2 ], NearClip[ 1 ], NearClip[ 0 ] );
	    Planes[ LN_FRUSTUM_FAR ].create(    FarClip[ 0 ],  FarClip[ 1 ],  FarClip[ 2 ] );
	    Planes[ LN_FRUSTUM_RIGHT ].create(  FarClip[ 2 ],  FarClip[ 1 ],  origin);
	    Planes[ LN_FRUSTUM_TOP ].create(    FarClip[ 1 ],  FarClip[ 0 ],  origin);
	    Planes[ LN_FRUSTUM_LEFT ].create(   FarClip[ 0 ],  FarClip[ 3 ],  origin);
	    Planes[ LN_FRUSTUM_BOTTOM ].create( FarClip[ 3 ],  FarClip[ 2 ],  origin);
    }

    // 作成
    void LViewFrustum::create2DProjection( lnfloat width_, lnfloat height_, lnfloat near_clip_, lnfloat far_clip_ )
    {
        Aspect = 0;//width_ / height_;  // 実際使わないので…
	    Near   = near_clip_;
	    Far    = far_clip_;

        // 右上から時計回りに手前、奥の四角形を作っていく
        NearClip[ 0 ].set( 0, 0, Near );
	    NearClip[ 1 ].set( width_, 0, Near );
	    NearClip[ 2 ].set( 0, height_, Near );
	    NearClip[ 3 ].set( width_, height_, Near );

        FarClip[ 0 ].set( 0, 0, Far );
	    FarClip[ 1 ].set( width_, 0, Far );
	    FarClip[ 2 ].set( 0, height_, Far );
	    FarClip[ 3 ].set( width_, height_, Far );

        // Planes
        LVector3 origin( 0.0f, 0.0f, 0.0f );
	    Planes[ LN_FRUSTUM_NEAR ].create(   NearClip[ 2 ], NearClip[ 1 ], NearClip[ 0 ] );
	    Planes[ LN_FRUSTUM_FAR ].create(    FarClip[ 0 ],  FarClip[ 1 ],  FarClip[ 2 ] );
	    Planes[ LN_FRUSTUM_RIGHT ].create(  FarClip[ 2 ],  FarClip[ 1 ],  origin);
	    Planes[ LN_FRUSTUM_TOP ].create(    FarClip[ 1 ],  FarClip[ 0 ],  origin);
	    Planes[ LN_FRUSTUM_LEFT ].create(   FarClip[ 0 ],  FarClip[ 3 ],  origin);
	    Planes[ LN_FRUSTUM_BOTTOM ].create( FarClip[ 3 ],  FarClip[ 2 ],  origin);
    }

    /// 設定されているデータをもとに視錐台を作成する (transform_ は NULL 指定可能 その場合、Z+ が正面)
    void LViewFrustum::update( const LMatrix* transform_ )
    {
        lnfloat fov_over2 = ::tan( Fov / 2.0f );
	    LVector3 near_right = Right * Aspect * ( Near * fov_over2 ); // 手前右
	    LVector3 far_right  = Right * Aspect * ( Far  * fov_over2 ); // 奥右
	    LVector3 near_up    = Up * ( Near * fov_over2 );             // 手前上
	    LVector3 far_up     = Up * ( Far  * fov_over2 );             // 奥上

        // 右上から時計回りに手前、奥の四角形を作っていく
	    NearClip[ 0 ] = ( Forward * Near ) - near_right + near_up;
	    NearClip[ 1 ] = ( Forward * Near ) + near_right + near_up;
	    NearClip[ 2 ] = ( Forward * Near ) + near_right - near_up;
	    NearClip[ 3 ] = ( Forward * Near ) - near_right - near_up;

	    FarClip[ 0 ] = ( Forward * Far ) - far_right + far_up;
	    FarClip[ 1 ] = ( Forward * Far ) + far_right + far_up;
	    FarClip[ 2 ] = ( Forward * Far ) + far_right - far_up;
	    FarClip[ 3 ] = ( Forward * Far ) - far_right - far_up;

        if ( transform_ )
        {
            for ( int i = 0; i < 4; ++i )
            {
                NearClip[ i ].transform( *transform_ );
                FarClip[ i ].transform( *transform_ );
            } 
        }

        // ※もし反時計回りに定義する場合、法線は自分から離れる方向を指している

	    LVector3 origin( 0.0f, 0.0f, 0.0f );
	    Planes[ LN_FRUSTUM_NEAR ].create(   NearClip[ 2 ], NearClip[ 1 ], NearClip[ 0 ] );
	    Planes[ LN_FRUSTUM_FAR ].create(    FarClip[ 0 ],  FarClip[ 1 ],  FarClip[ 2 ] );
	    Planes[ LN_FRUSTUM_RIGHT ].create(  FarClip[ 2 ],  FarClip[ 1 ],  origin);
	    Planes[ LN_FRUSTUM_TOP ].create(    FarClip[ 1 ],  FarClip[ 0 ],  origin);
	    Planes[ LN_FRUSTUM_LEFT ].create(   FarClip[ 0 ],  FarClip[ 3 ],  origin);
	    Planes[ LN_FRUSTUM_BOTTOM ].create( FarClip[ 3 ],  FarClip[ 2 ],  origin);
    }

    // 点が錐台の内側にあるかを判定する
    bool LViewFrustum::checkInside( const LVector3& point_ ) const
    {
        for ( int i = 0; i < LN_FRUSTUM_MAX_PANELS; ++i )
	    {
		    if ( !Planes[ i ].checkInside( point_ ) )
            {
			    return false;
            }
	    }
	    return true;
    }

    // 球が錐台の内側にあるかを判定する
	bool LViewFrustum::checkInside( const LVector3& point_, lnfloat radius_ ) const
    {
        for ( int i = 0; i < LN_FRUSTUM_MAX_PANELS; ++i )
	    {
		    if ( !Planes[ i ].checkInside( point_, radius_ ) )
            {
			    return false;
            }
	    }
	    return true;
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Core

} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================