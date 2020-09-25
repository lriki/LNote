//=============================================================================
//�y XInputModule �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "XInputModule.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Input
{
namespace Win32
{

//=============================================================================
// �� XInputModule �N���X
//=============================================================================

    // �R���X�g���N�^
    XInputModule::XInputModule()
        : mXInputModule     ( NULL )
        , mXInputGetState   ( NULL )
    {
    }

    // �f�X�g���N�^
    XInputModule::~XInputModule()
    {
        if ( mXInputModule )
        {
            ::FreeLibrary( mXInputModule );
        }
    }

    // ������
    LNRESULT XInputModule::initialize()
    {
        // ���W���[���ǂݍ���
        mXInputModule = ::LoadLibrary( XINPUT_DLL );
        LN_SETERR_R_S_( ( mXInputModule ), LN_ERR_SYSTEM,
            "#XInput �� DLL �̓ǂݍ��݂Ɏ��s���܂����B\n"
            "DirectX ���������C���X�g�[������Ă��Ȃ����A\n"
            "�Â��o�[�W�����ł���\��������܂��B" );

        mXInputGetState = reinterpret_cast< MD_XInputGetState >( ::GetProcAddress( mXInputModule, "XInputGetState" ) );
        mXInputSetState = reinterpret_cast< MD_XInputSetState >( ::GetProcAddress( mXInputModule, "XInputSetState" ) );

        return LN_OK;
    }

    // XInputGetState
    DWORD XInputModule::XInputGetState( DWORD dwUserIndex, XINPUT_STATE* pState )
    {
        return mXInputGetState( dwUserIndex, pState );
    }

    /// XInputSetState
    DWORD XInputModule::XInputSetState( DWORD dwUserIndex, XINPUT_VIBRATION* pVibration )
    {
        return mXInputSetState( dwUserIndex, pVibration );
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Win32
} // namespace Input
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================