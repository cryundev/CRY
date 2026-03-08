#pragma once

#include "CRActorMarkerProvider.h"
#include "CRGizmoActor.h"
#include "Extras/nlohmann/json.hpp" // nlohmann/json v3.11.x (pinned)
#include "Source/Core/CRSmartPtrMacro.h"
#include "Source/Core/CRTypes.h"
#include "Source/RHI/CRRHITypes.h"


struct CRRay;
class CRWorld;


//---------------------------------------------------------------------------------------------------------------------
/// CRActorMarkerSystem
//---------------------------------------------------------------------------------------------------------------------
class CRActorMarkerSystem
{
private:
    struct CRMarkerAssetConfig
    {
        CRName MarkerType = {};
        CRPath AssetPath  = {};
    };

    struct CRMarkerSystemConfig
    {
        CRArray< CRMarkerAssetConfig > MarkerAssets;
        CRArray< CRLightMarkerRule >   LightRules;
    };

private:
    struct CRMarkerMeshResource
    {
        CRName         MarkerType   = {};
        CRPath         AssetPath    = {};
        CRPath         ResolvedPath = {};
        ICRRHIMeshSPtr Mesh         = nullptr;
        bool           bReady       = false;
    };

private:
    bool                                             bVisible = true;
    CRMap< CRName, CRMarkerMeshResource >            MarkerResources;
    CRArray< CRUniquePtr< ICRActorMarkerProvider > > Providers;

public:
    /// Initialize.
    bool Initialize();

    /// Shutdown.
    void Shutdown();

    /// Set visibility.
    void SetVisible( bool bInVisible ) { bVisible = bInVisible; }

    /// Is visible.
    bool IsVisible() const { return bVisible; }

    /// Register marker provider.
    void RegisterProvider( CRUniquePtr< ICRActorMarkerProvider > Provider );

    /// Register marker mesh asset path.
    bool RegisterMarkerAsset( const CRName& MarkerType, const CRPath& AssetPath );

    /// Gather marker render elements.
    void GatherRenderElements( CRArray< CRGizmoRenderElement >& OutRenderElements ) const;

    /// Try pick actor id from marker set by ray.
    CRIdentity::id_t TryPickActorByRay( const CRRay& Ray ) const;

private:
    /// Collect markers from all providers.
    void _CollectMarkers( const CRWorld& World, CRArray< CRActorMarkerDescriptor >& OutMarkers ) const;

    /// Load marker config from json file.
    bool _LoadConfigFromJson( CRMarkerSystemConfig& OutConfig ) const;

    /// Parse marker asset entries from json root.
    bool _ParseMarkerAssets( const nlohmann::json& Root, CRMarkerSystemConfig& OutConfig ) const;

    /// Parse light rule entries from json root.
    bool _ParseLightRules( const nlohmann::json& Root, CRMarkerSystemConfig& OutConfig ) const;

    /// Validate parsed config.
    bool _ValidateConfig( const CRMarkerSystemConfig& Config ) const;

    /// Apply validated config.
    bool _ApplyConfig( const CRMarkerSystemConfig& Config );

    /// Resolve marker asset path.
    static CRPath _ResolveAssetPath( const CRPath& AssetPath );

    /// Load marker mesh resource.
    bool _LoadMarkerMeshResource( CRMarkerMeshResource& Resource );
};


extern CRActorMarkerSystem GActorMarkerSystem;
