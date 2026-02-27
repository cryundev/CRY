#include "Engine.h"
#include "EngineDLL.h"
#include "../EditorRuntime/EditorRuntime.h"
#include "Source/RHI/CRRHI.h"
#include "Source/World/CRWorld.h"


//---------------------------------------------------------------------------------------------------------------------
/// InitializeViewport
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API bool InitializeViewport( HWND hWnd, i32 Width, i32 Height )
{
    return CREngine::Initialize( hWnd, (u32)Width, (u32)Height );
}

//---------------------------------------------------------------------------------------------------------------------
/// ShutdownViewport
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API void ShutdownViewport()
{
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
