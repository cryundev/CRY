#pragma once


#include "CRAsset.h"
#include "Source/Core/Containers/CRContainerInc.h"
#include "Source/Core/CRTextureFormat.h"


//---------------------------------------------------------------------------------------------------------------------
/// ECRTextureColorSpace
//---------------------------------------------------------------------------------------------------------------------
enum class ECRTextureColorSpace : u32
{
    Linear = 0,
    SRGB   = 1,
};

//---------------------------------------------------------------------------------------------------------------------
/// CRTextureAssetSubresource
//---------------------------------------------------------------------------------------------------------------------
struct CRTextureAssetSubresource
{
    u64 Offset     = 0;
    u64 Size       = 0;
    u32 RowPitch   = 0;
    u32 SlicePitch = 0;
    u32 Width      = 0;
    u32 Height     = 0;
};

//---------------------------------------------------------------------------------------------------------------------
/// CRTextureAsset
//---------------------------------------------------------------------------------------------------------------------
class CRTextureAsset : public CRAsset
{
public:
    u32                   Width       = 0;
    u32                   Height      = 0;
    u32                   MipCount    = 0;
    u32                   ArraySize   = 0;
    ECRTextureFormat      Format      = ECRTextureFormat::Unknown;
    ECRTextureColorSpace  ColorSpace  = ECRTextureColorSpace::Linear;

    CRArray< CRTextureAssetSubresource > Subresources;
    CRArray< u8 >                        Pixels;

public:
    /// Constructor.
    CRTextureAsset() = default;

    /// Destructor.
    virtual ~CRTextureAsset() override {}

    /// Get asset type.
    virtual ECRAssetType GetAssetType() const override { return ECRAssetType::Texture2D; }

    /// Initialize as a single-subresource 2D texture.
    void Initialize2D( u32 InWidth, u32 InHeight, ECRTextureFormat InFormat, const u8* InPixels, u64 InPixelSize, u32 InRowPitch, u32 InSlicePitch, ECRTextureColorSpace InColorSpace = ECRTextureColorSpace::Linear );

    /// Returns true when this asset contains usable texture data.
    bool IsValid() const;

protected:
    /// Returns true when the asset has enough data to save.
    virtual bool CanSave() const override { return IsValid(); }

    /// Clear current loaded data.
    virtual void ClearAsset() override;

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
