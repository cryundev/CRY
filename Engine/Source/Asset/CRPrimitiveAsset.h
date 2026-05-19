#pragma once


#include "CRAsset.h"
#include "Source/Core/CRTypes.h"
#include "Source/Core/Containers/CRContainerInc.h"
#include "Source/Core/Math/CRAABB.h"
#include "Source/Core/Math/CRMath.h"
#include "Source/Core/Strings/CRStringInc.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRPrimitiveAsset
//---------------------------------------------------------------------------------------------------------------------
class CRPrimitiveAsset : public CRAsset
{
public:
    CRArray< CRVector   > Positions;
    CRArray< CRVector   > Normals;
    CRArray< CRVector   > Tangents;
    CRArray< CRVector   > Binormals;
    CRArray< CRVector   > Colors;
    CRArray< CRVector2D > UVs;    
    CRArray< u32        > Indices;

    u32 VertexCount = 0;

public:
    /// Constructor.
    CRPrimitiveAsset() = default;

    /// Destructor.
    virtual ~CRPrimitiveAsset() override {}

    /// Get asset type.
    virtual ECRAssetType GetAssetType() const override { return ECRAssetType::Primitive; }

    /// Calculate local-space bounds from positions.
    CRAABB CalculateBounds() const;

    /// Initialize.
    void Initialize( u32 InVertexCount );

    /// Returns true when tangent/binormal channels are present and usable for all vertices.
    bool HasValidTangentBasis() const;

    /// Rebuild tangent/binormal channels for triangle-list data.
    void EnsureTangentBasis();

    /// Build tangent/binormal channels without mutating the asset.
    void BuildTangentBasis( CRArray< CRVector >& OutTangents, CRArray< CRVector >& OutBinormals ) const;

protected:
    /// Clear current loaded data.
    virtual void ClearAsset() override;

    /// Called after a successful load.
    virtual void PostLoad() override;

    /// Get metadata byte size.
    virtual u64 GetMetadataSize() const override;

    /// Get payload byte size.
    virtual u64 GetPayloadSize() const override;

    /// Save metadata bytes.
    virtual bool SaveMetadata( CRAssetFile::Writer& Writer ) const override;

    /// Save payload bytes.
    virtual bool SavePayload( CRAssetFile::Writer& Writer ) const override;

    /// Load metadata bytes.
    virtual bool LoadMetadata( CRAssetFile::Reader& Reader ) override;

    /// Load payload bytes.
    virtual bool LoadPayload( CRAssetFile::Reader& Reader ) override;

};
