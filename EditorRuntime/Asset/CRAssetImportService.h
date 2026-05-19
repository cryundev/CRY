#pragma once


#include "CRAssetImporter.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRAssetImportService
//---------------------------------------------------------------------------------------------------------------------
class CRAssetImportService
{
public:
    /// Import source asset into the destination path.
    CRAssetImportResult Import( const CRAssetImportRequest& Request ) const;
};
