
#pragma once


//---------------------------------------------------------------------
// include

#define LNOTE_NOT_LINK_LIB     // ヘッダファイル内で lib をリンクしないようにする
#include "../../lnote.hpp"
using namespace LNote;
using namespace LNote::Core;

#include <ruby/ruby.h>



//---------------------------------------------------------------------
// define

#define TO_RUBY_FUNC( f ) reinterpret_cast< VALUE(__cdecl *)(...) >( f )

// 整数 -> 実数 変換時の Float オブジェクトの生成をカットするためのマクロ
#define NUM2FLOAT( x ) ((float)( FIXNUM_P(x) ? FIX2INT(x) : NUM2DBL(x) ))

// VALUE からインスタンスポインタを取得する
template<typename T>
inline T* lnGetStructFromRubyObject(VALUE self_)
{
    T* obj;
    Data_Get_Struct(self_, T, obj);
    return obj;
}
#define GET_STRUCT_PTR(self_, type_) lnGetStructFromRubyObject<type_>(self_)