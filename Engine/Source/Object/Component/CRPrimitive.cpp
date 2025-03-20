#include "CRPrimitive.h"
#include "Source/Asset/CRPrimitiveAsset.h"
#include "Source/RHI/CRRHI.h"
#include "Source/RHI/ICRRHIMesh.h"
#include "Source/RHI/ICRRHIRenderer.h"


//---------------------------------------------------------------------------------------------------------------------
/// Update component.
//---------------------------------------------------------------------------------------------------------------------
void CRPrimitive::UpdateComponent( float DeltaSeconds )
{
    if ( RHI.expired() ) return;
    
    if ( bPrevRender )
    {
        if ( !bRender )
        {
            GRHI.GetRenderer()->RemoveRenderMesh( RHI );
        }
    }
    else
    {
        if ( bRender )
        {
            GRHI.GetRenderer()->AddRenderMesh( RHI );
        }
    }

    bPrevRender = bRender;
}

//---------------------------------------------------------------------------------------------------------------------
/// Load asset.
//---------------------------------------------------------------------------------------------------------------------
void CRPrimitive::LoadAsset( const CRString& InAssetPath )
{
    AssetPath = InAssetPath;
    
    CRPrimitiveAsset asset;
    asset.Load( AssetPath );

    RHI = GRHI.CreateMesh();
    if ( RHI.expired() ) return;
    
    RHI.lock()->InitializePrimitive( ObjectName, asset );
    RHI.lock()->InitializeMaterial();
}
