//=============================================================================
//�y LFFramework �z
//-----------------------------------------------------------------------------
///**
//  @file       LFFramework.h
//  @brief      LFFramework
//  @author     Riki
//*/
//=============================================================================

#include "../Core/BuildSetting.h"
#include "../lnote.hpp"

using namespace LNote;

//-----------------------------------------------------------------------------

// �|�C���^�� int �L���X�g
#define LN_TO_INT( ptr_ )           reinterpret_cast< int >( ptr_ )
#define LN_INT_TO( type_, value_ )  reinterpret_cast< type_ >( value_ )

// x_ y_ �� Geometry::Size �ɑ��
#define LNFUNC_LSize_TO( v_, w_, h_ ) \
        const Geometry::Size& v = ( v_ ); \
        w_ = v.w; \
        h_ = v.h;

// xyz �� Vector3 �ɑ��
#define LNFUNC_SUBSTITUTE_VECTOR3_TO_XYZ( vec3_, x_, y_, z_ ) \
        const LVector3& v3 = ( vec3_ ); \
        *x_ = v3.x; \
        *y_ = v3.y; \
        *z_ = v3.z;


//-----------------------------------------------------------------------------
// �֐����C�u�������ŋ��ʂ̂���

namespace LNote
{
namespace Core
{
namespace Function
{
    extern Framework* gFramework;
    extern Graphics::IGraphicsDevice* gGraphicsDevice;
    extern Graphics::IRenderer* gRenderer;
}
}
}

//=============================================================================
//								end of file
//=============================================================================