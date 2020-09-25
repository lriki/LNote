//=============================================================================
//�y XInputModule �z
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
// �� XInputModule �N���X
//-----------------------------------------------------------------------------
/**
// @brief      XInput DLL ���W���[��
//*/
//=============================================================================
class XInputModule
{
public:

    /// �R���X�g���N�^
    XInputModule();

    /// �f�X�g���N�^
    ~XInputModule();

public:

    /// ������
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

    HMODULE             mXInputModule;   ///< XINPUT_DLL ("XInput�`.dll")
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