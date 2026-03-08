#include "CRActorMarkerSystem.h"
#include "CRLightActorMarkerProvider.h"
#include "Engine.h"
#include "Source/Asset/CRPrimitiveAsset.h"
#include "Source/Core/Math/CRRay.h"
#include "Source/Object/Camera/CRCamera.h"
#include "Source/RHI/CRRHI.h"
#include "Source/RHI/ICRRHIMesh.h"
#include "Source/RHI/ICRRHIRenderer.h"
#include "Source/Utility/UtilJson.h"
#include "Source/Utility/Log/CRLog.h"
#include "Source/Utility/UtilPath.h"
#include "Source/World/CRWorld.h"
#include <filesystem>
#include <fstream>
#include <unordered_set>
#include <utility>


CRActorMarkerSystem GActorMarkerSystem;


namespace
{
static const CRPath   MarkerConfigPath = CRPath( "Asset/Gizmo/ActorMarkerConfig.json" );
static const CRString LogPrefix        = "[CRActorMarkerSystem] ";

//---------------------------------------------------------------------------------------------------------------------
/// Parse component kind string.
//---------------------------------------------------------------------------------------------------------------------
bool _TryParseComponentKind( const CRString& Value, ECRLightMarkerComponentKind& OutKind )
{
    if ( Value == "directional_light" ) { OutKind = ECRLightMarkerComponentKind::Directional; return true; }
    if ( Value == "spot_light"        ) { OutKind = ECRLightMarkerComponentKind::Spot;        return true; }
    if ( Value == "point_light"       ) { OutKind = ECRLightMarkerComponentKind::Point;       return true; }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Log marker-system error and return false.
//---------------------------------------------------------------------------------------------------------------------
bool _Fail( const CRString& Message )
{
    GLog.AddLog( LogPrefix + Message );
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Build indexed item context string.
//---------------------------------------------------------------------------------------------------------------------
CRString _BuildContext( const CRString& SectionName, size_t Index )
{
    return SectionName + "[" + std::to_string( Index ) + "]";
}

//---------------------------------------------------------------------------------------------------------------------
/// Read non-empty string field.
//---------------------------------------------------------------------------------------------------------------------
bool _GetRequiredStringField( const nlohmann::json& Object, const CRString& FieldName, CRString& OutValue, const CRString& Context )
{
    if ( !Object.contains( FieldName ) || !Object[ FieldName ].is_string() )
    {
        return _Fail( Context + " missing string field '" + FieldName + "'." );
    }

    OutValue = Object[ FieldName ].get< CRString >();
    if ( OutValue.empty() )
    {
        return _Fail( Context + " field '" + FieldName + "' must not be empty." );
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Read numeric float field.
//---------------------------------------------------------------------------------------------------------------------
bool _GetRequiredFloatField( const nlohmann::json& Object, const CRString& FieldName, f32& OutValue, const CRString& Context )
{
    if ( !Object.contains( FieldName ) || !Object[ FieldName ].is_number() )
    {
        return _Fail( Context + " missing numeric field '" + FieldName + "'." );
    }

    OutValue = Object[ FieldName ].get< f32 >();
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Read integer field.
//---------------------------------------------------------------------------------------------------------------------
bool _GetRequiredIntField( const nlohmann::json& Object, const CRString& FieldName, i32& OutValue, const CRString& Context )
{
    if ( !Object.contains( FieldName ) || !Object[ FieldName ].is_number_integer() )
    {
        return _Fail( Context + " missing integer field '" + FieldName + "'." );
    }

    OutValue = Object[ FieldName ].get< i32 >();
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Read bool field.
//---------------------------------------------------------------------------------------------------------------------
bool _GetRequiredBoolField( const nlohmann::json& Object, const CRString& FieldName, bool& OutValue, const CRString& Context )
{
    if ( !Object.contains( FieldName ) || !Object[ FieldName ].is_boolean() )
    {
        return _Fail( Context + " missing bool field '" + FieldName + "'." );
    }

    OutValue = Object[ FieldName ].get< bool >();
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Read float4 color field.
//---------------------------------------------------------------------------------------------------------------------
bool _GetRequiredColorField( const nlohmann::json& Object, const CRString& FieldName, CRVector4D& OutColor, const CRString& Context )
{
    if ( !Object.contains( FieldName ) || !Object[ FieldName ].is_array() || Object[ FieldName ].size() != 4 )
    {
        return _Fail( Context + " field '" + FieldName + "' must be float[4]." );
    }

    for ( i32 colorIndex = 0; colorIndex < 4; ++colorIndex )
    {
        if ( !Object[ FieldName ][ colorIndex ].is_number() )
        {
            return _Fail( Context + " " + FieldName + "[" + std::to_string( colorIndex ) + "] must be numeric." );
        }
    }

    OutColor = CRVector4D
    (
        Object[ FieldName ][ 0 ].get< f32 >(),
        Object[ FieldName ][ 1 ].get< f32 >(),
        Object[ FieldName ][ 2 ].get< f32 >(),
        Object[ FieldName ][ 3 ].get< f32 >()
    );

    return true;
}
}


//---------------------------------------------------------------------------------------------------------------------
/// Initialize.
//---------------------------------------------------------------------------------------------------------------------
bool CRActorMarkerSystem::Initialize()
{
    Shutdown();

    bVisible = true;

    CRMarkerSystemConfig config;
    if ( !_LoadConfigFromJson( config ) )
    {
        GLog.AddLog( LogPrefix + "Failed to load marker json config. Actor marker disabled." );
        return false;
    }

    if ( !_ApplyConfig( config ) )
    {
        GLog.AddLog( LogPrefix + "Failed to apply marker json config. Actor marker disabled." );
        Shutdown();
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Shutdown.
//---------------------------------------------------------------------------------------------------------------------
void CRActorMarkerSystem::Shutdown()
{
    for ( auto& [ markerType, resource ] : MarkerResources )
    {
        (void)markerType;
        resource.Mesh.reset();
        resource.bReady = false;
    }

    MarkerResources.clear();
    Providers.clear();
    bVisible = true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Register marker provider.
//---------------------------------------------------------------------------------------------------------------------
void CRActorMarkerSystem::RegisterProvider( CRUniquePtr< ICRActorMarkerProvider > Provider )
{
    if ( !Provider ) return;

    Providers.push_back( std::move( Provider ) );
}

//---------------------------------------------------------------------------------------------------------------------
/// Register marker mesh asset path.
//---------------------------------------------------------------------------------------------------------------------
bool CRActorMarkerSystem::RegisterMarkerAsset( const CRName& MarkerType, const CRPath& AssetPath )
{
    if ( MarkerType.empty() ) return false;
    if ( AssetPath .empty() ) return false;

    CRMarkerMeshResource& resource = MarkerResources[ MarkerType ];
    resource.MarkerType = MarkerType;
    resource.AssetPath  = AssetPath;

    return _LoadMarkerMeshResource( resource );
}

//---------------------------------------------------------------------------------------------------------------------
/// Gather marker render elements.
//---------------------------------------------------------------------------------------------------------------------
void CRActorMarkerSystem::GatherRenderElements( CRArray< CRGizmoRenderElement >& OutRenderElements ) const
{
    if ( !bVisible ) return;
    if ( !GWorld ) return;

    CRArray< CRActorMarkerDescriptor > markers;
    _CollectMarkers( *GWorld, markers );

    for ( const CRActorMarkerDescriptor& marker : markers )
    {
        if ( marker.VisualKind != ECRActorMarkerVisualKind::Mesh3D ) continue;
        if ( !CRIdentity::IsValid( marker.ActorId ) ) continue;

        const auto foundResource = MarkerResources.find( marker.MarkerType );
        if ( foundResource == MarkerResources.end() ) continue;

        const CRMarkerMeshResource& resource = foundResource->second;
        if ( !resource.bReady || !resource.Mesh ) continue;

        CRGizmoRenderElement renderElement;
        renderElement.Mesh      = resource.Mesh;
        renderElement.Transform = CRMatrix::CreateFromQuaternion( marker.Rotation ) * CRMatrix::CreateTranslation( marker.Position );
        renderElement.Color     = marker.Color;
        renderElement.Pivot     = marker.Position;
        renderElement.PixelSize = marker.PixelSize;
        renderElement.Type      = 0.0f;

        OutRenderElements.push_back( renderElement );
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Try pick actor id from marker set by ray.
//---------------------------------------------------------------------------------------------------------------------
CRIdentity::id_t CRActorMarkerSystem::TryPickActorByRay( const CRRay& Ray ) const
{
    if ( !bVisible ) return CRIdentity::InvalidId;
    if ( !GWorld ) return CRIdentity::InvalidId;

    f32 viewportHeight = 1080.0f;
    if ( ICRRHIRenderer* renderer = GRHI.GetRenderer() )
    {
        viewportHeight = CRMath::Max( 1.0f, (f32)renderer->GetViewportHeight() );
    }

    CRCamera* camera = GWorld->GetCamera();
    if ( !camera ) return CRIdentity::InvalidId;

    f32 projectionCotHalfFovY = camera->GetProjectionMatrix()._22;
    if ( CRMath::IsNearlyZero( projectionCotHalfFovY ) )
    {
        projectionCotHalfFovY = 1.0f;
    }

    const f32 safeProjectionCotHalfFovY = CRMath::Abs( projectionCotHalfFovY );

    CRArray< CRActorMarkerDescriptor > markers;
    _CollectMarkers( *GWorld, markers );

    CRIdentity::id_t bestActorId = CRIdentity::InvalidId;
    i32              bestPriority = CRMath::i32_min;
    f32              bestT        = CRMath::f32_max;

    for ( const CRActorMarkerDescriptor& marker : markers )
    {
        if ( marker.VisualKind != ECRActorMarkerVisualKind::Mesh3D ) continue;
        if ( !CRIdentity::IsValid( marker.ActorId ) ) continue;

        const auto foundResource = MarkerResources.find( marker.MarkerType );
        if ( foundResource == MarkerResources.end() ) continue;

        const CRMarkerMeshResource& resource = foundResource->second;
        if ( !resource.bReady || !resource.Mesh ) continue;

        const CRVector toMarker = marker.Position - Ray.Origin;
        const f32 markerRayT = toMarker.Dot( Ray.Direction );
        if ( markerRayT < 0.0f ) continue;

        const CRVector closestPoint = Ray.Origin + Ray.Direction * markerRayT;
        const f32 markerDistanceSquared = ( marker.Position - closestPoint ).LengthSquared();

        const f32 pickRadiusPixels = CRMath::Max( marker.PickRadiusPixels, 1.0f );
        const f32 safeDistance = CRMath::Max( markerRayT, 0.01f );
        f32 markerWorldRadius = ( 2.0f * safeDistance / safeProjectionCotHalfFovY ) * ( pickRadiusPixels / viewportHeight );
        markerWorldRadius = CRMath::Clamp( markerWorldRadius, 0.0001f, 2048.0f );

        if ( markerDistanceSquared > markerWorldRadius * markerWorldRadius ) continue;

        bool bIsBetterCandidate = false;
        if ( !CRIdentity::IsValid( bestActorId ) )
        {
            bIsBetterCandidate = true;
        }
        else if ( marker.Priority > bestPriority )
        {
            bIsBetterCandidate = true;
        }
        else if ( marker.Priority == bestPriority && markerRayT < bestT )
        {
            bIsBetterCandidate = true;
        }

        if ( !bIsBetterCandidate ) continue;

        bestActorId = marker.ActorId;
        bestPriority = marker.Priority;
        bestT = markerRayT;
    }

    return bestActorId;
}

//---------------------------------------------------------------------------------------------------------------------
/// Collect markers from all providers.
//---------------------------------------------------------------------------------------------------------------------
void CRActorMarkerSystem::_CollectMarkers( const CRWorld& World, CRArray< CRActorMarkerDescriptor >& OutMarkers ) const
{
    for ( const CRUniquePtr< ICRActorMarkerProvider >& provider : Providers )
    {
        if ( !provider ) continue;
        provider->CollectMarkers( World, OutMarkers );
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Load marker config from json file.
//---------------------------------------------------------------------------------------------------------------------
bool CRActorMarkerSystem::_LoadConfigFromJson( CRMarkerSystemConfig& OutConfig ) const
{
    OutConfig.MarkerAssets.clear();
    OutConfig.LightRules  .clear();

    const CRPath configPath = _ResolveAssetPath( MarkerConfigPath );
    if ( configPath.empty() ) return _Fail( "Missing required config file: " + MarkerConfigPath.string() );

    std::ifstream ifs( configPath );
    if ( !ifs ) return _Fail( "Failed to open config file: " + configPath.string() );

    nlohmann::json root = nlohmann::json::parse( ifs, nullptr, false );
    if ( root.is_discarded() ) return _Fail( "Failed to parse json: " + configPath.string() );

    if ( !UtilJson::IsObject      ( root            ) ) return _Fail( "Config root must be a json object."           );
    if ( !UtilJson::IsIntegerField( root, "version" ) ) return _Fail( "Config must contain integer field 'version'." );

    if ( root[ "version" ].get< i32 >() != 1 )
    {
        return _Fail( "Unsupported marker config version. Expected version=1." );
    }

    if ( !UtilJson::IsValidArray( root, "marker_assets" ) ) return _Fail( "Config must contain array field 'marker_assets'." );
    if ( !UtilJson::IsValidArray( root, "light_rules"   ) ) return _Fail( "Config must contain array field 'light_rules'."   );

    if ( !_ParseMarkerAssets( root, OutConfig ) ) return false;
    if ( !_ParseLightRules  ( root, OutConfig ) ) return false;

    return _ValidateConfig( OutConfig );
}

//---------------------------------------------------------------------------------------------------------------------
/// Parse marker asset entries from json root.
//---------------------------------------------------------------------------------------------------------------------
bool CRActorMarkerSystem::_ParseMarkerAssets( const nlohmann::json& Root, CRMarkerSystemConfig& OutConfig ) const
{
    const nlohmann::json& markerAssets = Root[ "marker_assets" ];
    for ( size_t i = 0; i < markerAssets.size(); ++i )
    {
        const CRString        context = _BuildContext( "marker_assets", i );
        const nlohmann::json& asset   = markerAssets[ i ];
        if ( !asset.is_object() ) return _Fail( context + " must be an object." );

        CRMarkerAssetConfig parsedAsset;
        if ( !_GetRequiredStringField( asset, "marker_type", parsedAsset.MarkerType, context ) ) return false;

        CRString assetPath;
        if ( !_GetRequiredStringField( asset, "asset_path", assetPath, context ) ) return false;

        parsedAsset.AssetPath = CRPath( assetPath );
        OutConfig.MarkerAssets.push_back( parsedAsset );
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Parse light rule entries from json root.
//---------------------------------------------------------------------------------------------------------------------
bool CRActorMarkerSystem::_ParseLightRules( const nlohmann::json& Root, CRMarkerSystemConfig& OutConfig ) const
{
    const nlohmann::json& lightRules = Root[ "light_rules" ];
    for ( size_t i = 0; i < lightRules.size(); ++i )
    {
        const CRString context = _BuildContext( "light_rules", i );
        const nlohmann::json& rule = lightRules[ i ];
        if ( !rule.is_object() )
        {
            return _Fail( context + " must be an object." );
        }

        CRString componentName;
        if ( !_GetRequiredStringField( rule, "component", componentName, context ) ) return false;

        ECRLightMarkerComponentKind componentKind = ECRLightMarkerComponentKind::Directional;
        if ( !_TryParseComponentKind( componentName, componentKind ) )
        {
            return _Fail( context + " invalid component: " + componentName );
        }

        CRLightMarkerRule parsedRule;
        parsedRule.ComponentKind = componentKind;

        if ( !_GetRequiredStringField( rule, "marker_type",        parsedRule.MarkerType,        context ) ) return false;
        if ( !_GetRequiredColorField ( rule, "color",              parsedRule.Color,             context ) ) return false;
        if ( !_GetRequiredFloatField ( rule, "pixel_size",         parsedRule.PixelSize,         context ) ) return false;
        if ( !_GetRequiredFloatField ( rule, "pick_radius_pixels", parsedRule.PickRadiusPixels,  context ) ) return false;
        if ( !_GetRequiredIntField   ( rule, "priority",           parsedRule.Priority,          context ) ) return false;
        if ( !_GetRequiredBoolField  ( rule, "use_actor_rotation", parsedRule.bUseActorRotation, context ) ) return false;

        if ( parsedRule.PixelSize <= 0.0f || parsedRule.PickRadiusPixels <= 0.0f )
        {
            return _Fail( context + " pixel_size and pick_radius_pixels must be > 0." );
        }

        OutConfig.LightRules.push_back( parsedRule );
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Validate parsed config.
//---------------------------------------------------------------------------------------------------------------------
bool CRActorMarkerSystem::_ValidateConfig( const CRMarkerSystemConfig& Config ) const
{
    if ( Config.MarkerAssets.empty() ) return _Fail( "Config marker_assets must not be empty." );
    if ( Config.LightRules  .empty() ) return _Fail( "Config light_rules must not be empty."   );

    std::unordered_set< CRName > markerTypes;
    for ( const CRMarkerAssetConfig& asset : Config.MarkerAssets )
    {
        if ( asset.MarkerType.empty() || asset.AssetPath.empty() )
        {
            return _Fail( "marker_assets contains empty marker_type/asset_path." );
        }

        if ( markerTypes.contains( asset.MarkerType ) )
        {
            return _Fail( "Duplicate marker_type in marker_assets: " + asset.MarkerType );
        }

        markerTypes.insert( asset.MarkerType );
    }

    for ( const CRLightMarkerRule& rule : Config.LightRules )
    {
        if ( !markerTypes.contains( rule.MarkerType ) )
        {
            return _Fail( "light_rules references missing marker_type: " + rule.MarkerType );
        }
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Apply validated config.
//---------------------------------------------------------------------------------------------------------------------
bool CRActorMarkerSystem::_ApplyConfig( const CRMarkerSystemConfig& Config )
{
    for ( const CRMarkerAssetConfig& markerAsset : Config.MarkerAssets )
    {
        if ( !RegisterMarkerAsset( markerAsset.MarkerType, markerAsset.AssetPath ) )
        {
            return _Fail( "Failed to register marker asset: " + markerAsset.MarkerType );
        }
    }

    CRUniquePtr< CRLightActorMarkerProvider > lightProvider = CRMakeUnique( new CRLightActorMarkerProvider() );
    if ( !lightProvider )
    {
        return _Fail( "Failed to create light marker provider." );
    }

    if ( !lightProvider->InitializeRules( Config.LightRules ) )
    {
        return _Fail( "Failed to initialize light marker rules." );
    }

    RegisterProvider( std::move( lightProvider ) );

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Resolve marker asset path.
//---------------------------------------------------------------------------------------------------------------------
CRPath CRActorMarkerSystem::_ResolveAssetPath( const CRPath& AssetPath )
{
    if ( AssetPath.empty() ) return {};

    return UtilPath::ResolveExistingEnginePath( AssetPath );
}

//---------------------------------------------------------------------------------------------------------------------
/// Load marker mesh resource.
//---------------------------------------------------------------------------------------------------------------------
bool CRActorMarkerSystem::_LoadMarkerMeshResource( CRMarkerMeshResource& Resource )
{
    Resource.Mesh.reset();
    Resource.ResolvedPath.clear();
    Resource.bReady = false;

    const CRPath resolvedPath = _ResolveAssetPath( Resource.AssetPath );
    if ( resolvedPath.empty() )
    {
        return _Fail( "Failed to resolve marker asset path: " + Resource.AssetPath.string() );
    }

    CRPrimitiveAsset primitiveAsset;
    primitiveAsset.Load( resolvedPath );

    if ( primitiveAsset.VertexCount == 0 )
    {
        return _Fail( "Marker asset has no vertices: " + resolvedPath.string() );
    }

    if ( primitiveAsset.Positions.size() != primitiveAsset.VertexCount ||
         primitiveAsset.Normals  .size() != primitiveAsset.VertexCount ||
         primitiveAsset.UVs      .size() != primitiveAsset.VertexCount )
    {
        return _Fail( "Marker asset channel size mismatch: " + resolvedPath.string() );
    }

    ICRRHIMeshSPtr mesh = GRHI.CreateMesh();
    if ( !mesh )
    {
        return _Fail( "Failed to create marker mesh resource: " + resolvedPath.string() );
    }

    mesh->InitializePrimitive( resolvedPath.string(), primitiveAsset );

    Resource.Mesh         = mesh;
    Resource.ResolvedPath = resolvedPath;
    Resource.bReady       = true;

    return true;
}
