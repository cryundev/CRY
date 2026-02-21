#include "CRSpotLightComponent.h"
#include <cmath>
#include "Source/RHI/CRRHI.h"
#include "Source/RHI/ICRRHIRenderer.h"


//---------------------------------------------------------------------------------------------------------------------
/// Destroy component.
//---------------------------------------------------------------------------------------------------------------------
void CRSpotLightComponent::DestroyComponent()
{
    OnDisabled();
}

//---------------------------------------------------------------------------------------------------------------------
/// Called when component is disabled.
//---------------------------------------------------------------------------------------------------------------------
void CRSpotLightComponent::OnDisabled()
{
    _FlushToRenderer();
}

//---------------------------------------------------------------------------------------------------------------------
/// Pre-render component.
//---------------------------------------------------------------------------------------------------------------------
void CRSpotLightComponent::PreRenderComponent( float DeltaSeconds )
{
    _FlushToRenderer();
}

//---------------------------------------------------------------------------------------------------------------------
/// Build GPU light data.
//---------------------------------------------------------------------------------------------------------------------
CRSpotLightData CRSpotLightComponent::BuildLightData() const
{
    CRSpotLightData data;
    data.Color        = Color;
    data.Range        = Range;
    data.SpotInnerCos = cosf( DirectX::XMConvertToRadians( InnerAngle ) );
    data.SpotOuterCos = cosf( DirectX::XMConvertToRadians( OuterAngle ) );

    if ( CRTransformComponent* transform = CRTransformComponent::Get( ObjectId ) )
    {
        const CRVector& pos = transform->GetLocation();
        
        data.Position = CRVector4D( pos.x, pos.y, pos.z, 0.0f );

        const CRVector& fwd = transform->GetForward();
        
        data.Direction = CRVector4D( fwd.x, fwd.y, fwd.z, 0.0f );
    }

    return data;
}

//---------------------------------------------------------------------------------------------------------------------
/// Flush to renderer.
//---------------------------------------------------------------------------------------------------------------------
void CRSpotLightComponent::_FlushToRenderer()
{
    CRArray< CRSpotLightData > lights;

    for ( CRSpotLightComponent& comp : GetComponents() )
    {
        if ( !comp.IsEnabled() ) continue;
        
        lights.push_back( comp.BuildLightData() );
        
        if ( lights.size() >= CRMaxSpotLights ) break;
    }

    if ( ICRRHIRenderer* renderer = GRHI.GetRenderer() )
    {
        renderer->SetSpotLights( lights );
    }
}
