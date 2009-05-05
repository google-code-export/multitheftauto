/*****************************************************************************
*
*  PROJECT:		Multi Theft Auto v1.0
*  LICENSE:		See LICENSE in the top level directory
*  FILE:		sdk/game/CFire.h
*  PURPOSE:		Fire interface
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#ifndef __CGAME_FIRE
#define __CGAME_FIRE

#include <windows.h>
#include <CVector.h>
#include "CEntity.h"

class CFireInterface
{
public:
};

class CFire
{
public:
	virtual void					Extinguish (  )=0;
	virtual CVector				    * GetPosition ( )=0;
	virtual void					SetPosition ( CVector & vecPosition )=0;
	virtual void					SetTimeToBurnOut ( DWORD dwTime )=0;
	virtual DWORD					GetTimeToBurnOut (  )=0;
	virtual CEntity					* GetCreator (  )=0;
	virtual CEntity					* GetEntityOnFire (  )=0;
	virtual void					SetTarget ( CEntity * entity )=0;
	virtual bool					IsIgnited (  )=0;
	virtual void					SetSilent ( bool bSilent )=0;
	virtual bool					IsBeingExtinguished ()=0;
	virtual void					Ignite( )=0;
	virtual	FLOAT					GetStrength (  )=0;
	virtual	void					SetStrength ( FLOAT fStrength )=0;
    virtual	void                    SetNumGenerationsAllowed ( char generations )=0;
    virtual CFireInterface*         GetInterface ( ) = 0;
};

#endif