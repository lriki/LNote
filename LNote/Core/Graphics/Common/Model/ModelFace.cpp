//=============================================================================
//�y ModelFace �z
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
// �� ModelFace �N���X
//=============================================================================

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    ModelFace::ModelFace()
        : mVerticesNum      ( 0 )
        , mFaceVertices     ( NULL )
    {
    }

    //---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
    ModelFace::~ModelFace()
    {
        _release();
    }

    //---------------------------------------------------------------------
	// �� ������
    //---------------------------------------------------------------------
    void ModelFace::initializePMD( const PMD_Face* face_data_, const ModelFace* base_face_ )
    {
        _release();

        // �\��̃R�s�[
        char name[ 21 ];
        strncpy( name, face_data_->szName, 20 );
	    name[ 20 ] = '\0';
        mName = name;

        // ���_��
        mVerticesNum = face_data_->ulNumVertices;

        // ���_�f�[�^�̃R�s�[
        mFaceVertices = LN_NEW PMD_FaceVtx[ mVerticesNum ];
        memcpy( mFaceVertices, face_data_->pVertices, sizeof(PMD_FaceVtx) * mVerticesNum );

	    // base�Ƃ̑��΃C���f�b�N�X���΃C���f�b�N�X�ɕϊ�
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
	// �� �K�p
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
	// �� ���
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