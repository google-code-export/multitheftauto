/*****************************************************************************
*
*  PROJECT:		Multi Theft Auto v1.0
*  LICENSE:		See LICENSE in the top level directory
*  FILE:		game_sa/CWaterSA.cpp
*  PURPOSE:		Control the lakes and seas
*  DEVELOPERS:	arc_
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#include "StdInc.h"

extern CWaterManagerSA* g_pWaterManager;

// -----------------------------------------------------
// Vertices

short CWaterVertexSA::GetID ()
{
    if ( !m_pInterface )
        return ~0;
    return (short)(m_pInterface - g_pWaterManager->m_VertexPool);
}

void CWaterVertexSA::GetPosition ( CVector& vec )
{
    vec.setX( (float)m_pInterface->m_sX );
    vec.setY( (float)m_pInterface->m_sY );
    vec.setZ( m_pInterface->m_fZ );
}

bool CWaterVertexSA::SetPosition ( const CVector& vec, void* pChangeSource )
{
    if ( pChangeSource )
        g_pWaterManager->AddChange ( pChangeSource, this, new CWaterChangeVertexMove ( this ) );
    
    m_pInterface->m_sX = ((short)vec.getX()) & ~1;
    m_pInterface->m_sY = ((short)vec.getY()) & ~1;
    m_pInterface->m_fZ = vec.getZ();
    return true;
}

// -----------------------------------------------------
// Polygons

void CWaterQuadSA::SetInterface ( CWaterPolySAInterface* pInterface )
{
    m_pInterface = pInterface;
    m_wID = (short)(pInterface - g_pWaterManager->m_QuadPool);
}

void CWaterTriangleSA::SetInterface ( CWaterPolySAInterface* pInterface )
{
    m_pInterface = pInterface;
    m_wID = (short)(pInterface - g_pWaterManager->m_TrianglePool);
}

CWaterVertex* CWaterPolySA::GetVertex ( int index )
{
    if ( index < 0 || index >= GetNumVertices () )
        return NULL;

    return &g_pWaterManager->m_Vertices [
        GetType () == WATER_POLY_QUAD ? ((CWaterQuadSA *)this)->GetInterface ()->m_wVertexIDs[index]
                                  : ((CWaterTriangleSA *)this)->GetInterface ()->m_wVertexIDs[index]
    ];
}

bool CWaterPolySA::ContainsPoint ( float fX, float fY )
{
    bool bInside = false;
    int numVertices = GetNumVertices ();
    short* pwVertexIDs =
        GetType () == WATER_POLY_QUAD ? ((CWaterQuadSA *)this)->GetInterface ()->m_wVertexIDs
                                  : ((CWaterTriangleSA *)this)->GetInterface ()->m_wVertexIDs;
    
    CWaterVertexSA* pFrom;
    CWaterVertexSA* pTo;
    CVector vecFrom;
    CVector vecTo;
    int next[4];
    if ( GetType () == WATER_POLY_QUAD )
    {
        next[0] = 1;
        next[1] = 3;
        next[2] = 0;
        next[3] = 2;
    }
    else
    {
        next[0] = 1;
        next[1] = 2;
        next[2] = 0;
    }
    for ( int i = 0; i < numVertices; i++ )
    {
        pFrom = &g_pWaterManager->m_Vertices [ pwVertexIDs[i] ];
        pTo   = &g_pWaterManager->m_Vertices [ pwVertexIDs[next[i]] ];
        pFrom->GetPosition ( vecFrom );
        pTo->GetPosition ( vecTo );
        
        if ( (vecFrom.getY() > fY) != (vecTo.getY() > fY) &&
             fX < vecFrom.getX() + (vecTo.getX() - vecFrom.getX()) * (fY - vecFrom.getX()) / (vecTo.getY() - vecFrom.getY()) )
           bInside = !bInside;
    }
    return bInside;
}