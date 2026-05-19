#include "EngineDLL.h"
#include "Asset/CRAssetImportFormats.h"
#include "Asset/CRAssetImportService.h"
#include "Asset/CRTextureImportSettings.h"
#include "Asset/CRTextureImporter.h"
#include <cstring>


namespace
{
//---------------------------------------------------------------------------------------------------------------------
/// Copy import message to caller-owned buffer.
//---------------------------------------------------------------------------------------------------------------------
void WriteOutputMessage( const CRString& Message, char* OutMessage, i32 MessageCapacity )
{
    if ( !OutMessage || MessageCapacity <= 0 ) return;

    const size_t messageCapacity = static_cast< size_t >( MessageCapacity );
    const size_t writableSize    = messageCapacity - 1;
    const size_t messageSize     = Message.size() < writableSize ? Message.size() : writableSize;

    if ( messageSize > 0 )
    {
        std::memcpy( OutMessage, Message.data(), messageSize );
    }

    OutMessage[ messageSize ] = '\0';
}

//---------------------------------------------------------------------------------------------------------------------
/// Fill request fields shared by every asset import type.
//---------------------------------------------------------------------------------------------------------------------
void InitializeAssetImportRequest( CRAssetImportRequest& Request, const char* SourcePath, const char* DestinationPath, ECRAssetImportType ImportType )
{
    Request.SourcePath      = SourcePath      ? CRPath( SourcePath      ) : CRPath();
    Request.DestinationPath = DestinationPath ? CRPath( DestinationPath ) : CRPath();
    Request.ImportType      = ImportType;
}
}


//---------------------------------------------------------------------------------------------------------------------
/// Import asset.
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API bool ImportAsset( const char* SourcePath, const char* DestinationPath, i32 ImportType, i32 TextureColorSpace, i32 TextureCompression, i32 TextureWidth, i32 TextureHeight, bool GenerateMipmaps, char* OutMessage, i32 MessageCapacity )
{
    const CRAssetImportService importService;
    const ECRAssetImportType   importType = static_cast< ECRAssetImportType >( ImportType );
    CRAssetImportResult        result;

    if ( importType == ECRAssetImportType::Primitive )
    {
        CRAssetImportRequest request;
        InitializeAssetImportRequest( request, SourcePath, DestinationPath, importType );

        result = importService.Import( request );
    }
    else
    {
        CRTextureImportRequest request;
        InitializeAssetImportRequest( request, SourcePath, DestinationPath, importType );

        request.TextureOptions.ColorSpace       = static_cast< ECRTextureColorSpace >( TextureColorSpace );
        request.TextureOptions.TargetFormat     = CRTextureImportSettings::GetTextureTargetFormat( TextureCompression, request.TextureOptions.ColorSpace );
        request.TextureOptions.Width            = TextureWidth  > 0 ? static_cast< u32 >( TextureWidth  ) : 0;
        request.TextureOptions.Height           = TextureHeight > 0 ? static_cast< u32 >( TextureHeight ) : 0;
        request.TextureOptions.bGenerateMipmaps = GenerateMipmaps;

        result = importService.Import( request );
    }

    WriteOutputMessage( result.Message, OutMessage, MessageCapacity );

    return result.bSucceeded;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get asset import type for a source path.
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API i32 GetAssetImportType( const char* SourcePath )
{
    return static_cast< i32 >( CRAssetImportFormats::GetImportType( SourcePath ? CRPath( SourcePath ) : CRPath() ) );
}

//---------------------------------------------------------------------------------------------------------------------
/// Get asset import open-file dialog filter.
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API const char* GetAssetImportOpenFileFilter()
{
    return CRAssetImportFormats::GetOpenFileFilter().c_str();
}

//---------------------------------------------------------------------------------------------------------------------
/// Get asset import save-file dialog filter.
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API const char* GetAssetImportSaveFileFilter()
{
    return CRAssetImportFormats::GetSaveFileFilter().c_str();
}

//---------------------------------------------------------------------------------------------------------------------
/// Get asset file extension produced by importers.
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API const char* GetAssetImportCassetExtension()
{
    return CRAssetImportFormats::CassetExtension;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get texture import color space option count.
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API i32 GetTextureImportColorSpaceOptionCount()
{
    return CRTextureImportSettings::GetColorSpaceOptionCount();
}

//---------------------------------------------------------------------------------------------------------------------
/// Get texture import color space option value by index.
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API i32 GetTextureImportColorSpaceOptionValue( i32 Index )
{
    const CRTextureImportColorSpaceOptionDescriptor* descriptor = CRTextureImportSettings::GetColorSpaceOptionByIndex( Index );

    return descriptor ? static_cast< i32 >( descriptor->ColorSpace ) : 0;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get texture import color space option display name by index.
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API const char* GetTextureImportColorSpaceOptionName( i32 Index )
{
    const CRTextureImportColorSpaceOptionDescriptor* descriptor = CRTextureImportSettings::GetColorSpaceOptionByIndex( Index );

    return descriptor ? descriptor->DisplayName.c_str() : "";
}

//---------------------------------------------------------------------------------------------------------------------
/// Get texture import compression option count.
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API i32 GetTextureImportCompressionOptionCount()
{
    return CRTextureImportSettings::GetCompressionOptionCount();
}

//---------------------------------------------------------------------------------------------------------------------
/// Get texture import compression option value by index.
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API i32 GetTextureImportCompressionOptionValue( i32 Index )
{
    const CRTextureImportCompressionOptionDescriptor* descriptor = CRTextureImportSettings::GetCompressionOptionByIndex( Index );

    return descriptor ? static_cast< i32 >( descriptor->Option ) : 0;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get texture import compression option display name by index.
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API const char* GetTextureImportCompressionOptionName( i32 Index )
{
    const CRTextureImportCompressionOptionDescriptor* descriptor = CRTextureImportSettings::GetCompressionOptionByIndex( Index );

    return descriptor ? descriptor->DisplayName.c_str() : "";
}

//---------------------------------------------------------------------------------------------------------------------
/// Returns true when the current texture importer backend can produce the selected compression option.
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API bool IsTextureImportCompressionOptionSupported( i32 TextureCompression )
{
    return CRTextureImportSettings::IsCompressionOptionSupported( TextureCompression );
}

//---------------------------------------------------------------------------------------------------------------------
/// Get unsupported compression option reason.
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API const char* GetTextureImportCompressionOptionUnsupportedReason( i32 TextureCompression )
{
    return CRTextureImportSettings::GetCompressionUnsupportedReason( TextureCompression ).c_str();
}

//---------------------------------------------------------------------------------------------------------------------
/// Returns true when texture import can generate mipmaps.
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API bool IsTextureImportMipmapGenerationSupported()
{
    return CRTextureImportSettings::IsMipmapGenerationSupported();
}

//---------------------------------------------------------------------------------------------------------------------
/// Get unsupported mipmap generation reason.
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API const char* GetTextureImportMipmapGenerationUnsupportedReason()
{
    return CRTextureImportSettings::GetMipmapGenerationUnsupportedReason().c_str();
}

//---------------------------------------------------------------------------------------------------------------------
/// Get minimum selectable texture import size.
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API i32 GetTextureImportMinSize()
{
    return static_cast< i32 >( CRTextureImportSettings::MinImportSize );
}

//---------------------------------------------------------------------------------------------------------------------
/// Get maximum selectable texture import size.
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API i32 GetTextureImportMaxSize()
{
    return static_cast< i32 >( CRTextureImportSettings::MaxImportSize );
}
