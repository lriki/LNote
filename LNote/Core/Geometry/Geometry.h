//=============================================================================
//�y Geometry �z
//-----------------------------------------------------------------------------
///**
//  @file       Geometry.h
//  @brief      Geometry
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Math/Math.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{

//=============================================================================
// �� LSquare �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�l�p�`�̃N���X
//*/
//=============================================================================
class LSquare
{
public:

};


//=============================================================================
// �� LSphere �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		���̃N���X
//*/
//=============================================================================
struct LSphere
{
    LVector3     Position;  ///< ���S�ʒu
    lnfloat     Radius;    ///< ���a
};

//=============================================================================
// �� LRay �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�����̃N���X
//*/
//=============================================================================
struct LRay
{
    LVector3     Start;      ///< �n�_
    LVector3     End;        ///< �I�_
};

//=============================================================================
// �� LBox �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�{�b�N�X�̃N���X (�����s���E�{�b�N�X�EAABB)
//*/
//=============================================================================
struct LBox
{
    LVector3     Min;       ///< �ŏ��_
    LVector3     Max;       ///< �ő�_

public:

    /// �C�ӂ�2�_����{�b�N�X�����
    void create( const LVector3& p0_, const LVector3& p1_ );

};


//=============================================================================
// �� LOBBox �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�L�����E�{�b�N�X�̃N���X
//*/
//=============================================================================
struct LOBBox
{
public:

    LVector3     Position;          ///< �{�b�N�X�̒��S
    LVector3     Length;            ///< �{�b�N�X�̊e�ӂ̒���
    LVector3     Direction[ 3 ];    ///< �e���̌���
};


//=============================================================================
// �� LPlane �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		���ʂ̃N���X
//*/
//=============================================================================
class LPlane
{
public:

    lnfloat a, b, c, d;

public:

    /// �f�t�H���g�R���X�g���N�^
    LPlane();

    /// �R���X�g���N�^
    LPlane( lnfloat a_, lnfloat b_, lnfloat c_, lnfloat d_ );

    /// �R���X�g���N�^ (3�_����쐬����)
    LPlane( const LVector3& v0_, const LVector3& v1_, const LVector3& v2_ );

public:

    /// 3�_����쐬���� (�쐬��A���K������)
    void create( const LVector3& v0_, const LVector3& v1_, const LVector3& v2_ );

    /// ���ʂ� 3D �x�N�g���̓��ς��v�Z���� (w = 1 �Ƃ��Čv�Z����)
    lnfloat dotCoord( const LVector3& v0_ ) const;

    /// �_�����ʂ̓����ɂ��邩�𔻒肷��
    bool checkInside( const LVector3& point_ ) const;

    /// �������ʂ̓����ɂ��邩�𔻒肷��
	bool checkInside( const LVector3& point_, lnfloat radius_ ) const;

    /// �R���\�[���ɏo�͂���
	void dump( const char* str_ = NULL ) const;
};


/// ����̊e��
enum LNFrustumSide
{
    LN_FRUSTUM_NEAR = 0,
    LN_FRUSTUM_FAR,
    LN_FRUSTUM_TOP,
    LN_FRUSTUM_RIGHT,
    LN_FRUSTUM_BOTTOM,
    LN_FRUSTUM_LEFT,
    LN_FRUSTUM_MAX_PANELS
};

//=============================================================================
// �� LFrustum �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		����̃N���X
//*/
//=============================================================================
class LFrustum
{
public:

    LVector3     NearClip[ 4 ];  ///< (���Ԃ͉E�ォ�玞�v���)
    LVector3     FarClip[ 4 ];   ///< )���Ԃ͉E�ォ�玞�v���)
};

//=============================================================================
// �� LViewFrustum �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		������̃N���X
//*/
//=============================================================================
class LViewFrustum
    : public LFrustum
{
public:

    LPlane      Planes[ LN_FRUSTUM_MAX_PANELS ];
    LVector3    Right;      /// ���K���ς݉E���� ( ( 1.0f, 0.0f, 0.0f ) ��{�I�ɒ萔�Bconst �ł����������H)
    LVector3    Up;         /// ���K���ςݏ���� ( ( 0.0f, 1.0f, 0.0f ) ��{�I�ɒ萔�Bconst �ł����������H)
    LVector3    Forward;    /// ���K���ςݑO���� ( ( 0.0f, 0.0f, 1.0f ) ��{�I�ɒ萔�Bconst �ł����������H)
    lnfloat     Fov;        ///< 2D �̏ꍇ�� 0
	lnfloat     Aspect;     ///< 2D �̏ꍇ�� 0
	lnfloat     Near;
	lnfloat     Far;

public:

    /// �f�t�H���g�R���X�g���N�^
    LViewFrustum();

    /// �R���X�g���N�^
    LViewFrustum( lnfloat fov_, lnfloat aspect_, lnfloat near_clip_, lnfloat far_clip_ );

public:

    /// �쐬
    void create( lnfloat fov_, lnfloat aspect_, lnfloat near_clip_, lnfloat far_clip_ );

    /// �쐬
    void create2DProjection( lnfloat width_, lnfloat height_, lnfloat near_clip_, lnfloat far_clip_ );
/*
    /// ����p�̐ݒ�
    void setFOV( lnfloat fov_ ) { Fov = fov_; create( Fov, Aspect, Near, Far ); }

    /// �A�X�y�N�g��̐ݒ�
	void setAspect( lnfloat aspect_ ) { Aspect = aspect_; create( Fov, Aspect, Near, Far ); }

    /// ��O�̃r���[���� Z �l�̐ݒ�
	void setNear( lnfloat near_clip_ ) { Near = near_clip_; create( Fov, Aspect, Near, Far ); }

    /// ���̃r���[���� Z �l�̐ݒ�
	void setFar( lnfloat far_clip_ ) { Far = far_clip_; create( Fov, Aspect, Near, Far ); }
*/
    /// �ݒ肳��Ă���f�[�^�����ƂɎ�������쐬���� (transform_ �� NULL �w��\ ���̏ꍇ�AZ+ ������)
    void update( const LMatrix* transform_ );

    /// ���ʂ̎擾
    const LPlane& getPlane( LNFrustumSide side_ ) { return Planes[ side_ ]; }

    /// �_������̓����ɂ��邩�𔻒肷��
    bool checkInside( const LVector3& point_ ) const;

    /// ��������̓����ɂ��邩�𔻒肷��
	bool checkInside( const LVector3& point_, lnfloat radius_ ) const;
};


//=============================================================================
// �� LCapsule �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�J�v�Z���̃N���X
//*/
//=============================================================================
class LCapsule
{
public:

    LVector3     Position;          ///< ���S���W
    LVector3     Radius;            ///< ���a
    LVector3     Height;            ///< ����
};

//=============================================================================
// �� LCone �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		�~���̃N���X
//*/
//=============================================================================
class LCone
{
public:

    LVector3     Position;          ///< ���S���W
    LVector3     Radius;            ///< ���a
    LVector3     Height;            ///< ����
};



enum LNCollisionSource3DType
{
    LN_COLSRC3D_SPHERE = 0,     ///< ��
    LN_COLSRC3D_RAY,            ///< ����
    LN_COLSRC3D_BOX,            ///< ��
    LN_COLSRC3D_OBB,            ///< �L�����E�{�b�N�X
};

//=============================================================================
// �� CollisionSource3D �N���X
//-----------------------------------------------------------------------------
///**
//  @brief		3D �p�̏Փ˔���̌`����J�v�Z��������N���X
//*/
//=============================================================================
class LCollisionSource3D
{
public:

    
};


//=============================================================================
// �� LGeometry �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �􉽊w�֌W�̃��W���[��
//
//  @note
//              LMath �Ɋ܂߂�Ɨ��΂ɐ��������Ȃ�̂Łc�B
//*/
//=============================================================================
class LGeometry
{
public:

    //---------------------------------------------------------------------
	///**
    //  @brief      �_�ƕ��ʂ̋��������߂�i�ŒZ�����j
    //
    //  @param[in]  pos1_   : ���ׂ�_
    //  @param[in]  pos2_   : ���ʏ�̓_
    //  @param[in]  normal_ : ���ʖ@��
    //*/
    //---------------------------------------------------------------------
    static lnfloat PointPolyDistance( const LVector3& pos1_, const LVector3& pos2_, const LVector3& normal_ )
    {
        return LVector3::dot( pos1_ - pos2_, normal_ );
    }
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================