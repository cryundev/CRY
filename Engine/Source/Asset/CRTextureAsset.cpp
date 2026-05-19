#include "CRTextureAsset.h"


namespace
{
//---------------------------------------------------------------------------------------------------------------------
/// CRTextureAssetMetadata
//---------------------------------------------------------------------------------------------------------------------
struct CRTextureAssetMetadata
{
    u32 Width            = 0;
    u32 Height           = 0;
    u32 MipCount         = 0;
    u32 ArraySize        = 0;
    u32 Format           = 0;
    u32 ColorSpace       = 0;
    u32 SubresourceCount = 0;
    u64 PixelSize        = 0;
};

//---------------------------------------------------------------------------------------------------------------------
/// Returns true when a texture format is one of CRY's supported asset formats.
//---------------------------------------------------------------------------------------------------------------------
bool _IsValidTextureFormat( ECRTextureFormat Format )
{
    switch ( Format )
    {
        case ECRTextureFormat::R32G32B32A32_Float:
        case ECRTextureFormat::R32G32B32_Float:
        case ECRTextureFormat::R16G16B16A16_Float:
        case ECRTextureFormat::R16G16B16A16_UNorm:
        case ECRTextureFormat::R10G10B10_XR_Bias_A2_UNorm:
        case ECRTextureFormat::R10G10B10A2_UNorm:
        case ECRTextureFormat::R9G9B9E5_SharedExp:
        case ECRTextureFormat::R8G8B8A8_UNorm:
        case ECRTextureFormat::B8G8R8A8_UNorm:
        case ECRTextureFormat::B8G8R8X8_UNorm:
        case ECRTextureFormat::B5G5R5A1_UNorm:
        case ECRTextureFormat::B5G6R5_UNorm:
        case ECRTextureFormat::R32_Float:
        case ECRTextureFormat::R16_Float:
        case ECRTextureFormat::R16_UNorm:
        case ECRTextureFormat::R8_UNorm:
        case ECRTextureFormat::A8_UNorm:
        case ECRTextureFormat::BC1_UNorm:
        case ECRTextureFormat::BC1_UNorm_SRGB:
        case ECRTextureFormat::BC3_UNorm:
        case ECRTextureFormat::BC3_UNorm_SRGB:
        case ECRTextureFormat::BC5_UNorm:
        case ECRTextureFormat::BC7_UNorm:
        case ECRTextureFormat::BC7_UNorm_SRGB:
            return true;

        default: break;
    }

    return false;
}
}


//---------------------------------------------------------------------------------------------------------------------
/// Clear current loaded data.
//---------------------------------------------------------------------------------------------------------------------
void CRTextureAsset::ClearAsset()
{
    Width      = 0;
    Height     = 0;
    MipCount   = 0;
    ArraySize  = 0;
    Format     = ECRTextureFormat::Unknown;
    ColorSpace = ECRTextureColorSpace::Linear;

    Subresources.clear();
    Pixels.clear();
}

//---------------------------------------------------------------------------------------------------------------------
/// Get metadata byte size.
//---------------------------------------------------------------------------------------------------------------------
u64 CRTextureAsset::GetMetadataSize() const
{
    return sizeof( CRTextureAssetMetadata ) + sizeof( CRTextureAssetSubresource ) * Subresources.size();
}

//---------------------------------------------------------------------------------------------------------------------
/// Get payload byte size.
//---------------------------------------------------------------------------------------------------------------------
u64 CRTextureAsset::GetPayloadSize() const
{
    return Pixels.size();
}

