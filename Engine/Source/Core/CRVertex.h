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
    CRVector   Tangent   = CRVector( 1.0f, 0.0f, 0.0f );
    CRVector   Bitangent = CRVector( 0.0f, 1.0f, 0.0f );

public:
    /// Load vertices from primitive data.
    static void LoadFromPrimitiveData( const CRPrimitiveAsset& PrimitiveData, CRArray< CRVertex >& Vertices )
    {
        Vertices.clear();
        Vertices.reserve( PrimitiveData.VertexCount );

        for ( u32 i = 0; i < PrimitiveData.VertexCount; ++i )
        {
            const CRVector tangent   = ( PrimitiveData.Tangents .size() > i ) ? PrimitiveData.Tangents [ i ] : CRVector( 1.0f, 0.0f, 0.0f );
            const CRVector bitangent = ( PrimitiveData.Binormals.size() > i ) ? PrimitiveData.Binormals[ i ] : CRVector( 0.0f, 1.0f, 0.0f );

            Vertices.emplace_back
            (
                PrimitiveData.Positions[ i ],
                PrimitiveData.UVs      [ i ],
                PrimitiveData.Normals  [ i ],
                tangent,
                bitangent
            );
        }
    }
};


extern CRVertex GCRVTriangle[ 3 ];
extern CRVertex GCRVRect    [ 4 ];
