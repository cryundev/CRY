#include "EditorRuntime.h"
#include "Engine.h"
#include "Source/Core/Math/CRRay.h"
#include "Source/Object/CRActor.h"
#include "Source/Object/Camera/CRCamera.h"
#include "Source/Object/Component/CRTransformComponent.h"
#include "Source/RHI/Gizmo/CRGizmoSystem.h"
#include "Source/Utility/UtilRay.h"
#include "Source/World/CRWorld.h"


namespace
{
//---------------------------------------------------------------------------------------------------------------------
/// Mouse state cache for editor viewport bridge.
//---------------------------------------------------------------------------------------------------------------------
struct CRViewportMouseState
{
    i32 PixelX = 0;
    i32 PixelY = 0;
    
    i32 PreviousPixelX = 0;
    i32 PreviousPixelY = 0;

    i32 ViewportWidth  = 1;
    i32 ViewportHeight = 1;
    
    i32 WheelDelta     = 0;

    f32 NdcX = 0.0f;
    f32 NdcY = 0.0f;
    
    bool bLeftPressed  = false;
    bool bRightPressed = false;
    
    bool bHasPreviousPixel = false;

    CRRay Ray;
    bool  bHasRay = false;
};


//---------------------------------------------------------------------------------------------------------------------
/// Keyboard state cache for viewport camera control.
//---------------------------------------------------------------------------------------------------------------------
struct CRViewportKeyboardState
{
    bool bMoveForward  = false;
    bool bMoveBackward = false;
    bool bMoveLeft     = false;
    bool bMoveRight    = false;
    bool bMoveDown     = false;
    bool bMoveUp       = false;
};

//---------------------------------------------------------------------------------------------------------------------
/// Editor selection state shared by viewport input bridges.
//---------------------------------------------------------------------------------------------------------------------
struct CRViewportSelectionState
{
    CRIdentity::id_t SelectedActorId = CRIdentity::InvalidId;
};

    
CRViewportMouseState     GViewportMouseState;
CRViewportKeyboardState  GViewportKeyboardState;
CRViewportSelectionState GViewportSelectionState;

static constexpr f32 CameraMoveSpeed       = 50.0f;
static constexpr f32 CameraLookSensitivity = 0.0025f;

    
//---------------------------------------------------------------------------------------------------------------------
/// Convert pixel coordinates to NDC.
//---------------------------------------------------------------------------------------------------------------------
bool TryConvertToNdc( i32 PixelX, i32 PixelY, i32 ViewportW, i32 ViewportH, f32& OutNdcX, f32& OutNdcY )
{
    if ( ViewportW <= 0 || ViewportH <= 0 ) return false;

    OutNdcX = ( (f32)PixelX / (f32)ViewportW ) * 2.0f - 1.0f;
    OutNdcY = 1.0f - ( (f32)PixelY / (f32)ViewportH ) * 2.0f;

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Store latest pointer state.
//---------------------------------------------------------------------------------------------------------------------
void StorePointerState( i32 PixelX, i32 PixelY, i32 ViewportW, i32 ViewportH, f32 NdcX, f32 NdcY )
{
    GViewportMouseState.PixelX = PixelX;
    GViewportMouseState.PixelY = PixelY;

    GViewportMouseState.ViewportWidth  = ViewportW;
    GViewportMouseState.ViewportHeight = ViewportH;

    GViewportMouseState.NdcX = NdcX;
    GViewportMouseState.NdcY = NdcY;
}

//---------------------------------------------------------------------------------------------------------------------
/// Commit current pointer position as previous position.
//---------------------------------------------------------------------------------------------------------------------
void CommitPointerAsPrevious()
{
    GViewportMouseState.PreviousPixelX    = GViewportMouseState.PixelX;
    GViewportMouseState.PreviousPixelY    = GViewportMouseState.PixelY;
    GViewportMouseState.bHasPreviousPixel = true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Update cached mouse ray from current NDC coordinates.
//---------------------------------------------------------------------------------------------------------------------
bool TryUpdateMouseRay( f32 NdcX, f32 NdcY )
{
    GViewportMouseState.bHasRay = false;
    
    if ( !GWorld ) return false;
    if ( !GWorld->GetCamera() ) return false;

    CRRay ray;
    
    if ( !UtilRay::TryCreateRayFromNDC( NdcX, NdcY, ray ) ) return false;

    GViewportMouseState.Ray     = ray;
    GViewportMouseState.bHasRay = true;

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Set movement key state from virtual-key code.
//---------------------------------------------------------------------------------------------------------------------
void SetMovementKeyState( i32 VirtualKey, bool bPressed )
{
    switch ( VirtualKey )
    {
        case 'W': GViewportKeyboardState.bMoveForward  = bPressed; break;
        case 'S': GViewportKeyboardState.bMoveBackward = bPressed; break;
        case 'A': GViewportKeyboardState.bMoveLeft     = bPressed; break;
        case 'D': GViewportKeyboardState.bMoveRight    = bPressed; break;
        case 'Q': GViewportKeyboardState.bMoveDown     = bPressed; break;
        case 'E': GViewportKeyboardState.bMoveUp       = bPressed; break;
        default: break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Apply keyboard movement from WASD/QE state.
//---------------------------------------------------------------------------------------------------------------------
void ApplyViewportMove( CRCamera* Camera, CRTransformComponent* Transform, f32 DeltaSeconds )
{
    if ( !Camera ) return;
    if ( !Transform ) return;
    if ( DeltaSeconds <= 0.0f ) return;

    CRVector movement = CRVector::Zero;

    const CRVector& forward = Camera->GetLookDirection();
    const CRVector& right   = Camera->GetRightDirection();
    const CRVector& up      = Camera->GetUpDirection();

    if ( GViewportKeyboardState.bMoveForward  ) movement += forward;
    if ( GViewportKeyboardState.bMoveBackward ) movement -= forward;
    if ( GViewportKeyboardState.bMoveLeft     ) movement -= right;
    if ( GViewportKeyboardState.bMoveRight    ) movement += right;
    if ( GViewportKeyboardState.bMoveDown     ) movement -= up;
    if ( GViewportKeyboardState.bMoveUp       ) movement += up;

    if ( movement.LengthSquared() <= CRMath::Epsilon ) return;

    movement.Normalize();
    Transform->Translate( movement * CameraMoveSpeed * DeltaSeconds );
}

//---------------------------------------------------------------------------------------------------------------------
/// Apply RMB drag look from pointer delta.
//---------------------------------------------------------------------------------------------------------------------
void ApplyViewportLook( CRTransformComponent* Transform )
{
    if ( !Transform ) return;

    if ( !GViewportMouseState.bRightPressed )
    {
        GViewportMouseState.bHasPreviousPixel = false;
        return;
    }

    if ( !GViewportMouseState.bHasPreviousPixel )
    {
        CommitPointerAsPrevious();
        return;
    }

    i32 deltaX = GViewportMouseState.PixelX - GViewportMouseState.PreviousPixelX;
    i32 deltaY = GViewportMouseState.PixelY - GViewportMouseState.PreviousPixelY;

    CommitPointerAsPrevious();

    if ( deltaX == 0 && deltaY == 0 ) return;

    if ( deltaX != 0 )
    {
        Transform->Rotate( CRVector::Up, (f32)deltaX * CameraLookSensitivity );
    }

    if ( deltaY != 0 )
    {
        Transform->Rotate( Transform->GetRight(), (f32)deltaY * CameraLookSensitivity );
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Update gizmo pivot from selected actor state.
//---------------------------------------------------------------------------------------------------------------------
void UpdateSelectionGizmoState()
{
    if ( !GGizmoSystem.IsReady() ) return;
    
    GGizmoSystem.SetVisible( false );

    if ( !GWorld ) return;
    if ( !CRIdentity::IsValid( GViewportSelectionState.SelectedActorId ) ) return;

    CRActor* selectedActor = GWorld->GetActor( CRObjectId( GViewportSelectionState.SelectedActorId ) );
    if ( !selectedActor )
    {
        GViewportSelectionState.SelectedActorId = CRIdentity::InvalidId;        
        return;
    }

    CRTransformComponent* selectedTransform = selectedActor->GetTransform();
    if ( !selectedTransform ) return;

    GGizmoSystem.SetPivot( selectedTransform->GetLocation() );
    GGizmoSystem.SetVisible( true );
}
}


namespace CREditorRuntime
{
//---------------------------------------------------------------------------------------------------------------------
/// OnViewportMouseMove
//---------------------------------------------------------------------------------------------------------------------
void OnViewportMouseMove( i32 PixelX, i32 PixelY, i32 ViewportW, i32 ViewportH )
{
    f32 ndcX = 0.0f;
    f32 ndcY = 0.0f;

    if ( !TryConvertToNdc( PixelX, PixelY, ViewportW, ViewportH, ndcX, ndcY ) ) return;

    StorePointerState( PixelX, PixelY, ViewportW, ViewportH, ndcX, ndcY );
    TryUpdateMouseRay( ndcX, ndcY );
}

//---------------------------------------------------------------------------------------------------------------------
/// OnViewportMouseButton
//---------------------------------------------------------------------------------------------------------------------
void OnViewportMouseButton( i32 PixelX, i32 PixelY, i32 ViewportW, i32 ViewportH, i32 Button, bool bPressed )
{
    f32 ndcX = 0.0f;
    f32 ndcY = 0.0f;

    if ( !TryConvertToNdc( PixelX, PixelY, ViewportW, ViewportH, ndcX, ndcY ) ) return;

    StorePointerState( PixelX, PixelY, ViewportW, ViewportH, ndcX, ndcY );
    TryUpdateMouseRay( ndcX, ndcY );
    
    switch ( Button )
    {
    case 0: GViewportMouseState.bLeftPressed  = bPressed; break;
    case 1: GViewportMouseState.bRightPressed = bPressed;
        {
            if ( bPressed )
            {
                CommitPointerAsPrevious();
            }
            else
            {
                GViewportMouseState.bHasPreviousPixel = false;
            }
        }
        break;
    default: break; 
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// OnViewportKeyState
//---------------------------------------------------------------------------------------------------------------------
void OnViewportKeyState( i32 VirtualKey, bool bPressed )
{
    SetMovementKeyState( VirtualKey, bPressed );
}

//---------------------------------------------------------------------------------------------------------------------
/// OnViewportMouseWheel
//---------------------------------------------------------------------------------------------------------------------
void OnViewportMouseWheel( i32 WheelDelta )
{
    GViewportMouseState.WheelDelta += WheelDelta;
}

//---------------------------------------------------------------------------------------------------------------------
/// OnActorPicked
//---------------------------------------------------------------------------------------------------------------------
void OnActorPicked( CRIdentity::id_t ActorId )
{
    GViewportSelectionState.SelectedActorId = ActorId;

    UpdateSelectionGizmoState();
}

//---------------------------------------------------------------------------------------------------------------------
/// ApplyViewportCameraInput
//---------------------------------------------------------------------------------------------------------------------
void ApplyViewportCameraInput( f32 DeltaSeconds )
{
    if ( GWorld )
    {
        if ( CRCamera* camera = GWorld->GetCamera() )
        {
            if ( CRTransformComponent* transform = camera->GetTransform() )
            {
                ApplyViewportLook( transform );

                if ( DeltaSeconds > 0.0f )
                {
                    ApplyViewportMove( camera, transform, DeltaSeconds );
                }
            }
        }
    }

    GViewportMouseState.WheelDelta = 0;
    UpdateSelectionGizmoState();
}
}
