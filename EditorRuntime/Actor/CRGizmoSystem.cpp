#include "CRGizmoSystem.h"
#include "CRAxisGizmoActor.h"
#include "Engine.h"
#include "Source/Asset/CRPrimitiveAsset.h"
#include "Source/Utility/Log/CRLog.h"
#include "Source/World/CRWorld.h"
#include <filesystem>


CRGizmoSystem GGizmoSystem;


namespace
{
//---------------------------------------------------------------------------------------------------------------------
/// Resolve prebuilt gizmo arrow asset path.
//---------------------------------------------------------------------------------------------------------------------
std::filesystem::path _ResolveGizmoArrowAssetPath()
{
    std::filesystem::path assetPath = std::filesystem::path( __FILE__ ).parent_path() / "../../Asset/Gizmo/Arrow.cra";
    if ( std::filesystem::exists( assetPath ) )
    {
        return assetPath.lexically_normal();
    }

    std::filesystem::path currentPath = std::filesystem::current_path();
    for ( i32 pathDepth = 0; pathDepth < 8; ++pathDepth )
    {
        const std::filesystem::path candidate = currentPath / "Asset/Gizmo/Arrow.cra";
        if ( std::filesystem::exists( candidate ) )
        {
            return candidate.lexically_normal();
        }

        if ( !currentPath.has_parent_path() )
        {
            break;
        }

        currentPath = currentPath.parent_path();
    }

    return {};
}
}


//---------------------------------------------------------------------------------------------------------------------
/// Initialize.
//---------------------------------------------------------------------------------------------------------------------
bool CRGizmoSystem::Initialize()
{
    Shutdown();

    const std::filesystem::path assetPath = _ResolveGizmoArrowAssetPath();
    if ( assetPath.empty() ) return false;

    CRPrimitiveAsset primitiveAsset;
    primitiveAsset.Load( assetPath.string() );

    if ( primitiveAsset.VertexCount == 0 )
    {
        GLog.AddLog( "[CRGizmoSystem] Gizmo/Arrow.cra has no vertices. Gizmo rendering disabled." );
        return false;
    }

    if ( primitiveAsset.Positions.size() != primitiveAsset.VertexCount ||
         primitiveAsset.Normals  .size() != primitiveAsset.VertexCount ||
         primitiveAsset.UVs      .size() != primitiveAsset.VertexCount )
    {
        GLog.AddLog( "[CRGizmoSystem] Gizmo/Arrow.cra channel size mismatch. Gizmo rendering disabled." );
        return false;
    }

    if ( !GWorld )
    {
        GLog.AddLog( "[CRGizmoSystem] GWorld is not available. Gizmo actor not spawned." );
        return true;
    }

    GizmoActor = GWorld->SpawnActor< CRAxisGizmoActor >();
    if ( !GizmoActor )
    {
        GLog.AddLog( "[CRGizmoSystem] Failed to spawn CRAxisGizmoActor." );
        return true;
    }

    if ( !GizmoActor->InitializeGizmo( assetPath.string(), primitiveAsset ) )
    {
        GLog.AddLog( "[CRGizmoSystem] Failed to initialize gizmo actor resources." );
    }

    bVisible = false;

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Shutdown.
//---------------------------------------------------------------------------------------------------------------------
void CRGizmoSystem::Shutdown()
{
    if ( GizmoActor && GWorld )
    {
        GWorld->DespawnActor( GizmoActor->GetObjectId() );
        GizmoActor = nullptr;
    }

    bVisible = false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Is ready.
//---------------------------------------------------------------------------------------------------------------------
bool CRGizmoSystem::IsReady() const
{
    if ( !GizmoActor ) return false;

    const u32 renderElementCount = GizmoActor->GetRenderElementCount();
    if ( renderElementCount == 0 ) return false;

    for ( u32 elementIndex = 0; elementIndex < renderElementCount; ++elementIndex )
    {
        CRGizmoRenderElement renderElement;
        if ( !GizmoActor->GetRenderElement( elementIndex, renderElement ) ) return false;
        if ( !renderElement.Mesh ) return false;
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Set pivot.
//---------------------------------------------------------------------------------------------------------------------
void CRGizmoSystem::SetPivot( const CRVector& InPivot )
{
    if ( GizmoActor )
    {
        GizmoActor->SetPivot( InPivot );
    }

    if ( IsReady() )
    {
        bVisible = true;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Get pivot.
//---------------------------------------------------------------------------------------------------------------------
CRVector CRGizmoSystem::GetPivot() const
{
    return GizmoActor ? GizmoActor->GetPivot() : CRVector::Zero;
}
