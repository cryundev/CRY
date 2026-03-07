#include "Engine.h"
#include "Source/Object/Camera/CRCamera.h"
#include "Source/Object/Component/CRComponentRegistry.h"
#include "Source/RHI/CRRHI.h"
#include "Source/RHI/ICRRHIRenderer.h"
#include "Source/Utility/Log/CRLog.h"
#include "Source/Utility/Time/CRFrameUpdator.h"
#include "Source/Utility/WIC/CRWICTextureLoader.h"
#include "Source/World/CRWorld.h"


CRSharedPtr< CRWorld > GWorld = CRMakeShared< CRWorld >(new CRWorld() );

CRTime         GFrameTime;
CRFrameUpdator GFrameUpdator;


//---------------------------------------------------------------------------------------------------------------------
/// Initialize.
//---------------------------------------------------------------------------------------------------------------------
bool CREngine::Initialize( HWND hWnd, u32 Width, u32 Height )
{
    if ( !GWorld )
    {
        GWorld = CRMakeShared< CRWorld >( new CRWorld() );
    }

    if ( !GRHI.Initialize( hWnd, Width, Height ) )
    {
        GLog.AddLog( "[CREngine::Initialize] Failed to initialize RHI." );
        return false;
    }

    CRCamera* camera = GWorld->GetCamera();
    if ( !camera )
    {
        GLog.AddLog( "[CREngine::Initialize] Failed to get camera." );
        return false;
    }
    
    camera->Initialize( CRCamera::EProjectionType::Perspective, 90.0f, (f32)Width, (f32)Height, 0.1f, 1000.0f );
    camera->SetLookAtDirection( 0.f, 0.f, 1.f );

    CRTransformComponent* cameraTransform = camera->GetTransform();
    if ( !cameraTransform )
    {
        GLog.AddLog( "[CREngine::Initialize] Failed to get camera transform." );
        return false;
    }

    cameraTransform->SetLocation( 0.f, 0.f, -15.0f );
    
    ICRRHIRenderer* renderer = GRHI.GetRenderer();
    if ( !renderer )
    {
        GLog.AddLog( "[CREngine::Initialize] Failed to get renderer." );
        return false;
    }

    renderer->UpdateViewProjectionBuffer( camera->GetViewMatrix(), camera->GetProjectionMatrix() );
    
    GFrameUpdator.Initialize( 30 );

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Tick.
//---------------------------------------------------------------------------------------------------------------------
void CREngine::Tick( float DeltaSeconds )
{
    CRComponentRegistry::Tick( DeltaSeconds );
}

//---------------------------------------------------------------------------------------------------------------------
/// Pre-render.
//---------------------------------------------------------------------------------------------------------------------
bool CREngine::PreRender( float DeltaSeconds )
{
    bool bRender = GFrameUpdator.Update( DeltaSeconds );

    if ( bRender )
    {
        CRComponentRegistry::PreRender( DeltaSeconds );
    }

    return bRender;
}

//---------------------------------------------------------------------------------------------------------------------
/// Render.
//---------------------------------------------------------------------------------------------------------------------
void CREngine::Render( float DeltaSeconds )
{
    GRHI.RenderFrame();
}

//---------------------------------------------------------------------------------------------------------------------
/// Post-render.
//---------------------------------------------------------------------------------------------------------------------
void CREngine::PostRender( float DeltaSeconds )
{
    GRHI.Present();
}

//---------------------------------------------------------------------------------------------------------------------
/// Shutdown.
//---------------------------------------------------------------------------------------------------------------------
void CREngine::Shutdown()
{
    if ( GWorld )
    {
        GWorld.reset();
    }

    GRHI.Shutdown();
    CRWICTextureLoader::ShutdownWICFactory();
}


//---------------------------------------------------------------------------------------------------------------------
/// main
//---------------------------------------------------------------------------------------------------------------------
int main()
{
    return 0;
}
