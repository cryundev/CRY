#include "CRD11OverlayPass.h"
#include "Engine.h"
#include "Source/RHI/DX11/CRD11.h"
#include "Source/RHI/DX11/CRD11RenderingPipeline.h"
#include "Source/RHI/DX11/CRD11ResourceManager.h"
#include "Source/RHI/DX11/Resource/CRD11CompiledShader.h"
#include "Source/RHI/DX11/Resource/CRD11DepthStencilState.h"
#include "Source/RHI/DX11/Resource/CRD11InputLayout.h"
#include "Source/RHI/DX11/Resource/CRD11PixelShader.h"
#include "Source/RHI/DX11/Resource/CRD11RasterizerState.h"
#include "Source/RHI/DX11/Resource/CRD11RenderTargetView.h"
#include "Source/RHI/DX11/Resource/CRD11VertexShader.h"
#include "Source/Object/Camera/CRCamera.h"
#include "Source/RHI/CRRHI.h"
#include "Source/RHI/ICRRHIMesh.h"
#include "Source/RHI/ICRRHIRenderer.h"
#include "Source/Utility/Log/CRLog.h"
#include "Source/Utility/UtilPath.h"
#include "Source/World/CRWorld.h"
#include <filesystem>


//---------------------------------------------------------------------------------------------------------------------
/// Initialize pass resources.
//---------------------------------------------------------------------------------------------------------------------
void CRD11OverlayPass::Initialize( u32 Width, u32 Height )
{
    Resize( Width, Height );
}

