#include "Engine.h"
#include "Source/Asset/CRPrimitiveAsset.h"
#include "Source/Input/CRInputProcessorCamera.h"
#include "Source/Object/Camera/CRCamera.h"
#include "Source/RHI/CRRHI.h"
#include "Source/RHI/ICRRHIMesh.h"
#include "Source/RHI/ICRRHIRenderer.h"
#include "Source/RHI/DX11/CRD11Renderer.h"
#include "Source/Utility/FBX/CRFbxLoader.h"
#include "Source/Utility/Time/CRFrameUpdator.h"


CRSharedPtr< CRCamera > GCamera = CRMakeShared< CRCamera >(new CRCamera() );

CRTime         GFrameTime;
CRFrameUpdator GFrameUpdator;

CRInputProcessorCamera GInputProcessorCamera;


//---------------------------------------------------------------------------------------------------------------------
/// Initialize.
//---------------------------------------------------------------------------------------------------------------------
void CREngine::Initialize( HWND hWnd, unsigned int Width, unsigned int Height )
{
    GRHI.Initialize( hWnd, Width, Height );
    
    GCamera->Initialize( CRCamera::EProjectionType::Perspective, 90.0f, Width, Height, 0.1f, 1000.0f );
    GCamera->SetLookAtDirection( 0.f, 0.f, -1.f );
    GCamera->Transform.SetLocation( 0.f, 0.f, 15.0f );
    
    GRHI.GetRenderer()->UpdateViewProjectionBuffer( GCamera->GetViewMatrix(), GCamera->GetProjectionMatrix() );

    // const CRString& loadFbxPaht = "../Asset/Minion";
    // CRFbxLoader fbxLoader;
    // fbxLoader.Load( loadFbxPaht + ".fbx" );
    // fbxLoader.GetPrimitives()[0].Save( loadFbxPaht + ".cra" );
    //
    // int i = 0;
    // for ( auto& primitive : fbxLoader.GetPrimitives() )
    // {
    //     if ( const ICRRHIMeshSPtr& rhiMesh = GRHI.CreateMesh() )
    //     {
    //         CRName name = "Minion";
    //         name.append( std::to_string( i++ ) );
    //         
    //         rhiMesh->InitializePrimitive( name, primitive );
    //         rhiMesh->InitializeMaterial();
    //
    //         GRHI.GetRenderer()->AddRenderMesh( rhiMesh );
    //
    //         ++i;
    //     }
    // }
    
    CRPrimitiveAsset primitive;
    primitive.Load( "../Asset/Minion.cra" );
    
    const ICRRHIMeshWPtr& rhiMesh = GRHI.CreateMesh();
    
    rhiMesh.lock()->InitializePrimitive( "Minion", primitive );
    rhiMesh.lock()->InitializeMaterial();
    
    GRHI.GetRenderer()->AddRenderMesh( rhiMesh ); 
    
    GFrameUpdator.Initialize( 30 );
}

//---------------------------------------------------------------------------------------------------------------------
/// Call per message loop.
//---------------------------------------------------------------------------------------------------------------------
void CREngine::MessageLoop()
{
    float deltaSeconds = 1/30.f;
    GFrameTime.Start();

    GInputProcessorCamera.Tick( deltaSeconds );

    if ( GFrameUpdator.Update( deltaSeconds ) )
    {
        GRHI.RenderFrame();
    }

    deltaSeconds = GFrameTime.Finish();
}