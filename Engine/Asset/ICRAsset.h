#pragma once


#include "Core/Strings/CRStringInc.h"


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
    virtual void Save( const CRString& Path ) {}

    /// Load.
    virtual void Load( const CRString& Path ) {}
};
