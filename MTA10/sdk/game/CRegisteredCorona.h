/*****************************************************************************
*
*  PROJECT:		Multi Theft Auto v1.0
*  LICENSE:		See LICENSE in the top level directory
*  FILE:		sdk/game/CRegisteredCorona.h
*  PURPOSE:		Registered corona entity interface
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#ifndef __CGAME_REGISTEREDCORONA
#define __CGAME_REGISTEREDCORONA

#include "Common.h"
#include <CVector.h>

#include <windows.h>
#include "RenderWare.h"

class CRegisteredCorona
{
public:
	virtual const CVector	GetPosition( void ) = 0;
	virtual void			SetPosition( const CVector& vector ) = 0;

	virtual float			GetSize( void ) = 0;
	virtual void			SetSize( float fSize ) = 0;

	virtual float			GetRange( void ) = 0;
	virtual void			SetRange( float fRange ) = 0;

	virtual float			GetPullTowardsCamera( void ) = 0;
	virtual void			SetPullTowardsCamera( float fPullTowardsCamera ) = 0;

	virtual void			SetColor( BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha ) = 0;
	virtual void			SetTexture( RwTexture * texture ) = 0;
	virtual void			SetTexture( eCoronaType texture ) = 0;

	virtual BYTE			GetFlareType( void ) = 0;
	virtual void			SetFlareType( BYTE fFlareType ) = 0;

	virtual DWORD			GetIdentifier( void ) = 0;
	virtual DWORD			GetID( void ) = 0;

	virtual void			Refresh( void ) = 0;
	virtual void			Disable( void ) = 0;
};
#endif