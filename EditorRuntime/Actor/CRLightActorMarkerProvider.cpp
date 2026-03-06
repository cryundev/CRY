#include "CRLightActorMarkerProvider.h"
#include "Source/Object/CRActor.h"
#include "Source/Object/Component/CRDirectionalLightComponent.h"
#include "Source/Object/Component/CRPointLightComponent.h"
#include "Source/Object/Component/CRSpotLightComponent.h"
#include "Source/Object/Component/CRTransformComponent.h"
#include "Source/World/CRWorld.h"
#include <algorithm>


namespace
{
//---------------------------------------------------------------------------------------------------------------------
/// Return true when actor has enabled component type.
//---------------------------------------------------------------------------------------------------------------------
template< typename TComponent >
bool _HasEnabledComponentTyped( const CRActor& Actor )
{
    if ( const TComponent* component = Actor.GetComponent< TComponent >() )
    {
        return component->IsEnabled();
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Return true when actor has the enabled light component kind.
//---------------------------------------------------------------------------------------------------------------------
bool _HasEnabledComponent( const CRActor& Actor, ECRLightMarkerComponentKind ComponentKind )
{
    switch ( ComponentKind )
    {
        case ECRLightMarkerComponentKind::Directional: return _HasEnabledComponentTyped< CRDirectionalLightComponent >( Actor );
        case ECRLightMarkerComponentKind::Spot:        return _HasEnabledComponentTyped< CRSpotLightComponent        >( Actor );
        case ECRLightMarkerComponentKind::Point:       return _HasEnabledComponentTyped< CRPointLightComponent       >( Actor );
    }

    return false;
}
}

//---------------------------------------------------------------------------------------------------------------------
/// Initialize marker rules.
//---------------------------------------------------------------------------------------------------------------------
bool CRLightActorMarkerProvider::InitializeRules( const CRArray< CRLightMarkerRule >& InRules )
{
    Rules.clear();

    if ( InRules.empty() ) return false;

    Rules = InRules;
    std::sort( Rules.begin(), Rules.end(), [] ( const CRLightMarkerRule& A, const CRLightMarkerRule& B )
    {
        return A.Priority > B.Priority;
    } );

    return true;
}


//---------------------------------------------------------------------------------------------------------------------
/// Collect actor markers from world.
//---------------------------------------------------------------------------------------------------------------------
void CRLightActorMarkerProvider::CollectMarkers( const CRWorld& World, CRArray< CRActorMarkerDescriptor >& OutMarkers ) const
{
    if ( Rules.empty() ) return;

    for ( CRActor* actor : World.GetAllActors() )
    {
        if ( !actor || !actor->IsValid() ) continue;

        CRTransformComponent* transform = actor->GetTransform();
        if ( !transform ) continue;

        const CRLightMarkerRule* selectedRule = nullptr;
        for ( const CRLightMarkerRule& rule : Rules )
        {
            if ( !_HasEnabledComponent( *actor, rule.ComponentKind ) ) continue;

            selectedRule = &rule;
            break;
        }

        if ( !selectedRule ) continue;

        CRActorMarkerDescriptor marker;
        marker.ActorId          = (CRIdentity::id_t)actor->GetObjectId();
        marker.MarkerType       = selectedRule->MarkerType;
        marker.Position         = transform->GetLocation();
        marker.Rotation         = selectedRule->bUseActorRotation ? transform->GetRotation() : CRQuaternion::Identity;
        marker.Color            = selectedRule->Color;
        marker.PixelSize        = selectedRule->PixelSize;
        marker.PickRadiusPixels = selectedRule->PickRadiusPixels;
        marker.Priority         = selectedRule->Priority;
        marker.VisualKind       = ECRActorMarkerVisualKind::Mesh3D;

        OutMarkers.push_back( marker );
    }
}
