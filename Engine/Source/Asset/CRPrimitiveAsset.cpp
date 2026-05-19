#include "CRPrimitiveAsset.h"


namespace
{
static constexpr f32 BasisReferenceAxisParallelDotThreshold = 0.999f;

//---------------------------------------------------------------------------------------------------------------------
/// CRPrimitiveAssetMetadata
//---------------------------------------------------------------------------------------------------------------------
struct CRPrimitiveAssetMetadata
{
    u32 VertexCount   = 0;
    u32 PositionCount = 0;
    u32 NormalCount   = 0;
    u32 TangentCount  = 0;
    u32 BinormalCount = 0;
    u32 ColorCount    = 0;
    u32 UVCount       = 0;
    u32 IndexCount    = 0;
};

//---------------------------------------------------------------------------------------------------------------------
/// Check whether a tangent-space basis vector can be normalized and used.
//---------------------------------------------------------------------------------------------------------------------
bool _IsUsableBasisVector( const CRVector& Value )
{
    return CRMath::IsFinite( Value ) && Value.LengthSquared() > CRMath::Epsilon;
}

//---------------------------------------------------------------------------------------------------------------------
/// Check whether tangent/binormal vectors need a fallback basis.
//---------------------------------------------------------------------------------------------------------------------
bool _NeedsFallbackBasis( const CRVector& Tangent, const CRVector& Binormal )
{
    return !_IsUsableBasisVector( Tangent ) || !_IsUsableBasisVector( Binormal );
}

//---------------------------------------------------------------------------------------------------------------------
/// Build a safe, normalized normal vector.
//---------------------------------------------------------------------------------------------------------------------
CRVector _BuildSafeNormal( const CRVector& Normal )
{
    CRVector safeNormal = Normal;
    
    if ( !_IsUsableBasisVector( safeNormal ) )
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
    const CRVector safeNormal            = _BuildSafeNormal( Normal );
    const f32      normalUpDotAbs        = CRMath::Abs( safeNormal.Dot( CRVector::Up ) );
    const bool     bIsNearlyParallelToUp = normalUpDotAbs >= BasisReferenceAxisParallelDotThreshold;
    const CRVector referenceAxis         = bIsNearlyParallelToUp ? CRVector::Right : CRVector::Up;

    OutTangent = referenceAxis.Cross( safeNormal );
    if ( !_IsUsableBasisVector( OutTangent ) )
    {
        OutTangent = CRVector::Right;
    }
    OutTangent.Normalize();

    OutBinormal = safeNormal.Cross( OutTangent );
    if ( !_IsUsableBasisVector( OutBinormal ) )
    {
        OutBinormal = CRVector::Up;
    }
    OutBinormal.Normalize();
}

}


//---------------------------------------------------------------------------------------------------------------------
/// Clear current loaded data.
//---------------------------------------------------------------------------------------------------------------------
void CRPrimitiveAsset::ClearAsset()
{
    Positions.clear();
    Normals  .clear();
    Tangents .clear();
    Binormals.clear();
    Colors   .clear();
    UVs      .clear();
    Indices  .clear();
    
    VertexCount = 0;
}

