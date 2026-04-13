#include "CRPrimitiveAsset.h"
#include <cmath>
#include <fstream>
#include <ios>


namespace
{
//---------------------------------------------------------------------------------------------------------------------
/// Check vector component finiteness.
//---------------------------------------------------------------------------------------------------------------------
bool _IsFiniteVector( const CRVector& Value )
{
    return std::isfinite( Value.x ) && std::isfinite( Value.y ) && std::isfinite( Value.z );
}

//---------------------------------------------------------------------------------------------------------------------
/// Build a safe, normalized normal vector.
//---------------------------------------------------------------------------------------------------------------------
CRVector _BuildSafeNormal( const CRVector& Normal )
{
    CRVector safeNormal = Normal;
    if ( !_IsFiniteVector( safeNormal ) || safeNormal.LengthSquared() <= CRMath::Epsilon )
    {
        safeNormal = CRVector::Forward;
    }

    safeNormal.Normalize();
    return safeNormal;
}

//---------------------------------------------------------------------------------------------------------------------
/// Build a fallback orthonormal tangent basis from a normal.
//---------------------------------------------------------------------------------------------------------------------
void _BuildFallbackBasis( const CRVector& Normal, CRVector& OutTangent, CRVector& OutBinormal )
{
    const CRVector safeNormal = _BuildSafeNormal( Normal );
    const CRVector referenceAxis = ( CRMath::Abs( safeNormal.Dot( CRVector::Up ) ) < 0.999f ) ? CRVector::Up : CRVector::Right;

    OutTangent = referenceAxis.Cross( safeNormal );
    if ( OutTangent.LengthSquared() <= CRMath::Epsilon )
    {
        OutTangent = CRVector::Right;
    }
    OutTangent.Normalize();

    OutBinormal = safeNormal.Cross( OutTangent );
    if ( OutBinormal.LengthSquared() <= CRMath::Epsilon )
    {
        OutBinormal = CRVector::Up;
    }
    OutBinormal.Normalize();
}
}


