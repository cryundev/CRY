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
    if ( RenderElementHandle.IsValid() )
    {
        GRHI.GetRenderer()->RemoveRenderElement( RenderElementHandle );
        RenderElementHandle = {};
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Called when component is disabled.
//---------------------------------------------------------------------------------------------------------------------
void CRPrimitiveComponent::OnDisabled()
{
    if ( RenderElementHandle.IsValid() )
    {
        GRHI.GetRenderer()->RemoveRenderElement( RenderElementHandle );
        RenderElementHandle = {};
    }

    bPrevRender = false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Update component.
//---------------------------------------------------------------------------------------------------------------------
void CRPrimitiveComponent::UpdateComponent( float DeltaSeconds )
{
    if ( Mesh.expired() || Material.expired() )
    {
        if ( RenderElementHandle.IsValid() )
        {
            GRHI.GetRenderer()->RemoveRenderElement( RenderElementHandle );
            RenderElementHandle = {};
        }

        bPrevRender = false;
        
        return;
    }
    
    if ( bPrevRender )
    {
        if ( !bRender )
        {
            GRHI.GetRenderer()->RemoveRenderElement( RenderElementHandle );
            RenderElementHandle = {};
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
    if ( RenderElementHandle.IsValid() )
    {
        GRHI.GetRenderer()->RemoveRenderElement( RenderElementHandle );
        RenderElementHandle = {};
    }

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
