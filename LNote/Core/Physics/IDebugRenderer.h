//=============================================================================
//【 IDebugRenderer 】
//-----------------------------------------------------------------------------
///**
//  @file       IDebugRenderer.h
//  @brief      IDebugRenderer
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Geometry/Vector3.h"
#include "../Graphics/Common/Common.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Physics
{

//=============================================================================
// ■ IDebugRenderer クラス
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class IDebugRenderer
    : public Base::Interface
{
public:

    /// 座標変換行列の設定 (ワールド行列)
    virtual void setMatrix( const LMatrix& matrix_ ) = 0;

    /// 球の描画
    virtual LNRESULT drawSphere( const LVector3& pos_, lnfloat radius_, const LColor& color_ ) = 0;

    /// ボックスの描画
    virtual LNRESULT drawBox( const LVector3& bb_min_, const LVector3& bb_max_, const LColor& color_ ) = 0;
    
    /// カプセルの描画
    virtual LNRESULT drawCapsule( lnfloat radius_, lnfloat half_height_, const LColor& color_ ) = 0;

protected:

    virtual ~IDebugRenderer() {}
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Physics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================