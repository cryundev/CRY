#include "CRPointLightComponent.h"
#include "Source/RHI/CRRHI.h"
#include "Source/RHI/ICRRHIRenderer.h"


//---------------------------------------------------------------------------------------------------------------------
/// Destroy component.
//---------------------------------------------------------------------------------------------------------------------
void CRPointLightComponent::DestroyComponent()
{
    OnDisabled();
}

//---------------------------------------------------------------------------------------------------------------------
/// Called when component is disabled.
//---------------------------------------------------------------------------------------------------------------------
void CRPointLightComponent::OnDisabled()
{
    _FlushToRenderer();
}

//---------------------------------------------------------------------------------------------------------------------
/// Pre-render component.
//---------------------------------------------------------------------------------------------------------------------
void CRPointLightComponent::PreRenderComponent( float DeltaSeconds )
{
    _FlushToRenderer();
}

//---------------------------------------------------------------------------------------------------------------------
/// Build GPU light data.
//---------------------------------------------------------------------------------------------------------------------
CRPointLightData CRPointLightComponent::BuildLightData() const
{
    CRPointLightData data;
    data.Color = Color;
    data.Range = Range;

    if ( CRTransformComponent* transform = CRTransformComponent::Get( ObjectId ) )
    {
        const CRVector& pos = transform->GetLocation();
        
        data.Position = CRVector4D( pos.x, pos.y, pos.z, 0.0f );
    }

    return data;
}

//---------------------------------------------------------------------------------------------------------------------
/// Flush to renderer.
//---------------------------------------------------------------------------------------------------------------------
void CRPointLightComponent::_FlushToRenderer()
{
    CRArray< CRPointLightData > lights;

    for ( CRPointLightComponent& comp : GetComponents() )
    {
        if ( !comp.IsEnabled() ) continue;
        
        lights.push_back( comp.BuildLightData() );
        
        if ( lights.size() >= CRMaxPointLights ) break;
    }

    if ( ICRRHIRenderer* renderer = GRHI.GetRenderer() )
    {
        renderer->SetPointLights( lights );
    }
}
