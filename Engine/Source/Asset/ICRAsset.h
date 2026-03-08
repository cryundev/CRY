#pragma once

#include "Source/Core/CRTypes.h"
#include "Source/Core/Strings/CRStringInc.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRAsset
//---------------------------------------------------------------------------------------------------------------------
class ICRAsset
{
public:
    /// Constructor.
    ICRAsset() = default;

    /// Destructor.
    virtual ~ICRAsset() = 0 {}

    /// Save.
    virtual void Save( const CRPath& Path ) {}

    /// Load.
    virtual void Load( const CRPath& Path ) {}
};
