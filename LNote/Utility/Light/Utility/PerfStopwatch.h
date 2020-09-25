//=============================================================================
//�y PerfStopwatch �z
//-----------------------------------------------------------------------------
///**
//  @file       PerfStopwatch.h
//  @brief      PerfStopwatch
//  @version    1.0
//  @date       2011/11/23
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Utility
{

//=============================================================================
// �� PerfStopwatch �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �v���O�����̎��Ԍv���Ɏg���N���X
//*/
//=============================================================================
class PerfStopwatch
{
public:

    /// �R���X�g���N�^
    PerfStopwatch()
        : mBegan    ( false )
    {
        memset( &mFreq,   0x00, sizeof( mFreq ) );
        memset( &mBefore, 0x00, sizeof( mBefore ) );
        memset( &mAfter,  0x00, sizeof( mAfter ) );
        start();
    }
    
	/// �f�X�g���N�^
	~PerfStopwatch()
    {
        stopS();
    }

public:

    /// �v���J�n
	void start()
    {
        ::QueryPerformanceFrequency( &mFreq );
        ::QueryPerformanceCounter( &mBefore );
        mBegan = true;
    }

    /// �v���I��
	u32 stop()
    {
        if ( mBegan )
        {
            mBegan = false;
            ::QueryPerformanceCounter( &mAfter );
            return static_cast< u32 >( ( mAfter.QuadPart - mBefore.QuadPart ) * 1000000 / mFreq.QuadPart );
        }
        return 0;
    }

    /// �v���I�� (���ʂ� printf() �ŕ\������)
	void stopS()
    {
        if ( mBegan )
        {
            printf( "pasttime:%d (ns)\n", stop() );
        }
    }

private:

    LARGE_INTEGER   mFreq;
    LARGE_INTEGER   mBefore;
    LARGE_INTEGER   mAfter;
    bool            mBegan;
};


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Utility
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================