//=============================================================================
//【 SQTTransform 】
//-----------------------------------------------------------------------------
///**
//  @file       SQTTransform.h
//  @brief      SQTTransform
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "Common.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Geometry
{

//=============================================================================
// ■ SQTTransform クラス
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class SQTTransform
{
public:

    Vector3        Scale;          ///< 拡大
    Quaternion     Rotation;       ///< 回転
    Vector3        Translation;    ///< 平行移動

public:

    static const SQTTransform   IDENTITY;  ///< ( Vector3::UNIT_SCALE, Quaternion::IDENTITY, Vector3::ZERO )

public:

    /// デフォルトコンストラクタ (IDENTITY で初期化)
    SQTTransform();

    /// コンストラクタ
    SQTTransform( const Vector3& scale_, const Quaternion& rotation_, const Vector3& translation_ );

    /// コピーコンストラクタ
    SQTTransform( const SQTTransform& transform_ );

public:

    SQTTransform& operator = ( const SQTTransform t_ );

    

   
    /// 標準出力へ出力する
    //void cdump( const lnChar* str_ = NULL ) const { LN_COSOLEDUMP( str_, "SQTTransform", "( %f, %f )", x, y ); }

public:

    //-------------------------------------------------------------------------
	// ▼ static

    

};
	


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Geometry
} // namespace Core

typedef Core::Geometry::SQTTransform	LSQTTransform;

} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================