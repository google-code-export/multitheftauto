/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        Shared/sdk/net/SyncStructures.h
*  PURPOSE:     Structures used for syncing stuff through the network.
*  DEVELOPERS:  Alberto Alonso <rydencillo@gmail.com>
*               
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#pragma once

#include <CVector.h>
#include <net/bitstream.h>

#pragma pack(push)
#pragma pack(1)

// Interface for all sync structures
struct ISyncStructure
{
    virtual         ~ISyncStructure () {}
    virtual bool    Read            ( NetBitStreamInterface& bitStream ) = 0;
    virtual void    Write           ( NetBitStreamInterface& bitStream ) = 0;
};



//////////////////////////////////////////
//                                      //
//           Player pure-sync           //
//                                      //
//////////////////////////////////////////
struct SPlayerPuresyncFlags : public ISyncStructure
{
    enum { BITCOUNT = 11 };

    bool Read ( NetBitStreamInterface& bitStream )
    {
        return bitStream.ReadBits ( reinterpret_cast < char* > ( &data ), BITCOUNT );
    }
    void Write ( NetBitStreamInterface& bitStream )
    {
        bitStream.WriteBits ( reinterpret_cast < const char* > ( &data ), BITCOUNT );
    }

    struct
    {
        bool bIsInWater : 1;
        bool bIsOnGround : 1;
        bool bHasJetPack : 1;
        bool bIsDucked : 1;
        bool bWearsGoogles : 1;
        bool bHasContact : 1;
        bool bIsChoking : 1;
        bool bAkimboTargetUp : 1;
        bool bIsOnFire : 1;
        bool bHasAWeapon : 1;
        bool bSyncingVelocity : 1;
    } data;
};



//////////////////////////////////////////
//                                      //
//               Keysync                //
//                                      //
//////////////////////////////////////////
struct SKeysyncFlags : public ISyncStructure
{
    enum { BITCOUNT = 3 };

    bool Read ( NetBitStreamInterface& bitStream )
    {
        return bitStream.ReadBits ( reinterpret_cast < char* > ( &data ), BITCOUNT );
    }
    void Write ( NetBitStreamInterface& bitStream )
    {
        bitStream.WriteBits ( reinterpret_cast < const char* > ( &data ), BITCOUNT );
    }

    struct
    {
        bool bIsDucked : 1;
        bool bIsChoking : 1;
        bool bAkimboTargetUp : 1;
    } data;
};

struct SFullKeysyncSync : public ISyncStructure
{
    bool Read ( NetBitStreamInterface& bitStream )
    {
        bool bState = bitStream.ReadBits ( reinterpret_cast < char* > ( &data ), 8 );
        if ( bState )
        {
            char cLeftStickX;
            bState = bitStream.Read ( cLeftStickX );
            if ( bState )
            {
                data.sLeftStickX = static_cast < short > ( (float)cLeftStickX * 128.0f/127.0f );
                char cLeftStickY;
                bState = bitStream.Read ( cLeftStickY );
                if ( bState )
                {
                    data.sLeftStickY = static_cast < short > ( (float)cLeftStickY * 128.0f/127.0f );
                }
            }
        }

        return bState;
    }
    void Write ( NetBitStreamInterface& bitStream )
    {
        bitStream.WriteBits ( reinterpret_cast < const char* > ( &data ), 8 );
        char cLeftStickX = static_cast < char > ( (float)data.sLeftStickX * 127.0f/128.0f );
        bitStream.Write ( cLeftStickX );
        char cLeftStickY = static_cast < char > ( (float)data.sLeftStickY * 127.0f/128.0f );
        bitStream.Write ( cLeftStickY );
    }

    struct
    {
        bool bLeftShoulder1 : 1;
        bool bRightShoulder1 : 1;
        bool bButtonSquare : 1;
        bool bButtonCross : 1;
        bool bButtonCircle : 1;
        bool bButtonTriangle : 1;
        bool bShockButtonL : 1;
        bool bPedWalk : 1;
        short sLeftStickX;
        short sLeftStickY;
    } data;
};



