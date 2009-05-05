/*****************************************************************************
*
*  PROJECT:		Multi Theft Auto v1.0
*  LICENSE:		See LICENSE in the top level directory
*  FILE:		game_sa/CMarkerSA.h
*  PURPOSE:		Header file for marker entity class
*  DEVELOPERS:	Ed Lyons <eai@opencoding.net>
*               Christian Myhre Lundheim <>
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#ifndef __CGAMESA_MARKER
#define __CGAMESA_MARKER

#include <game/CMarker.h>
#include "Common.h"

/**
 * \todo Ignore this class?
 */
class CEntryExit;

class CMarkerSAInterface
{
public:
	/* GTA Variables */
	DWORD nColour;
	// The following parameter is used when Blip Type is CAR, CHAR or OBJECT
	long PoolIndex;	
	// The following parameter is used when Blip Type is COORDS	
	CVector position;//vec2DBlip;

	WORD ReferenceIndex;

	FLOAT fBlipMarkerScale;
	WORD nBlipScale;
	CEntryExit* pEntryExit;

	BYTE nBlipSprite;

	BYTE bBright				: 1;
	BYTE bTrackingBlip			: 1;
	BYTE bShortRange			: 1;
	BYTE bFriendly				: 1;	
	BYTE bBlipRemain			: 1;
	BYTE bBlipFade				: 1;
	BYTE nCoordBlipAppearance	: 2;

	BYTE nBlipDisplayFlag		: 2;
	BYTE BlipType				: 4;
};

class CMarkerSA : public CMarker
{
public:
	/* Our Functions */
						CMarkerSA       ( CMarkerSAInterface * markerInterface ) { internalInterface = markerInterface; };

    void                Init            ( void );

	void				SetSprite ( eMarkerSprite Sprite );
	void				SetDisplay ( eMarkerDisplay wDisplay );
	void				SetScale ( WORD wScale );
	void				SetColor ( eMarkerColor color );
	void				SetColor ( RGBA color );
	void				Remove ( );
	bool				IsActive (  );
	void				SetPosition ( CVector * vecPosition );
	void				SetEntity ( CVehicle * vehicle );
	void				SetEntity ( CPed * ped );
	void				SetEntity ( CObject * object );
	CEntity				* GetEntity (  );
	CVector			* GetPosition (  );
	CMarkerSAInterface	* GetInterface (  ) { return internalInterface; };
private:
	CMarkerSAInterface	* internalInterface;
};

#endif