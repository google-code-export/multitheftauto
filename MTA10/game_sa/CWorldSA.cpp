/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        game_sa/CWorldSA.cpp
*  PURPOSE:     Game world/entity logic
*  DEVELOPERS:  Ed Lyons <eai@opencoding.net>
*               Cecill Etheredge <ijsf@gmx.net>
*               Christian Myhre Lundheim <>
*               Jax <>
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#include "StdInc.h"

void CWorldSA::Add( CEntity * pEntity )
{
    CEntitySA* pEntitySA = dynamic_cast < CEntitySA* > ( pEntity );

	if ( pEntitySA )
	{
		DWORD dwEntity = (DWORD) pEntitySA->GetInterface();
		DWORD dwFunction = FUNC_Add;
		_asm
		{
			push	dwEntity
			call	dwFunction
			add		esp, 4
		}
	}
}


void CWorldSA::Add( CEntitySAInterface * entityInterface )
{
	DWORD dwFunction = FUNC_Add;
	_asm
	{
		push	entityInterface
		call	dwFunction
		add		esp, 4
	}
}

void CWorldSA::Remove( CEntity* pEntity )
{
    CEntitySA* pEntitySA = dynamic_cast < CEntitySA* > ( pEntity );

	if ( pEntitySA )
	{
		DWORD dwEntity = (DWORD)pEntitySA->GetInterface();
		DWORD dwFunction = FUNC_Remove;
		_asm
		{
			push	dwEntity
			call	dwFunction
			add		esp, 4
		}
	}
}

void CWorldSA::Remove( CEntitySAInterface* entityInterface )
{
	DWORD dwFunction = FUNC_Remove;
	_asm
	{
		push	entityInterface
		call	dwFunction
		add		esp, 4

	/*	mov		ecx, entityInterface
		mov		esi, [ecx]
		push	1
		call	dword ptr [esi+8]*/				
	}
}

void CWorldSA::RemoveReferencesToDeletedObject( CEntitySAInterface* entity )
{
    DWORD dwFunc = FUNC_RemoveReferencesToDeletedObject;
    DWORD dwEntity = (DWORD)entity;
    _asm
    {
        push    dwEntity
        call    dwFunc
        add     esp, 4
    }
}

bool CWorldSA::TestLineSphere( const CVector& vecStart, const CVector& vecEnd, const CVector& vecSphereCenter, float fSphereRadius, CColPoint ** colCollision )
{
    // TODO: THIS FUNCTION IS INCOMPLETE AND SHOULD NOT BE USED
	CVectorGTA vecE = vecEnd;
	CVectorGTA vecS = vecStart;
	CVectorGTA vecC = vecSphereCenter;

    // Create a CColLine for us
    DWORD dwFunc = FUNC_CColLine_Constructor;
    DWORD dwCColLine[10]; // I don't know how big CColLine is, so we'll just be safe
    _asm
    {
        lea     ecx, dwCColLine
        push    vecE
        push    vecS
        call    dwFunc
    }

    // Now, lets make a CColSphere
    BYTE byteColSphere[18]; // looks like its 18 bytes { vecPos, fSize, byteUnk, byteUnk, byteUnk }
    dwFunc = FUNC_CColSphere_Set;
    _asm
    {
        lea     ecx, byteColSphere
        push    255
        push    0
        push    0
        push    vecC
        push    fSphereRadius
        call    dwFunc
    }
}

bool CWorldSA::ProcessLineOfSight( const CVector& vecStart, const CVector& vecEnd, CColPoint ** colCollision, 
								   CEntity ** CollisionEntity, bool bCheckBuildings, 
								   bool bCheckVehicles, bool bCheckPeds, 
								   bool bCheckObjects, bool bCheckDummies , 
								   bool bSeeThroughStuff, bool bIgnoreSomeObjectsForCamera, 
								   bool bShootThroughStuff )
{
  	DWORD dwPadding[100]; // stops the function missbehaving and overwriting the return address
    dwPadding [0] = 0;  // prevent the warning and eventual compiler optimizations from removing it

	CColPointSA * pColPointSA = new CColPointSA();
	CColPointSAInterface * pColPointSAInterface = pColPointSA->GetInterface();	

	//DWORD targetEntity;
    CEntitySAInterface * targetEntity = NULL;
	bool bReturn = false;

	DWORD dwFunc = FUNC_ProcessLineOfSight;
	// bool bCheckBuildings = true,					bool bCheckVehicles = true,		bool bCheckPeds = true, 
	// bool bCheckObjects = true,					bool bCheckDummies = true,		bool bSeeThroughStuff = false, 
	// bool bIgnoreSomeObjectsForCamera = false,	bool bShootThroughStuff = false

	CVectorGTA vecS = vecStart;
	CVectorGTA vecE = vecEnd;

	_asm
	{
		push	bShootThroughStuff
		push	bIgnoreSomeObjectsForCamera
		push	bSeeThroughStuff
		push	bCheckDummies
		push	bCheckObjects
		push	bCheckPeds
		push	bCheckVehicles
		push	bCheckBuildings
		lea		eax, targetEntity
		push	eax
		push	pColPointSAInterface	
		push	vecE
		push	vecS
		call	dwFunc
		mov		bReturn, al
		add		esp, 0x30
	}

    if ( CollisionEntity )
    {
	    CPoolsSA * pPools = ((CPoolsSA *)pGame->GetPools());
	    if(pPools)
	    {
		    if(targetEntity)
		    {
                switch (targetEntity->nType)
                {
                    case ENTITY_TYPE_PED:
                        *CollisionEntity = pPools->GetPed((DWORD *)targetEntity);
                        break;
                    case ENTITY_TYPE_OBJECT:
                        *CollisionEntity = pPools->GetObject((DWORD *)targetEntity);
                        break;
                    case ENTITY_TYPE_VEHICLE:
                        *CollisionEntity = pPools->GetVehicle((DWORD *)targetEntity);
				        break;
                }

                /*CEntitySA * entity = new CEntitySA();
			    entity->SetInterface((CEntitySAInterface *)targetEntity);
			    eEntityType EntityType = entity->GetEntityType();
			    delete entity;
			    switch(EntityType)
			    {
			    case ENTITY_TYPE_PED:
			    case ENTITY_TYPE_OBJECT:
				    *CollisionEntity = pPools->GetPed((DWORD *)targetEntity);
				    if ( *CollisionEntity )
					    break;
				    *CollisionEntity = pPools->GetObject((CObjectSAInterface *)targetEntity);
				    break;
			    case ENTITY_TYPE_VEHICLE:
				    *CollisionEntity = pPools->GetVehicle((CVehicleSAInterface *)targetEntity);
				    break;

			    }*/
		    }
	    }
    }
    if ( colCollision ) *colCollision = pColPointSA;
    else delete pColPointSA;

	return bReturn;
}


