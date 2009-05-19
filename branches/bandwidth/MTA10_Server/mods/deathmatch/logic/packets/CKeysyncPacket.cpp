/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        mods/deathmatch/logic/packets/CKeysyncPacket.cpp
*  PURPOSE:     Key controls synchronization packet class
*  DEVELOPERS:  Christian Myhre Lundheim <>
*               Chris McArthur <>
*               Jax <>
*               Cecill Etheredge <>
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#include "StdInc.h"

CKeysyncPacket::CKeysyncPacket ( CPlayer * pPlayer )
{
    m_pSourceElement = pPlayer;
}

bool CKeysyncPacket::Read ( NetBitStreamInterface& BitStream )
{
    // Got a player to write?
    if ( m_pSourceElement )
    {
        CPlayer * pSourcePlayer = static_cast < CPlayer * > ( m_pSourceElement );

        // Read out the controller states
        CControllerState ControllerState;
        CControllerState LastControllerState = pSourcePlayer->GetPad ()->GetLastControllerState ();
        ReadSmallKeysync ( ControllerState, LastControllerState, BitStream );        

        // Flags
        SKeysyncFlags flags;
        BitStream.Read ( &flags );

        // Set the ducked and choking state
        pSourcePlayer->SetDucked ( flags.data.bIsDucked );
        pSourcePlayer->SetChoking ( flags.data.bIsChoking );

        // If he's shooting
        if ( ControllerState.ButtonCircle )
        {
            bool bHasWeapon = BitStream.ReadBit ();

            if ( bHasWeapon )
            {
                // Read out the current weapon slot and set it
                SWeaponSlotSync slot;
                BitStream.Read ( &slot );
                unsigned int uiSlot = slot.data.uiSlot;

                pSourcePlayer->SetWeaponSlot ( uiSlot );

                // Did he have a weapon?
                if ( CWeaponNames::DoesSlotHaveAmmo ( uiSlot ) )
                {
                    // And ammo in clip
                    SWeaponAmmoSync ammo ( pSourcePlayer->GetWeaponType (), false, true );
                    BitStream.Read ( &ammo );
                    pSourcePlayer->SetWeaponAmmoInClip ( ammo.data.usAmmoInClip );

				    // Read out the aim directions
				    float fArmX, fArmY;
				    BitStream.Read ( fArmX );
				    BitStream.Read ( fArmY );

				    // Set the arm directions and whether or not arms are up
				    pSourcePlayer->SetAimDirections ( fArmX, fArmY );
                    pSourcePlayer->SetAkimboArmUp ( flags.data.bAkimboTargetUp );

                    // Source vector
                    CVector vecTemp;
                    BitStream.Read ( vecTemp.fX );
                    BitStream.Read ( vecTemp.fY );
                    BitStream.Read ( vecTemp.fZ );
                    pSourcePlayer->SetSniperSourceVector ( vecTemp );

                    // Read out the weapon target vector
                    CVector vecTarget;
                    BitStream.Read ( vecTarget.fX );
                    BitStream.Read ( vecTarget.fY );
                    BitStream.Read ( vecTarget.fZ );
                    pSourcePlayer->SetTargettingVector ( vecTarget );

                    // Read out the driveby direction
                    unsigned char ucDriveByDirection;
                    BitStream.Read ( ucDriveByDirection );
                    pSourcePlayer->SetDriveByDirection ( ucDriveByDirection );
                }
            }
            else
            {
                pSourcePlayer->SetWeaponSlot ( 0 );
            }
        }

        // If he's in a vehicle, read out the small vehicle specific data
        CVehicle* pVehicle = pSourcePlayer->GetOccupiedVehicle ();
        if ( pVehicle && pSourcePlayer->GetOccupiedVehicleSeat () == 0 )
        {
            ReadVehicleSpecific ( pVehicle, BitStream );

            if ( pVehicle->GetUpgrades ()->HasUpgrade ( 1087 ) ) // Hydraulics?
            {
                short sRightStickX, sRightStickY;
                BitStream.Read ( sRightStickX );
                BitStream.Read ( sRightStickY );

                ControllerState.RightStickX = sRightStickX;
                ControllerState.RightStickY = sRightStickY;
            }
        }

        // Set the controller states
        pSourcePlayer->GetPad ()->NewControllerState ( ControllerState );

        return true;
    }
    
    return false;
}


