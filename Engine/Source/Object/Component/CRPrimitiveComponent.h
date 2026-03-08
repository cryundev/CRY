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
public:
    static constexpr ECRComponentPriority Priority    = ECRComponentPriority::Primitive; /// Component priority.
    static constexpr bool                 IsExclusive = false;                           /// Allows multiple primitive components per actor.

private:
    CRPath                AssetPath;
    
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
    void LoadAsset( const CRPath& InAssetPath );

    /// Set render enabled.
    void SetRenderEnabled( bool bInRender );

    /// Is render enabled.
    bool IsRenderEnabled() const { return bRender; }

    /// Get mesh.
    ICRRHIMeshSPtr GetMesh() const { return Mesh.lock(); }

    /// Get material.
    ICRRHIMaterialSPtr GetMaterial() const;

private:
    /// Remove this component's render element from renderer.
    void _UnregisterRenderElement();
};
