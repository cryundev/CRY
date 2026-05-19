#include "CRAssetImportService.h"
#include "CRPrimitiveAssetImporter.h"
#include "CRTextureImporter.h"


namespace
{
//---------------------------------------------------------------------------------------------------------------------
/// Make a failed import result.
//---------------------------------------------------------------------------------------------------------------------
CRAssetImportResult _MakeFailedResult( const CRAssetImportRequest& Request, const CRString& Message )
{
    CRAssetImportResult result;
    result.SourcePath      = Request.SourcePath;
    result.DestinationPath = Request.DestinationPath;
    result.Message         = Message;

    return result;
}
}


//---------------------------------------------------------------------------------------------------------------------
/// Import source asset into the destination path.
//---------------------------------------------------------------------------------------------------------------------
CRAssetImportResult CRAssetImportService::Import( const CRAssetImportRequest& Request ) const
{
    if ( Request.SourcePath.empty() )
    {
        return _MakeFailedResult( Request, "Asset import source path is empty." );
    }

    if ( Request.DestinationPath.empty() )
    {
        return _MakeFailedResult( Request, "Asset import destination path is empty." );
    }

    CRPrimitiveAssetImporter primitiveImporter;
    if ( primitiveImporter.CanImport( Request ) )
    {
        return primitiveImporter.Import( Request );
    }

    CRTextureImporter textureImporter;
    if ( textureImporter.CanImport( Request ) )
    {
        return textureImporter.Import( Request );
    }

    return _MakeFailedResult( Request, "No asset importer can handle this import request." );
}
