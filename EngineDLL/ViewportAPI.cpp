#include "Engine.h"
#include "EngineDLL.h"
#include "../EditorRuntime/EditorRuntime.h"
#include "Source/RHI/CRRHI.h"
#include "Source/RHI/ICRRHIRenderer.h"
#include "Source/World/CRWorld.h"
#include "WorldAPI.TransformInternal.h"


//---------------------------------------------------------------------------------------------------------------------
/// InitializeViewport
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API bool InitializeViewport( HWND hWnd, i32 Width, i32 Height )
{
    const bool bResult = CREngine::Initialize( hWnd, (u32)Width, (u32)Height );

    if ( bResult )
    {
        CREditorRuntime::InitializeRuntime();
    }

    return bResult;
}

//---------------------------------------------------------------------------------------------------------------------
/// ShutdownViewport
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API void ShutdownViewport()
{
    CREditorRuntime::ShutdownRuntime();
    CREngine::Shutdown();
}

//---------------------------------------------------------------------------------------------------------------------
/// ResizeViewport
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API void ResizeViewport( i32 Width, i32 Height )
{
    if ( !GRHI.IsInitialized() ) return;

    if ( GWorld )
    {
        if ( CRCamera* camera = GWorld->GetCamera() )
        {
            camera->SetViewSize( (f32)Width, (f32)Height );
        }
    }

    GRHI.Resize( (u32)Width, (u32)Height );
}

//---------------------------------------------------------------------------------------------------------------------
/// TickViewport
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API void TickViewport( f32 DeltaSeconds )
{
    CREditorRuntime::ApplyViewportCameraInput( DeltaSeconds );
    
    CREngine::Tick( DeltaSeconds );
}

//---------------------------------------------------------------------------------------------------------------------
/// RenderViewport
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API void RenderViewport( f32 DeltaSeconds )
{
    if ( CREngine::PreRender( DeltaSeconds ) )
    {
        CREngine::Render    ( DeltaSeconds );
        CREngine::PostRender( DeltaSeconds );
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// SetViewportCamera
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API void SetViewportCamera( const CRDllVector3* Position, const CRDllVector3* Direction )
{
    if ( !Position || !Direction ) return;
    if ( !GWorld ) return;

    CRCamera* camera = GWorld->GetCamera();
    if ( !camera ) return;

    CRTransformComponent* cameraTransform = camera->GetTransform();
    if ( !cameraTransform ) return;

    CRVector lookDirection( Direction->X, Direction->Y, Direction->Z );
    if ( lookDirection.LengthSquared() <= CRMath::Epsilon )
    {
        lookDirection = CRVector( 0.0f, 0.0f, 1.0f );
    }
    else
    {
        lookDirection.Normalize();
    }

    cameraTransform->SetLocation( Position->X, Position->Y, Position->Z );
    cameraTransform->SetRotation( CRQuaternion::Identity );
    camera->SetLookAtDirection( lookDirection );

    if ( ICRRHIRenderer* renderer = GRHI.GetRenderer() )
    {
        renderer->UpdateViewProjectionBuffer( camera->GetViewMatrix(), camera->GetProjectionMatrix() );
    }
}
