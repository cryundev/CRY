#include "CRDirectionalLightComponent.h"
#include "Source/RHI/CRRHI.h"
#include "Source/RHI/ICRRHIRenderer.h"


//---------------------------------------------------------------------------------------------------------------------
/// Destroy component.
//---------------------------------------------------------------------------------------------------------------------
void CRDirectionalLightComponent::DestroyComponent()
{
    OnDisabled();
}

//---------------------------------------------------------------------------------------------------------------------
/// Called when component is disabled.
//---------------------------------------------------------------------------------------------------------------------
void CRDirectionalLightComponent::OnDisabled()
{
    _FlushToRenderer();
}

//---------------------------------------------------------------------------------------------------------------------
/// Update component.
//---------------------------------------------------------------------------------------------------------------------
void CRDirectionalLightComponent::UpdateComponent( float DeltaSeconds )
{
    if ( !bUseTransformForward ) return;

    if ( CRTransformComponent* transform = CRTransformComponent::Get( ObjectId ) )
    {
        const CRVector& fwd = transform->GetForward();
        
        Direction = CRVector4D( fwd.x, fwd.y, fwd.z, 0.0f );
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Pre-render component.
//---------------------------------------------------------------------------------------------------------------------
void CRDirectionalLightComponent::PreRenderComponent( float DeltaSeconds )
{
    _FlushToRenderer();
}

//---------------------------------------------------------------------------------------------------------------------
/// Build GPU light data.
//---------------------------------------------------------------------------------------------------------------------
CRDirectionalLightData CRDirectionalLightComponent::BuildLightData() const
{
    CRDirectionalLightData data;
    data.Direction = Direction;
    data.Color     = Color;
    
    return data;
}

//---------------------------------------------------------------------------------------------------------------------
/// Flush to renderer.
//---------------------------------------------------------------------------------------------------------------------
void CRDirectionalLightComponent::_FlushToRenderer()
{
    CRArray< CRDirectionalLightData > lights;

    for ( CRDirectionalLightComponent& comp : GetComponents() )
    {
        if ( !comp.IsEnabled() ) continue;
        
        lights.push_back( comp.BuildLightData() );
        
        if ( lights.size() >= CRMaxDirectionalLights ) break;
    }

    ;
    if ( ICRRHIRenderer* renderer = GRHI.GetRenderer() )
    {
        renderer->SetDirectionalLights( lights );    
    }
}
