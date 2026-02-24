#include "CRD11GizmoPass.h"
#include "../CRD11.h"
#include "../CRD11RenderingPipeline.h"
#include "../CRD11ResourceManager.h"
#include "../Resource/CRD11CompiledShader.h"
#include "../Resource/CRD11DepthStencilState.h"
#include "../Resource/CRD11DepthStencilView.h"
#include "../Resource/CRD11InputLayout.h"
#include "../Resource/CRD11PixelShader.h"
#include "../Resource/CRD11RasterizerState.h"
#include "../Resource/CRD11RenderTargetView.h"
#include "../Resource/CRD11VertexShader.h"
#include "Source/Utility/Log/CRLog.h"
#include <filesystem>


//---------------------------------------------------------------------------------------------------------------------
/// Initialize pass resources.
//---------------------------------------------------------------------------------------------------------------------
void CRD11GizmoPass::Initialize( u32 Width, u32 Height )
{
    Resize( Width, Height );
}

//---------------------------------------------------------------------------------------------------------------------
/// Resize pass resources.
//---------------------------------------------------------------------------------------------------------------------
void CRD11GizmoPass::Resize( u32 /*Width*/, u32 /*Height*/ )
{
    Release();

    _CreateStates();
    _CreateShadersAndLayout();

    Buffer.Create( "GizmoBuffer", (u32)( EConstBufferSlotVS::Gizmo ), ED11RenderingPipelineStage::VS );

    CRGizmoVSData gizmoData;
    gizmoData.GizmoTransform = CRMatrix::Identity.Transpose();
    gizmoData.GizmoColor     = CRVector4D( 1.0f, 1.0f, 0.0f, 1.0f );

    Buffer.Update( gizmoData );
}

//---------------------------------------------------------------------------------------------------------------------
/// Prepare gizmo pass render state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11GizmoPass::OnPreDraw()
{
    bHasCapturedPipelineState = false;
    if ( !_BindSceneTargets() ) return;
    
    GD11RP.CapturePipelineStates();
    bHasCapturedPipelineState = true;

    if ( !DepthState  .expired() ) GD11RP.SetDepthStencilState( DepthState  .lock()->GetObjectPtr()    );
    if ( !RasterState .expired() ) GD11RP.SetRasterizerState  ( RasterState .lock()->GetObjectPtr()    );
    if ( !InputLayout .expired() ) GD11RP.SetInputLayout      ( InputLayout .lock()->GetObjectPtr() );
    if ( !VertexShader.expired() ) GD11RP.SetVertexShader     ( VertexShader.lock()->GetObjectPtr()    );
    if ( !PixelShader .expired() ) GD11RP.SetPixelShader      ( PixelShader .lock()->GetObjectPtr()    );

    Buffer.SetInRenderingPipeline();
}

//---------------------------------------------------------------------------------------------------------------------
/// Draw gizmo elements.
//---------------------------------------------------------------------------------------------------------------------
void CRD11GizmoPass::OnDrawRenderElements( const CRPackedArray< CRRenderElement >& RenderElements )
{
    (void)RenderElements;

    _DrawGizmoElementsFromSystem();
}

