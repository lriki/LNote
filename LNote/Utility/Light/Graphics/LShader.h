//=============================================================================
//�y LShader �z
//-----------------------------------------------------------------------------
///**
//  @file       LShader.h
//  @brief      LShader
//  @version    1.0
//  @date       2010/11/07
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Core/Scene/Manager.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

//=============================================================================
// �� LShader �N���X
//-----------------------------------------------------------------------------
///**
//  @brief      �V�F�[�_�v���O�����̃N���X
//*/
//=============================================================================
class LShader
    : public LRefClass< LShader, Core::Scene::SceneShader >
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      �V�F�[�_�v���O�������쐬����
    //*/
    //---------------------------------------------------------------------
    static LShader create( const char* filename_ );

    LN_UTIL_REFCLASS_METHODS( LShader, Core::Scene::SceneShader );
};


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================