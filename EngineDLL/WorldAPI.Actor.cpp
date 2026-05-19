#include "EngineDLL.h"
#include "WorldAPI.ActorInternal.h"
#include "Source/Asset/CRPrimitiveAsset.h"
#include "Source/Object/CRActor.h"
#include "Source/Object/Component/CRCollisionComponent.h"
#include "Source/Object/Component/CRDirectionalLightComponent.h"
#include "Source/Object/Component/CRPointLightComponent.h"
#include "Source/Object/Component/CRPrimitiveComponent.h"
#include "Source/Object/Component/CRSpotLightComponent.h"
#include "Source/RHI/ICRRHIMaterial.h"
#include "Source/Utility/Log/CRLog.h"
#include "Source/Utility/UtilPath.h"
#include <cstring>
#include <filesystem>


namespace
{
constexpr const char* MinionActorName           = "Minion";
constexpr const char* DirectionalLightActorName = "TestDirectionalLight";
constexpr const char* PointLightActorName       = "TestPointLight";
constexpr const char* SpotLightActorName        = "TestSpotLight";
constexpr const char* ProjectContentDirectory   = "Content";
constexpr const char* EngineAssetDirectory      = "Asset";
constexpr const char* MinionAssetName           = "Minion.cra";
constexpr const char* MinionDebugNormalMapPath  = "Asset/debug_normal.png";

//---------------------------------------------------------------------------------------------------------------------
/// Check whether primitive asset has enough vertex channels to render.
//---------------------------------------------------------------------------------------------------------------------
bool IsRenderablePrimitiveAsset( const CRPrimitiveAsset& Asset )
{
    if ( Asset.VertexCount == 0 ) return false;

    const size_t vertexCount = static_cast< size_t >( Asset.VertexCount );

    if ( Asset.Positions.size() < vertexCount ) return false;
    if ( Asset.Normals  .size() < vertexCount ) return false;
    if ( Asset.UVs      .size() < vertexCount ) return false;

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Try loading a renderable primitive asset.
//---------------------------------------------------------------------------------------------------------------------
bool TryLoadPrimitiveAsset( const CRPath& AssetPath, CRPrimitiveAsset& OutAsset )
{
    if ( AssetPath.empty() ) return false;
    if ( !std::filesystem::exists( AssetPath ) ) return false;

    CRPrimitiveAsset asset;
    asset.Load( AssetPath );
    if ( !IsRenderablePrimitiveAsset( asset ) ) return false;

    OutAsset = asset;
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Remove default-managed components.
//---------------------------------------------------------------------------------------------------------------------
void ClearNamedActorDefaults( CRActor& Actor )
{
    Actor.RemoveComponent< CRCollisionComponent >();
    Actor.RemoveComponent< CRPrimitiveComponent >();
    Actor.RemoveComponent< CRDirectionalLightComponent >();
    Actor.RemoveComponent< CRPointLightComponent >();
    Actor.RemoveComponent< CRSpotLightComponent >();
}

//---------------------------------------------------------------------------------------------------------------------
/// Resolve and load default actor primitive asset.
//---------------------------------------------------------------------------------------------------------------------
bool TryLoadDefaultActorPrimitiveAsset
(
    const char*       ProjectPath,
    const char*       AssetName,
    CRPath&           OutAssetPath,
    CRPrimitiveAsset& OutAsset
)
{
    if ( !AssetName || !AssetName[ 0 ] )
    {
        return false;
    }

    if ( ProjectPath && ProjectPath[ 0 ] )
    {
        const CRPath projectContentPath = CRPath( ProjectPath ) / ProjectContentDirectory;
        const CRPath projectAssetPath   = ( projectContentPath / AssetName ).lexically_normal();

        if ( TryLoadPrimitiveAsset( projectAssetPath, OutAsset ) )
        {
            OutAssetPath = projectAssetPath;
            return true;
        }
    }

    const CRPath engineAssetPath = UtilPath::ResolveExistingEnginePath( CRPath( EngineAssetDirectory ) / AssetName );
    if ( TryLoadPrimitiveAsset( engineAssetPath, OutAsset ) )
    {
        OutAssetPath = engineAssetPath;
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Apply Minion defaults.
//---------------------------------------------------------------------------------------------------------------------
bool ApplyMinionDefault( CRActor& Actor, const char* ProjectPath )
{
    CRPath assetPath;
    CRPrimitiveAsset primitiveAsset;
    if ( !TryLoadDefaultActorPrimitiveAsset( ProjectPath, MinionAssetName, assetPath, primitiveAsset ) )
    {
        GLog.AddLog( "[ApplyNamedActorDefaults] Failed to load Minion.cra." );
        return false;
    }

    CRPrimitiveComponent* primitive = Actor.Add< CRPrimitiveComponent >();
    if ( !primitive ) return false;

    CRCollisionComponent* collision = Actor.Add< CRCollisionComponent >();

    if ( collision )
    {
        const CRAABB localBounds = primitiveAsset.CalculateBounds();
        if ( localBounds.IsValid() )
        {
            collision->SetLocalCenter( localBounds.GetCenter() );
            collision->SetLocalHalfExtents( localBounds.GetExtents() * 0.5f );
        }
    }

    primitive->LoadAsset( assetPath );

    if ( ICRRHIMaterialSPtr material = primitive->GetMaterial() )
    {
        material->SetTexture( ECRMaterialTextureSlot::Normal, CRPath( MinionDebugNormalMapPath ) );
        
        material->SetDiffuseColor ( CRVector4D( 0.85f, 0.85f, 0.85f,  1.0f ) );
        material->SetSpecularColor( CRVector4D( 0.35f, 0.35f, 0.35f, 12.0f ) );

        GLog.AddLog( "[ApplyNamedActorDefaults] Applied Minion debug normal map." );
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Apply Directional Light defaults.
//---------------------------------------------------------------------------------------------------------------------
bool ApplyDirectionalLightDefault( CRActor& Actor )
{
    if ( CRDirectionalLightComponent* directionalLight = Actor.Add< CRDirectionalLightComponent >() )
    {
        directionalLight->Direction = CRVector4D( 0.35f, -0.80f, 0.48f, 0.0f );
        directionalLight->Color     = CRVector4D( 1.0f, 1.0f, 1.0f, 1.0f );
        directionalLight->bUseTransformForward = false;
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Apply Point Light defaults.
//---------------------------------------------------------------------------------------------------------------------
bool ApplyPointLightDefault( CRActor& Actor )
{
    if ( CRPointLightComponent* pointLight = Actor.Add< CRPointLightComponent >() )
    {
        pointLight->Range = 180.0f;
        pointLight->Color = CRVector4D( 1.0f, 0.85f, 0.70f, 2.0f );
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Apply Spot Light defaults.
//---------------------------------------------------------------------------------------------------------------------
bool ApplySpotLightDefault( CRActor& Actor )
{
    if ( CRSpotLightComponent* spotLight = Actor.Add< CRSpotLightComponent >() )
    {
        spotLight->Range      = 200.0f;
        spotLight->InnerAngle = 16.0f;
        spotLight->OuterAngle = 32.0f;
        spotLight->Color      = CRVector4D( 0.70f, 0.80f, 1.0f, 2.0f );
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Apply actor defaults by name.
//---------------------------------------------------------------------------------------------------------------------
bool ApplyNamedActorDefaultsInternal( CRActor& Actor, const char* Name, const char* ProjectPath )
{
    ClearNamedActorDefaults( Actor );

    const char* actorName = Name ? Name : Actor.GetName().c_str();
    if ( !actorName || !actorName[ 0 ] ) return true;

    if ( std::strcmp( actorName, MinionActorName ) == 0 )
    {
        return ApplyMinionDefault( Actor, ProjectPath );
    }

    if ( std::strcmp( actorName, DirectionalLightActorName ) == 0 )
    {
        return ApplyDirectionalLightDefault( Actor );
    }

    if ( std::strcmp( actorName, PointLightActorName ) == 0 )
    {
        return ApplyPointLightDefault( Actor );
    }

    if ( std::strcmp( actorName, SpotLightActorName ) == 0 )
    {
        return ApplySpotLightDefault( Actor );
    }

    return true;
}
}


namespace CRActorApiInternal
{
//---------------------------------------------------------------------------------------------------------------------
/// Get actor by id.
//---------------------------------------------------------------------------------------------------------------------
CRActor* GetActorById( CRIdentity::id_t Id )
{
    if ( !GWorld || !CRIdentity::IsValid( Id ) ) return nullptr;

    return GWorld->GetActor( CRObjectId( Id ) );
}

//---------------------------------------------------------------------------------------------------------------------
/// Get transform by actor id.
//---------------------------------------------------------------------------------------------------------------------
CRTransformComponent* GetTransform( CRIdentity::id_t Id )
{
    if ( CRActor* actor = GetActorById( Id ) ) return actor->GetTransform();

    return nullptr;
}
}


//---------------------------------------------------------------------------------------------------------------------
/// Set actor name.
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API bool SetActorName( CRIdentity::id_t Id, const char* Name )
{
    if ( CRActor* actor = CRActorApiInternal::GetActorById( Id ) )
    {
        actor->SetName( CRName( Name ? Name : "" ) );
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Apply named actor defaults.
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API bool ApplyNamedActorDefaults( CRIdentity::id_t Id, const char* Name, const char* ProjectPath )
{
    if ( CRActor* actor = CRActorApiInternal::GetActorById( Id ) )
    {
        return ApplyNamedActorDefaultsInternal( *actor, Name, ProjectPath );
    }

    return false;
}
