#include "CRPrimitiveAssetImporter.h"
#include "CRAssetImportFormats.h"
#include "Source/Utility/FBX/CRFbxLoader.h"
#include <filesystem>


//---------------------------------------------------------------------------------------------------------------------
/// Returns true when this importer can handle the request.
//---------------------------------------------------------------------------------------------------------------------
bool CRPrimitiveAssetImporter::CanImport( const CRAssetImportRequest& Request ) const
{
    if ( Request.ImportType == ECRAssetImportType::Primitive ) return true;
    if ( Request.ImportType != ECRAssetImportType::Auto ) return false;

    return CRAssetImportFormats::IsPrimitiveSourceExtension( Request.SourcePath );
}

//---------------------------------------------------------------------------------------------------------------------
/// Import source primitive into the destination path.
//---------------------------------------------------------------------------------------------------------------------
CRAssetImportResult CRPrimitiveAssetImporter::Import( const CRAssetImportRequest& Request ) const
{
    CRAssetImportResult result;
    result.AssetType       = ECRAssetType::Primitive;
    result.SourcePath      = Request.SourcePath;
    result.DestinationPath = Request.DestinationPath;

    if ( Request.SourcePath.empty() )
    {
        result.Message = "Primitive import source path is empty.";
        return result;
    }

    if ( Request.DestinationPath.empty() )
    {
        result.Message = "Primitive import destination path is empty.";
        return result;
    }

    if ( !CanImport( Request ) )
    {
        result.Message = "Primitive importer cannot handle this import request.";
        return result;
    }

    const CRPath destinationDirectory = Request.DestinationPath.parent_path();
    if ( !destinationDirectory.empty() )
    {
        std::filesystem::create_directories( destinationDirectory );
    }

    result.bSucceeded = ImportToCasset( Request.SourcePath, Request.DestinationPath );
    result.Message    = result.bSucceeded ? "Primitive import succeeded." : "Primitive import failed.";

    return result;
}

//---------------------------------------------------------------------------------------------------------------------
/// Import a source primitive into a raw primitive asset.
//---------------------------------------------------------------------------------------------------------------------
bool CRPrimitiveAssetImporter::ImportFromFile( const CRPath& SourcePath, CRPrimitiveAsset& OutAsset )
{
    CRFbxLoader loader;
    if ( !loader.Load( SourcePath ) ) return false;

    CRArray< CRPrimitiveAsset >& primitives = loader.GetPrimitives();
    if ( primitives.empty() ) return false;

    const CRPrimitiveAsset& asset = primitives.front();
    if ( asset.VertexCount == 0 || asset.Positions.empty() ) return false;

    OutAsset = asset;
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Import a source primitive and save it as a primitive casset.
//---------------------------------------------------------------------------------------------------------------------
bool CRPrimitiveAssetImporter::ImportToCasset( const CRPath& SourcePath, const CRPath& DestinationPath )
{
    CRPrimitiveAsset asset;
    if ( !ImportFromFile( SourcePath, asset ) ) return false;

    asset.Save( DestinationPath );

    return true;
}
