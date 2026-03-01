#pragma once


#include "CRGizmoActor.h"
#include "Source/Core/CRTypes.h"
#include "Source/Core/Math/CRMath.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRGizmoSystem
//---------------------------------------------------------------------------------------------------------------------
class CRGizmoSystem
{
private:
    CRGizmoActor* GizmoActor = nullptr;
    bool          bVisible   = false;

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
    void SetVisible( bool bInVisible ) { bVisible = bInVisible; }

    /// Set pivot.
    void SetPivot( const CRVector& InPivot );

    /// Get pivot.
    CRVector GetPivot() const;

    /// Get gizmo actor.
    CRGizmoActor* GetGizmoActor() const { return GizmoActor; }
};


extern CRGizmoSystem GGizmoSystem;
