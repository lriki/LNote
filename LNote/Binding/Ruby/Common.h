
#pragma once


//---------------------------------------------------------------------
// include

#define LNOTE_NOT_LINK_LIB     // �w�b�_�t�@�C������ lib �������N���Ȃ��悤�ɂ���
#include "../../lnote.hpp"
using namespace LNote;
using namespace LNote::Core;

#include <ruby/ruby.h>



//---------------------------------------------------------------------
// define

#define TO_RUBY_FUNC( f ) reinterpret_cast< VALUE(__cdecl *)(...) >( f )

// ���� -> ���� �ϊ����� Float �I�u�W�F�N�g�̐������J�b�g���邽�߂̃}�N��
#define NUM2FLOAT( x ) ((float)( FIXNUM_P(x) ? FIX2INT(x) : NUM2DBL(x) ))

// VALUE ����C���X�^���X�|�C���^���擾����
template<typename T>
inline T* lnGetStructFromRubyObject(VALUE self_)
{
    T* obj;
    Data_Get_Struct(self_, T, obj);
    return obj;
}
#define GET_STRUCT_PTR(self_, type_) lnGetStructFromRubyObject<type_>(self_)