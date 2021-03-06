//=============================================================================
//【 ModelFace 】
//-----------------------------------------------------------------------------
///**
//  @file       ModelFace.h
//  @brief      ModelFace
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../../Base/RefString.h"
#include "PMDTypes.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace Lumine
{
namespace Graphics
{

//=============================================================================
// ■ ModelFace クラス
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class ModelFace
{
public:

    ModelFace();

    virtual ~ModelFace();

public:

    /// 初期化
    void initializePMD( const PMD_Face* face_data_, const ModelFace* base_face_ );

    /// 名前の取得
    const LRefTString& getName() const { return mName; }

    /// 適用
    void blendFace( Vertex::PMDVertex* vertices_, float rate_ );

private:

    /// 解放
    void _release();

private:

    LRefTString     mName;              ///< 表情名
    lnU32	        mVerticesNum;       ///< 表情頂点数
    PMD_FaceVtx*    mFaceVertices;	    ///< 表情頂点データ
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Lumine

//=============================================================================
//								end of file
//=============================================================================