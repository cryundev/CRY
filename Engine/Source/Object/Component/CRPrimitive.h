#pragma once


#include "CRComponent.h"
#include "Source/RHI/CRRHITypes.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRPrimitive
//---------------------------------------------------------------------------------------------------------------------
class CRPrimitive : public CRComponent< CRPrimitive >
{
private:
    CRString       AssetPath;
    bool           bPrevRender = false;
    bool           bRender     = true;
    ICRRHIMeshWPtr RHI;

public:
    /// Constructor
    CRPrimitive() = default;
    
    /// Update component.
    virtual void UpdateComponent( float DeltaSeconds ) override;

    /// Load asset.
    void LoadAsset( const CRString& InAssetPath );
};
