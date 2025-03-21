#include "Engine.h"
#include "Source/Asset/CRPrimitiveAsset.h"
#include "Source/Object/Camera/CRCamera.h"
#include "Source/Object/Component/CRPrimitiveComponent.h"
#include "Source/RHI/CRRHI.h"
#include "Source/RHI/ICRRHIRenderer.h"
#include "Source/Utility/FBX/CRFbxLoader.h"
#include "Source/Utility/Time/CRFrameUpdator.h"
#include "Source/World/CRWorld.h"


CRSharedPtr< CRWorld > GWorld = CRMakeShared< CRWorld >(new CRWorld() );
 ;

CRTime         GFrameTime;
CRFrameUpdator GFrameUpdator;


//---------------------------------------------------------------------------------------------------------------------
/// Initialize.
//---------------------------------------------------------------------------------------------------------------------
void CREngine::Initialize( HWND hWnd, u32 Width, u32 Height )
{
    GRHI.Initialize( hWnd, Width, Height );
    
    GWorld->GetCamera()->Initialize( CRCamera::EProjectionType::Perspective, 90.0f, (f32)Width, (f32)Height, 0.1f, 1000.0f );
    GWorld->GetCamera()->SetLookAtDirection( 0.f, 0.f, -1.f );
    GWorld->GetCamera()->GetTransform()->SetLocation( 0.f, 0.f, 15.0f );
    
    GRHI.GetRenderer()->UpdateViewProjectionBuffer( GWorld->GetCamera()->GetViewMatrix(), GWorld->GetCamera()->GetProjectionMatrix() );

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

    if ( CRActor* minion = GWorld->SpawnActor< CRActor >() )
    {
        minion->SetName( CRName( "Minion" ) );
        
        if ( CRPrimitiveComponent* primitive = minion->AddComponent< CRPrimitiveComponent >() )
        {
            primitive->LoadAsset( "../Asset/Minion.cra" );
        }
    }
    
    GFrameUpdator.Initialize( 30 );
}

//---------------------------------------------------------------------------------------------------------------------
/// Tick.
//---------------------------------------------------------------------------------------------------------------------
void CREngine::Tick( float DeltaSeconds )
{
    CRTransformComponent::UpdateComponents( DeltaSeconds );
    CRPrimitiveComponent::UpdateComponents( DeltaSeconds );
}

//---------------------------------------------------------------------------------------------------------------------
/// Pre-render.
//---------------------------------------------------------------------------------------------------------------------
bool CREngine::PreRender( float DeltaSeconds )
{
    bool bRender = GFrameUpdator.Update( DeltaSeconds );

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
/// main
//---------------------------------------------------------------------------------------------------------------------
int main()
{
    return 0;
}