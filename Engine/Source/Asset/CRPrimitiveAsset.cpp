#include "CRPrimitiveAsset.h"
#include <fstream>
#include <ios>


//---------------------------------------------------------------------------------------------------------------------
/// Save to file.
//---------------------------------------------------------------------------------------------------------------------
void CRPrimitiveAsset::Save( const CRString& Path )
{
    std::ofstream ofs( Path, std::ios::binary );
    if ( !ofs ) return;

    ofs.write( (const char*)( &VertexCount ), sizeof( VertexCount ) );

    ofs.write( (const char*)( Positions.data()), Positions.size() * sizeof( CRVector     ) );
    ofs.write( (const char*)( Normals  .data()), Normals  .size() * sizeof( CRVector     ) );
    ofs.write( (const char*)( Tangents .data()), Tangents .size() * sizeof( CRVector     ) );
    ofs.write( (const char*)( Binormals.data()), Binormals.size() * sizeof( CRVector     ) );
    ofs.write( (const char*)( Colors   .data()), Colors   .size() * sizeof( CRVector     ) );
    ofs.write( (const char*)( UVs      .data()), UVs      .size() * sizeof( CRVector2D   ) );
    ofs.write( (const char*)( Indices  .data()), Indices  .size() * sizeof( unsigned int ) );

    ofs.close();
}

//---------------------------------------------------------------------------------------------------------------------
/// Load from file.
//---------------------------------------------------------------------------------------------------------------------
void CRPrimitiveAsset::Load( const CRString& Path )
{
    std::ifstream ifs( Path, std::ios::binary );
    if ( !ifs ) return;

    ifs.read( (char*)( &VertexCount ), sizeof( VertexCount ) );

    Positions.resize( VertexCount );
    ifs.read( (char*)( Positions.data() ), Positions.size() * sizeof( CRVector ) );

    Normals.resize(VertexCount);
    ifs.read( (char*)( Normals.data() ), Normals.size() * sizeof( CRVector ) );

    Tangents.resize(VertexCount);
    ifs.read( (char*)( Tangents.data() ), Tangents.size() * sizeof( CRVector ) );

    Binormals.resize(VertexCount);
    ifs.read( (char*)( Binormals.data() ), Binormals.size() * sizeof( CRVector ) );

    Colors.resize(VertexCount);
    ifs.read( (char*)( Colors.data() ), Colors.size() * sizeof( CRVector ) );

    UVs.resize(VertexCount);
    ifs.read( (char*)( UVs.data() ), UVs.size() * sizeof( CRVector2D ) );

    Indices.resize(VertexCount);
    ifs.read( (char*)( Indices.data() ), Indices.size() * sizeof( u32 ) );

    ifs.close();
}

//---------------------------------------------------------------------------------------------------------------------
/// Calculate local-space bounds from positions.
//---------------------------------------------------------------------------------------------------------------------
CRAABB CRPrimitiveAsset::CalculateBounds() const
{
    CRAABB bounds;
    if ( Positions.empty() ) return bounds;

    CRVector minPosition = Positions[ 0 ];
    CRVector maxPosition = Positions[ 0 ];

    for ( size_t i = 1; i < Positions.size(); ++i )
    {
        const CRVector& position = Positions[ i ];

        minPosition.x = CRMath::Min( minPosition.x, position.x );
        minPosition.y = CRMath::Min( minPosition.y, position.y );
        minPosition.z = CRMath::Min( minPosition.z, position.z );

        maxPosition.x = CRMath::Max( maxPosition.x, position.x );
        maxPosition.y = CRMath::Max( maxPosition.y, position.y );
        maxPosition.z = CRMath::Max( maxPosition.z, position.z );
    }

    bounds.Min = minPosition;
    bounds.Max = maxPosition;
    
    return bounds;
}

//---------------------------------------------------------------------------------------------------------------------
/// Initialize
//---------------------------------------------------------------------------------------------------------------------
void CRPrimitiveAsset::Initialize( u32 InVertexCount )
{
    VertexCount = InVertexCount;
        
    Positions.resize( InVertexCount );
    Normals  .resize( InVertexCount );
    Tangents .resize( InVertexCount );
    Binormals.resize( InVertexCount );
    Colors   .resize( InVertexCount );
    UVs      .resize( InVertexCount );
}
