#include "CRGizmoActor.h"
#include "Source/Object/Component/CRTransformComponent.h"


//---------------------------------------------------------------------------------------------------------------------
/// Initialize gizmo actor.
//---------------------------------------------------------------------------------------------------------------------
bool CRGizmoActor::InitializeGizmo( const CRString& AssetPath, const CRPrimitiveAsset& PrimitiveAsset )
{
    (void)AssetPath;
    (void)PrimitiveAsset;

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get render element count.
//---------------------------------------------------------------------------------------------------------------------
u32 CRGizmoActor::GetRenderElementCount() const
{
    return 0;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get render element.
//---------------------------------------------------------------------------------------------------------------------
bool CRGizmoActor::GetRenderElement( u32 ElementIndex, CRGizmoRenderElement& OutRenderElement ) const
{
    (void)ElementIndex;
    (void)OutRenderElement;

    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Set gizmo world position.
//---------------------------------------------------------------------------------------------------------------------
void CRGizmoActor::SetPivot( const CRVector& InPivot )
{
    CRTransformComponent* transform = GetTransform();
    if ( !transform ) return;

    transform->SetLocation( InPivot );
}

//---------------------------------------------------------------------------------------------------------------------
/// Get current gizmo world position.
//---------------------------------------------------------------------------------------------------------------------
CRVector CRGizmoActor::GetPivot() const
{
    CRTransformComponent* transform = GetTransform();
    if ( !transform ) return CRVector::Zero;

    return transform->GetLocation();
}
