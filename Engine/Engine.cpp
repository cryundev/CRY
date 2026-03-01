#include "Engine.h"
#include "Source/Asset/CRPrimitiveAsset.h"
#include "Source/Object/Camera/CRCamera.h"
#include "Source/Object/Component/CRComponentRegistry.h"
#include "Source/Object/Component/CRCollisionComponent.h"
#include "Source/Object/Component/CRDirectionalLightComponent.h"
#include "Source/Object/Component/CRPointLightComponent.h"
#include "Source/Object/Component/CRPrimitiveComponent.h"
#include "Source/Object/Component/CRSpotLightComponent.h"
#include "Source/RHI/CRRHI.h"
#include "Source/RHI/ICRRHIRenderer.h"
#include "Source/Utility/FBX/CRFbxLoader.h"
#include "Source/Utility/Log/CRLog.h"
#include "Source/Utility/Time/CRFrameUpdator.h"
#include "Source/Utility/WIC/CRWICTextureLoader.h"
#include "Source/World/CRWorld.h"
#include <filesystem>


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

        CRCollisionComponent* collision = minion->Add< CRCollisionComponent >();
        
        if ( CRPrimitiveComponent* primitive = minion->Add< CRPrimitiveComponent >() )
        {
            std::filesystem::path assetPath = std::filesystem::path( __FILE__ ).parent_path() / "../Asset/Minion.cra";
            if ( !std::filesystem::exists( assetPath ) )
            {
                assetPath = std::filesystem::path( "../Asset/Minion.cra" );
            }

            if ( std::filesystem::exists( assetPath ) )
            {
                const CRString normalizedAssetPath = assetPath.lexically_normal().string();

                if ( collision )
                {
                    CRPrimitiveAsset primitiveAsset;
                    primitiveAsset.Load( normalizedAssetPath );

                    const CRAABB localBounds = primitiveAsset.CalculateBounds();
                    if ( localBounds.IsValid() )
                    {
                        collision->SetLocalCenter( localBounds.GetCenter() );
                        collision->SetLocalHalfExtents( localBounds.GetExtents() * 0.5f );
                    }
                }
                
                primitive->LoadAsset( normalizedAssetPath );
            }
            else
            {
                GLog.AddLog( "[CREngine::Initialize] Failed to find Minion.cra." );
            }
        }
    }

    if ( CRActor* directionalLightActor = GWorld->SpawnActor< CRActor >() )
    {
        directionalLightActor->SetName( CRName( "TestDirectionalLight" ) );

        if ( CRDirectionalLightComponent* directionalLight = directionalLightActor->Add< CRDirectionalLightComponent >() )
        {
            directionalLight->Direction = CRVector4D( 0.35f, -0.80f, 0.48f, 0.0f );
            directionalLight->Color     = CRVector4D( 1.0f, 1.0f, 1.0f, 1.0f );
        }
    }

    if ( CRActor* pointLightActor = GWorld->SpawnActor< CRActor >() )
    {
        pointLightActor->SetName( CRName( "TestPointLight" ) );

        if ( CRTransformComponent* pointTransform = pointLightActor->GetTransform() )
        {
            pointTransform->SetLocation( -3.0f, 2.0f, 3.0f );
        }

        if ( CRPointLightComponent* pointLight = pointLightActor->Add< CRPointLightComponent >() )
        {
            pointLight->Range = 180.0f;
            pointLight->Color = CRVector4D( 1.0f, 0.85f, 0.70f, 2.0f );
        }
    }

    if ( CRActor* spotLightActor = GWorld->SpawnActor< CRActor >() )
    {
        spotLightActor->SetName( CRName( "TestSpotLight" ) );

        if ( CRTransformComponent* spotTransform = spotLightActor->GetTransform() )
        {
            spotTransform->SetLocation( 3.0f, 3.0f, 3.0f );
            spotTransform->Rotate( CRVector( 1.0f, 0.0f, 0.0f ), -0.75f );
        }

        if ( CRSpotLightComponent* spotLight = spotLightActor->Add< CRSpotLightComponent >() )
        {
            spotLight->Range      = 200.0f;
            spotLight->InnerAngle = 16.0f;
            spotLight->OuterAngle = 32.0f;
            spotLight->Color      = CRVector4D( 0.70f, 0.80f, 1.0f, 2.0f );
        }
    }
    
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
