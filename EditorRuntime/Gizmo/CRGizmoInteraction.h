#pragma once


#include "Source/Core/CRTypes.h"
#include "Source/Core/Identify/CRIdentity.h"
#include "Source/Core/Math/CRRay.h"
#include "Source/RHI/Gizmo/CRGizmoSystem.h"


namespace CREditorRuntimeGizmo
{
//---------------------------------------------------------------------------------------------------------------------
/// Gizmo drag state.
//---------------------------------------------------------------------------------------------------------------------
struct CRGizmoDragState
{
    bool         bDragging = false;
    ECRGizmoAxis ActiveAxis = ECRGizmoAxis::X;

    CRVector DragStartActorLocation = CRVector::Zero;
    CRVector DragStartHitPoint      = CRVector::Zero;
};

//---------------------------------------------------------------------------------------------------------------------
/// Editor runtime data used for gizmo interaction.
//---------------------------------------------------------------------------------------------------------------------
struct CRGizmoInteractionContext
{
    CRIdentity::id_t SelectedActorId = CRIdentity::InvalidId;

    bool  bHasMouseRay = false;
    CRRay MouseRay;
};


bool TryBeginDrag( CRGizmoDragState& InOutDragState, const CRGizmoInteractionContext& Context );
void ApplyDrag   ( CRGizmoDragState& InOutDragState, const CRGizmoInteractionContext& Context );
void EndDrag     ( CRGizmoDragState& InOutDragState );
}
