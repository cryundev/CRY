#include "CRTextureImporter.h"
#include "CRAssetImportFormats.h"
#include "CRTextureImportSettings.h"
#include "Source/Utility/WIC/CRWICTextureLoader.h"
#include <filesystem>


namespace
{
//---------------------------------------------------------------------------------------------------------------------
/// Get texture options or defaults when a generic request is routed to the texture importer.
//---------------------------------------------------------------------------------------------------------------------
const CRTextureImportOptions& _GetTextureImportOptions( const CRAssetImportRequest& Request )
{
    static const CRTextureImportOptions DefaultOptions;

    const CRTextureImportRequest* textureRequest = dynamic_cast< const CRTextureImportRequest* >( &Request );
    return textureRequest ? textureRequest->TextureOptions : DefaultOptions;
}
}


//---------------------------------------------------------------------------------------------------------------------
/// Returns true when this importer can handle the request.
//---------------------------------------------------------------------------------------------------------------------
bool CRTextureImporter::CanImport( const CRAssetImportRequest& Request ) const
{
    if ( Request.ImportType == ECRAssetImportType::Texture ) return true;
    if ( Request.ImportType != ECRAssetImportType::Auto    ) return false;

    return CRAssetImportFormats::IsTextureSourceExtension( Request.SourcePath );
}

//---------------------------------------------------------------------------------------------------------------------
/// Import source texture into the destination path.
//---------------------------------------------------------------------------------------------------------------------
CRAssetImportResult CRTextureImporter::Import( const CRAssetImportRequest& Request ) const
{
    CRAssetImportResult result;
    result.AssetType       = ECRAssetType::Texture2D;
    result.SourcePath      = Request.SourcePath;
    result.DestinationPath = Request.DestinationPath;

    if ( Request.SourcePath.empty() )
    {
        result.Message = "Texture import source path is empty.";
        return result;
    }

    if ( Request.DestinationPath.empty() )
    {
        result.Message = "Texture import destination path is empty.";
        return result;
    }

    if ( !CanImport( Request ) )
    {
        result.Message = "Texture importer cannot handle this import request.";
        return result;
    }

    const CRTextureImportOptions& textureOptions = _GetTextureImportOptions( Request );

    if ( !CRTextureImportSettings::IsTextureTargetFormatSupported( textureOptions.TargetFormat ) )
    {
        result.Message = "Selected texture compression format is not supported by the current importer backend.";
        return result;
    }

    if ( textureOptions.bGenerateMipmaps && !CRTextureImportSettings::IsMipmapGenerationSupported() )
    {
        result.Message = CRTextureImportSettings::GetMipmapGenerationUnsupportedReason();
        return result;
    }

    const CRPath destinationDirectory = Request.DestinationPath.parent_path();
    if ( !destinationDirectory.empty() )
    {
        std::filesystem::create_directories( destinationDirectory );
    }

    result.bSucceeded = ImportToCasset( Request );
    result.Message    = result.bSucceeded ? "Texture import succeeded." : "Texture import failed.";

    return result;
}

//---------------------------------------------------------------------------------------------------------------------
/// Import a source image into a raw texture asset.
//---------------------------------------------------------------------------------------------------------------------
bool CRTextureImporter::ImportFromFile( const CRAssetImportRequest& Request, CRTextureAsset& OutAsset )
{
    CRWICTextureLoader loader;
    const CRTextureImportOptions& textureOptions = _GetTextureImportOptions( Request );
    const bool bHasRequestedSize = textureOptions.Width > 0 || textureOptions.Height > 0;

    if ( bHasRequestedSize )
    {
        if ( !loader.LoadFromFile( Request.SourcePath, textureOptions.Width, textureOptions.Height ) ) return false;
    }
    else
    {
        if ( !loader.LoadFromFile( Request.SourcePath ) ) return false;
    }

    const ECRTextureFormat format = loader.GetTextureFormat();
    if ( format == ECRTextureFormat::Unknown ) return false;

    OutAsset.Initialize2D
    (
        loader.GetWidth(),
        loader.GetHeight(),
        format,
        loader.GetPixels(),
        loader.GetImageSize(),
        loader.GetRowPitch(),
        loader.GetImageSize(),
        textureOptions.ColorSpace
    );

    return OutAsset.IsValid();
}

//---------------------------------------------------------------------------------------------------------------------
/// Import a source image and save it as a texture casset.
//---------------------------------------------------------------------------------------------------------------------
bool CRTextureImporter::ImportToCasset( const CRAssetImportRequest& Request )
{
    CRTextureAsset asset;
    if ( !ImportFromFile( Request, asset ) ) return false;

    asset.Save( Request.DestinationPath );

    return true;
}
