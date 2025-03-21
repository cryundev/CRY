#pragma once


#include "Source/Asset/CRPrimitiveAsset.h"
#include "CRInclue.h"
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
    static void LoadFromPrimitiveData( const CRPrimitiveAsset& PrimitiveData, CRArray< CRVertex >& Vertices )
    {
        Vertices.clear();
        Vertices.reserve( PrimitiveData.VertexCount );

        for ( u32 i = 0; i < PrimitiveData.VertexCount; ++i )
        {
            Vertices.emplace_back( PrimitiveData.Positions[ i ], PrimitiveData.UVs[ i ], PrimitiveData.Normals[ i ] );
        }
    }
};


extern CRVertex GCRVTriangle[ 3 ];
extern CRVertex GCRVRect    [ 4 ];
