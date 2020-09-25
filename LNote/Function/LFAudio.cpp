//=============================================================================
//【 LFAudio 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../Core/Audio/Manager.h"
#include "../Core/Audio/GameAudio.h"
#include "FuncLibManager.h"
#include "LFStdAfx.h"
#include "LFAudio.h"

#define LN_INT_TO_SOUND( v_ )   reinterpret_cast< Core::Audio::ISound* >( v_ )

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
LNOTE_FUNC_NAMESPACE_BEGIN

//=============================================================================
// LAudio
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void LAudio_End()
    {
        Manager::finalize();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    HRESULT LAudio_PlayBGM( const TCHAR* filename, int volume, int pitch, int fade_time )
    {
        LN_FUNC_CALL_THROW(
            Manager::AudioManager->getGameAudio()->playBGM( filename, volume, pitch, fade_time ) );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    HRESULT LAudio_PlayBGMMem( const void* data, int data_size, int volume, int pitch, int fade_time )
    {
        LN_FUNC_CALL_THROW(
            Core::Audio::ISound* sound = LN_INT_TO_SOUND( LSound_CreateMem( data, data_size, false ) );
            Manager::AudioManager->getGameAudio()->playBGMFromSound( sound, volume, pitch, fade_time );
            LSound_Release( LN_TO_INT( sound ) );
        );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
	HRESULT LAudio_StopBGM( int fade_time )
    {
        LN_FUNC_CALL_THROW( Manager::AudioManager->getGameAudio()->stopBGM( fade_time ) );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    HRESULT LAudio_PlayBGS( const TCHAR* filename, int volume, int pitch, int fade_time )
    {
        LN_FUNC_CALL_THROW( Manager::AudioManager->getGameAudio()->playBGS( filename, volume, pitch, fade_time ) );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    HRESULT LAudio_PlayBGSMem( const void* data, int data_size, int volume, int pitch, int fade_time )
    {
        LN_FUNC_CALL_THROW(
            Core::Audio::ISound* sound = LN_INT_TO_SOUND( LSound_CreateMem( data, data_size, false ) );
            Manager::AudioManager->getGameAudio()->playBGSFromSound( sound, volume, pitch, fade_time );
            LSound_Release( LN_TO_INT( sound ) );
        );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
	HRESULT LAudio_StopBGS( int fade_time )
    {
        LN_FUNC_CALL_THROW( Manager::AudioManager->getGameAudio()->stopBGS( fade_time ) );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    HRESULT LAudio_PlayME( const TCHAR* filename, int volume, int pitch )
    {
        LN_FUNC_CALL_THROW( Manager::AudioManager->getGameAudio()->playME( filename, volume, pitch ) );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    HRESULT LAudio_PlayMEMem( const void* data, int data_size, int volume, int pitch )
    {
        LN_FUNC_CALL_THROW(
            Core::Audio::ISound* sound = LN_INT_TO_SOUND( LSound_CreateMem( data, data_size, false ) );
            Manager::AudioManager->getGameAudio()->playMEFromSound( sound, volume, pitch );
            LSound_Release( LN_TO_INT( sound ) );
        );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
	HRESULT LAudio_StopME()
    {
        LN_FUNC_CALL_THROW( Manager::AudioManager->getGameAudio()->stopME() );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    HRESULT LAudio_PlaySE( const TCHAR* filename, int volume, int pitch )
    {
        LN_FUNC_CALL_THROW( Manager::AudioManager->getGameAudio()->playSE( filename, volume, pitch ) );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    HRESULT LAudio_PlaySE3D( const TCHAR* filename, float x, float y, float z, float distance, int volume, int pitch )
    {
        LN_FUNC_CALL_THROW( Manager::AudioManager->getGameAudio()->playSE( filename, LVector3( x, y, z ), distance, volume, pitch ) );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    HRESULT LAudio_PlaySEMem( const void* data, int data_size, int volume, int pitch )
    {
        LN_FUNC_CALL_THROW(
            Core::Audio::ISound* sound = LN_INT_TO_SOUND( LSound_CreateMem( data, data_size, false ) );
            Manager::AudioManager->getGameAudio()->playSEFromSound( sound, volume, pitch );
            LSound_Release( LN_TO_INT( sound ) );
        );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    HRESULT LAudio_PlaySE3DMem( const void* data, int data_size, float x, float y, float z, float distance, int volume, int pitch )
    {
        LN_FUNC_CALL_THROW(
            Core::Audio::ISound* sound = LN_INT_TO_SOUND( LSound_CreateMem( data, data_size, true ) );
            sound->setPosition( LVector3( x, y, z ) );
            sound->setMaxDistance( distance );
            Manager::AudioManager->getGameAudio()->playMEFromSound( sound, volume, pitch );
            LSound_Release( LN_TO_INT( sound ) );
        );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
	HRESULT LAudio_StopSE()
    {
        LN_FUNC_CALL_THROW( Manager::AudioManager->getGameAudio()->stopSE() );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
	HRESULT LAudio_SetMetreUnitDistance( float distance )
    {
        LN_FUNC_CALL_THROW( Manager::AudioManager->getAudioDevice()->setMetreUnitDistance( distance ) );
    }

//=============================================================================
// L3DSoundListener
//=============================================================================

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    HRESULT L3DSoundListener_SetPosition( float x, float y, float z )
    {
        LN_FUNC_CALL_THROW( Manager::AudioManager->getAudioDevice()->get3DSoundListener()->Position.set( x, y, z ) );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    HRESULT L3DSoundListener_SetDirection( float x, float y, float z )
    {
        LN_FUNC_CALL_THROW( Manager::AudioManager->getAudioDevice()->get3DSoundListener()->Direction.set( x, y, z ) );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    HRESULT L3DSoundListener_SetUpDirection( float x, float y, float z )
    {
        LN_FUNC_CALL_THROW( Manager::AudioManager->getAudioDevice()->get3DSoundListener()->UpDirection.set( x, y, z ) );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    HRESULT L3DSoundListener_Velocity( float x, float y, float z )
    {
        LN_FUNC_CALL_THROW( Manager::AudioManager->getAudioDevice()->get3DSoundListener()->Velocity.set( x, y, z ) );
    }

//=============================================================================
// LSound
//=============================================================================

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    LNHandle LSound_Load( const TCHAR* filename, bool enable_3d, LNSoundPlayType play_type )
    {
        Core::Audio::ISound* sound = NULL;
        LN_FUNC_CALL_RETURN_0( 
            Manager::AudioManager->createSound( &sound, filename, play_type, enable_3d );
            Manager::SoundList.add( sound );
        );
        return LN_TO_INT( sound );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    LNHandle LSound_CreateMem( const void* data, int data_size, bool enable_3d )
    {
        Audio::ISound* sound;
        LN_FUNC_CALL_RETURN_0( 
            Base::FinallyReleasePtr< Core::File::IInStream > stream;
            File::createInStreamFromMemoryManaged( &stream, data, data_size );
            
            Manager::AudioManager->createSound(
                &sound, stream, LN_SOUNDPLAYTYPE_ONMEMORY, enable_3d, lnNullKey );

            Manager::SoundList.add( sound );
        );
        return LN_TO_INT( sound );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    HRESULT LSound_LoadToPtr( LNHandle* out_handle, const TCHAR* filename, bool enable_3d, LNSoundPlayType play_type )
    {
        *out_handle = LSound_Load( filename, enable_3d, play_type );
        return Manager::getLastErrorCode();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void LSound_Release( LNHandle sound_handle )
    {
        Manager::SoundList.release( LN_INT_TO_SOUND( sound_handle ) );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    HRESULT LSound_SetVolume( LNHandle sound_handle, int volume )
    {
        LN_FUNC_CALL_THROW( LN_INT_TO_SOUND( sound_handle )->setVolume( volume ) );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    int LSound_GetVolume( LNHandle sound_handle )
    {
        return LN_INT_TO_SOUND( sound_handle )->getVolume();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    HRESULT LSound_SetPitch( LNHandle sound_handle, int pitch )
    {
        LN_FUNC_CALL_THROW( LN_INT_TO_SOUND( sound_handle )->setPitch( pitch ) );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    int LSound_GetPitch( LNHandle sound_handle )
    {
        return LN_INT_TO_SOUND( sound_handle )->getPitch();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    HRESULT LSound_SetLoopState( LNHandle sound_handle, bool loop, int begin, int length )
    {
        LN_FUNC_CALL_THROW(
            LN_INT_TO_SOUND( sound_handle )->loop( loop );
            LN_INT_TO_SOUND( sound_handle )->setLoopState( begin, length );
        );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    bool LSound_IsLoop( int sound_handle )
    {
        return LN_INT_TO_SOUND( sound_handle )->isLoop();
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    LNSoundPlayState LSound_GetPlayState( LNHandle sound_handle )
    {
        LNAudioPlayState state;
        LN_FUNC_CALL_RETURN_ENUM( LN_INT_TO_SOUND( sound_handle )->getState( &state );, LN_SOUNDPLAYSTATE_STOP );
        return state.SoundPlayState;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    HRESULT LSound_Play( LNHandle sound_handle )
    {
        LN_FUNC_CALL_THROW( LN_INT_TO_SOUND( sound_handle )->play() );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    HRESULT LSound_Stop( LNHandle sound_handle )
    {
        LN_FUNC_CALL_THROW( LN_INT_TO_SOUND( sound_handle )->stop() );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
	HRESULT LSound_Pause( LNHandle sound_handle, bool pause )
    {
        LN_FUNC_CALL_THROW( LN_INT_TO_SOUND( sound_handle )->pause( pause ) );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    HRESULT LSound_FadeVolume( LNHandle sound_handle, int target_volume, int time, LNSoundFadeState fade_state )
    {
        LN_FUNC_CALL_THROW( LN_INT_TO_SOUND( sound_handle )->fadeVolume( target_volume, time, fade_state ) );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    int LSound_GetUnitsPlayed( LNHandle sound_handle )
    {
        LNAudioPlayState state;
        LN_FUNC_CALL_RETURN_0( LN_INT_TO_SOUND( sound_handle )->getState( &state ); );
        return state.CurrentUnits;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    int LSound_GetTotalUnits( LNHandle sound_handle )
    {
        LNAudioPlayState state;
        LN_FUNC_CALL_RETURN_0( LN_INT_TO_SOUND( sound_handle )->getState( &state ); );
        return state.TotalUnits;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    int LSound_GetUnitsParSecond( LNHandle sound_handle )
    {
        LNAudioPlayState state;
        LN_FUNC_CALL_RETURN_0( LN_INT_TO_SOUND( sound_handle )->getState( &state ); );
        return state.SamplesPerSec;
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    bool LSound_Is3DSound( LNHandle sound_handle )
    {
        return LN_INT_TO_SOUND( sound_handle )->is3DSound();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    HRESULT LSound_SetEmitterPosition( LNHandle sound_handle, float x, float y, float z )
    {
        LN_FUNC_CALL_THROW( 
            LN_INT_TO_SOUND( sound_handle )->setPosition( LVector3( x, y, z ) ) );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    HRESULT LSound_SetEmitterVelocity( int sound_handle, float x, float y, float z )
    {
        LN_FUNC_CALL_THROW( 
            LN_INT_TO_SOUND( sound_handle )->setVelocity( LVector3( x, y, z ) ) );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    HRESULT LSound_SetEmitterDistance( int sound_handle, float distance )
    {
        LN_FUNC_CALL_THROW( 
            LN_INT_TO_SOUND( sound_handle )->setMaxDistance( distance ) );
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

LNOTE_FUNC_NAMESPACE_END

//=============================================================================
//                              end of file
//=============================================================================