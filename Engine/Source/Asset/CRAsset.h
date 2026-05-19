#pragma once

#include "CRAssetFile.h"
#include "ICRAsset.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRAsset
//---------------------------------------------------------------------------------------------------------------------
class CRAsset : public ICRAsset
{
public:
    /// Constructor.
    CRAsset() = default;

    /// Destructor.
    virtual ~CRAsset() override {}

    /// Save.
    virtual void Save( const CRPath& Path ) override;

    /// Load.
    virtual void Load( const CRPath& Path ) override;

protected:
    /// Returns true when the asset has enough data to save.
    virtual bool CanSave() const { return true; }

    /// Clear current loaded data.
    virtual void ClearAsset() {}

    /// Called after a successful load.
    virtual void PostLoad() {}

    /// Get metadata byte size.
    virtual u64 GetMetadataSize() const = 0;

    /// Get payload byte size.
    virtual u64 GetPayloadSize() const = 0;

    /// Save metadata bytes.
    virtual bool SaveMetadata( CRAssetFile::Writer& Writer ) const = 0;

    /// Save payload bytes.
    virtual bool SavePayload( CRAssetFile::Writer& Writer ) const = 0;

    /// Load metadata bytes.
    virtual bool LoadMetadata( CRAssetFile::Reader& Reader ) = 0;

    /// Load payload bytes.
    virtual bool LoadPayload( CRAssetFile::Reader& Reader ) = 0;

private:
    /// Try loading asset data from reader.
    bool TryLoad( CRAssetFile::Reader& Reader );
};
