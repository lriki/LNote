//=============================================================================
//【 Window 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"

#if defined(LNOTE_ANDROID)
#include "../../../AndroidMain/native_app_glue.h"
#include <android/native_window.h>
#include "../Manager.h"
#include "AndroidWindow.h"
#include "AndroidApplication.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace System
{

//=============================================================================
// ■ AndroidWindow クラス
//=============================================================================

    // 初期化
    LNRESULT AndroidWindow::initialize( const InitData& init_data_ )
    {
        
        
        mClientSize.set(
            ANativeWindow_getWidth( mApplication->getAndroidApp()->window ),
            ANativeWindow_getHeight( mApplication->getAndroidApp()->window ) );
        return LN_OK;
    }
    
    // ANativeWindow の取得
    ANativeWindow* AndroidWindow::getNativeWindow()
    {
        return mApplication->getAndroidApp()->window;
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace System
} // namespace Core

} // namespace LNote

#endif // LNOTE_ANDROID
//=============================================================================
//								end of file
//=============================================================================