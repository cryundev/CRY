#pragma once


#include "CRGizmoActor.h"
#include "Source/Core/Math/CRAxis.h"
#include "Source/Object/Component/CRCollisionComponent.h"
#include "Source/Object/Component/CRPrimitiveComponent.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRAxisGizmoActor
//---------------------------------------------------------------------------------------------------------------------
class CRAxisGizmoActor : public CRGizmoActor
{
public:
    /// Initialize gizmo actor.
    virtual bool InitializeGizmo( const CRPath& AssetPath, const CRPrimitiveAsset& PrimitiveAsset ) override;

    /// Get render element count.
    virtual u32 GetRenderElementCount() const override { return (u32)ECRAxis::Max; }

    /// Get render element.
    virtual bool GetRenderElement( u32 ElementIndex, CRGizmoRenderElement& OutRenderElement ) const override;

    /// Get axis primitive component.
    CRPrimitiveComponent* GetAxisPrimitive( ECRAxis Axis ) const;

    /// Test axis hit.
    ECRAxis HitTestAxis( const CRVector& RayOrigin, const CRVector& RayDir ) const;

    /// Set axis query enabled.
    void SetAxisQueryEnabled( bool bEnabled );

private:
    /// Get axis collision component.
    CRCollisionComponent* _GetAxisCollision( ECRAxis Axis ) const;

    /// Compute current gizmo world scale used by shader.
    f32 _ComputeGizmoWorldScale() const;

    /// Get axis transform.
    CRMatrix _GetAxisTransform( ECRAxis Axis ) const;

    /// Get axis color.
    CRVector4D _GetAxisColor( ECRAxis Axis ) const;
};
