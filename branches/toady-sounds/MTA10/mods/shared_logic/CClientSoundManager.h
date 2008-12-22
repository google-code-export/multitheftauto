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

class CClientSoundManager;

#ifndef __CCLIENTSOUNDMANAGER_H
#define __CCLIENTSOUNDMANAGER_H

#include <list>
#include <irrKlang.h>
#include "CClientSound.h"

using namespace irrklang;

class CClientSoundManager
{
public:

                        CClientSoundManager         ( CClientManager* pClientManager );
                        ~CClientSoundManager        ( void );

    ISoundEngine*       GetEngine                   ( void )                    { return m_pSoundEngine; };

    void                DoPulse                     ( void );

    CClientSound*       PlaySound2D                 ( const char* szFile );
    CClientSound*       PlaySound3D                 ( const char* szFile, CVector vecPosition );

private:

    CClientManager*     m_pClientManager;
    ISoundEngine*       m_pSoundEngine;
};

#endif
