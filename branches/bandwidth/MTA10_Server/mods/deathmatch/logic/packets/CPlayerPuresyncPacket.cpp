/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        mods/deathmatch/logic/packets/CPlayerPuresyncPacket.cpp
*  PURPOSE:     Player pure synchronization packet class
*  DEVELOPERS:  Christian Myhre Lundheim <>
*               Jax <>
*               Cecill Etheredge <>
*               Alberto Alonso <rydencillo@gmail.com>
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#include "StdInc.h"

extern CGame* g_pGame;

bool CPlayerPuresyncPacket::Read ( NetServerBitStreamInterface& BitStream )
{
    if ( m_pSourceElement )
    {
        CPlayer * pSourcePlayer = static_cast < CPlayer * > ( m_pSourceElement );

        // Read out the time context
        unsigned char ucTimeContext = 0;
        BitStream.Read ( ucTimeContext );

        // Only read this packet if it matches the current time context that
        // player is in.
        if ( !pSourcePlayer->CanUpdateSync ( ucTimeContext ) )
        {
            return false;
        }

        // Read out keys
        CControllerState ControllerState;
        ReadFullKeysync ( ControllerState, BitStream );
        pSourcePlayer->GetPad ()->NewControllerState ( ControllerState );

        // Read the flags
        SPlayerPuresyncFlags flags;
        BitStream.ReadBits ( reinterpret_cast < char * > ( &flags ), SPlayerPuresyncFlags::BITCOUNT );

        pSourcePlayer->SetInWater ( flags.bIsInWater );
        pSourcePlayer->SetOnGround ( flags.bIsOnGround );
        pSourcePlayer->SetHasJetPack ( flags.bHasJetPack );
        pSourcePlayer->SetDucked ( flags.bIsDucked );
        pSourcePlayer->SetWearingGoggles ( flags.bWearsGoogles );
        pSourcePlayer->SetChoking ( flags.bIsChoking );
        pSourcePlayer->SetAkimboArmUp ( flags.bAkimboTargetUp );
        pSourcePlayer->SetOnFire ( flags.bIsOnFire );

        // Contact element
        CElement* pContactElement = NULL;
        if ( flags.bHasContact )
        {            
            ElementID Temp;
            BitStream.Read ( Temp );
            pContactElement = CElementIDs::GetElement ( Temp );
        }        
        CElement * pPreviousContactElement = pSourcePlayer->GetContactElement ();        
        pSourcePlayer->SetContactElement ( pContactElement );

        if ( pPreviousContactElement != pContactElement )
        {
            // Call our onPlayerContact event
            CLuaArguments Arguments;
            if ( pPreviousContactElement )
                Arguments.PushElement ( pPreviousContactElement );
            else
                Arguments.PushNil ();
            if ( pContactElement )
                Arguments.PushElement ( pContactElement );
            else
                Arguments.PushNil ();

            pSourcePlayer->CallEvent ( "onPlayerContact", Arguments );
        }

        // Player position
        CVector vecTemp;
        BitStream.Read ( vecTemp.fX );
        BitStream.Read ( vecTemp.fY );
        BitStream.Read ( vecTemp.fZ );
        if ( pContactElement )
        {
            pSourcePlayer->SetContactPosition ( vecTemp );
            
            // Get the true position
            CVector vecTempPos = pContactElement->GetPosition ();
            vecTemp += vecTempPos;
        }
        pSourcePlayer->SetPosition ( vecTemp );

        // Player rotation
        float fRotation = 0.0f;
        BitStream.Read ( fRotation );
        pSourcePlayer->SetRotation ( fRotation );

        // Move speed vector
        BitStream.Read ( vecTemp.fX );
        BitStream.Read ( vecTemp.fY );
        BitStream.Read ( vecTemp.fZ );
        pSourcePlayer->SetVelocity ( vecTemp );

        // Health ( stored with damage )
        unsigned char ucHealth;
	    BitStream.Read ( ucHealth );

        // Armor
        unsigned char ucArmor;
        BitStream.Read ( ucArmor );

        float fArmor = static_cast < float > ( ucArmor ) / 1.25f;
        float fOldArmor = pSourcePlayer->GetArmor ();
		float fArmorLoss = fOldArmor - fArmor;
        
        pSourcePlayer->SetArmor ( fArmor );

        // Read out and set the camera rotation
        float fCameraRotation;
        BitStream.Read ( fCameraRotation );
        pSourcePlayer->SetCameraRotation ( fCameraRotation );        

        if ( flags.bHasAWeapon )
        {
            // Current weapon slot
            SWeaponSlotSync slot;
            BitStream.Read ( reinterpret_cast < char* > ( &slot ), SWeaponSlotSync::BITCOUNT );

            pSourcePlayer->SetWeaponSlot ( slot.uiSlot );
            if ( slot.uiSlot != 0 && slot.uiSlot != 1 && slot.uiSlot != 10 && slot.uiSlot != 11 )
            {
                // Read out the ammo states
                unsigned short usAmmoInClip;
                BitStream.Read ( usAmmoInClip );
                pSourcePlayer->SetWeaponAmmoInClip ( usAmmoInClip );
            
                unsigned short usTotalAmmo;
                BitStream.Read ( usTotalAmmo );
                pSourcePlayer->SetWeaponTotalAmmo ( usTotalAmmo );

                // Read out the aim directions
                float fArmX, fArmY;
			    BitStream.Read ( fArmX );
			    BitStream.Read ( fArmY );

			    // Set the arm directions and whether or not arms are up
			    pSourcePlayer->SetAimDirections ( fArmX, fArmY );

                // Source vector
                BitStream.Read ( vecTemp.fX );
                BitStream.Read ( vecTemp.fY );
                BitStream.Read ( vecTemp.fZ );
                pSourcePlayer->SetSniperSourceVector ( vecTemp );

                // Target vector
                BitStream.Read ( vecTemp.fX );
                BitStream.Read ( vecTemp.fY );
                BitStream.Read ( vecTemp.fZ );
                pSourcePlayer->SetTargettingVector ( vecTemp );
            }
        }
        else
        {
            pSourcePlayer->SetWeaponSlot ( 0 );
        }

        // Read out damage info if changed
        ElementID DamagerID;
        BitStream.Read ( DamagerID );
        
        // If it's different, carry on reading
        if ( DamagerID != RESERVED_ELEMENT_ID )
        {            
            CElement* pElement = CElementIDs::GetElement ( DamagerID );

            unsigned char ucWeapon, ucBodyPart;
            BitStream.Read ( ucWeapon );
            BitStream.Read ( ucBodyPart );

            pSourcePlayer->SetDamageInfo ( pElement, ucWeapon, ucBodyPart );
        }

        float fHealth = static_cast < float > ( ucHealth ) / 1.25f;

        // If we know the player's dead, make sure the health we send on is 0
        if ( pSourcePlayer->IsDead () )
            fHealth = 0.0f;

        float fOldHealth = pSourcePlayer->GetHealth ();
		float fHealthLoss = fOldHealth - fHealth;
        pSourcePlayer->SetHealth ( fHealth );

        // Less than last packet's frame?
        if ( fHealthLoss > 0 || fArmorLoss > 0 )
        {            
            float fDamage = 0.0f;
            if ( fHealthLoss > 0 ) fDamage += fHealthLoss;
            if ( fArmorLoss > 0 ) fDamage += fArmorLoss;

            // Call the onPlayerDamage event
            CLuaArguments Arguments;
            CElement* pKillerElement = pSourcePlayer->GetPlayerAttacker ();
            if ( pKillerElement ) Arguments.PushElement ( pKillerElement );
            else Arguments.PushNil ();
            Arguments.PushNumber ( pSourcePlayer->GetAttackWeapon () );
            Arguments.PushNumber ( pSourcePlayer->GetAttackBodyPart () );
            Arguments.PushNumber ( fDamage );

            pSourcePlayer->CallEvent ( "onPlayerDamage", Arguments );
        }        
        
        // Success
        return true;
    }

    return false;
}

