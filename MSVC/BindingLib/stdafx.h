#pragma once

#include "../../LNote/Core/BuildSetting.h"
//#include "../../LNote/lnote.hpp"
//using namespace LNote;

//---------------------------------------------------------------------

#define LNOTE_MRUBY     0
#define LNOTE_RUBY      1

//---------------------------------------------------------------------
#if LNOTE_MRUBY
    #include "../../LNote/Binding/mruby/Common.h"
#endif
#if LNOTE_RUBY
    #include "../../LNote/Binding/Ruby/Common.h"
#endif
