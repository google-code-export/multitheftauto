/*****************************************************************************
*
*  PROJECT:		Multi Theft Auto v1.0
*  LICENSE:		See LICENSE in the top level directory
*  FILE:		sdk/game/CPickup.h
*  PURPOSE:		Pickup entity interface
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#ifndef __CGAME_PICKCUP
#define __CGAME_PICKCUP

#include <windows.h>
#include <CVector.h>
#include "Common.h"

class CObject;

class CPickup
{
public:
    virtual CObject *           GetObject()=0;
	virtual void				SetPosition(CVector * vecPosition)=0;
	virtual CVector	*		    GetPosition(CVector * vecPosition)=0;
	
	virtual ePickupType			GetType()=0;
	virtual void				SetType(ePickupType type)=0;
	virtual FLOAT				GetCurrentValue()=0;
	virtual void				SetCurrentValue(FLOAT fCurrentValue)=0;
	virtual void				SetRegenerationTime(DWORD dwTime)=0;
	virtual void				SetMoneyPerDay(WORD wMoneyPerDay)=0;
	virtual WORD				GetMoneyPerDay()=0;
	virtual WORD				GetModel()=0;
	virtual void				SetModel(WORD wModelIndex)=0; // do not export
	virtual ePickupState		GetState()=0;
	virtual void				SetState(ePickupState bState)=0;
	virtual BYTE				GetAmmo()=0;
	virtual void				SetAmmo(BYTE bAmmo)=0;
	virtual long				GetMonetaryValue()=0;
	virtual void				SetMonetaryValue(long lMonetaryValue)=0;
	virtual BYTE				IsNearby()=0;
    virtual void                Remove()=0;
    virtual void				GiveUsAPickUpObject(int ForcedObjectIndex=-1)=0;
    virtual void				GetRidOfObjects()=0;

};

// not used
typedef bool (*CBPickup) (CPickup *);

#endif