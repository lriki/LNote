
#pragma once

#define LNOTE_LIB_DIR "../../LNote"

#include "../../LNote/Binding/mruby/EntryPoint.h"
#include "../../LNote/Binding/Ruby/RubyEngine.h"

//---------------------------------------------------------------------
// lib

#pragma comment( lib, "../../LNote/msvcr90-ruby191-static.lib" )
#pragma comment( lib, "ws2_32.lib" )
#pragma comment( lib, "imagehlp.lib" )
#pragma comment( lib, "Shlwapi.lib" )