//=============================================================================
//【 ModelFace 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "ModelCore.h"
#include "ModelFace.h"

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
// ■ ModelFace クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    ModelFace::ModelFace()
        : mVerticesNum      ( 0 )
        , mFaceVertices     ( NULL )
    {
    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    ModelFace::~ModelFace()
    {
        _release();
    }

    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
    void ModelFace::initializePMD( const PMD_Face* face_data_, const ModelFace* base_face_ )
    {
        _release();

        // 表情名のコピー
        char name[ 21 ];
        strncpy( name, face_data_->szName, 20 );
	    name[ 20 ] = '\0';
        mName = name;

        // 頂点数
        mVerticesNum = face_data_->ulNumVertices;

        // 頂点データのコピー
        mFaceVertices = LN_NEW PMD_FaceVtx[ mVerticesNum ];
        memcpy( mFaceVertices, face_data_->pVertices, sizeof(PMD_FaceVtx) * mVerticesNum );

	    // baseとの相対インデックスを絶対インデックスに変換
	    if ( base_face_ && base_face_ != this )
	    {
		    PMD_FaceVtx	*v = mFaceVertices;
		    for ( lnU32 i = 0 ; i < mVerticesNum ; i++, v++ )
		    {
                v->vec3Pos = base_face_->mFaceVertices[ v->ulIndex ].vec3Pos + v->vec3Pos;
			    v->ulIndex = base_face_->mFaceVertices[ v->ulIndex ].ulIndex;
		    }
	    }
    }

    //---------------------------------------------------------------------
	// ● 適用
    //---------------------------------------------------------------------
    void ModelFace::blendFace( Vertex::PMDVertex* vertices_, float rate_ )
    {
        if ( !mFaceVertices )	return;

	    PMD_FaceVtx* v = mFaceVertices;
	    lnU32 idx;
	    for ( lnU32 i = 0 ; i < mVerticesNum ; ++i, ++v )
	    {
		    idx = v->ulIndex;
            LVector3::lerp( &vertices_[ idx ].Position, vertices_[ idx ].Position, v->vec3Pos, rate_ );
	    }
    }

    //---------------------------------------------------------------------
	// ● 解放
    //---------------------------------------------------------------------
    void ModelFace::_release()
    {
        SAFE_DELETE_ARRAY( mFaceVertices );
        mVerticesNum = 0;
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