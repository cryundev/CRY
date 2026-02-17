#pragma once


#include "CRComponent.h"
#include "Source/RHI/CRRenderElement.h"
#include "Source/RHI/CRRHITypes.h"
#include "Source/RHI/ICRRHIMaterial.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRPrimitiveComponent
//---------------------------------------------------------------------------------------------------------------------
class CRPrimitiveComponent : public CRComponent< CRPrimitiveComponent >
{
private:
    CRString              AssetPath;
    
    bool                  bPrevRender = false;
    bool                  bRender     = true;
    
    ICRRHIMeshWPtr        Mesh;
    ICRRHIMaterialWPtr    Material;
    
    CRRenderElementHandle RenderElementHandle;

public:
    /// Constructor
    CRPrimitiveComponent() = default;

    /// Destroy component.
    virtual void DestroyComponent() override;

    /// Called when component is disabled.
    virtual void OnDisabled() override;

    /// Update component.
    virtual void UpdateComponent( float DeltaSeconds ) override;

    /// Load asset.
    void LoadAsset( const CRString& InAssetPath );

    /// Get material.
    ICRRHIMaterialSPtr GetMaterial() const;

private:
    /// Remove this component's render element from renderer.
    void _UnregisterRenderElement();
};