//---------------------------------------------------------------------------------------------------------------------
/// Save metadata bytes.
//---------------------------------------------------------------------------------------------------------------------
bool CRTextureAsset::SaveMetadata( CRAssetFile::Writer& Writer ) const
{
    if ( !IsValid() ) return false;

    CRTextureAssetMetadata metadata;
    metadata.Width            = Width;
    metadata.Height           = Height;
    metadata.MipCount         = MipCount;
    metadata.ArraySize        = ArraySize;
    metadata.Format           = static_cast< u32 >( Format );
    metadata.ColorSpace       = static_cast< u32 >( ColorSpace );
    metadata.SubresourceCount = static_cast< u32 >( Subresources.size() );
    metadata.PixelSize        = Pixels.size();

    if ( !Writer.WriteValue( metadata ) ) return false;
    if ( !Writer.WriteArray( Subresources.data(), Subresources.size() ) ) return false;

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Save payload bytes.
//---------------------------------------------------------------------------------------------------------------------
bool CRTextureAsset::SavePayload( CRAssetFile::Writer& Writer ) const
{
    if ( !IsValid() ) return false;

    return Writer.WriteArray( Pixels.data(), Pixels.size() );
}

//---------------------------------------------------------------------------------------------------------------------
/// Load metadata bytes.
//---------------------------------------------------------------------------------------------------------------------
bool CRTextureAsset::LoadMetadata( CRAssetFile::Reader& Reader )
{
    const CRAssetFile::Header& header = Reader.GetHeader();
    
    CRTextureAssetMetadata metadata;
    if ( !Reader.ReadValue( metadata ) ) return false;

    const u64 expectedMetadataSize = sizeof( CRTextureAssetMetadata ) + sizeof( CRTextureAssetSubresource ) * metadata.SubresourceCount;
    if ( header.MetadataSize != expectedMetadataSize ) return false;
    if ( header.PayloadSize != metadata.PixelSize ) return false;

    Width       = metadata.Width;
    Height      = metadata.Height;
    MipCount    = metadata.MipCount;
    ArraySize   = metadata.ArraySize;
    Format      = static_cast< ECRTextureFormat >( metadata.Format );
    ColorSpace  = static_cast< ECRTextureColorSpace >( metadata.ColorSpace );

    Subresources.resize( metadata.SubresourceCount );
    Pixels.resize( static_cast< size_t >( metadata.PixelSize ) );

    return Reader.ReadArray( Subresources.data(), Subresources.size() );
}

//---------------------------------------------------------------------------------------------------------------------
/// Load payload bytes.
//---------------------------------------------------------------------------------------------------------------------
bool CRTextureAsset::LoadPayload( CRAssetFile::Reader& Reader )
{
    if ( Reader.GetHeader().PayloadSize != Pixels.size() ) return false;
    if ( !Reader.ReadArray( Pixels.data(), Pixels.size() ) ) return false;

    return IsValid();
}

//---------------------------------------------------------------------------------------------------------------------
/// Initialize as a single-subresource 2D texture.
//---------------------------------------------------------------------------------------------------------------------
void CRTextureAsset::Initialize2D( u32 InWidth, u32 InHeight, ECRTextureFormat InFormat, const u8* InPixels, u64 InPixelSize, u32 InRowPitch, u32 InSlicePitch, ECRTextureColorSpace InColorSpace )
{
    Width       = InWidth;
    Height      = InHeight;
    MipCount    = 1;
    ArraySize   = 1;
    Format      = InFormat;
    ColorSpace  = InColorSpace;

    Pixels.assign( InPixels, InPixels + InPixelSize );

    Subresources.clear();
    Subresources.push_back( CRTextureAssetSubresource() );

    CRTextureAssetSubresource& subresource = Subresources.back();
    subresource.Offset     = 0;
    subresource.Size       = InPixelSize;
    subresource.RowPitch   = InRowPitch;
    subresource.SlicePitch = InSlicePitch;
    subresource.Width      = InWidth;
    subresource.Height     = InHeight;
}

//---------------------------------------------------------------------------------------------------------------------
/// Returns true when this asset contains usable texture data.
//---------------------------------------------------------------------------------------------------------------------
bool CRTextureAsset::IsValid() const
{
    if ( Width == 0 || Height == 0 ) return false;
    if ( MipCount == 0 || ArraySize == 0 ) return false;
    if ( !_IsValidTextureFormat( Format ) ) return false;
    if ( Subresources.empty() || Pixels.empty() ) return false;

    for ( const CRTextureAssetSubresource& subresource : Subresources )
    {
        if ( subresource.Size == 0 ) return false;
        if ( subresource.Offset + subresource.Size > Pixels.size() ) return false;
        if ( subresource.RowPitch == 0 || subresource.SlicePitch == 0 ) return false;
    }

    return true;
}
