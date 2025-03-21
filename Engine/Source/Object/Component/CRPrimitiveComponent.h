#pragma once


#include "CRComponent.h"
#include "Source/RHI/CRRHITypes.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRPrimitiveComponent
//---------------------------------------------------------------------------------------------------------------------
class CRPrimitiveComponent : public CRComponent< CRPrimitiveComponent >
{
private:
    CRString       AssetPath;
    bool           bPrevRender = false;
    bool           bRender     = true;
    ICRRHIMeshWPtr RHI;

public:
    /// Constructor
    CRPrimitiveComponent() = default;
    
    /// Update component.
    virtual void UpdateComponent( float DeltaSeconds ) override;

    /// Load asset.
    void LoadAsset( const CRString& InAssetPath );
};
