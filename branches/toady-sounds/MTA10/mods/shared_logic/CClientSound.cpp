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

extern CClientGame* g_pClientGame;
extern CCoreInterface* g_pCore;
CClientSound::CClientSound ( CClientManager* pManager, ElementID ID ) : CClientEntity ( ID )
{
    m_pSoundEngine = pManager->GetSoundManager()->GetEngine();
    m_pSound = NULL;
}

CClientSound::~CClientSound ( void )
{
    if ( m_pSound )
        m_pSound->drop ();
}

void CClientSound::Play ( const char* szPath )
{
    m_pSound = m_pSoundEngine->play2D ( szPath, false, false, true );
    if ( m_pSound )
    {
        m_pSound->setSoundStopEventReceiver ( this );
    }
}

void CClientSound::Play3D ( const char* szPath, CVector vecPosition )
{
    vec3df pos ( vecPosition.fX, vecPosition.fY, vecPosition.fZ );
	m_pSound = m_pSoundEngine->play3D ( szPath, pos, false, false, true );
    if ( m_pSound )
    {
        m_pSound->setSoundStopEventReceiver ( this );
    }
}

void CClientSound::Stop ( void )
{
    if ( m_pSound )
    {
        m_pSound->stop ();
    }
    g_pClientGame->GetElementDeleter()->Delete ( this );
}

void CClientSound::SetPaused ( bool bPaused )
{
    if ( m_pSound )
    {
        return m_pSound->setIsPaused ( bPaused );
    }
}

bool CClientSound::IsPaused ( void )
{
    if ( m_pSound )
    {
        return m_pSound->getIsPaused ();
    }
    return false;
}

bool CClientSound::IsFinished ( void )
{
    if ( m_pSound )
    {
        return m_pSound->isFinished ();
    }
    return false;
}

void CClientSound::SetPlayPosition ( unsigned int uiPosition )
{
    if ( m_pSound )
    {
        m_pSound->setPlayPosition ( uiPosition );
    }
}

unsigned int CClientSound::GetPlayPosition ( void )
{
    if ( m_pSound )
    {
        return m_pSound->getPlayPosition ();
    }
    return 0;
}

unsigned int CClientSound::GetLength ( void )
{
    if ( m_pSound )
    {
        return m_pSound->getPlayLength ();
    }
}

void CClientSound::SetVolume ( float fVolume )
{
    if ( m_pSound )
    {
        m_pSound->setVolume ( fVolume );
    }
}

float CClientSound::GetVolume ( void )
{
    if ( m_pSound )
    {
        return m_pSound->getVolume ();
    }
    return 0.0f;
}

void CClientSound::SetPosition ( const CVector& vecPosition )
{
    if ( m_pSound )
    {
        m_pSound->setPosition ( vec3df ( vecPosition.fX, vecPosition.fY, vecPosition.fZ ) );
    }
}

void CClientSound::GetPosition ( CVector& vecPosition ) const
{
    if ( m_pSound )
    {
        vec3df pos = m_pSound->getPosition ();
        vecPosition.fX = pos.X;
        vecPosition.fY = pos.Y;
        vecPosition.fZ = pos.Z;
    }
}

void CClientSound::OnSoundStopped ( ISound* sound, E_STOP_EVENT_CAUSE reason, void* pObj )
{
    g_pClientGame->GetElementDeleter()->Delete ( this );
}