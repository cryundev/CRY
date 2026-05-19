#pragma once

#include "ICRAsset.h"
#include "Source/Core/CRSmartPtrMacro.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRAssetLoader
//---------------------------------------------------------------------------------------------------------------------
class CRAssetLoader
{
public:
    /// Read asset type from a casset file header.
    static ECRAssetType ReadAssetType( const CRPath& Path );

    /// Load an asset by file type metadata.
    static CRSharedPtr< ICRAsset > Load( const CRPath& Path );
};