/*
template < class T >
bool CompareAndSet ( const T& current, T& previous )
{
    bool bChanged = current != previous;
    previous = current;
    return bChanged;
}
*/

bool CPlayerPuresyncPacket::Write ( NetServerBitStreamInterface& BitStream ) const
{
    if ( m_pSourceElement )
    {
        CPlayer * pSourcePlayer = static_cast < CPlayer * > ( m_pSourceElement );

        ElementID PlayerID = pSourcePlayer->GetID ();
        unsigned short usLatency = pSourcePlayer->GetPing ();
        const CControllerState& ControllerState = pSourcePlayer->GetPad ()->GetCurrentControllerState ();
        CElement* pContactElement = pSourcePlayer->GetContactElement ();

        // Get current weapon slot
        unsigned char ucWeaponSlot = pSourcePlayer->GetWeaponSlot ();

        // Flags
        SPlayerPuresyncFlags flags;
        flags.bIsInWater = ( pSourcePlayer->IsInWater () == true );
        flags.bIsOnGround = ( pSourcePlayer->IsOnGround () == true );
        flags.bHasJetPack = ( pSourcePlayer->HasJetPack () == true );
        flags.bIsDucked = ( pSourcePlayer->IsDucked () == true );
        flags.bWearsGoogles = ( pSourcePlayer->IsWearingGoggles () == true );
        flags.bHasContact = ( pContactElement != NULL );
        flags.bIsChoking = ( pSourcePlayer->IsChoking () == true );
        flags.bAkimboTargetUp = ( pSourcePlayer->IsAkimboArmUp () == true );
        flags.bIsOnFire = ( pSourcePlayer->IsOnFire () == true );
        flags.bHasAWeapon = ( ucWeaponSlot != 0 );

        CVector vecPosition = pSourcePlayer->GetPosition ();
        if ( pContactElement )
            pSourcePlayer->GetContactPosition ( vecPosition );
        float fRotation = pSourcePlayer->GetRotation ();
        CVector vecVelocity;
        pSourcePlayer->GetVelocity ( vecVelocity );
        unsigned char ucHealth = static_cast < unsigned char > ( pSourcePlayer->GetHealth () * 1.25f );
        unsigned char ucArmor = static_cast < unsigned char > ( pSourcePlayer->GetArmor () * 1.25f );
        float fCameraRotation = pSourcePlayer->GetCameraRotation ();

        BitStream.Write ( PlayerID );

        // Write the time context
        BitStream.Write ( pSourcePlayer->GetSyncTimeContext () );

        BitStream.Write ( usLatency );
        WriteFullKeysync ( ControllerState, BitStream );   
/*
        // Figure out what to send
        SPlayerPuresyncSentHeader sent;
        sent.bFlags             = CompareAndSet ( usFlags,          pSourcePlayer->lastSent.usFlags );
        sent.bPosition          = CompareAndSet ( vecPosition,      pSourcePlayer->lastSent.vecPosition );
        sent.bRotation          = CompareAndSet ( fRotation,        pSourcePlayer->lastSent.fRotation );
        sent.bVelocity          = CompareAndSet ( vecVelocity,      pSourcePlayer->lastSent.vecVelocity );
        sent.bHealth            = CompareAndSet ( ucHealth,         pSourcePlayer->lastSent.ucHealth );
        sent.bArmor             = CompareAndSet ( ucArmor,          pSourcePlayer->lastSent.ucArmor );
        sent.bCameraRotation    = CompareAndSet ( fCameraRotation,  pSourcePlayer->lastSent.fCameraRotation );
        sent.bWeaponType        = CompareAndSet ( ucWeaponType,     pSourcePlayer->lastSent.ucWeaponType );
        sent.Write ( BitStream );

        if ( sent.bPosition )
        {
            BitStream.Write ( vecPosition.fX );
            BitStream.Write ( vecPosition.fY );
            BitStream.Write ( vecPosition.fZ ); 
        }

        if ( sent.bRotation )
            BitStream.Write ( fRotation );    

        etc... Could also do a 'sent' header in WriteFullKeysync
*/
        BitStream.WriteBits ( reinterpret_cast < const char* > ( &flags ), SPlayerPuresyncFlags::BITCOUNT );
        
        if ( pContactElement )
            BitStream.Write ( pContactElement->GetID () );            

        BitStream.Write ( vecPosition.fX );
        BitStream.Write ( vecPosition.fY );
        BitStream.Write ( vecPosition.fZ ); 

        BitStream.Write ( fRotation );       
        BitStream.Write ( vecVelocity.fX );
        BitStream.Write ( vecVelocity.fY );
        BitStream.Write ( vecVelocity.fZ );            
	    BitStream.Write ( ucHealth );
        BitStream.Write ( ucArmor );
        BitStream.Write ( fCameraRotation );

        if ( flags.bHasAWeapon )
        {
            SWeaponSlotSync slot;
            slot.uiSlot = ucWeaponSlot;
            BitStream.WriteBits ( reinterpret_cast < const char* > ( &slot ), SWeaponSlotSync::BITCOUNT );
            if ( slot.uiSlot != 0 && slot.uiSlot != 1 && slot.uiSlot != 10 && slot.uiSlot != 11 )
            {
                unsigned short usWeaponAmmoInClip = pSourcePlayer->GetWeaponAmmoInClip ();
                float fAimDirectionX = pSourcePlayer->GetAimDirectionX ();
                float fAimDirectionY = pSourcePlayer->GetAimDirectionY ();
			    bool bArmUp = pSourcePlayer->IsAkimboArmUp ();
                CVector vecSniperSource = pSourcePlayer->GetSniperSourceVector ();
                CVector vecTargetting;
                pSourcePlayer->GetTargettingVector ( vecTargetting );
/*
            // Figure out what to send
            SPlayerPuresyncWeaponSentHeader sent;
            sent.bWeaponAmmoInClip      = CompareAndSet ( usWeaponAmmoInClip,   pSourcePlayer->lastSent.usWeaponAmmoInClip );
            sent.bAimDirectionX         = CompareAndSet ( fAimDirectionX,       pSourcePlayer->lastSent.fAimDirectionX );
            sent.bAimDirectionY         = CompareAndSet ( fAimDirectionY,       pSourcePlayer->lastSent.fAimDirectionY );
            sent.bSniperSource          = CompareAndSet ( vecSniperSource,      pSourcePlayer->lastSent.vecSniperSource );
            sent.bTargetting            = CompareAndSet ( vecTargetting,        pSourcePlayer->lastSent.vecTargetting );
            sent.Write ( BitStream );

            if ( sent.bWeaponAmmoInClip )
                BitStream.Write ( usWeaponAmmoInClip );

            if ( sent.bAimDirectionX )
			    BitStream.Write ( fAimDirectionX );
            if ( sent.bAimDirectionY )
			    BitStream.Write ( fAimDirectionY );

            etc...
*/
                BitStream.Write ( usWeaponAmmoInClip );

			    BitStream.Write ( fAimDirectionX );
			    BitStream.Write ( fAimDirectionY );

                BitStream.Write ( vecSniperSource.fX );
                BitStream.Write ( vecSniperSource.fY );
                BitStream.Write ( vecSniperSource.fZ );
                BitStream.Write ( vecTargetting.fX );
                BitStream.Write ( vecTargetting.fY );
                BitStream.Write ( vecTargetting.fZ );
            }
        }

        // Success
        return true;
    }

    return false;
}
