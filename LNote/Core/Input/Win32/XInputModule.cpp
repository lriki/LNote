//=============================================================================
//【 XInputModule 】
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
// ■ XInputModule クラス
//=============================================================================

    // コンストラクタ
    XInputModule::XInputModule()
        : mXInputModule     ( NULL )
        , mXInputGetState   ( NULL )
    {
    }

    // デストラクタ
    XInputModule::~XInputModule()
    {
        if ( mXInputModule )
        {
            ::FreeLibrary( mXInputModule );
        }
    }

    // 初期化
    LNRESULT XInputModule::initialize()
    {
        // モジュール読み込み
        mXInputModule = ::LoadLibrary( XINPUT_DLL );
        LN_SETERR_R_S_( ( mXInputModule ), LN_ERR_SYSTEM,
            "#XInput の DLL の読み込みに失敗しました。\n"
            "DirectX が正しくインストールされていないか、\n"
            "古いバージョンである可能性があります。" );

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