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

// WARNING: the this+4 is an ugly hack to avoid the vtable first 4 bytes.
// If you have a better solution that is not as ugly as this, please fix it.

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
        return bitStream.ReadBits ( reinterpret_cast < char* > ( this ) + 4, BITCOUNT );
    }
    void Write ( NetBitStreamInterface& bitStream )
    {
        bitStream.WriteBits ( reinterpret_cast < const char* > ( this ) + 4, BITCOUNT );
    }

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
};

struct SKeysyncFlags : public ISyncStructure
{
    enum { BITCOUNT = 3 };

    bool Read ( NetBitStreamInterface& bitStream )
    {
        return bitStream.ReadBits ( reinterpret_cast < char* > ( this ) + 4, BITCOUNT );
    }
    void Write ( NetBitStreamInterface& bitStream )
    {
        bitStream.WriteBits ( reinterpret_cast < const char* > ( this ) + 4, BITCOUNT );
    }

    bool bIsDucked : 1;
    bool bIsChoking : 1;
    bool bAkimboTargetUp : 1;
};

struct SWeaponSlotSync : public ISyncStructure
{
    enum { BITCOUNT = 4 };

    bool Read ( NetBitStreamInterface& bitStream )
    {
        return bitStream.ReadBits ( reinterpret_cast < char* > ( this ) + 4, BITCOUNT );
    }
    void Write ( NetBitStreamInterface& bitStream )
    {
        bitStream.WriteBits ( reinterpret_cast < const char* > ( this ) + 4, BITCOUNT );
    }

    unsigned int uiSlot : 4;
};
