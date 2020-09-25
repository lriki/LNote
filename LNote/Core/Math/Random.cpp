//=============================================================================
//�y Random �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Math.h"
#include "Random.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Math
{

//=============================================================================
// �� Random �N���X
//=============================================================================

#define DEFAULT_X 123456789
#define DEFAULT_Y 362436069
#define DEFAULT_Z 521288629
#define DEFAULT_W 88675123

    static int gDefaultSeed = 0;

    //---------------------------------------------------------------------
	// �� �f�t�H���g�̗����V�[�h�̐ݒ�
	//---------------------------------------------------------------------
    void Random::setDefaultSeed( int seed_ )
    {
        gDefaultSeed = seed_;
    }

    //---------------------------------------------------------------------
	// �� �f�t�H���g�̗����V�[�h�̎擾
	//---------------------------------------------------------------------
    int Random::getDefaultSeed()
    {
        return gDefaultSeed;
    }

    //---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    int Random::getIntHash( int seed_ )
    {
        int w = ( seed_ <= 0 ) ? DEFAULT_W : seed_;
        unsigned t = DEFAULT_X ^ ( DEFAULT_X << 11 );
		w = ( w ^ ( w >> 19 ) ) ^ ( t ^ ( t >> 8 ) );
		return static_cast< int >( w );
    }

    //---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    float Random::getFloatHash( int seed_ )
    {
        int r = getIntHash( seed_ );
		r = ( r & 0x007fffff ) | 0x3f800000;
		float f = *reinterpret_cast< float* >( &r );
		return f - 1.f;
    }

    //---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    float Random::getFloatHash( int seed_, float a_, float b_ )
    {
        float r = getFloatHash( seed_ );
		r *= ( b_ - a_ );
		r += a_;
		return r;
    }

    //---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    float Random::getFloatHashEx( int seed_, LNExRandType rand_type_, float v0_, float v1_ )
    {
        if ( rand_type_ == LN_RAND_RANGE )
        {
            return getFloatHash( seed_, v0_ - v1_, v0_ + v1_ );
        }
        else if ( rand_type_ == LN_RAND_MINMAX )
        {
            return getFloatHash( seed_, v0_, v1_ );
        }
        else
        {
            return v0_;
        }
    }

	//---------------------------------------------------------------------
	// �� �f�t�H���g�R���X�g���N�^
	//---------------------------------------------------------------------
	Random::Random()
        : x( DEFAULT_X )
        , y( DEFAULT_Y )
        , z( DEFAULT_Z )
        , w( gDefaultSeed )
    {
    }

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^ (�����V�[�h�l���w��)
	//---------------------------------------------------------------------
	Random::Random( int seed_ )
        : x( DEFAULT_X )
        , y( DEFAULT_Y )
        , z( DEFAULT_Z )
        , w( gDefaultSeed )
    {
        w = ( seed_ <= 0 ) ? 88675123 : seed_;
    }

    //---------------------------------------------------------------------
	// �� �����V�[�h�̐ݒ�
	//---------------------------------------------------------------------
    void Random::setSeed( int seed_ )
	{
		w = ( seed_ <= 0 ) ? 88675123 : seed_;
	}

	//---------------------------------------------------------------------
	// �� �����l�����̎擾
	//---------------------------------------------------------------------
    int Random::getInt()
	{
		unsigned t = x ^ ( x << 11 );
		x = y;
		y = z;
		z = w;
		w = ( w ^ ( w >> 19 ) ) ^ ( t ^ ( t >> 8 ) );
		return static_cast< int >( w );
	}

	//---------------------------------------------------------------------
	// �� �����l�����̎擾 ( 0 <= x < a_ )
	//---------------------------------------------------------------------
    int Random::getInt( int a_ )
	{
		if ( a_ == 0 ) { return 0; }
		int r = getInt() & 0x7fffffff;
		r %= a_;
		return r;
	}

	//---------------------------------------------------------------------
	// �� �����l�����̎擾 ( _a <= x < b_ )
	//---------------------------------------------------------------------
    int Random::getInt( int a_, int b_ )
	{
		if ( b_ - a_ == 0 ) { return 0; }
		int r = getInt() & 0x7fffffff;
		r %= b_ - a_;
		r += a_;
		return r;
	}

	//---------------------------------------------------------------------
	// �� �����l�����̎擾
	//---------------------------------------------------------------------
    float Random::getFloat()
	{
		int r = getInt();
		r = ( r & 0x007fffff ) | 0x3f800000;
		float f = *reinterpret_cast< float* >( &r );
		return f - 1.f;
	}

	//---------------------------------------------------------------------
	// �� �����l�����̎擾 ( 0 <= x < a_ )
	//---------------------------------------------------------------------
    float Random::getFloat( float a_ )
	{
		float r = getFloat();
		r *= a_;
		return r;
	}

    //---------------------------------------------------------------------
	// �� �����l�����̎擾 ( _a <= x < b_ )
	//---------------------------------------------------------------------
    float Random::getFloat( float a_, float b_ )
	{
		float r = getFloat();
		r *= ( b_ - a_ );
		r += a_;
		return r;
	}

    //---------------------------------------------------------------------
	// �� �����l�����̎擾
	//---------------------------------------------------------------------
    float Random::getFloatEx( float v0_, float v1_, LNExRandType rand_type_, float rate_, LNExRandRateType rate_type_ ) 
    {
        if ( rand_type_ == LN_RAND_NONE )
        {
            return v0_;
        }
        else
        {
            float min, max;

            if ( rand_type_ == LN_RAND_RANGE )
            {
                min = v0_ - v1_;
                max = v0_ + v1_;
            }
            else // if ( rand_type_ == LN_RAND_MINMAX )
            {
                min = v0_;
                max = v1_;
            }

            switch ( rate_type_ )
            {

                default:
                {
                    return getFloat( min, max );
                }
                // rate = 1.0 �̂Ƃ��ɍŏ��l
			    case LN_RANDRATE_MIN:
                {
				    return ( max - min ) * ( 1.0f - rate_ ) + min;
                }
			    // rate = 1.0 �̂Ƃ��ɍő�l
			    case LN_RANDRATE_MAX:
                {
				    return ( max - min ) * rate_ + min;
                }
                // rate = 1.0 �̂Ƃ��ɒ����l
                case LN_RANDRATE_CENTER:
                {
                    float median = ( max + min ) * 0.5f;

                    if ( getInt( 2 ) & 1 )
                    {
                        return LMath::linear( max, median, rate_ ); // 0.0 �̂Ƃ��� max
                    }
                    else
                    {
                        return LMath::linear( min, median, rate_ ); // 0.0 �̂Ƃ��� min
                    }
                }
                // rate = 1.0 �̂Ƃ��� min �܂��� max
                case LN_RANDRATE_OUTER:
                {
                    float median = ( max + min ) * 0.5f;

                    if ( getInt( 2 ) & 1 )
                    {
                        return LMath::linear( median, max, rate_ ); // 1.0 �̂Ƃ��� max
                    }
                    else
                    {
                        return LMath::linear( median, min, rate_ ); // 1.0 �̂Ƃ��� max
                    }
                }
            }
        }
        return v0_;
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Math
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================