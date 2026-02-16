#include "CRPrimitiveComponent.h"
#include "Source/Asset/CRPrimitiveAsset.h"
#include "Source/RHI/CRRHI.h"
#include "Source/RHI/ICRRHIMaterial.h"
#include "Source/RHI/ICRRHIMesh.h"
#include "Source/RHI/ICRRHIRenderer.h"


//---------------------------------------------------------------------------------------------------------------------
/// Destroy component.
//---------------------------------------------------------------------------------------------------------------------
void CRPrimitiveComponent::DestroyComponent()
{
    _UnregisterRenderElement();

    Mesh.reset();
    Material.reset();

    bPrevRender = false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Called when component is disabled.
//---------------------------------------------------------------------------------------------------------------------
void CRPrimitiveComponent::OnDisabled()
{
    _UnregisterRenderElement();

    bPrevRender = false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Update component.
//---------------------------------------------------------------------------------------------------------------------
void CRPrimitiveComponent::UpdateComponent( float DeltaSeconds )
{
    if ( Mesh.expired() || Material.expired() )
    {
        _UnregisterRenderElement();

        bPrevRender = false;
        
        return;
    }
    
    if ( bPrevRender )
    {
        if ( !bRender )
        {
            _UnregisterRenderElement();
        }
    }
    else
    {
        if ( bRender )
        {
            if ( !RenderElementHandle.IsValid() )
            {
                CRRenderElement renderElement;
                renderElement.Mesh     = Mesh;
                renderElement.Material = Material;
                
                RenderElementHandle = GRHI.GetRenderer()->AddRenderElement( renderElement );
            }
        }
    }

    bPrevRender = bRender && RenderElementHandle.IsValid();
}

//---------------------------------------------------------------------------------------------------------------------
/// Load asset.
//---------------------------------------------------------------------------------------------------------------------
void CRPrimitiveComponent::LoadAsset( const CRString& InAssetPath )
{
    // P0: Always unregister previous render element before mesh/material replacement.
    _UnregisterRenderElement();

    Mesh.reset();
    Material.reset();

    bPrevRender = false;

    AssetPath = InAssetPath;
    
    CRPrimitiveAsset asset;
    asset.Load( AssetPath );

    Mesh = GRHI.CreateMesh();
    if ( Mesh.expired() ) return;

    Mesh.lock()->InitializePrimitive( ObjectName, asset );

    Material = GRHI.CreateMaterial();
    if ( Material.expired() ) return;

    Material.lock()->Initialize( ObjectName );

    if ( bRender )
    {
        CRRenderElement renderElement;
        renderElement.Mesh     = Mesh;
        renderElement.Material = Material;
        
        RenderElementHandle = GRHI.GetRenderer()->AddRenderElement( renderElement );
    }

    bPrevRender = bRender && RenderElementHandle.IsValid();
}

//---------------------------------------------------------------------------------------------------------------------
/// Unregister render element.
//---------------------------------------------------------------------------------------------------------------------
void CRPrimitiveComponent::_UnregisterRenderElement()
{
    if ( !RenderElementHandle.IsValid() ) return;

    GRHI.GetRenderer()->RemoveRenderElement( RenderElementHandle );
    RenderElementHandle = {};
}
