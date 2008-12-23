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

class CClientSound : public CClientEntity
{
    friend CClientSoundManager;

public:

                            CClientSound            ( CClientManager* pManager, ElementID ID );
                            ~CClientSound           ( void );

    eClientEntityType       GetType                 ( void ) const                      { return CCLIENTSOUND; }

    bool                    Play                    ( const char* szPath, bool bLoop );
    bool                    Play3D                  ( const char* szPath, CVector vecPosition, bool bLoop );
    void                    Stop                    ( void );

    void                    SetPaused               ( bool bPaused );
    bool                    IsPaused                ( void );

    bool                    IsFinished              ( void );

    void                    SetPlayPosition         ( unsigned int uiPosition );
    unsigned int            GetPlayPosition         ( void );

    unsigned int            GetLength               ( void );

    void                    SetVolume               ( float fVolume );
    float                   GetVolume               ( void );

    void                    SetMinDistance          ( float fDistance );
    float                   GetMinDistance          ( void );

    void                    SetMaxDistance          ( float fDistance );
    float                   GetMaxDistance          ( void );

    void                    Unlink                  ( void ) {};
    void                    GetPosition             ( CVector& vecPosition ) const;
    void                    SetPosition             ( const CVector& vecPosition );

protected:

    ISound*                 GetSound                ( void )                            { return m_pSound; };

private:

    CClientSoundManager*    m_pSoundManager;
    ISound*                 m_pSound;
};

#endif