//---------------------------------------------------------------------------------------------------------------------
/// Resize pass resources.
//---------------------------------------------------------------------------------------------------------------------
void CRD11OverlayPass::Resize( u32 /*Width*/, u32 /*Height*/ )
{
    Release();

    const CROverlayPassConfig config = GetOverlayConfig();
    if ( !_CreateStates( config ) ) return;
    if ( !_CreateShadersAndLayout( config ) )
    {
        Release();
        return;
    }

    Buffer.Create( config.ConstantBufferName, (u32)( EConstBufferSlotVS::Gizmo ), ED11RenderingPipelineStage::VS );

    CROverlayVSConstants overlayData;
    overlayData.GizmoTransform = CRMatrix::Identity.Transpose();
    overlayData.GizmoColor     = config.DefaultColor;
    overlayData.GizmoPixelSize = config.DefaultPixelSize;

    Buffer.Update( overlayData );

    bReady = true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Prepare overlay pass render state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11OverlayPass::OnPreDraw()
{
    bHasCapturedPipelineState = false;
    if ( !bReady ) return;
    if ( !_BindSceneTargets() ) return;

    GD11RP.CapturePipelineStates();
    bHasCapturedPipelineState = true;

    if ( !DepthState  .expired() ) GD11RP.SetDepthStencilState( DepthState  .lock()->GetObjectPtr() );
    if ( !RasterState .expired() ) GD11RP.SetRasterizerState  ( RasterState .lock()->GetObjectPtr() );
    if ( !InputLayout .expired() ) GD11RP.SetInputLayout      ( InputLayout .lock()->GetObjectPtr() );
    if ( !VertexShader.expired() ) GD11RP.SetVertexShader     ( VertexShader.lock()->GetObjectPtr() );
    if ( !PixelShader .expired() ) GD11RP.SetPixelShader      ( PixelShader .lock()->GetObjectPtr() );

    Buffer.SetInRenderingPipeline();
}

//---------------------------------------------------------------------------------------------------------------------
/// Draw overlay elements.
//---------------------------------------------------------------------------------------------------------------------
void CRD11OverlayPass::OnDrawRenderElements( const CRPackedArray< CRRenderElement >& RenderElements )
{
    (void)RenderElements;

    if ( !bReady ) return;
    if ( !CanDrawOverlay() ) return;

    DrawOverlayElements();
}

//---------------------------------------------------------------------------------------------------------------------
/// Restore pass-local state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11OverlayPass::OnPostDraw()
{
    if ( !bHasCapturedPipelineState ) return;

    GD11RP.SetShaderResourceView( nullptr, 0, ED11RenderingPipelineStage::PS );
    GD11RP.RestorePipelineStates();

    bHasCapturedPipelineState = false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Release pass resources.
//---------------------------------------------------------------------------------------------------------------------
void CRD11OverlayPass::Release()
{
    bReady = false;
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
/// Build common camera parameters.
//---------------------------------------------------------------------------------------------------------------------
CROverlayCameraParams CRD11OverlayPass::BuildCameraParams() const
{
    CROverlayCameraParams cameraParams;

    if ( ICRRHIRenderer* renderer = GRHI.GetRenderer() )
    {
        cameraParams.ViewportHeight = CRMath::Max( 1.0f, (f32)renderer->GetViewportHeight() );
    }

    if ( GWorld )
    {
        if ( CRCamera* camera = GWorld->GetCamera() )
        {
            cameraParams.ProjectionCotHalfFovY = camera->GetProjectionMatrix()._22;
        }
    }

    if ( CRMath::IsNearlyZero( cameraParams.ProjectionCotHalfFovY ) )
    {
        cameraParams.ProjectionCotHalfFovY = 1.0f;
    }

    return cameraParams;
}

//---------------------------------------------------------------------------------------------------------------------
/// Draw one overlay mesh using common constants.
//---------------------------------------------------------------------------------------------------------------------
void CRD11OverlayPass::DrawOverlayMesh( const CRGizmoRenderElement& Element, const CROverlayCameraParams& CameraParams, f32 DefaultPixelSize )
{
    if ( !Element.Mesh ) return;

    const f32 pixelSize = ( Element.PixelSize > 0.0f ) ? Element.PixelSize : DefaultPixelSize;

    CROverlayVSConstants overlayData;
    overlayData.GizmoTransform        = Element.Transform.Transpose();
    overlayData.GizmoColor            = Element.Color;
    overlayData.GizmoPivot            = CRVector4D( Element.Pivot.x, Element.Pivot.y, Element.Pivot.z, 1.0f );
    overlayData.GizmoPixelSize        = pixelSize;
    overlayData.ViewportHeight        = CameraParams.ViewportHeight;
    overlayData.ProjectionCotHalfFovY = CameraParams.ProjectionCotHalfFovY;
    overlayData.AxisType              = Element.Type;

    Buffer.Update( overlayData );

    Element.Mesh->SetInRenderingPipeline();
    Element.Mesh->Draw();
}

//---------------------------------------------------------------------------------------------------------------------
/// Create depth/rasterizer states.
//---------------------------------------------------------------------------------------------------------------------
bool CRD11OverlayPass::_CreateStates( const CROverlayPassConfig& Config )
{
    DepthState = GD11RM.GetDepthStencilState( Config.DepthStateName );
    if ( DepthState.expired() )
    {
        GLog.AddLog( Config.LogPrefix + " Failed to get depth state resource." );
        return false;
    }

    D3D11_DEPTH_STENCIL_DESC dsd;
    ZeroMemory( &dsd, sizeof( D3D11_DEPTH_STENCIL_DESC ) );

    dsd.DepthEnable    = false;
    dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    dsd.DepthFunc      = D3D11_COMPARISON_ALWAYS;

    DepthState.lock()->Create( dsd );

    RasterState = GD11RM.GetRasterizerState( Config.RasterStateName );
    if ( RasterState.expired() )
    {
        GLog.AddLog( Config.LogPrefix + " Failed to get rasterizer state resource." );
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
/// Create shader and input layout resources.
//---------------------------------------------------------------------------------------------------------------------
bool CRD11OverlayPass::_CreateShadersAndLayout( const CROverlayPassConfig& Config )
{
    const CRPath shaderPath = UtilPath::ResolveExistingEnginePath( CRPath( "EditorRuntime/Render/Shader" ) / Config.ShaderFileName );
    if ( shaderPath.empty() )
    {
        GLog.AddLog( Config.LogPrefix + " Failed to find shader file." );
        return false;
    }

    CRD11CompiledShader compiledVS;
    CRD11CompiledShader compiledPS;

    compiledVS.Create( shaderPath.wstring(), "VS", "vs_5_0" );
    compiledPS.Create( shaderPath.wstring(), "PS", "ps_5_0" );

    if ( !compiledVS.GetObjectPtr() || !compiledPS.GetObjectPtr() )
    {
        GLog.AddLog( Config.LogPrefix + " Failed to compile shader." );
        return false;
    }

    VertexShader = GD11RM.GetVertexShader( Config.ShaderName );
    PixelShader  = GD11RM.GetPixelShader ( Config.ShaderName );
    InputLayout  = GD11RM.GetInputLayout ( Config.ShaderName );

    if ( VertexShader.expired() || PixelShader.expired() || InputLayout.expired() )
    {
        GLog.AddLog( Config.LogPrefix + " Failed to get shader resources." );
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
/// Bind back-buffer render target.
//---------------------------------------------------------------------------------------------------------------------
bool CRD11OverlayPass::_BindSceneTargets() const
{
    CRD11RenderTargetViewSPtr backBufferRTV = GD11RM.GetRenderTargetView( "BackBuffer" );
    if ( !backBufferRTV || !backBufferRTV->GetObjectPtr() ) return false;

    GD11RP.SetRenderTargetView( backBufferRTV->GetObjectPtr(), nullptr );

    return true;
}