//////////////////////////////////////////
//                                      //
//             Weapon sync              //
//                                      //
//////////////////////////////////////////
struct SWeaponSlotSync : public ISyncStructure
{
    enum { BITCOUNT = 4 };

    bool Read ( NetBitStreamInterface& bitStream )
    {
        return bitStream.ReadBits ( reinterpret_cast < char* > ( &data ), BITCOUNT );
    }
    void Write ( NetBitStreamInterface& bitStream )
    {
        bitStream.WriteBits ( reinterpret_cast < const char* > ( &data ), BITCOUNT );
    }

    struct
    {
        unsigned int uiSlot : 4;
    } data;
};

struct SWeaponTypeSync : public ISyncStructure
{
    enum { BITCOUNT = 6 };

    bool Read ( NetBitStreamInterface& bitStream )
    {
        return bitStream.ReadBits ( reinterpret_cast < char* > ( &data ), BITCOUNT );
    }
    void Write ( NetBitStreamInterface& bitStream )
    {
        bitStream.WriteBits ( reinterpret_cast < const char* > ( &data ), BITCOUNT );
    }

    struct
    {
        unsigned int uiWeaponType : 6;
    } data;
};

struct IAmmoInClipSync : public virtual ISyncStructure
{
    virtual ~IAmmoInClipSync () {}
    virtual inline unsigned short GetAmmoInClip () const = 0;
};

#define DECLARE_WEAPON_AMMOINCLIP_STRUCT(weaponName, bitCount) \
struct S ## weaponName ## AmmoInClipSync : public IAmmoInClipSync \
{ \
    S ## weaponName ## AmmoInClipSync ( unsigned short usAmmoInClip ) { data.usAmmoInClip = usAmmoInClip; } \
    \
    bool Read ( NetBitStreamInterface& bitStream ) \
    { \
        return bitStream.ReadBits ( reinterpret_cast < char* > ( &data ), (bitCount) ); \
    } \
    void Write ( NetBitStreamInterface& bitStream ) \
    { \
        bitStream.WriteBits ( reinterpret_cast < const char* > ( &data ), (bitCount) ); \
    } \
    \
    inline unsigned short GetAmmoInClip () const { return data.usAmmoInClip; } \
    \
private: \
    struct \
    { \
        unsigned short usAmmoInClip : ## bitCount ; \
    } data; \
}

// Declare specific weapon ammo in clip sync structures
//                               wepname,   bitcount
DECLARE_WEAPON_AMMOINCLIP_STRUCT(Pistol,    6);
DECLARE_WEAPON_AMMOINCLIP_STRUCT(Silenced,  5);
DECLARE_WEAPON_AMMOINCLIP_STRUCT(Deagle,    3);
DECLARE_WEAPON_AMMOINCLIP_STRUCT(Shotgun,   1);
DECLARE_WEAPON_AMMOINCLIP_STRUCT(Sawnoff,   3);
DECLARE_WEAPON_AMMOINCLIP_STRUCT(Spas12,    3);
DECLARE_WEAPON_AMMOINCLIP_STRUCT(Uzi,       7);
DECLARE_WEAPON_AMMOINCLIP_STRUCT(Mp5,       5);
DECLARE_WEAPON_AMMOINCLIP_STRUCT(Tec9,      7);
DECLARE_WEAPON_AMMOINCLIP_STRUCT(Ak47,      5);
DECLARE_WEAPON_AMMOINCLIP_STRUCT(M4,        6);
DECLARE_WEAPON_AMMOINCLIP_STRUCT(Rifle,     1);
DECLARE_WEAPON_AMMOINCLIP_STRUCT(Sniper,    1);
DECLARE_WEAPON_AMMOINCLIP_STRUCT(RLauncher, 1);
DECLARE_WEAPON_AMMOINCLIP_STRUCT(RPG,       1);
DECLARE_WEAPON_AMMOINCLIP_STRUCT(FThrower,  6);
DECLARE_WEAPON_AMMOINCLIP_STRUCT(Minigun,   9);
DECLARE_WEAPON_AMMOINCLIP_STRUCT(Grenade,   1);
DECLARE_WEAPON_AMMOINCLIP_STRUCT(TearGas,   1);
DECLARE_WEAPON_AMMOINCLIP_STRUCT(Molotov,   1);
DECLARE_WEAPON_AMMOINCLIP_STRUCT(Satchel,   1);
DECLARE_WEAPON_AMMOINCLIP_STRUCT(Spraycan,  9);
DECLARE_WEAPON_AMMOINCLIP_STRUCT(FireExt,   9);
DECLARE_WEAPON_AMMOINCLIP_STRUCT(Camera,    6);

