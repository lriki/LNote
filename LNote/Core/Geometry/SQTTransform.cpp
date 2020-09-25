//=============================================================================
//�y SQTTransform �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Math.h"
#include "SQTTransform.h"

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
// �� SQTTransform �N���X
//=============================================================================

    const SQTTransform   SQTTransform::IDENTITY( Vector3( 1.0f, 1.0f, 1.0f ), Quaternion( 0.0f, 0.0f, 0.0f, 1.0f ), Vector3( 0.0f, 0.0f, 0.0f ) );

    //---------------------------------------------------------------------
	// �� �f�t�H���g�R���X�g���N�^ (IDENTITY �ŏ�����)
    //---------------------------------------------------------------------
    SQTTransform::SQTTransform()
        : Scale         ( Vector3::UNIT_SCALE )
        , Rotation      ( Quaternion::IDENTITY )
        , Translation   ( Vector3::ZERO )
    {
    	}

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    SQTTransform::SQTTransform( const Vector3& scale_, const Quaternion& rotation_, const Vector3& translation_ )
        : Scale         ( scale_ )
        , Rotation      ( rotation_ )
        , Translation   ( translation_ )
    {
    	}

    //---------------------------------------------------------------------
	// �� �R�s�[�R���X�g���N�^
    //---------------------------------------------------------------------
    SQTTransform::SQTTransform( const SQTTransform& transform_ )
        : Scale         ( transform_.Scale )
        , Rotation      ( transform_.Rotation )
        , Translation   ( transform_.Translation )
    {
    	}

    //---------------------------------------------------------------------
	// 
    //---------------------------------------------------------------------
    SQTTransform& SQTTransform::operator = ( const SQTTransform t_ )
    {
        Scale       = t_.Scale;
        Rotation    = t_.Rotation;
        Translation = t_.Translation;
        return *this;
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Geometry
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================