//---------------------------------------------------------------------------------------------------------------------
/// Restore pass-local state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11GizmoPass::OnPostDraw()
{
    if ( !bHasCapturedPipelineState ) return;

    GD11RP.SetShaderResourceView( nullptr, 0, ED11RenderingPipelineStage::PS );
    GD11RP.RestorePipelineStates();
    bHasCapturedPipelineState = false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Release pass resources.
//---------------------------------------------------------------------------------------------------------------------
void CRD11GizmoPass::Release()
{
    bHasCapturedPipelineState = false;

    if ( !DepthState  .expired() ) DepthState  .lock()->SetObjectPtr( nullptr );
    if ( !RasterState .expired() ) RasterState .lock()->SetObjectPtr( nullptr );
    if ( !VertexShader.expired() ) VertexShader.lock()->SetObjectPtr( nullptr );
    if ( !PixelShader .expired() ) PixelShader .lock()->SetObjectPtr( nullptr );
    if ( !InputLayout .expired() ) InputLayout .lock()->SetObjectPtr( nullptr );

    if ( !Buffer.GetConstantBuffer().expired() )
    {
        Buffer.GetConstantBuffer().lock()->SetObjectPtr( nullptr );
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Create depth/rasterizer states.
//---------------------------------------------------------------------------------------------------------------------
bool CRD11GizmoPass::_CreateStates()
{
    DepthState = GD11RM.GetDepthStencilState( "GizmoDepth" );
    if ( DepthState.expired() )
    {
        GLog.AddLog( "[CRD11GizmoPass] Failed to get gizmo depth state resource." );
        return false;
    }

    D3D11_DEPTH_STENCIL_DESC dsd;
    ZeroMemory( &dsd, sizeof( D3D11_DEPTH_STENCIL_DESC ) );

    dsd.DepthEnable    = true;
    dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    dsd.DepthFunc      = D3D11_COMPARISON_LESS_EQUAL;

    DepthState.lock()->Create( dsd );

    RasterState = GD11RM.GetRasterizerState( "GizmoCullNone" );
    if ( RasterState.expired() )
    {
        GLog.AddLog( "[CRD11GizmoPass] Failed to get gizmo rasterizer state resource." );
        return false;
    }

    D3D11_RASTERIZER_DESC rd;
    ZeroMemory( &rd, sizeof( D3D11_RASTERIZER_DESC ) );

    rd.CullMode = D3D11_CULL_NONE;
    rd.FillMode = D3D11_FILL_SOLID;
    rd.FrontCounterClockwise = false;

    RasterState.lock()->Create( rd );

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Create gizmo shader and input layout resources.
//---------------------------------------------------------------------------------------------------------------------
bool CRD11GizmoPass::_CreateShadersAndLayout()
{
    std::filesystem::path shaderPath = std::filesystem::path( __FILE__ ).parent_path() / "../HLSL/gizmo.hlsl";
    if ( !std::filesystem::exists( shaderPath ) )
    {
        GLog.AddLog( "[CRD11GizmoPass] Failed to find gizmo shader file." );
        return false;
    }

    CRD11CompiledShader compiledVS;
    CRD11CompiledShader compiledPS;
    compiledVS.Create( shaderPath.wstring(), "VS", "vs_5_0" );
    compiledPS.Create( shaderPath.wstring(), "PS", "ps_5_0" );

    if ( !compiledVS.GetObjectPtr() || !compiledPS.GetObjectPtr() )
    {
        GLog.AddLog( "[CRD11GizmoPass] Failed to compile gizmo shader." );
        return false;
    }

    VertexShader = GD11RM.GetVertexShader( "Gizmo" );
    PixelShader  = GD11RM.GetPixelShader ( "Gizmo" );
    InputLayout  = GD11RM.GetInputLayout ( "Gizmo" );

    if ( VertexShader.expired() || PixelShader.expired() || InputLayout.expired() )
    {
        GLog.AddLog( "[CRD11GizmoPass] Failed to get gizmo shader resources." );
        return false;
    }

    VertexShader.lock()->Create( compiledVS.GetObjectPtr() );
    PixelShader .lock()->Create( compiledPS.GetObjectPtr() );

    D3D11_INPUT_ELEMENT_DESC elements[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    InputLayout.lock()->Create( elements, ARRAYSIZE( elements ), compiledVS.GetObjectPtr() );

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Bind scene render/depth targets for overlay drawing.
//---------------------------------------------------------------------------------------------------------------------
bool CRD11GizmoPass::_BindSceneTargets() const
{
    CRD11RenderTargetViewSPtr sceneRTV = GD11RM.GetRenderTargetView( "SceneColor_RTV" );
    if ( !sceneRTV || !sceneRTV->GetObjectPtr() )
    {
        return false;
    }

    CRD11DepthStencilViewSPtr sceneDSV = GD11RM.GetDepthStencilView( "DepthStencilView" );

    GD11RP.SetRenderTargetView( sceneRTV->GetObjectPtr(), sceneDSV ? sceneDSV->GetObjectPtr() : nullptr );

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Draw gizmo elements from gizmo system (Phase2-1 stub).
//---------------------------------------------------------------------------------------------------------------------
void CRD11GizmoPass::_DrawGizmoElementsFromSystem()
{
}
