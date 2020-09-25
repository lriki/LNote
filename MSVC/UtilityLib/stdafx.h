#pragma once

#include "../../LNote/Core/BuildSetting.h"

/*

#include "../../LNote/Engine/Common.h"


//#include "../../LNote/RefClass/Typedef.h"

#include "../../LNote/Engine/Framework/Framework.h"

#include "../../LNote/Engine/Scene/Manager.h"
#include "../../LNote/Engine/Scene/Manager.h"


*/

#include "../../LNote/lnote.hpp"
//using namespace LNote;

#include "../../LNote/Core/Base/RefClass.h"
#include "../../LNote/Utility/Light/UtilCommon.h"
#include "../../LNote/Utility/Light/Base/LRefClass.h"

namespace LNote
{
    namespace Utility
    {
        class GameSceneManager;
    }


    extern Core::Framework*           gFramework;
    extern Utility::GameSceneManager*    gGameSceneManager;  
    
}