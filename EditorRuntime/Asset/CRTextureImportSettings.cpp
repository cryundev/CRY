#include "CRTextureImportSettings.h"


namespace
{
const CRString EmptyString;
const CRString UnknownTextureCompressionOptionReason = "Unknown texture compression option.";
const CRString BCCompressionUnsupportedReason        = "BC compression requires a texture compression backend before import can run.";
const CRString MipmapGenerationUnsupportedReason     = "Mipmap generation requires a mip build backend before import can run.";

//---------------------------------------------------------------------------------------------------------------------
/// Texture color space options exposed to editor import UI.
//---------------------------------------------------------------------------------------------------------------------
const CRTextureImportColorSpaceOptionDescriptor TextureImportColorSpaceOptions[] =
{
    { "Linear", ECRTextureColorSpace::Linear },
    { "sRGB",   ECRTextureColorSpace::SRGB   },
};

//---------------------------------------------------------------------------------------------------------------------
/// Texture compression options exposed to editor import UI.
//---------------------------------------------------------------------------------------------------------------------
const CRTextureImportCompressionOptionDescriptor TextureImportCompressionOptions[] =
{
    { "Auto Format",      EmptyString,                    ECRTextureImportCompressionOption::Auto,         ECRTextureFormat::Unknown,   ECRTextureFormat::Unknown,        true  },
    { "Uncompressed",     EmptyString,                    ECRTextureImportCompressionOption::Uncompressed, ECRTextureFormat::Unknown,   ECRTextureFormat::Unknown,        true  },
    { "BC1 RGB",          BCCompressionUnsupportedReason, ECRTextureImportCompressionOption::BC1,          ECRTextureFormat::BC1_UNorm, ECRTextureFormat::BC1_UNorm_SRGB, false },
    { "BC3 RGBA",         BCCompressionUnsupportedReason, ECRTextureImportCompressionOption::BC3,          ECRTextureFormat::BC3_UNorm, ECRTextureFormat::BC3_UNorm_SRGB, false },
    { "BC5 Normal",       BCCompressionUnsupportedReason, ECRTextureImportCompressionOption::BC5,          ECRTextureFormat::BC5_UNorm, ECRTextureFormat::BC5_UNorm,      false },
    { "BC7 High Quality", BCCompressionUnsupportedReason, ECRTextureImportCompressionOption::BC7,          ECRTextureFormat::BC7_UNorm, ECRTextureFormat::BC7_UNorm_SRGB, false },
};
}


//---------------------------------------------------------------------------------------------------------------------
/// Get color space option count.
//---------------------------------------------------------------------------------------------------------------------
i32 CRTextureImportSettings::GetColorSpaceOptionCount()
{
    return static_cast< i32 >( sizeof( TextureImportColorSpaceOptions ) / sizeof( TextureImportColorSpaceOptions[ 0 ] ) );
}

//---------------------------------------------------------------------------------------------------------------------
/// Get color space option by index.
//---------------------------------------------------------------------------------------------------------------------
const CRTextureImportColorSpaceOptionDescriptor* CRTextureImportSettings::GetColorSpaceOptionByIndex( i32 Index )
{
    if ( Index < 0 || Index >= GetColorSpaceOptionCount() ) return nullptr;

    return &TextureImportColorSpaceOptions[ Index ];
}

//---------------------------------------------------------------------------------------------------------------------
/// Get compression option count.
//---------------------------------------------------------------------------------------------------------------------
i32 CRTextureImportSettings::GetCompressionOptionCount()
{
    return static_cast< i32 >( sizeof( TextureImportCompressionOptions ) / sizeof( TextureImportCompressionOptions[ 0 ] ) );
}

//---------------------------------------------------------------------------------------------------------------------
/// Get compression option by index.
//---------------------------------------------------------------------------------------------------------------------
const CRTextureImportCompressionOptionDescriptor* CRTextureImportSettings::GetCompressionOptionByIndex( i32 Index )
{
    if ( Index < 0 || Index >= GetCompressionOptionCount() ) return nullptr;

    return &TextureImportCompressionOptions[ Index ];
}

//---------------------------------------------------------------------------------------------------------------------
/// Find compression option by value.
//---------------------------------------------------------------------------------------------------------------------
const CRTextureImportCompressionOptionDescriptor* CRTextureImportSettings::FindCompressionOption( i32 TextureCompression )
{
    const ECRTextureImportCompressionOption option = static_cast< ECRTextureImportCompressionOption >( TextureCompression );

    for ( const CRTextureImportCompressionOptionDescriptor& descriptor : TextureImportCompressionOptions )
    {
        if ( descriptor.Option == option ) return &descriptor;
    }

    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/// Convert import compression option to engine texture format.
//---------------------------------------------------------------------------------------------------------------------
ECRTextureFormat CRTextureImportSettings::GetTextureTargetFormat( i32 TextureCompression, ECRTextureColorSpace TextureColorSpace )
{
    const CRTextureImportCompressionOptionDescriptor* descriptor = FindCompressionOption( TextureCompression );
    if ( !descriptor ) return ECRTextureFormat::Unknown;

    return TextureColorSpace == ECRTextureColorSpace::SRGB ? descriptor->SRGBFormat : descriptor->LinearFormat;
}

//---------------------------------------------------------------------------------------------------------------------
/// Returns true when the requested output format can be produced by the current texture import backend.
//---------------------------------------------------------------------------------------------------------------------
bool CRTextureImportSettings::IsTextureTargetFormatSupported( ECRTextureFormat Format )
{
    return Format == ECRTextureFormat::Unknown;
}

//---------------------------------------------------------------------------------------------------------------------
/// Returns true when the current texture importer backend can produce the selected compression option.
//---------------------------------------------------------------------------------------------------------------------
bool CRTextureImportSettings::IsCompressionOptionSupported( i32 TextureCompression )
{
    const CRTextureImportCompressionOptionDescriptor* descriptor = FindCompressionOption( TextureCompression );

    return descriptor ? descriptor->bSupported : false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get unsupported compression reason.
//---------------------------------------------------------------------------------------------------------------------
const CRString& CRTextureImportSettings::GetCompressionUnsupportedReason( i32 TextureCompression )
{
    const CRTextureImportCompressionOptionDescriptor* descriptor = FindCompressionOption( TextureCompression );

    return descriptor ? descriptor->UnsupportedReason : UnknownTextureCompressionOptionReason;
}

//---------------------------------------------------------------------------------------------------------------------
/// Returns true when texture import can generate mipmaps.
//---------------------------------------------------------------------------------------------------------------------
bool CRTextureImportSettings::IsMipmapGenerationSupported()
{
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get unsupported mipmap generation reason.
//---------------------------------------------------------------------------------------------------------------------
const CRString& CRTextureImportSettings::GetMipmapGenerationUnsupportedReason()
{
    return MipmapGenerationUnsupportedReason;
}
