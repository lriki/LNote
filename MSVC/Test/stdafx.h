
#pragma once

#include <iostream>
#define LNOTE_LIB_DIR "../../LNote"

#include "../../LNote/lnote.hpp"
#include "../../LNote/lnote.h"
using namespace LNote;


//-----------------------------
#define STATIC_TEST
//#define FUNCTION_TEST
//#define UTILITY_TEST

//-----------------------------
#ifdef STATIC_TEST

    #include "../../LNote/lnote.hpp"
    //-----------------------------
    using namespace LNote;
	using namespace LNote::Core;
    
#endif

//-----------------------------
#ifdef FUNCTION_TEST

    #include "../../LNote/lnote.h"

#endif
//-----------------------------
#ifdef UTILITY_TEST

    #include "../../LNote/lnote.h"
    #include "../../LNote/Utility/Light/Common.h"
    #pragma comment (lib, LNOTE_UTILITY_LIB_NAME)
    using namespace LNote::Accessor;

#endif



