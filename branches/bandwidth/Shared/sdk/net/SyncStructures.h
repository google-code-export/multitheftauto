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

#include <net/bitstream.h>

#pragma pack(push)
#pragma pack(1)

struct ISyncStructure
{
    virtual         ~ISyncStructure () {}
    virtual bool    Read            ( NetBitStreamInterface& bitStream ) = 0;
    virtual void    Write           ( NetBitStreamInterface& bitStream ) = 0;
};

struct SPlayerPuresyncFlags : public ISyncStructure
{
    enum { BITCOUNT = 10 };

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
    } data;
};

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

#pragma pack(pop)
