/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        game_sa/CRegisteredCoronaSA.cpp
*  PURPOSE:     Registered corona entity
*  DEVELOPERS:  Ed Lyons <eai@opencoding.net>
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#include "StdInc.h"

CRegisteredCoronaSA::CRegisteredCoronaSA(CRegisteredCoronaSAInterface * coronaInterface)
{
	DEBUG_TRACE("CRegisteredCoronaSA::CRegisteredCoronaSA(CRegisteredCoronaSAInterface * coronaInterface)");
	internalInterface = coronaInterface;
}

CVector * CRegisteredCoronaSA::GetPosition()
{
	DEBUG_TRACE("CVector * CRegisteredCoronaSA::GetPosition()");
	return &internalInterface->Coordinates;
}

void CRegisteredCoronaSA::SetPosition(CVector * vector)
{
	DEBUG_TRACE("void CRegisteredCoronaSA::SetPosition(CVector * vector)");
	memcpy(&internalInterface->Coordinates, vector, sizeof(CVector));
}

FLOAT CRegisteredCoronaSA::GetSize()
{
	DEBUG_TRACE("FLOAT CRegisteredCoronaSA::GetSize()");
	return internalInterface->Size;
}

void CRegisteredCoronaSA::SetSize(FLOAT fSize)
{
	DEBUG_TRACE("void CRegisteredCoronaSA::SetSize(FLOAT fSize)");
	internalInterface->Size = fSize;
}

FLOAT CRegisteredCoronaSA::GetRange()
{
	DEBUG_TRACE("FLOAT CRegisteredCoronaSA::GetRange()");
	return internalInterface->Range;
}

void CRegisteredCoronaSA::SetRange(FLOAT fRange)
{
	DEBUG_TRACE("void CRegisteredCoronaSA::SetRange(FLOAT fRange)");
	internalInterface->Range = fRange;
}

FLOAT CRegisteredCoronaSA::GetPullTowardsCamera()
{
	DEBUG_TRACE("FLOAT CRegisteredCoronaSA::GetPullTowardsCamera()");
	return internalInterface->PullTowardsCam;
}

void CRegisteredCoronaSA::SetPullTowardsCamera(FLOAT fPullTowardsCamera)
{
	DEBUG_TRACE("void CRegisteredCoronaSA::SetPullTowardsCamera(FLOAT fPullTowardsCamera)");
	internalInterface->PullTowardsCam = fPullTowardsCamera;
}


void CRegisteredCoronaSA::SetColor(BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha)
{
	DEBUG_TRACE("void CRegisteredCoronaSA::SetColor(BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha)");
	internalInterface->Red = Red;
	internalInterface->Green = Green;
	internalInterface->Blue = Blue;
	internalInterface->Intensity = Alpha;
	internalInterface->FadedIntensity = Alpha;
}

void CRegisteredCoronaSA::SetTexture(RwTexture * texture)
{
	DEBUG_TRACE("void CRegisteredCoronaSA::SetTexture(RwTexture * texture)");
	internalInterface->pTex = texture;
}

void CRegisteredCoronaSA::SetTexture(eCoronaType texture)
{
	DEBUG_TRACE("void CRegisteredCoronaSA::SetTexture(eCoronaType texture)");
	CCoronasSA * coronas = ((CCoronasSA *)pGame->GetCoronas());
	RwTexture * tex =  coronas->GetTexture(texture);
	if(tex)
		internalInterface->pTex = tex;
}

/**
 * Lens flare
 * \todo Test what the types are
 */
BYTE CRegisteredCoronaSA::GetFlareType()
{
	DEBUG_TRACE("BYTE CRegisteredCoronaSA::GetFlareType()");
	return internalInterface->FlareType;
}

void CRegisteredCoronaSA::SetFlareType(BYTE fFlareType)
{
	DEBUG_TRACE("void CRegisteredCoronaSA::SetFlareType(BYTE fFlareType)");
	internalInterface->FlareType = fFlareType;
}

DWORD CRegisteredCoronaSA::GetID()
{
	DEBUG_TRACE("DWORD CRegisteredCoronaSA::GetID()");
	return ((DWORD)internalInterface - ARRAY_CORONAS) / sizeof(CRegisteredCoronaSAInterface);
}

void CRegisteredCoronaSA::Init(DWORD Identifier)
{
	DEBUG_TRACE("void CRegisteredCoronaSA::Init(DWORD Identifier)");
	internalInterface->Identifier = Identifier;
	this->SetSize(2.5f);
	internalInterface->NormalAngle = 0.0f;
	internalInterface->Range = 450.0f;
	internalInterface->PullTowardsCam = 1.5f;
	internalInterface->Red = 255;
	internalInterface->Green = 55;
	internalInterface->Blue = 255;
	internalInterface->Intensity = 255;
	internalInterface->FadedIntensity = 255;
	internalInterface->FlareType = 0;
	internalInterface->ReflectionType = 1;
	internalInterface->JustCreated = 1;
	internalInterface->RegisteredThisFrame = 1; // won't appear in-game without this
}

void CRegisteredCoronaSA::Refresh()
{
	DEBUG_TRACE("void CRegisteredCoronaSA::Refresh()");
	internalInterface->JustCreated = 1;
}

void CRegisteredCoronaSA::Disable()
{
	internalInterface->Identifier = 0;
   // internalInterface->Intensity = 0; // wasn't working before?
}