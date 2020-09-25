//=============================================================================
//�y SQTTransform �z
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
// �� SQTTransform �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      
//*/
//=============================================================================
class SQTTransform
{
public:

    Vector3        Scale;          ///< �g��
    Quaternion     Rotation;       ///< ��]
    Vector3        Translation;    ///< ���s�ړ�

public:

    static const SQTTransform   IDENTITY;  ///< ( Vector3::UNIT_SCALE, Quaternion::IDENTITY, Vector3::ZERO )

public:

    /// �f�t�H���g�R���X�g���N�^ (IDENTITY �ŏ�����)
    SQTTransform();

    /// �R���X�g���N�^
    SQTTransform( const Vector3& scale_, const Quaternion& rotation_, const Vector3& translation_ );

    /// �R�s�[�R���X�g���N�^
    SQTTransform( const SQTTransform& transform_ );

public:

    SQTTransform& operator = ( const SQTTransform t_ );

    

   
    /// �W���o�͂֏o�͂���
    //void cdump( const lnChar* str_ = NULL ) const { LN_COSOLEDUMP( str_, "SQTTransform", "( %f, %f )", x, y ); }

public:

    //-------------------------------------------------------------------------
	// �� static

    

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