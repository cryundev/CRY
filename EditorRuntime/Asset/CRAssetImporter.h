#pragma once


#include "Source/Asset/CRAssetTypes.h"
#include "Source/Core/CRTypes.h"
#include "Source/Core/Strings/CRStringInc.h"


//---------------------------------------------------------------------------------------------------------------------
/// ECRAssetImportType
//---------------------------------------------------------------------------------------------------------------------
enum class ECRAssetImportType : u32
{
    Auto      = 0,
    Texture   = 1,
    Primitive = 2,
};

//---------------------------------------------------------------------------------------------------------------------
/// CRAssetImportRequest
//---------------------------------------------------------------------------------------------------------------------
struct CRAssetImportRequest
{
    virtual ~CRAssetImportRequest() = default;

    CRPath SourcePath;
    CRPath DestinationPath;

    ECRAssetImportType ImportType = ECRAssetImportType::Auto;
};

//---------------------------------------------------------------------------------------------------------------------
/// CRAssetImportResult
//---------------------------------------------------------------------------------------------------------------------
struct CRAssetImportResult
{
    bool         bSucceeded = false;
    ECRAssetType AssetType  = ECRAssetType::Unknown;
    CRPath       SourcePath;
    CRPath       DestinationPath;
    CRString     Message;
};

//---------------------------------------------------------------------------------------------------------------------
/// CRAssetImporter
//---------------------------------------------------------------------------------------------------------------------
class CRAssetImporter
{
public:
    /// Constructor.
    CRAssetImporter() = default;

    /// Destructor.
    virtual ~CRAssetImporter() = default;

    /// Returns true when this importer can handle the request.
    virtual bool CanImport( const CRAssetImportRequest& Request ) const = 0;

    /// Import source asset into the destination path.
    virtual CRAssetImportResult Import( const CRAssetImportRequest& Request ) const = 0;
};
