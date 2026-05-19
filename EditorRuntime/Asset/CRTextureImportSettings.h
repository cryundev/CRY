#pragma once


#include "Source/Asset/CRTextureAsset.h"
#include "Source/Core/Strings/CRStringInc.h"


//---------------------------------------------------------------------------------------------------------------------
/// ECRTextureImportCompressionOption
//---------------------------------------------------------------------------------------------------------------------
enum class ECRTextureImportCompressionOption : i32
{
    Auto         = 0,
    Uncompressed = 1,
    BC1          = 2,
    BC3          = 3,
    BC5          = 4,
    BC7          = 5,
};

//---------------------------------------------------------------------------------------------------------------------
/// CRTextureImportColorSpaceOptionDescriptor
//---------------------------------------------------------------------------------------------------------------------
struct CRTextureImportColorSpaceOptionDescriptor
{
    CRString             DisplayName;
    ECRTextureColorSpace ColorSpace;
};

//---------------------------------------------------------------------------------------------------------------------
/// CRTextureImportCompressionOptionDescriptor
//---------------------------------------------------------------------------------------------------------------------
struct CRTextureImportCompressionOptionDescriptor
{
    CRString                          DisplayName;
    CRString                          UnsupportedReason;
    ECRTextureImportCompressionOption Option;
    ECRTextureFormat                  LinearFormat;
    ECRTextureFormat                  SRGBFormat;
    bool                              bSupported;
};

//---------------------------------------------------------------------------------------------------------------------
/// CRTextureImportSettings
//---------------------------------------------------------------------------------------------------------------------
namespace CRTextureImportSettings
{
    constexpr u32 MinImportSize = 1;
    constexpr u32 MaxImportSize = 8192;

    i32 GetColorSpaceOptionCount();
    const CRTextureImportColorSpaceOptionDescriptor* GetColorSpaceOptionByIndex( i32 Index );

    i32 GetCompressionOptionCount();
    const CRTextureImportCompressionOptionDescriptor* GetCompressionOptionByIndex( i32 Index );
    const CRTextureImportCompressionOptionDescriptor* FindCompressionOption( i32 TextureCompression );

    ECRTextureFormat GetTextureTargetFormat( i32 TextureCompression, ECRTextureColorSpace TextureColorSpace );
    bool IsTextureTargetFormatSupported( ECRTextureFormat Format );
    bool IsCompressionOptionSupported( i32 TextureCompression );
    const CRString& GetCompressionUnsupportedReason( i32 TextureCompression );

    bool IsMipmapGenerationSupported();
    const CRString& GetMipmapGenerationUnsupportedReason();
}
