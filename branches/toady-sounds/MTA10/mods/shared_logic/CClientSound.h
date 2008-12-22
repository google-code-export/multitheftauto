/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*               (Shared logic for modifications)
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        mods/shared_logic/CClientSound.h
*  PURPOSE:     Sound entity class header
*  DEVELOPERS:  Stanislav Bobrov <lil_Toady@hotmail.com>
*
*****************************************************************************/

class CClientSound;

#ifndef __CCLIENTSOUND_H
#define __CCLIENTSOUND_H

#include "CClientSoundManager.h"
#include "CClientEntity.h"

using namespace irrklang;

class CClientSound : public CClientEntity, ISoundStopEventReceiver
{
public:

                            CClientSound            ( CClientManager* pManager, ElementID ID );
                            ~CClientSound           ( void );

    void                    Play                    ( const char* szPath );
    void                    Play3D                  ( const char* szPath, CVector vecPosition );
    void                    Stop                    ( void );

    void                    SetPaused               ( bool bPaused );
    bool                    IsPaused                ( void );

    bool                    IsFinished              ( void );

    void                    SetPlayPosition         ( unsigned int uiPosition );
    unsigned int            GetPlayPosition         ( void );

    unsigned int            GetLength               ( void );

    void                    SetVolume               ( float fVolume );
    float                   GetVolume               ( void );

    eClientEntityType       GetType                 ( void ) const                      { return CCLIENTSOUND; }

    void                    Unlink                  ( void ) {};
    void                    GetPosition             ( CVector& vecPosition ) const;
    void                    SetPosition             ( const CVector& vecPosition );

    virtual void            OnSoundStopped          ( ISound* sound, E_STOP_EVENT_CAUSE reason, void* pObj );

private:

    ISoundEngine*           m_pSoundEngine;
    ISound*                 m_pSound;
};

#endif
