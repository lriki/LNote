//=============================================================================
//【 lnote 】
//-----------------------------------------------------------------------------
/*
    ▼ このヘッダファイルの前に定義できるマクロ


    #define LNOTE_NOT_LINK_LIB

        ヘッダファイル内で #pragma comment によって 
        lib ファイルをリンクしないようにする。

*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
// include

#include "Core/Common.h"

#include "Core/Base/CommandLineArguments.h"
#include "Core/Base/Time.h"
#include "Core/Base/RefPtr.h"

#include "Core/Math/Enum.h"
#include "Core/Math/Math.h"
#include "Core/Math/Random.h"

#include "Core/Geometry/Vector2.h"
#include "Core/Geometry/Vector3.h"
#include "Core/Geometry/Vector4.h"
#include "Core/Geometry/Matrix.h"
#include "Core/Geometry/Quaternion.h"

#include "Core/Animation/RoundTripValue.h"

#include "Core/File/Manager.h"
#include "Core/File/InFile.h"
#include "Core/File/OutFile.h"

#include "Core/System/Common/PerfStopwatch.h"

#include "Core/Input/Manager.h"

#include "Core/Audio/Manager.h"

#include "Core/Physics/Manager.h"
#include "Core/Physics/RigidBody/Plane.h"
#include "Core/Physics/RigidBody/Box.h"

#include "Core/Graphics/Interface.h"
#include "Core/Graphics/Manager.h"
#include "Core/Graphics/Common/GraphicsUtil.h"
#include "Core/Graphics/Common/SpriteRenderer.h"
#include "Core/Graphics/Common/Model/ModelManager.h"
#include "Core/Graphics/Common/Model/Model.h"
#include "Core/Graphics/Common/Model/ModelCore.h"
#include "Core/Graphics/Common/Model/Motion.h"
#include "Core/Graphics/Common/DebugText.h"

#include "Core/Effect/Manager.h"
#include "Core/Effect/Instance/EffectNodeInstance.h"
#include "Core/Effect/Core/EffectCore.h"
#include "Core/Effect/Instance/EffectInstance.h"
#include "Core/Effect/Emitter/EmitterBase.h"
#include "Core/Effect/Emitter/Sprite3DEmitter.h"
#include "Core/Effect/Emitter/RingEmitter.h"

#include "Core/GUI/Manager.h"
#include "Core/GUI/GUIElement.h"
#include "Core/GUI/Shapes/GUITextBlock.h"
#include "Core/GUI/Controls/GUIControl.h"
#include "Core/GUI/Controls/GUIDesktop.h"
#include "Core/GUI/Controls/GUIButton.h"

#include "Core/Scene/Manager.h"
#include "Core/Scene/SceneGraph.h"
#include "Core/Scene/SceneShader/SceneShaderManager.h"
#include "Core/Scene/SceneShader/SceneShader.h"
#include "Core/Scene/SceneNode/SceneNode.h"
#include "Core/Scene/SceneNode/Grid.h"
#include "Core/Scene/SceneNode/Camera.h"
#include "Core/Scene/SceneNode/Light.h"
#include "Core/Scene/SceneNode/Sprite.h"
#include "Core/Scene/SceneNode/Sprite3D.h"
#include "Core/Scene/SceneNode/BatchSprite.h"
#include "Core/Scene/SceneNode/Model.h"
#include "Core/Scene/SceneNode/PointParticle.h"
#include "Core/Scene/SceneNode/Tilemap.h"
#include "Core/Scene/SceneNode/Effect.h"
#include "Core/Scene/SceneNode/Proxy.h"

#include "Core/Game/GameSceneManager.h"
#include "Core/Game/GameScene.h"
#include "Core/Game/GameObject.h"
#include "Core/Game/Component/ModelComponent.h"

#include "Core/Framework/Framework.h"

//-------------------------------------------------------------------------
// lib

#if defined(LNOTE_MSVC)
    #include "link.h"
    
    #ifndef LNOTE_NOT_LINK_LIB
        #pragma comment( lib, "winmm.lib" )
        #pragma comment( lib, "msacm32.lib" )
		#pragma comment( lib, "ws2_32.lib" )
        #pragma comment( lib, "OpenGL32.lib" )
		       
        #pragma comment( lib, LNOTE_LIB_NAME )
        #pragma comment( lib, LNOTE_DEPEND_LIB_NAME )
    #endif
#endif

//=============================================================================
//                              end of file
//=============================================================================