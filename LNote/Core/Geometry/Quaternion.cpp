//=============================================================================
//�y Quaternion �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Common.h"
#include "Matrix.h"
#include "Quaternion.h"

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
// �� Quaternion �N���X
//=============================================================================

    const Quaternion   Quaternion::IDENTITY( 0.0f, 0.0f, 0.0f, 1.0f );

	// �f�t�H���g�R���X�g���N�^
	Quaternion::Quaternion()
	{
		x = y = z = 0.0f;
        w = 1.0f;
	}

	// �R�s�[�R���X�g���N�^
	Quaternion::Quaternion( const Quaternion& vec_ )
	{
		x = vec_.x;
		y = vec_.y;
		z = vec_.z;
		w = vec_.w;
	}

	// �R���X�g���N�^
	Quaternion::Quaternion( lnfloat x_, lnfloat y_, lnfloat z_, lnfloat w_ )
	{
		x = x_;
		y = y_;
		z = z_;
		w = w_;
	}

	/// �R���X�g���N�^ ( ���Ɖ�]�p�x����쐬 )
	Quaternion::Quaternion( const Vector3& axis_, lnfloat r_ )
	{
		lnfloat r;
		lnfloat tx = axis_.x;
		lnfloat ty = axis_.y;
		lnfloat tz = axis_.z;
		lnfloat s = axis_.x * axis_.x + axis_.y * axis_.y + axis_.z * axis_.z;

		// �������K������Ă��Ȃ���ΐ��K��
		if ( s != 1.0f )
		{
			r = Asm::sqrt( axis_.x * axis_.x + axis_.y * axis_.y + axis_.z * axis_.z );  
			tx /= r;
			ty /= r;
			tz /= r;
		}

		Asm::sincos( r_ * 0.5f, &s, &w );
		x = s * tx;
		y = s * ty;
		z = s * tz;
	}

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^ ( �I�C���[�p�x����쐬 )
	//---------------------------------------------------------------------
	Quaternion::Quaternion( const Vector3& angle_ )
    {
        lnfloat	rx = angle_.x * 0.5f;
	    lnfloat	ry = angle_.y * 0.5f;
	    lnfloat	rz = angle_.z * 0.5f;
        lnfloat sx, sy, sz, cx, cy, cz;

        Asm::sincos( rx, &sx, &cx );
        Asm::sincos( ry, &sy, &cy );
        Asm::sincos( rz, &sz, &cz );

	    x = sx * cy * cz - cx * sy * sz;
	    y = cx * sy * cz + sx * cy * sz;
	    z = cx * cy * sz - sx * sy * cz;
	    w = cx * cy * cz + sx * sy * sz;
    }

	// �l�̐ݒ�
	void Quaternion::set( lnfloat x_, lnfloat y_, lnfloat z_, lnfloat w_ )
	{
		x = x_;
		y = y_;
		z = z_;
		w = w_;
	}


	//---------------------------------------------------------------------
	// �� ���K������
	//---------------------------------------------------------------------
	void Quaternion::normalize()
	{
		lnfloat r ;
		r = 1.0f / ::sqrt( x * x + y * y + z * z + w * w );
		x *= r;
		y *= r;
		z *= r;
		w *= r;
	}


	

    //---------------------------------------------------------------------
    // �� ���K�������N�H�[�^�j�I����Ԃ�
    //---------------------------------------------------------------------
    void Quaternion::normalize( Quaternion* out_, const Quaternion& qua_ )
    {
        lnfloat r = 1.0f / Asm::sqrt( qua_.x * qua_.x + qua_.y * qua_.y + qua_.z * qua_.z + qua_.w * qua_.w );
		out_->x = qua_.x * r;
		out_->y = qua_.y * r;
		out_->z = qua_.z * r;
		out_->w = qua_.w * r;
    }

    //---------------------------------------------------------------------
    // �� �N�H�[�^�j�I���̋�����Ԃ�
    //---------------------------------------------------------------------
    void Quaternion::conjugate( Quaternion* out_, const Quaternion& qua_ )
    {
        out_->x = -qua_.x;
        out_->y = -qua_.y;
        out_->z = -qua_.z;
        out_->w =  qua_.w;
    }

    //---------------------------------------------------------------------
    // �� �C�ӂ̎�����]���ɂ��ĉ�]����N�H�[�^�j�I�����쐬���� ( ���W�A���p�x )
    //---------------------------------------------------------------------
    void Quaternion::rotationAxis( Quaternion* out_, const Vector3& axis_, lnfloat r_ )
    {
        lnfloat r;
		lnfloat tx = axis_.x;
		lnfloat ty = axis_.y;
		lnfloat tz = axis_.z;
		lnfloat s = axis_.x * axis_.x + axis_.y * axis_.y + axis_.z * axis_.z;

		// �������K������Ă��Ȃ���ΐ��K��
		if ( s != 1.0f )
		{
			r = Asm::sqrt( axis_.x * axis_.x + axis_.y * axis_.y + axis_.z * axis_.z );  
			tx /= r;
			ty /= r;
			tz /= r;
		}

		Asm::sincos( r_ * 0.5f, &s, &out_->w );
		out_->x = s * tx;
		out_->y = s * ty;
		out_->z = s * tz;
    }


    //-------------------------------------------------------------------------
	// �� static
    
    // ��]�s�񁨃N�H�[�^�j�I���ϊ�
    //
    // qx, qy, qz, qw : �N�H�[�^�j�I�������i�o�́j
    // m11-m33 : ��]�s�񐬕�
    bool transformRotMatToQuaternion(
        float &qx, float &qy, float &qz, float &qw,
        float m11, float m12, float m13,
        float m21, float m22, float m23,
        float m31, float m32, float m33
    ) {
        // �ő听��������
        float elem[ 4 ]; // 0:x, 1:y, 2:z, 3:w
        elem[ 0 ] = m11 - m22 - m33 + 1.0f;
        elem[ 1 ] = -m11 + m22 - m33 + 1.0f;
        elem[ 2 ] = -m11 - m22 + m33 + 1.0f;
        elem[ 3 ] = m11 + m22 + m33 + 1.0f;

        unsigned biggestIndex = 0;
        for ( int i = 1; i < 4; i++ ) {
            if ( elem[i] > elem[biggestIndex] )
                biggestIndex = i;
        }

        if ( elem[biggestIndex] < 0.0f )
            return false; // �����̍s��ɊԈႢ����I

        // �ő�v�f�̒l���Z�o
        float *q[4] = {&qx, &qy, &qz, &qw};
        float v = sqrtf( elem[biggestIndex] ) * 0.5f;
        *q[biggestIndex] = v;
        float mult = 0.25f / v;

        switch ( biggestIndex ) {
        case 0: // x
            *q[1] = (m12 + m21) * mult;
            *q[2] = (m31 + m13) * mult;
            *q[3] = (m23 - m32) * mult;
            break;
        case 1: // y
            *q[0] = (m12 + m21) * mult;
            *q[2] = (m23 + m32) * mult;
            *q[3] = (m31 - m13) * mult;
            break;
        case 2: // z
            *q[0] = (m31 + m13) * mult;
            *q[1] = (m23 + m32) * mult;
            *q[3] = (m12 - m21) * mult;
        break;
        case 3: // w
            *q[0] = (m23 - m32) * mult;
            *q[1] = (m31 - m13) * mult;
            *q[2] = (m12 - m21) * mult;
            break;
        }

        return true;
    }

    //---------------------------------------------------------------------
	// �� ��]�s�񂩂�N�H�[�^�j�I�����쐬����
	//---------------------------------------------------------------------
    void Quaternion::rotationMatrix( Quaternion* out_, const Matrix& mat_ )
    {
        transformRotMatToQuaternion(
            out_->x, out_->y, out_->z, out_->w,
            mat_.m00, mat_.m01, mat_.m02,
            mat_.m10, mat_.m11, mat_.m12,
            mat_.m20, mat_.m21, mat_.m22 );
    }

    //---------------------------------------------------------------------
    // �� 2 �̃N�H�[�^�j�I���̐ς��v�Z����
    //---------------------------------------------------------------------
    void Quaternion::multiply( Quaternion* out_, const Quaternion& q0_, const Quaternion& q1_ )
    {
	    float	px, py, pz, pw;
	    float	qx, qy, qz, qw;

	    px = q0_.x; py = q0_.y; pz = q0_.z; pw = q0_.w;
	    qx = q1_.x; qy = q1_.y; qz = q1_.z; qw = q1_.w;

	    out_->x = pw * qx + px * qw + py * qz - pz * qy;
	    out_->y = pw * qy - px * qz + py * qw + pz * qx;
	    out_->z = pw * qz + px * qy - py * qx + pz * qw;
	    out_->w = pw * qw - px * qx - py * qy - pz * qz;

        // �� ModelIK �ł̎��ȑ���ɔ����邽�߁A����ȏ�œK�����Ȃ�
    }

    //---------------------------------------------------------------------
    // �� 2 �̃N�H�[�^�j�I���̐ς��v�Z���� ( �t�� )
    //---------------------------------------------------------------------
    void Quaternion::multiplyRO( Quaternion* out_, const Quaternion& q1_, const Quaternion& q2_ )
    {
        out_->set(
			q2_.w * q1_.x + q2_.x * q1_.w + q2_.y * q1_.z + ( -q2_.z * q1_.y ),
			q2_.w * q1_.y + q2_.y * q1_.w + q2_.z * q1_.x + ( -q2_.x * q1_.z ),
			q2_.w * q1_.z + q2_.z * q1_.w + q2_.x * q1_.y + ( -q2_.y * q1_.x ),
			q2_.w * q1_.w + ( -q2_.x * q1_.x ) + ( -q2_.y * q1_.y ) + ( -q2_.z * q1_.z ) );
    }

    //---------------------------------------------------------------------
    // �� �C�ӂ̎�����]���ɂ��ĉ�]����N�H�[�^�j�I�����쐬���� ( �C���f�b�N�X�p�x )
    //---------------------------------------------------------------------
	void Quaternion::rotationAxisIdx( Quaternion* out_, const Vector3& axis_, int r_ )
    {
        lnfloat r;
		lnfloat tx = axis_.x;
		lnfloat ty = axis_.y;
		lnfloat tz = axis_.z;
		lnfloat s = axis_.x * axis_.x + axis_.y * axis_.y + axis_.z * axis_.z;

		// �������K������Ă��Ȃ���ΐ��K��
		if ( s != 1.0f )
		{
			r = Asm::sqrt( axis_.x * axis_.x + axis_.y * axis_.y + axis_.z * axis_.z );  
			tx /= r;
			ty /= r;
			tz /= r;
		}

        s = LMath::sinIdx( r_ / 2 );
        out_->w = LMath::cosIdx( r_ / 2 );
		out_->x = s * tx;
		out_->y = s * ty;
		out_->z = s * tz;
    }

    

    //---------------------------------------------------------------------
    // �� ���ʐ��`��Ԃ��g���āA2 �̃N�H�[�^�j�I�����Ԃ���
    //---------------------------------------------------------------------
    void Quaternion::slerp( Quaternion* out_, const Quaternion& qua1_, const Quaternion& qua2_, lnfloat t_ )
    {
	    lnfloat qr = qua1_.x * qua2_.x + qua1_.y * qua2_.y + qua1_.z * qua2_.z + qua1_.w * qua2_.w;
	    lnfloat t0 = 1.0f - t_;

	    if ( qr < 0 )
	    {
		    out_->x = qua1_.x * t0 - qua2_.x * t_;
		    out_->y = qua1_.y * t0 - qua2_.y * t_;
		    out_->z = qua1_.z * t0 - qua2_.z * t_;
		    out_->w = qua1_.w * t0 - qua2_.w * t_;
	    }
	    else
	    {
		    out_->x = qua1_.x * t0 + qua2_.x * t_;
		    out_->y = qua1_.y * t0 + qua2_.y * t_;
		    out_->z = qua1_.z * t0 + qua2_.z * t_;
		    out_->w = qua1_.w * t0 + qua2_.w * t_;
	    }
        Quaternion::normalize( out_, *out_ );
    }

    //---------------------------------------------------------------------
    // �� �N�H�[�^�j�I�����I�C���[�p�ɕϊ�����
    //---------------------------------------------------------------------
    void Quaternion::toEuler( Vector3* out_, const Quaternion& qua1_ )
    {
        // XYZ����]�̎擾
	    // Y��]�����߂�
	    lnfloat	x2 = qua1_.x + qua1_.x;
	    lnfloat	y2 = qua1_.y + qua1_.y;
	    lnfloat	z2 = qua1_.z + qua1_.z;
	    lnfloat	xz2 = qua1_.x * z2;
	    lnfloat	wy2 = qua1_.w * y2;
	    lnfloat	temp = -(xz2 - wy2);

	    // �덷�΍�
	    if( temp >= 1.f ){ 
	temp = 1.f; }
	    else if( temp <= -1.f ){ 
	    	temp = -1.f; }

	    lnfloat	yRadian = asinf(temp);

	    // ���̉�]�����߂�
	    lnfloat	xx2 = qua1_.x * x2;
	    lnfloat	xy2 = qua1_.x * y2;
	    lnfloat	zz2 = qua1_.z * z2;
	    lnfloat	wz2 = qua1_.w * z2;

	    if( yRadian < 3.1415926f * 0.5f )
	    {
		    if( yRadian > -3.1415926f * 0.5f )
		    {
			    float	yz2 = qua1_.y * z2;
			    float	wx2 = qua1_.w * x2;
			    float	yy2 = qua1_.y * y2;
			    out_->x = atan2f( (yz2 + wx2), (1.f - (xx2 + yy2)) );
			    out_->y = yRadian;
			    out_->z = atan2f( (xy2 + wz2), (1.f - (yy2 + zz2)) );
		    }
		    else
		    {
			    out_->x = -atan2f( (xy2 - wz2), (1.f - (xx2 + zz2)) );
			    out_->y = yRadian;
			    out_->z = 0.f;
		    }
	    }
	    else
	    {
		    out_->x = atan2f( (xy2 - wz2), (1.f - (xx2 + zz2)) );
		    out_->y = yRadian;
		    out_->z = 0.f;
	    }
    }

    //---------------------------------------------------------------------
    // �� �I�C���[�p����N�H�[�^�j�I�����쐬����
    //---------------------------------------------------------------------
    void Quaternion::fromEuler( Quaternion* out_, const Vector3& angle_ )
    {
        lnfloat	rx = angle_.x * 0.5f;
	    lnfloat	ry = angle_.y * 0.5f;
	    lnfloat	rz = angle_.z * 0.5f;
        lnfloat sx, sy, sz, cx, cy, cz;

        Asm::sincos( rx, &sx, &cx );
        Asm::sincos( ry, &sy, &cy );
        Asm::sincos( rz, &sz, &cz );

	    out_->x = sx * cy * cz - cx * sy * sz;
	    out_->y = cx * sy * cz + sx * cy * sz;
	    out_->z = cx * cy * sz - sx * sy * cz;
	    out_->w = cx * cy * cz + sx * sy * sz;
    }


    //---------------------------------------------------------------------
    // �� �N�H�[�^�j�I����Yaw(Y��]), Pitch(X��]), Roll(Z��])�ɕ�������
    //---------------------------------------------------------------------
    bool Quaternion::factoringZXY( lnfloat* z_, lnfloat* x_, lnfloat* y_, const Quaternion& qua1_ )
    {
        // �N�H�[�^�j�I�����s��
        Quaternion input_q( qua1_ );
        input_q.normalize();
        Matrix rot( input_q );

        // �W���o�����b�N����
        if ( rot.m21 > 1.0f - 1.0e-4 || rot.m21 < -1.0f + 1.0e-4 )
        {
            *x_ = ( rot.m21 < 0 ? LMath::PI_OVER2 : -LMath::PI_OVER2 );
            *z_ = 0;
            *y_ = atan2( -rot.m02, rot.m00 );
            return false;
        }

        *x_ = -(float)asin( rot.m21 );
        *z_ = (float)asin( rot.m01 / cos( *x_ ) );

        //�R��΍�
        if ( LMath::isNaN( *z_ ) )
        {
            *x_ = ( rot.m21 < 0 ? LMath::PI_OVER2 : -LMath::PI_OVER2 );
            *z_ = 0;
            *y_ = atan2( -rot.m02, rot.m00 );
            return false;
        }

        if ( rot.m11 < 0 )
        {
            *z_ = LMath::PI - *z_;
        }

        // �s�b�`���擾
        *y_ = atan2( rot.m20, rot.m22 );

        //*x_ = -*x_;
        //*y_ = -*y_;
        //*z_ = -*z_;

        return true;
    }

    //---------------------------------------------------------------------
    // �� �N�H�[�^�j�I����X,Y,Z��]�ɕ�������
    //---------------------------------------------------------------------
    bool Quaternion::factoringXYZ( lnfloat* x_, lnfloat* y_, lnfloat* z_, const Quaternion& qua1_ )
    {
        // �N�H�[�^�j�I�����s��
        Quaternion input_q( qua1_ );
        input_q.normalize();
        Matrix rot( input_q );

        //�W���o�����b�N����
        if ( rot.m02 > 1.0f - 1.0e-4 || rot.m02 < -1.0f + 1.0e-4 )
        {
            *x_ = 0;
            *y_ = ( rot.m02 < 0 ? LMath::PI_OVER2 : -LMath::PI_OVER2 );
            *z_ = -atan2( -rot.m10, rot.m11 );
            return false;
        }

        *y_ = -asin( rot.m02 );
        *x_ = asin( rot.m12 / cos( *y_ ) );

        //�W���o�����b�N����(�R��΍�)
        if ( LMath::isNaN( *x_ ) )
        {
            *x_ = 0;
            *y_ = ( rot.m02 < 0 ? LMath::PI_OVER2 : -LMath::PI_OVER2 );
            *z_ = -atan2( -rot.m10, rot.m11 );
            return false;
        }

        if ( rot.m22 < 0 )
        {
            *x_ = LMath::PI - *x_;
        }

        *z_ = atan2( rot.m01, rot.m00 );


        //*x_ = -*x_;
        //*y_ = -*y_;
        //*z_ = -*z_;

        return true;
    }

    //---------------------------------------------------------------------
    // �� �N�H�[�^�j�I����Y,Z,X��]�ɕ�������
    //---------------------------------------------------------------------
    bool Quaternion::factoringYZX( lnfloat* y_, lnfloat* z_, lnfloat* x_, const Quaternion& qua1_ )
    {
        // �N�H�[�^�j�I�����s��
        Quaternion input_q( qua1_ );
        input_q.normalize();
        Matrix rot( input_q );

        //�W���o�����b�N����
        if ( rot.m10 > 1 - 1.0e-4 || rot.m10 < -1 + 1.0e-4 )
        {
            *y_ = 0;
            *z_ = ( rot.m10 < 0 ? LMath::PI_OVER2 : -LMath::PI_OVER2 );
            *x_ = -atan2( -rot.m21, rot.m22 );
            return false;
        }

        *z_ = -asin(rot.m10);
        
        *y_ = asin( rot.m20 / cos( *z_ ) );

        //�W���o�����b�N����(�R��΍�)
        if ( LMath::isNaN( *y_ ) )
        {
            *y_ = 0;
            *z_ = ( rot.m10 < 0 ? LMath::PI_OVER2 : -LMath::PI_OVER2 );
            *x_ = -atan2( -rot.m21, rot.m22 );
            return false;
        }

        if ( rot.m00 < 0 )
        {
            *y_ = LMath::PI - *y_;
        }
        
        *x_ = atan2( rot.m12, rot.m11 );

        //*x_ = -*x_;
        //*y_ = -*y_;
        //*z_ = -*z_;

        return true;
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