bool CKeysyncPacket::Write ( NetBitStreamInterface& BitStream ) const
{
    // Got a player to write?
    if ( m_pSourceElement )
    {
        CPlayer * pSourcePlayer = static_cast < CPlayer * > ( m_pSourceElement );

        // Write the source player id
        ElementID PlayerID = pSourcePlayer->GetID ();
        BitStream.Write ( PlayerID );

        // Write his ping divided with 2 plus a small number so the client can find out when this packet was sent
        unsigned short usLatency = pSourcePlayer->GetPing ();
        BitStream.Write ( usLatency );

        // Write the keysync data
        const CControllerState& ControllerState = pSourcePlayer->GetPad ()->GetCurrentControllerState ();
        const CControllerState& LastControllerState = pSourcePlayer->GetPad ()->GetLastControllerState ();
        WriteSmallKeysync ( ControllerState, LastControllerState, BitStream );

        // Flags
        SKeysyncFlags flags;
        flags.data.bIsDucked = ( pSourcePlayer->IsDucked () == true );
        flags.data.bIsChoking = ( pSourcePlayer->IsChoking () == true );
        flags.data.bAkimboTargetUp = ( pSourcePlayer->IsAkimboArmUp () == true );

        // Write the flags
        BitStream.Write ( &flags );

        // If he's shooting
        if ( ControllerState.ButtonCircle )
        {
            // Write his current weapon slot
            unsigned int uiSlot = pSourcePlayer->GetWeaponSlot ();
            SWeaponSlotSync slot;
            slot.data.uiSlot = uiSlot;
            BitStream.Write ( &slot );

            if ( CWeaponNames::DoesSlotHaveAmmo ( uiSlot ) )
            {
                // Write his ammo in clip
                SWeaponAmmoSync ammo ( pSourcePlayer->GetWeaponType (), false, true );
                ammo.data.usAmmoInClip = pSourcePlayer->GetWeaponAmmoInClip ();
                BitStream.Write ( &ammo );

                // Write aim directions
                BitStream.Write ( pSourcePlayer->GetAimDirectionX () );
				BitStream.Write ( pSourcePlayer->GetAimDirectionY () );

                // Source vector if sniper
                CVector vecTemp = pSourcePlayer->GetSniperSourceVector ();
                BitStream.Write ( vecTemp.fX );
                BitStream.Write ( vecTemp.fY );
                BitStream.Write ( vecTemp.fZ );

                // Write his current target vector
                pSourcePlayer->GetTargettingVector ( vecTemp );
                BitStream.Write ( vecTemp.fX );
                BitStream.Write ( vecTemp.fY );
                BitStream.Write ( vecTemp.fZ );

                // Write the driveby aim directoin
                BitStream.Write ( pSourcePlayer->GetDriveByDirection () );
            }
            else
            {
                pSourcePlayer->SetWeaponAmmoInClip ( 1 );
                pSourcePlayer->SetWeaponTotalAmmo ( 1 );
            }
        }

        // If he's in a vehicle, read out the small vehicle specific data
        CVehicle* pVehicle = pSourcePlayer->GetOccupiedVehicle ();
        if ( pVehicle && pSourcePlayer->GetOccupiedVehicleSeat () == 0 )
        {
            WriteVehicleSpecific ( pVehicle, BitStream );

            if ( pVehicle->GetUpgrades ()->HasUpgrade ( 1087 ) ) // Hydraulics?
            {
                BitStream.Write ( ControllerState.RightStickX );
                BitStream.Write ( ControllerState.RightStickY );
            }
        }

        return true;
    }

    return false;
}


void CKeysyncPacket::ReadVehicleSpecific ( CVehicle* pVehicle, NetBitStreamInterface& BitStream )
{
    // Turret states
    unsigned short usModel = pVehicle->GetModel ();
    if ( CVehicleManager::HasTurret ( usModel ) )
    {
        // Read out the turret position
        float fHorizontal;
        float fVertical;
        BitStream.Read ( fHorizontal );
        BitStream.Read ( fVertical );
        
        // Set it
        pVehicle->SetTurretPosition ( fHorizontal, fVertical );
    }
}


void CKeysyncPacket::WriteVehicleSpecific ( CVehicle* pVehicle, NetBitStreamInterface& BitStream ) const
{
    // Turret states
    unsigned short usModel = pVehicle->GetModel ();
    if ( CVehicleManager::HasTurret ( usModel ) )
    {
        // Grab the turret position
        float fHorizontal;
        float fVertical;
        pVehicle->GetTurretPosition ( fHorizontal, fVertical );

        // Write it
        BitStream.Write ( fHorizontal );
        BitStream.Write ( fVertical );
    }
}
