#pragma once


#include "Source/Core/CRTypes.h"
#include "Source/Core/Math/CRMath.h"
#include "Source/RHI/CRRHITypes.h"


//---------------------------------------------------------------------------------------------------------------------
/// Axis enum for translate gizmo rendering.
//---------------------------------------------------------------------------------------------------------------------
enum class ECRGizmoAxis : u32
{
    X = 0,
    Y,
    Z,
    Max
};


//---------------------------------------------------------------------------------------------------------------------
/// Runtime gizmo state shared between editor frontends.
//---------------------------------------------------------------------------------------------------------------------
class CRGizmoSystem
{
private:
    ICRRHIMeshWPtr ArrowMesh;
    CRVector       Pivot    = CRVector::Zero;
    bool           bVisible = false;

public:
    /// Initialize gizmo resources (loads prebuilt CRA asset).
    bool Initialize();

    /// Release gizmo resources.
    void Shutdown();

    /// Return true when gizmo mesh is available.
    bool IsReady() const { return !ArrowMesh.expired(); }

    /// Get gizmo arrow mesh.
    ICRRHIMeshSPtr GetArrowMesh() const;

    /// Set visibility.
    void SetVisible( bool bInVisible ) { bVisible = bInVisible; }

    /// Is visible.
    bool IsVisible() const { return bVisible && !ArrowMesh.expired(); }
    
    /// Set pivot and make gizmo visible.
    void SetPivot( const CRVector& InPivot );

    /// Get current pivot.
    const CRVector& GetPivot() const { return Pivot; }

    /// Create axis world transform.
    CRMatrix CreateAxisTransform( ECRGizmoAxis Axis ) const;

    /// Get axis render color.
    CRVector4D GetAxisColor( ECRGizmoAxis Axis ) const;
};


extern CRGizmoSystem GGizmoSystem;
