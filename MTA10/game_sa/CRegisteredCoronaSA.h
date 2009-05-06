/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        game_sa/CRegisteredCoronaSA.h
*  PURPOSE:     Header file for registered corona entity class
*  DEVELOPERS:  Ed Lyons <eai@opencoding.net>
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#ifndef __CGAMESA_REGISTEREDCORONA
#define __CGAMESA_REGISTEREDCORONA

#include <game/CRegisteredCorona.h>

class CEntitySAInterface;

#define ARRAY_CORONAS				0xC3E058 // ##SA##

class CRegisteredCoronaSAInterface // coronas are 104 bytes long, and theres 56 of them
{
public:
	CVectorGTA	Coordinates;			// Where is it exactly.
	DWORD		Identifier;				// Should be unique for each corona. Address or something (0 = empty)
	RwTexture	* pTex;					// Pointer to the actual texture to be rendered
	float		Size;					// How big is this fellow
	float 		NormalAngle;			// Is corona normal (if relevant) facing the camera?
	float		Range;					// How far away is this guy still visible
	float		PullTowardsCam;			// How far away is the z value pulled towards camera.
	float		HeightAboveGround;		// Stired so that we don't have to do a ProcessVerticalLine every frame
										// The following fields are used for trails behind coronas (glowy lights)
	float		FadeSpeed;				// The speed the corona fades in and out ##SA##
	BYTE		Red, Green, Blue;		// Rendering colour.
	BYTE		Intensity;				// 255 = full
	BYTE		FadedIntensity;			// Intensity that lags behind the given intenisty and fades out if the LOS is blocked
	BYTE		RegisteredThisFrame;	// Has this guy been registered by game code this frame
	BYTE		FlareType;				// What type of flare to render
	BYTE		ReflectionType;			// What type of reflection during wet weather
	BYTE		LOSCheck : 1;			// Do we check the LOS or do we render at the right Z value
	BYTE		OffScreen : 1;			// Set by the rendering code to be used by the update code
	BYTE		JustCreated;			// If this guy has been created this frame we won't delete it (It hasn't had the time to get its OffScreen cleared) ##SA removed from packed byte ##
	BYTE		NeonFade : 1;			// Does the guy fade out when closer to cam
	BYTE		OnlyFromBelow : 1;		// This corona is only visible if the camera is below it. ##SA##
	BYTE		bHasValidHeightAboveGround : 1;
	BYTE		WhiteCore : 1;			// This corona rendered with a small white core.
	BYTE		bIsAttachedToEntity : 1;
	
	CEntitySAInterface	* pEntityAttachedTo;
};

/**
 * \todo Add support for setting WhiteCore, OnlyFromBelow and AttachedToEntity
 */
class CRegisteredCoronaSA : public CRegisteredCorona
{
	CRegisteredCoronaSAInterface*	internalInterface;
public:
					CRegisteredCoronaSA( CRegisteredCoronaSAInterface* coronaInterface );

	const CVector	GetPosition( void );
	void			SetPosition( const CVector& vector );

	float			GetSize( void );
	void			SetSize( float fSize );

	float			GetRange( void );
	void			SetRange( float fRange );

	float			GetPullTowardsCamera( void );
	void			SetPullTowardsCamera( float fPullTowardsCamera );

	void			SetColor( BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha );
	void			SetTexture( RwTexture * texture );
	void			SetTexture( eCoronaType texture );

	BYTE			GetFlareType( void );
	void			SetFlareType( BYTE fFlareType );

    inline DWORD	GetIdentifier( void ) { return internalInterface->Identifier; }
	DWORD			GetID( void );

	void			Init( DWORD Identifier );
	void			Disable( void );
	void			Refresh( void );
};

#endif