/*****************************************************************************
*
*  PROJECT:		Multi Theft Auto v1.0
*  LICENSE:		See LICENSE in the top level directory
*  FILE:		game_sa/CAudioEngineSA.h
*  PURPOSE:		Header file for audio manager class
*  DEVELOPERS:	Ed Lyons <eai@opencoding.net>
*               Christian Myhre Lundheim <>
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#ifndef __CGAMESA_AUDIOENGINE
#define __CGAMESA_AUDIOENGINE

#include "Common.h"
#include <game/CAudioEngine.h>
#include "CVehicleSA.h"

#define FUNC_CAudioEngine_ReportFrontendAudioEvent			0x506EA0
#define FUNC_CAudioEngine_PreloadBeatTrack					0x507F40
#define FUNC_CAudioEngine_PlayPreloadedBeatTrack			0x507180

#define FUNC_CAudioEngine_SetEffectsMasterVolume			0x506E10
#define FUNC_CAudioEngine_SetMusicMasterVolume				0x506DE0

#define FUNC_CAudioEngine_ReportMissionAudioEvent_Vehicle	0x507390
#define FUNC_CAudioEngine_ReportMissionAudioEvent_Vector	0x507340
#define FUNC_CAudioEngine_PauseAllSounds                    0x507430
#define FUNC_CAudioEngine_ResumeAllSounds                   0x507440
#define FUNC_CAudioEngine_StopRadio							0x506F70
#define FUNC_CAudioEngine_StartRadio2						0x507DC0
#define FUNC_CAudioEngine_ReportBulletHit                   0x506ec0
#define FUNC_CAudioEngine_ReportWeaponEvent                 0x506f40

#define VAR_AudioEventVolumes                               0xBD00F8 // m_pAudioEventVolumes

#define CLASS_CAudioEngine						            0xB6BC90
#define CLASS_AERadioTrackManager                           0x8CB6F8
#define CLASS_AECutsceneTrackManager                        0x8AE554

class CAudioEngineSAInterface
{
};

class CAudioEngineSA : public CAudioEngine
{
public:
                                CAudioEngineSA                  ( CAudioEngineSAInterface * pInterface )    { m_pInterface = pInterface; }                    

	void                        PlayFrontEndSound               ( DWORD dwSound );
	void                        PlayBeatTrack                   ( short iTrack );
	void                        SetEffectsMasterVolume          ( BYTE bVolume );
	void                        SetMusicMasterVolume            ( BYTE bVolume );
	void                        ClearMissionAudio               ( int slot = 1 );
	void                        PreloadMissionAudio             ( unsigned short usAudioEvent, int slot = 1 );
	unsigned char               GetMissionAudioLoadingStatus    ( int slot = 1 );
	bool                        IsMissionAudioSampleFinished    ( int slot = 1 );
	void                        AttachMissionAudioToPhysical    ( CPhysical * physical, int slot = 1 );
	void                        SetMissionAudioPosition         ( CVector * position, int slot = 1 );
	bool                        PlayLoadedMissionAudio          ( int slot = 1 );
    void                        PauseAllSound                   ( bool bPaused );
	void                        StopRadio                       ( void );
    void                        StartRadio                      ( unsigned int station );

    void                        ReportBulletHit                 ( CEntity * pEntity, unsigned char ucSurfaceType, CVector * pvecPosition, float f_2 );
    void                        ReportWeaponEvent               ( int iEvent, eWeaponType weaponType, CPhysical * pPhysical );

private:
    CAudioEngineSAInterface *   m_pInterface;
};

#endif