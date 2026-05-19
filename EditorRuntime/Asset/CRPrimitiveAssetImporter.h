#pragma once


#include "CRAssetImporter.h"
#include "Source/Asset/CRPrimitiveAsset.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRPrimitiveAssetImporter
//---------------------------------------------------------------------------------------------------------------------
class CRPrimitiveAssetImporter : public CRAssetImporter
{
public:
    /// Returns true when this importer can handle the request.
    virtual bool CanImport( const CRAssetImportRequest& Request ) const override;

    /// Import source primitive into the destination path.
    virtual CRAssetImportResult Import( const CRAssetImportRequest& Request ) const override;

    /// Import a source primitive into a raw primitive asset.
    static bool ImportFromFile( const CRPath& SourcePath, CRPrimitiveAsset& OutAsset );

    /// Import a source primitive and save it as a primitive casset.
    static bool ImportToCasset( const CRPath& SourcePath, const CRPath& DestinationPath );
};