//---------------------------------------------------------------------------------------------------------------------
/// Save to file.
//---------------------------------------------------------------------------------------------------------------------
void CRPrimitiveAsset::Save( const CRPath& Path )
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
void CRPrimitiveAsset::Load( const CRPath& Path )
{
    std::ifstream ifs( Path, std::ios::binary );
    if ( !ifs ) return;

    ifs.read( (char*)( &VertexCount ), sizeof( VertexCount ) );
    
    Positions .resize( VertexCount );
    Normals   .resize( VertexCount );
    Tangents  .resize( VertexCount );
    Binormals .resize( VertexCount );
    Colors    .resize( VertexCount );
    UVs       .resize( VertexCount );
    Indices   .resize( VertexCount );

    ifs.read( (char*)( Positions.data() ), Positions .size() * sizeof( CRVector   ) );
    ifs.read( (char*)( Normals  .data() ), Normals   .size() * sizeof( CRVector   ) );
    ifs.read( (char*)( Tangents .data() ), Tangents  .size() * sizeof( CRVector   ) );
    ifs.read( (char*)( Binormals.data() ), Binormals .size() * sizeof( CRVector   ) );
    ifs.read( (char*)( Colors   .data() ), Colors    .size() * sizeof( CRVector   ) );
    ifs.read( (char*)( UVs      .data() ), UVs       .size() * sizeof( CRVector2D ) );
    ifs.read( (char*)( Indices  .data() ), Indices   .size() * sizeof( u32        ) );


    ifs.close();

    if ( !HasValidTangentBasis() )
    {
        EnsureTangentBasis();
    }
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

//---------------------------------------------------------------------------------------------------------------------
/// Returns true when tangent/binormal channels are present and usable for all vertices.
//---------------------------------------------------------------------------------------------------------------------
bool CRPrimitiveAsset::HasValidTangentBasis() const
{
    if ( Tangents .size() != VertexCount ) return false;
    if ( Binormals.size() != VertexCount ) return false;

    for ( u32 i = 0; i < VertexCount; ++i )
    {
        if ( !_IsFiniteVector( Tangents [ i ] ) ) return false;
        if ( !_IsFiniteVector( Binormals[ i ] ) ) return false;
        if ( Tangents [ i ].LengthSquared() <= CRMath::Epsilon ) return false;
        if ( Binormals[ i ].LengthSquared() <= CRMath::Epsilon ) return false;
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Rebuild tangent/binormal channels for triangle-list data.
//---------------------------------------------------------------------------------------------------------------------
void CRPrimitiveAsset::EnsureTangentBasis()
{
    BuildTangentBasis( Tangents, Binormals );
}

//---------------------------------------------------------------------------------------------------------------------
/// Build tangent/binormal channels without mutating the asset.
//---------------------------------------------------------------------------------------------------------------------
void CRPrimitiveAsset::BuildTangentBasis( CRArray< CRVector >& OutTangents, CRArray< CRVector >& OutBinormals ) const
{
    OutTangents .assign( VertexCount, CRVector::Zero );
    OutBinormals.assign( VertexCount, CRVector::Zero );

    const bool bHasTriangleChannels = Positions.size() >= VertexCount &&
                                      Normals  .size() >= VertexCount &&
                                      UVs      .size() >= VertexCount &&
                                      VertexCount >= 3;

    if ( bHasTriangleChannels )
    {
        for ( u32 vertexIndex = 0; vertexIndex + 2 < VertexCount; vertexIndex += 3 )
        {
            const CRVector&   position0 = Positions[ vertexIndex + 0 ];
            const CRVector&   position1 = Positions[ vertexIndex + 1 ];
            const CRVector&   position2 = Positions[ vertexIndex + 2 ];
            const CRVector2D& uv0       = UVs      [ vertexIndex + 0 ];
            const CRVector2D& uv1       = UVs      [ vertexIndex + 1 ];
            const CRVector2D& uv2       = UVs      [ vertexIndex + 2 ];

            const CRVector   edge0 = position1 - position0;
            const CRVector   edge1 = position2 - position0;
            const CRVector2D duv0  = uv1 - uv0;
            const CRVector2D duv1  = uv2 - uv0;
            const f32        det   = duv0.x * duv1.y - duv0.y * duv1.x;

            if ( CRMath::Abs( det ) <= CRMath::Epsilon )
            {
                continue;
            }

            const f32      invDet   = 1.0f / det;
            const CRVector tangent  = ( edge0 * duv1.y - edge1 * duv0.y ) * invDet;
            const CRVector binormal = ( edge1 * duv0.x - edge0 * duv1.x ) * invDet;

            if ( !_IsFiniteVector( tangent ) || !_IsFiniteVector( binormal ) )
            {
                continue;
            }

            for ( u32 triangleVertex = 0; triangleVertex < 3; ++triangleVertex )
            {
                OutTangents [ vertexIndex + triangleVertex ] += tangent;
                OutBinormals[ vertexIndex + triangleVertex ] += binormal;
            }
        }
    }

    for ( u32 vertexIndex = 0; vertexIndex < VertexCount; ++vertexIndex )
    {
        const CRVector safeNormal = ( Normals.size() > vertexIndex ) ? _BuildSafeNormal( Normals[ vertexIndex ] ) : CRVector::Forward;

        CRVector tangent  = OutTangents [ vertexIndex ];
        CRVector binormal = OutBinormals[ vertexIndex ];

        const bool bNeedsFallback = !_IsFiniteVector( tangent ) ||
                                    !_IsFiniteVector( binormal ) ||
                                    tangent .LengthSquared() <= CRMath::Epsilon ||
                                    binormal.LengthSquared() <= CRMath::Epsilon;

        if ( bNeedsFallback )
        {
            _BuildFallbackBasis( safeNormal, tangent, binormal );
            OutTangents [ vertexIndex ] = tangent;
            OutBinormals[ vertexIndex ] = binormal;
            continue;
        }

        tangent -= safeNormal * tangent.Dot( safeNormal );
        if ( tangent.LengthSquared() <= CRMath::Epsilon )
        {
            _BuildFallbackBasis( safeNormal, tangent, binormal );
            OutTangents [ vertexIndex ] = tangent;
            OutBinormals[ vertexIndex ] = binormal;
            continue;
        }

        tangent.Normalize();

        CRVector orthogonalBinormal = safeNormal.Cross( tangent );
        if ( orthogonalBinormal.LengthSquared() <= CRMath::Epsilon )
        {
            _BuildFallbackBasis( safeNormal, tangent, binormal );
            OutTangents [ vertexIndex ] = tangent;
            OutBinormals[ vertexIndex ] = binormal;
            continue;
        }

        const f32 handedness = ( orthogonalBinormal.Dot( binormal ) < 0.0f ) ? -1.0f : 1.0f;
        orthogonalBinormal.Normalize();

        OutTangents [ vertexIndex ] = tangent;
        OutBinormals[ vertexIndex ] = orthogonalBinormal * handedness;
    }
}
