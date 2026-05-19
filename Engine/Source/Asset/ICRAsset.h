#pragma once

#include "CRAssetTypes.h"
#include "Source/Core/CRTypes.h"
#include "Source/Core/Strings/CRStringInc.h"


//---------------------------------------------------------------------------------------------------------------------
/// ICRAsset
//---------------------------------------------------------------------------------------------------------------------
class ICRAsset
{
public:
    /// Constructor.
    ICRAsset() = default;

    /// Destructor.
    virtual ~ICRAsset() = default;

    /// Get asset type.
    virtual ECRAssetType GetAssetType() const = 0;

    /// Save.
    virtual void Save( const CRPath& Path ) = 0;

    /// Load.
    virtual void Load( const CRPath& Path ) = 0;
};
