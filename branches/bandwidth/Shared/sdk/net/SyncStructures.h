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

struct SPlayerPuresyncFlags
{
    enum { BITCOUNT = 9 };

    SPlayerPuresyncFlags () { memset ( this, 0, sizeof ( *this ) ); }

    bool bIsInWater : 1;
    bool bIsOnGround : 1;
    bool bHasJetPack : 1;
    bool bIsDucked : 1;
    bool bWearsGoogles : 1;
    bool bHasContact : 1;
    bool bIsChoking : 1;
    bool bAkimboTargetUp : 1;
    bool bIsOnFire : 1;
};