void CWorldSA::IgnoreEntity( CEntity* pEntity )
{
    CEntitySA* pEntitySA = dynamic_cast < CEntitySA* > ( pEntity );

	if ( pEntitySA )
		*(DWORD *)VAR_IgnoredEntity = (DWORD) pEntitySA->GetInterface ();
	else
		*(DWORD *)VAR_IgnoredEntity = 0;
}

// Technically this is in CTheZones
BYTE CWorldSA::GetLevelFromPosition( const CVector& vecPosition )
{
	CVectorGTA vec = vecPosition;
	DWORD dwFunc = FUNC_GetLevelFromPosition;
	BYTE bReturn = 0;
	_asm
	{
		push	vec
		call	dwFunc
		mov		bReturn, al
		pop		eax
	}
	return bReturn;
}

float CWorldSA::FindGroundZForPosition( float fX, float fY )
{
	DWORD dwFunc = FUNC_FindGroundZFor3DCoord;
	FLOAT fReturn = 0;
	_asm
	{
		push	fY
		push	fX
		call	dwFunc
		fstp	fReturn
		add		esp, 8
	}
	return fReturn;
}

float CWorldSA::FindGroundZFor3DPosition( const CVector& vecPosition )
{
	DWORD dwFunc = FUNC_FindGroundZFor3DCoord;
	FLOAT fReturn = 0;
	FLOAT fX = vecPosition.getX();
	FLOAT fY = vecPosition.getY();
	FLOAT fZ = vecPosition.getZ();
	_asm
	{
		push	0
		push	0
		push	fZ
		push	fY
		push	fX
		call	dwFunc
		fstp	fReturn
		add		esp, 0x14
	}
	return fReturn;
}

void CWorldSA::LoadMapAroundPoint( const CVector& vecPosition, FLOAT fRadius )
{
	CVectorGTA vec = vecPosition;

	DWORD dwFunc = FUNC_CTimer_Stop;
	_asm
	{
		call	dwFunc
	}

	dwFunc = FUNC_CRenderer_RequestObjectsInDirection;
	_asm
	{
		push	32
		push	fRadius
		push	vec
		call	dwFunc
		add		esp, 12
	}


	dwFunc = FUNC_CStreaming_LoadScene;
	_asm
	{
		push	vec
		call	dwFunc
		add		esp, 4
	}

	dwFunc = FUNC_CTimer_Update;
	_asm
	{
		call	dwFunc
	}

}

bool CWorldSA::IsLineOfSightClear( const CVector& vecStart, const CVector& vecEnd, bool bCheckBuildings,
								   bool bCheckVehicles, bool bCheckPeds, bool bCheckObjects,
								   bool bCheckDummies, bool bSeeThroughStuff, bool bIgnoreSomeObjectsForCamera )
{
	CVectorGTA vecS = vecStart;
	CVectorGTA vecE = vecEnd;
	DWORD dwFunc = FUNC_IsLineOfSightClear;
	bool bReturn = false;
	// bool bCheckBuildings = true, bool bCheckVehicles = true, bool bCheckPeds = true, 
	// bool bCheckObjects = true, bool bCheckDummies = true, bool bSeeThroughStuff = false, 
	// bool bIgnoreSomeObjectsForCamera = false

	_asm
	{
		push	bIgnoreSomeObjectsForCamera
		push	bSeeThroughStuff
		push	bCheckDummies
		push	bCheckObjects
		push	bCheckPeds
		push	bCheckVehicles
		push	bCheckBuildings
		push	vecE
		push	vecS
		call	dwFunc
		mov		bReturn, al
		add		esp, 0x24
	}
	return bReturn;
}

bool CWorldSA::HasCollisionBeenLoaded( const CVector& vecPosition )
{
	CVectorGTA vec = vecPosition;
    DWORD dwFunc = FUNC_HasCollisionBeenLoaded;
    bool bRet = false;
    _asm
    {
        push    0
        push    vec
        call    dwFunc
        mov     bRet, al
        add     esp, 8
    } 
    return bRet;
}

DWORD CWorldSA::GetCurrentArea( void )
{
    return *(DWORD *)VAR_currArea;
}

void CWorldSA::SetCurrentArea( DWORD dwArea )
{
    *(DWORD *)VAR_currArea = dwArea;

    DWORD dwFunc = FUNC_RemoveBuildingsNotInArea;
    _asm
    {
        push    dwArea
        call    dwFunc
        add     esp, 4
    }
}