#undef DECLARE_WEAPON_AMMOINCLIP_STRUCT

struct SWeaponAmmoSync : public ISyncStructure
{
    SWeaponAmmoSync ( unsigned char ucWeaponType, bool bSyncTotalAmmo = true, bool bSyncAmmoInClip = true )
    : m_ucWeaponType ( ucWeaponType ), m_bSyncTotalAmmo ( bSyncTotalAmmo ), m_bSyncAmmoInClip ( bSyncAmmoInClip )
    {
    }

    bool Read ( NetBitStreamInterface& bitStream )
    {
        bool bStatus = true;
        if ( m_bSyncTotalAmmo )
            bStatus = bitStream.ReadCompressed ( data.usTotalAmmo );

        if ( m_bSyncAmmoInClip && bStatus == true )
        {
            IAmmoInClipSync* pAmmoInClipSync = GetBestAmmoInClipSyncForWeapon ();
            if ( pAmmoInClipSync )
            {
                bStatus = bitStream.Read ( pAmmoInClipSync );
                if ( bStatus )
                    data.usAmmoInClip = pAmmoInClipSync->GetAmmoInClip ();
                else
                    data.usAmmoInClip = 0;
                delete pAmmoInClipSync;
            }
            else
                bStatus = false;
        }

        return bStatus;
    }

    void Write ( NetBitStreamInterface& bitStream )
    {
        if ( m_bSyncTotalAmmo )
            bitStream.WriteCompressed ( data.usTotalAmmo );
        if ( m_bSyncAmmoInClip )
        {
            IAmmoInClipSync* pAmmoInClipSync = GetBestAmmoInClipSyncForWeapon ();
            if ( pAmmoInClipSync )
            {
                bitStream.Write ( pAmmoInClipSync );
                delete pAmmoInClipSync;
            }
        }
    }

    struct
    {
        unsigned short usTotalAmmo;
        unsigned short usAmmoInClip;
    } data;

private:
    unsigned char   m_ucWeaponType;
    bool            m_bSyncTotalAmmo;
    bool            m_bSyncAmmoInClip;

