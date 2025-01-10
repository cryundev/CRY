#pragma once


#include "CRPrimitiveData.h"
#include "CRTypes.h"
#include "Containers/CRContainerInc.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRVertex
//---------------------------------------------------------------------------------------------------------------------
struct CRVertex
{
public:
    CRVector   Position;
    CRVector2D TexCoord;
    CRVector   Normal;

public:
    /// Load vertices from primitive data.
    static void LoadFromPrimitiveData( const CRPrimitiveData& PrimitiveData, CRArray< CRVertex >& Vertices )
    {
        Vertices.clear();
        Vertices.reserve( PrimitiveData.VertexCount );

        for ( unsigned int i = 0; i < PrimitiveData.VertexCount; ++i )
        {
            Vertices.emplace_back( PrimitiveData.Positions[ i ], PrimitiveData.UVs[ i ], PrimitiveData.Normals[ i ] );
        }
    }
};


extern CRVertex GCRVTriangle[ 3 ];
extern CRVertex GCRVRect    [ 4 ];
