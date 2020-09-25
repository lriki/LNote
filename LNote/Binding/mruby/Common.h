//=============================================================================
// ■ Common
//-----------------------------------------------------------------------------
//  このファイルを stdafx.h でインクルードするようにする。
//=============================================================================

#pragma once

//---------------------------------------------------------------------
// include

#define LNOTE_NOT_LINK_LIB     // ヘッダファイル内で lib をリンクしないようにする
#include "../../lnote.hpp"
//#include "../../../lnote.h"
using namespace LNote;
using namespace LNote::Core;

#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/proc.h>
#include <mruby/string.h>
#include <mruby/data.h>

#include "mrubyEngine.h"
#include "mrubyException.h"

//---------------------------------------------------------------------
// lib

#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "msacm32.lib" )
#pragma comment( lib, "OpenGL32.lib" )

//---------------------------------------------------------------------
//

// ポインタをキャスト後、実体を参照
#define LNMRUBY_CAST( type_, value_ ) (*((type_*)mrb_get_datatype( mrb_, value_, &MRB_DATA_TYPE )))

// mrb_get_datatype() でインスタンス取得 → メンバアクセスの簡略化
#define LNMRUBY_ACCESS_MEMBER( type_, member_ ) ((type_*)mrb_get_datatype( mrb_, self_, &MRB_DATA_TYPE ))->member_

// ruby 引数をキャスト後、実体を参照 ("o" のみ)
#define LNMRUBY_GET_ARG1( type_ ) (*((type_*)_lnmruby_cast_arg_( mrb_, &MRB_DATA_TYPE )))
inline void* _lnmruby_cast_arg_( mrb_state* mrb_, const mrb_data_type* data_type_ )
{
    mrb_value a1;
    mrb_get_args( mrb_, "o", &a1 );
    return mrb_get_datatype( mrb_, a1, data_type_ );
}

// 構造体ポインタのラップ
#define LNMRUBY_DATA_WRAP( obj_ ) mrb_obj_value( Data_Wrap_Struct( mrb_, mRubyClass, &MRB_DATA_TYPE, obj_ ) );

// bool
#define LN_MRB_BOOL_VALUE( x_ ) ( ( x_ ) ? mrb_true_value() : mrb_false_value() )

namespace LNote
{
namespace Binding
{
namespace mruby
{

// String オブジェクトから文字列取得
inline const char* GetCStr( mrb_state* mrb_, mrb_value* str_ )
{
    const char* c_str = (const char*)mrb_string_value_cstr( mrb_, str_ );

    // Visual C++ なら sjis に変換
    //    int nSize = 0;
        //ConvUtf8toSJis( pSource, NULL, &nSize );
        //BYTE* pDist = new BYTE[ nSize + 1 ];
        //ZeroMemory( pDist, nSize + 1 );
        //ConvUtf8toSJis( pSource, pDist, &nSize );
    return c_str;
}

// dump_p 実装
#define LNMRUBY_DUMP_P( type_ ) \
    static mrb_value dump_p( mrb_state* mrb_, mrb_value self_ ) \
    { \
        mrb_value a1; \
        int argc = mrb_get_args( mrb_, "|S", &a1 ); \
        LNMRUBY_CAST( type_, self_ ).dump( ( argc == 1 ) ? GetCStr( mrb_, &a1 ) : NULL ); \
        return self_; \
    }

} // namespace mruby
} // namespace Binding
} // namespace LNote

//=============================================================================
//
//=============================================================================