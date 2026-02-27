#include "CRGizmoSystem.h"
#include "Source/Asset/CRPrimitiveAsset.h"
#include "Source/RHI/CRRHI.h"
#include "Source/RHI/ICRRHIMesh.h"
#include "Source/Utility/Log/CRLog.h"
#include <filesystem>


CRGizmoSystem GGizmoSystem;


namespace
{
//---------------------------------------------------------------------------------------------------------------------
/// Resolve prebuilt gizmo arrow asset path.
//---------------------------------------------------------------------------------------------------------------------
std::filesystem::path _ResolveGizmoArrowAssetPath()
{
    std::filesystem::path assetPath = std::filesystem::path( __FILE__ ).parent_path() / "../../../../Asset/Gizmo/Arrow.cra";
    if ( std::filesystem::exists( assetPath ) )
    {
        return assetPath.lexically_normal();
    }

    // Runtime working directory can be x64/DebugEditor or Editor_WPF/bin/... .
    // Search current directory and parent hierarchy for Asset/Gizmo/Arrow.cra.
    std::filesystem::path currentPath = std::filesystem::current_path();
    for ( i32 i = 0; i < 8; ++i )
    {
        const std::filesystem::path candidate = currentPath / "Asset/Gizmo/Arrow.cra";
        if ( std::filesystem::exists( candidate ) )
        {
            return candidate.lexically_normal();
        }

        if ( !currentPath.has_parent_path() ) break;
        currentPath = currentPath.parent_path();
    }

    return {};
}
}


//---------------------------------------------------------------------------------------------------------------------
/// Initialize gizmo resources (loads prebuilt CRA asset).
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

    ICRRHIMeshSPtr arrowMesh = GRHI.CreateMesh();
    if ( !arrowMesh ) return false;

    arrowMesh->InitializePrimitive( "GizmoArrowMesh", primitiveAsset );
    ArrowMesh = arrowMesh;

    Pivot    = CRVector::Zero;
    bVisible = false;

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Release gizmo resources.
//---------------------------------------------------------------------------------------------------------------------
void CRGizmoSystem::Shutdown()
{
    ArrowMesh.reset();

    Pivot    = CRVector::Zero;
    bVisible = false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get gizmo arrow mesh.
//---------------------------------------------------------------------------------------------------------------------
ICRRHIMeshSPtr CRGizmoSystem::GetArrowMesh() const
{
    return ArrowMesh.lock();
}

//---------------------------------------------------------------------------------------------------------------------
/// Set pivot and make gizmo visible.
//---------------------------------------------------------------------------------------------------------------------
void CRGizmoSystem::SetPivot( const CRVector& InPivot )
{
    Pivot = InPivot;

    if ( IsReady() )
    {
        bVisible = true;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Create axis world transform.
//---------------------------------------------------------------------------------------------------------------------
CRMatrix CRGizmoSystem::CreateAxisTransform( ECRGizmoAxis Axis ) const
{
    CRMatrix rotation = CRMatrix::Identity;

    switch ( Axis )
    {
        case ECRGizmoAxis::X: rotation = CRMatrix::Identity; break;
        case ECRGizmoAxis::Y: rotation = CRMatrix::CreateFromAxisAngle( CRVector( 0.f,  0.f, 1.f ), DirectX::XM_PIDIV2 ); break;
        case ECRGizmoAxis::Z: rotation = CRMatrix::CreateFromAxisAngle( CRVector( 0.f, -1.f, 0.f ), DirectX::XM_PIDIV2 ); break;
        default: break;
    }

    return rotation * CRMatrix::CreateTranslation( Pivot );
}

//---------------------------------------------------------------------------------------------------------------------
/// Get axis render color.
//---------------------------------------------------------------------------------------------------------------------
CRVector4D CRGizmoSystem::GetAxisColor( ECRGizmoAxis Axis ) const
{
    switch ( Axis )
    {
        case ECRGizmoAxis::X: return CRVector4D( 1.0f, 0.25f, 0.25f, 1.0f );
        case ECRGizmoAxis::Y: return CRVector4D( 0.30f, 1.0f, 0.30f, 1.0f );
        case ECRGizmoAxis::Z: return CRVector4D( 0.30f, 0.55f, 1.0f, 1.0f );
    }

    return CRVector4D( 1.0f, 1.0f, 1.0f, 1.0f );
}
