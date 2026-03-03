#pragma once


#include "CRAxisGizmoActor.h"
#include "CRGizmoActor.h"
#include "Source/Core/CRTypes.h"
#include "Source/Core/Math/CRAxis.h"
#include "Source/Core/Math/CRMath.h"

struct CRRay;
class CRActor;


//---------------------------------------------------------------------------------------------------------------------
/// CRGizmoSystem
//---------------------------------------------------------------------------------------------------------------------
class CRGizmoSystem
{
private:
    CRAxisGizmoActor* GizmoActor = nullptr;
    bool              bVisible   = false;

    // Drag state.
    bool     bDragging          = false;
    ECRAxis  DragAxis           = ECRAxis::Max;
    CRVector ActorStartLocation = CRVector::Zero;
    f32      DragStartAxisT     = 0.0f;

public:
    /// Initialize.
    bool Initialize();

    /// Shutdown.
    void Shutdown();

    /// Is ready.
    bool IsReady() const;

    /// Is visible.
    bool IsVisible() const { return bVisible && IsReady(); }

    /// Set visibility.
    void SetVisible( bool bInVisible );

    /// Set pivot.
    void SetPivot( const CRVector& InPivot );

    /// Get pivot.
    CRVector GetPivot() const;

    /// Get gizmo actor.
    CRGizmoActor* GetGizmoActor() const { return GizmoActor; }

    /// Begin drag.
    bool BeginDrag( ECRAxis Axis, const CRRay& Ray );

    /// Try begin drag from ray.
    bool TryBeginDragFromRay( const CRRay& Ray );

    /// Update drag.
    bool UpdateDrag( const CRRay& Ray, CRVector& OutNewLocation );

    /// Apply drag from ray.
    bool ApplyDragFromRay( const CRRay& Ray, CRActor* TargetActor );

    /// End drag.
    void EndDrag();

    /// Is dragging.
    bool IsDragging() const { return bDragging; }

private:
    /// Get axis direction.
    static CRVector _GetAxisDirection( ECRAxis Axis );

    /// Try compute axis parameter from ray.
    bool _TryComputeAxisTFromRay( const CRRay& Ray, const CRVector& AxisOrigin, const CRVector& AxisDir, f32& OutAxisT ) const;
};


extern CRGizmoSystem GGizmoSystem;