//---------------------------------------------------------------------------------------------------------------------
/// Called after a successful load.
//---------------------------------------------------------------------------------------------------------------------
void CRPrimitiveAsset::PostLoad()
{
    if ( !HasValidTangentBasis() )
    {
        EnsureTangentBasis();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Get metadata byte size.
//---------------------------------------------------------------------------------------------------------------------
u64 CRPrimitiveAsset::GetMetadataSize() const
{
    return sizeof( CRPrimitiveAssetMetadata );
}

//---------------------------------------------------------------------------------------------------------------------
/// Get payload byte size.
//---------------------------------------------------------------------------------------------------------------------
u64 CRPrimitiveAsset::GetPayloadSize() const
{
    return sizeof( CRVector   ) * Positions.size() +
           sizeof( CRVector   ) * Normals  .size() +
           sizeof( CRVector   ) * Tangents .size() +
           sizeof( CRVector   ) * Binormals.size() +
           sizeof( CRVector   ) * Colors   .size() +
           sizeof( CRVector2D ) * UVs      .size() +
           sizeof( u32        ) * Indices  .size();
}

//---------------------------------------------------------------------------------------------------------------------
/// Save metadata bytes.
//---------------------------------------------------------------------------------------------------------------------
bool CRPrimitiveAsset::SaveMetadata( CRAssetFile::Writer& Writer ) const
{
    CRPrimitiveAssetMetadata metadata;
    
    metadata.VertexCount   = VertexCount;    
    metadata.PositionCount = static_cast< u32 >( Positions.size() );
    metadata.NormalCount   = static_cast< u32 >( Normals  .size() );
    metadata.TangentCount  = static_cast< u32 >( Tangents .size() );
    metadata.BinormalCount = static_cast< u32 >( Binormals.size() );
    metadata.ColorCount    = static_cast< u32 >( Colors   .size() );
    metadata.UVCount       = static_cast< u32 >( UVs      .size() );
    metadata.IndexCount    = static_cast< u32 >( Indices  .size() );

    return Writer.WriteValue( metadata );
}

//---------------------------------------------------------------------------------------------------------------------
/// Save payload bytes.
//---------------------------------------------------------------------------------------------------------------------
bool CRPrimitiveAsset::SavePayload( CRAssetFile::Writer& Writer ) const
{
    if ( !Writer.WriteArray( Positions.data(), Positions.size() ) ) return false;
    if ( !Writer.WriteArray( Normals  .data(), Normals  .size() ) ) return false;
    if ( !Writer.WriteArray( Tangents .data(), Tangents .size() ) ) return false;
    if ( !Writer.WriteArray( Binormals.data(), Binormals.size() ) ) return false;
    if ( !Writer.WriteArray( Colors   .data(), Colors   .size() ) ) return false;
    if ( !Writer.WriteArray( UVs      .data(), UVs      .size() ) ) return false;
    if ( !Writer.WriteArray( Indices  .data(), Indices  .size() ) ) return false;

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Load metadata bytes.
//---------------------------------------------------------------------------------------------------------------------
bool CRPrimitiveAsset::LoadMetadata( CRAssetFile::Reader& Reader )
{
    const CRAssetFile::Header& header = Reader.GetHeader();
    if ( header.MetadataSize != sizeof( CRPrimitiveAssetMetadata ) ) return false;

    CRPrimitiveAssetMetadata metadata;
    if ( !Reader.ReadValue( metadata ) ) return false;

    VertexCount = metadata.VertexCount;

    Positions .resize( metadata.PositionCount );
    Normals   .resize( metadata.NormalCount   );
    Tangents  .resize( metadata.TangentCount  );
    Binormals .resize( metadata.BinormalCount );
    Colors    .resize( metadata.ColorCount    );
    UVs       .resize( metadata.UVCount       );
    Indices   .resize( metadata.IndexCount    );

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Load payload bytes.
//---------------------------------------------------------------------------------------------------------------------
bool CRPrimitiveAsset::LoadPayload( CRAssetFile::Reader& Reader )
{
    if ( Reader.GetHeader().PayloadSize != GetPayloadSize() ) return false;

    if ( !Reader.ReadArray( Positions.data(), Positions.size() ) ) return false;
    if ( !Reader.ReadArray( Normals  .data(), Normals  .size() ) ) return false;
    if ( !Reader.ReadArray( Tangents .data(), Tangents .size() ) ) return false;
    if ( !Reader.ReadArray( Binormals.data(), Binormals.size() ) ) return false;
    if ( !Reader.ReadArray( Colors   .data(), Colors   .size() ) ) return false;
    if ( !Reader.ReadArray( UVs      .data(), UVs      .size() ) ) return false;
    if ( !Reader.ReadArray( Indices  .data(), Indices  .size() ) ) return false;

    return true;
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
        if ( _NeedsFallbackBasis( Tangents[ i ], Binormals[ i ] ) ) return false;
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

    bool bHasTriangleChannels = true;
    
    bHasTriangleChannels &= Positions.size() >= VertexCount;
    bHasTriangleChannels &= Normals  .size() >= VertexCount;
    bHasTriangleChannels &= UVs      .size() >= VertexCount;
    bHasTriangleChannels &= VertexCount >= 3;

    if ( bHasTriangleChannels )
    {
        for ( u32 vertexIndex = 0; vertexIndex + 2 < VertexCount; vertexIndex += 3 )
        {
            const CRVector& position0 = Positions[ vertexIndex + 0 ];
            const CRVector& position1 = Positions[ vertexIndex + 1 ];
            const CRVector& position2 = Positions[ vertexIndex + 2 ];
            
            const CRVector2D& uv0 = UVs[ vertexIndex + 0 ];
            const CRVector2D& uv1 = UVs[ vertexIndex + 1 ];
            const CRVector2D& uv2 = UVs[ vertexIndex + 2 ];

            const CRVector& edge0 = position1 - position0;
            const CRVector& edge1 = position2 - position0;
            
            const CRVector2D& duv0  = uv1 - uv0;
            const CRVector2D& duv1  = uv2 - uv0;
            
            const f32 det = duv0.x * duv1.y - duv0.y * duv1.x;

            if ( CRMath::Abs( det ) <= CRMath::Epsilon ) continue;

            const f32 invDet = 1.0f / det;
            
            const CRVector& tangent  = ( edge0 * duv1.y - edge1 * duv0.y ) * invDet;
            const CRVector& binormal = ( edge1 * duv0.x - edge0 * duv1.x ) * invDet;

            if ( !CRMath::IsFinite( tangent ) || !CRMath::IsFinite( binormal ) ) continue;

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

        if ( _NeedsFallbackBasis( tangent, binormal ) )
        {
            _BuildFallbackBasis( safeNormal, tangent, binormal );
            
            OutTangents [ vertexIndex ] = tangent;
            OutBinormals[ vertexIndex ] = binormal;
            
            continue;
        }

        tangent -= safeNormal * tangent.Dot( safeNormal );
        
        if ( !_IsUsableBasisVector( tangent ) )
        {
            _BuildFallbackBasis( safeNormal, tangent, binormal );
            
            OutTangents [ vertexIndex ] = tangent;
            OutBinormals[ vertexIndex ] = binormal;
            
            continue;
        }

        tangent.Normalize();

        CRVector orthogonalBinormal = safeNormal.Cross( tangent );
        if ( !_IsUsableBasisVector( orthogonalBinormal ) )
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
