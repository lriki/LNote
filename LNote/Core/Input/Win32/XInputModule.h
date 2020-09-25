//=============================================================================
//【 XInputModule 】
//-----------------------------------------------------------------------------
///**
//  @file       XInputModule.h
//  @brief      XInputModule
//  @author     Riki
//*/
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

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
//-----------------------------------------------------------------------------
/**
// @brief      XInput DLL モジュール
//*/
//=============================================================================
class XInputModule
{
public:

    /// コンストラクタ
    XInputModule();

    /// デストラクタ
    ~XInputModule();

public:

    /// 初期化
    LNRESULT initialize();

    /// XInputGetState
    DWORD XInputGetState( DWORD dwUserIndex, XINPUT_STATE* pState );

    /// XInputSetState
    DWORD XInputSetState( DWORD dwUserIndex, XINPUT_VIBRATION* pVibration );


private:

    typedef DWORD ( WINAPI *MD_XInputGetState )(
        DWORD         dwUserIndex,
        XINPUT_STATE* pState
    );

    typedef DWORD ( WINAPI *MD_XInputSetState )(
        DWORD             dwUserIndex,
        XINPUT_VIBRATION* pVibration
    );

private:

    HMODULE             mXInputModule;   ///< XINPUT_DLL ("XInput～.dll")
    MD_XInputGetState   mXInputGetState;
    MD_XInputSetState   mXInputSetState;

};

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