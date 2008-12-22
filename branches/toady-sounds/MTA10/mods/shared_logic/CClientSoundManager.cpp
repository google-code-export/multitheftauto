/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*               (Shared logic for modifications)
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        mods/shared_logic/CClientSound.h
*  PURPOSE:     Sound entity class
*  DEVELOPERS:  Stanislav Bobrov <lil_Toady@hotmail.com>
*
*****************************************************************************/

#include <StdInc.h>

CClientSoundManager::CClientSoundManager ( CClientManager* pClientManager )
{
    m_pClientManager = pClientManager;

    m_pSoundEngine = createIrrKlangDevice();
}

CClientSoundManager::~CClientSoundManager ( void )
{
    m_pSoundEngine->drop ();
}

void CClientSoundManager::DoPulse ( void )
{
    CClientCamera* pCamera = m_pClientManager->GetCamera();

    CVector vecPosition, vecLookAt;
    pCamera->GetPosition ( vecPosition );
    pCamera->GetTarget ( vecLookAt );

    m_pSoundEngine->setListenerPosition ( vec3df ( vecPosition.fX, vecPosition.fY, vecPosition.fZ ),
                                          vec3df ( vecLookAt.fX, vecLookAt.fY, vecLookAt.fZ ) );
}

CClientSound* CClientSoundManager::PlaySound2D ( const char* szFile )
{
    CClientSound* pSound = new CClientSound ( m_pClientManager, INVALID_ELEMENT_ID );
    pSound->Play ( szFile );
    return pSound;
}

CClientSound* CClientSoundManager::PlaySound3D ( const char* szFile, CVector vecPosition )
{
    CClientSound* pSound = new CClientSound ( m_pClientManager, INVALID_ELEMENT_ID );
    pSound->Play3D ( szFile, vecPosition );
    return pSound;
}