    IAmmoInClipSync* GetBestAmmoInClipSyncForWeapon ( )
    {
        switch ( m_ucWeaponType )
        {

            case 22: return new SPistolAmmoInClipSync ( data.usAmmoInClip ); break;
            case 23: return new SSilencedAmmoInClipSync ( data.usAmmoInClip ); break;
            case 24: return new SDeagleAmmoInClipSync ( data.usAmmoInClip ); break;
            case 25: return new SShotgunAmmoInClipSync ( data.usAmmoInClip ); break;
            case 26: return new SSawnoffAmmoInClipSync ( data.usAmmoInClip ); break;
            case 27: return new SSpas12AmmoInClipSync ( data.usAmmoInClip ); break;
            case 28: return new SUziAmmoInClipSync ( data.usAmmoInClip ); break;
            case 29: return new SMp5AmmoInClipSync ( data.usAmmoInClip ); break;
            case 32: return new STec9AmmoInClipSync ( data.usAmmoInClip ); break;
            case 30: return new SAk47AmmoInClipSync ( data.usAmmoInClip ); break;
            case 31: return new SM4AmmoInClipSync ( data.usAmmoInClip ); break;
            case 33: return new SRifleAmmoInClipSync ( data.usAmmoInClip ); break;
            case 34: return new SSniperAmmoInClipSync ( data.usAmmoInClip ); break;
            case 35: return new SRLauncherAmmoInClipSync ( data.usAmmoInClip ); break;
            case 36: return new SRPGAmmoInClipSync ( data.usAmmoInClip ); break;
            case 37: return new SFThrowerAmmoInClipSync ( data.usAmmoInClip ); break;
            case 38: return new SMinigunAmmoInClipSync ( data.usAmmoInClip ); break;
            case 16: return new SGrenadeAmmoInClipSync ( data.usAmmoInClip ); break;
            case 17: return new STearGasAmmoInClipSync ( data.usAmmoInClip ); break;
            case 18: return new SMolotovAmmoInClipSync ( data.usAmmoInClip ); break;
            case 39: return new SSatchelAmmoInClipSync ( data.usAmmoInClip ); break;
            case 41: return new SSpraycanAmmoInClipSync ( data.usAmmoInClip ); break;
            case 42: return new SFireExtAmmoInClipSync ( data.usAmmoInClip ); break;
            case 43: return new SCameraAmmoInClipSync ( data.usAmmoInClip ); break;
            case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 15: default:
                // Melee
                return NULL;
                break;
        }
    }
};

struct SWeaponAimSync : public ISyncStructure
{
    SWeaponAimSync ( float fWeaponRange = 0.0f ) : m_fWeaponRange ( fWeaponRange ) {}

    bool Read ( NetBitStreamInterface& bitStream )
    {
        bool bStatus = true;
        CVector vecDirection;

        if (( bStatus = bitStream.Read ( data.vecOrigin.fX ) ))
            if (( bStatus = bitStream.Read ( data.vecOrigin.fY ) ))
                if (( bStatus = bitStream.Read ( data.vecOrigin.fZ ) ))
                    bStatus = bitStream.ReadNormVector ( vecDirection.fX, vecDirection.fZ, vecDirection.fY );

        if ( bStatus )
            data.vecTarget = data.vecOrigin + ( vecDirection * m_fWeaponRange );
        else
            data.vecOrigin = data.vecTarget = CVector ( );

        return bStatus;
    }

    void Write ( NetBitStreamInterface& bitStream )
    {
        // Write the origin of the bullets
        bitStream.Write ( data.vecOrigin.fX );
        bitStream.Write ( data.vecOrigin.fY );
        bitStream.Write ( data.vecOrigin.fZ );

        // Get the direction of the bullets
        CVector vecDirection = data.vecTarget - data.vecOrigin;
        vecDirection.Normalize ();

        // Write the normalized vector
        bitStream.WriteNormVector ( vecDirection.fX, vecDirection.fZ, vecDirection.fY );
    }

    struct
    {
        CVector vecOrigin;
        CVector vecTarget;
    } data;

private:
    float   m_fWeaponRange;
};



//////////////////////////////////////////
//                                      //
//                Others                //
//                                      //
//////////////////////////////////////////
struct SBodypartSync : public ISyncStructure
{
    enum { BITCOUNT = 3 };

    bool Read ( NetBitStreamInterface& bitStream )
    {
        bool bStatus = bitStream.ReadBits ( reinterpret_cast < char* > ( &privateData ), BITCOUNT );
        if ( bStatus )
            data.uiBodypart = privateData.uiBodypart + 3;
        else
            data.uiBodypart = 0;
        return bStatus;
    }
    void Write ( NetBitStreamInterface& bitStream )
    {
        // Bodyparts go from 3 to 9, so substracting 3 from the value
        // and then restoring it will save 1 bit.
        privateData.uiBodypart = data.uiBodypart - 3;
        bitStream.WriteBits ( reinterpret_cast < const char* > ( &privateData ), BITCOUNT );
    }

    struct
    {
        unsigned int uiBodypart;
    } data;

private:
    struct
    {
        unsigned int uiBodypart : 3;
    } privateData;
};


#pragma pack(pop)
