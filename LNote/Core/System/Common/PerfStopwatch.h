//=============================================================================
//�y PerfStopwatch �z
//-----------------------------------------------------------------------------
///**
//  @file       PerfStopwatch.h
//  @brief      PerfStopwatch
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{
#if defined(LNOTE_WINDOWS)

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
            lnprintf( "pasttime:%d (ns)\n", stop() );
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

#endif
} // namespace System
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================