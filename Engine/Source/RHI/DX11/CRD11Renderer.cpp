#include "CRD11Renderer.h"
#include "CRD11.h"
#include "CRD11RenderingPipeline.h"
#include "CRD11ResourceManager.h"
#include "Engine.h"
#include "Passes/CRD11CompositePass.h"
#include "Passes/CRD11ScenePass.h"
#include "Resource/CRD11Device.h"
#include "Resource/CRD11RasterizerState.h"
#include "Source/World/CRWorld.h"
#include <utility>


//---------------------------------------------------------------------------------------------------------------------
/// Destructor.
//---------------------------------------------------------------------------------------------------------------------
CRD11Renderer::~CRD11Renderer()
{
    _ReleaseRenderPasses();
}

//---------------------------------------------------------------------------------------------------------------------
/// Initialize renderer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::Initialize( u32 Width, u32 Height )
{
    ViewportWidth  = Width;
    ViewportHeight = Height;

    RenderElements.Clear();
    _ReleaseRenderPasses();

    if ( CRRenderPassPtr scenePass = CRMakeUnique( new CRD11ScenePass() ) )
    {
        scenePass->Initialize( Width, Height );
        RenderPasses.push_back( std::move( scenePass ) );
    }

    if ( CRRenderPassPtr compositePass = CRMakeUnique( new CRD11CompositePass() ) )
    {
        compositePass->Initialize( Width, Height );
        RenderPasses.push_back( std::move( compositePass ) );
    }

    _InitializeViewport( (f32)( Width ), (f32)( Height ) );

    TransformBuffer.Create( "Transform", (u32)( EConstBufferSlotVS::Transform ), ED11RenderingPipelineStage::VS );
    TransformBuffer.SetInRenderingPipeline();

    TransformBuffer.Update( DirectX::XMMatrixTranspose( CRMatrix::Identity ) );

    ViewProjectionBuffer.Create( "ViewProjection", (u32)( EConstBufferSlotVS::ViewProjection ), ED11RenderingPipelineStage::VS );
    ViewProjectionBuffer.SetInRenderingPipeline();

    LightsBuffer.Create( "LightsBuffer", (u32)( EConstBufferSlotPS::LightProperties ), ED11RenderingPipelineStage::PS );
    LightsBuffer.SetInRenderingPipeline();

    LightsData.AmbientColor     = CRVector4D( 0.1f, 0.1f, 0.1f, 1.0f );
    LightsData.DirectionalCount = 0;
    LightsData.PointCount       = 0;
    LightsData.SpotCount        = 0;

    _FlushLightsBuffer();

    CameraPropertiesBuffer.Create( "CameraProperties", (u32)( EConstBufferSlotPS::CameraProperties ), ED11RenderingPipelineStage::PS );
    CameraPropertiesBuffer.SetInRenderingPipeline();

    RasterizerState = GD11RM.GetRasterizerState( "Default" );
    if ( !RasterizerState.expired() )
    {
        D3D11_RASTERIZER_DESC rd;
        ZeroMemory( &rd, sizeof( D3D11_RASTERIZER_DESC ) );

        rd.CullMode = D3D11_CULL_BACK;
        rd.FillMode = D3D11_FILL_SOLID;
        rd.FrontCounterClockwise = false;

        RasterizerState.lock()->Create( rd );

        GD11RP.SetRasterizerState( RasterizerState.lock()->GetObjectPtr() );
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Resize renderer without clearing render elements.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::Resize( u32 Width, u32 Height )
{
    ViewportWidth  = Width;
    ViewportHeight = Height;

    for ( const CRRenderPassPtr& renderPass : RenderPasses )
    {
        if ( renderPass )
        {
            renderPass->Resize( Width, Height );
        }
    }

    _InitializeViewport( (f32)Width, (f32)Height );
}

//---------------------------------------------------------------------------------------------------------------------
/// Add render element.
//---------------------------------------------------------------------------------------------------------------------
CRRenderElementHandle CRD11Renderer::AddRenderElement( const CRRenderElement& RenderElement )
{
    if ( RenderElement.Mesh    .expired() ) return {};
    if ( RenderElement.Material.expired() ) return {};

    return RenderElements.Insert( RenderElement );
}

//---------------------------------------------------------------------------------------------------------------------
/// Remove render element.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::RemoveRenderElement( const CRRenderElementHandle& Handle )
{
    RenderElements.Remove( Handle );
}

//---------------------------------------------------------------------------------------------------------------------
/// Update transform buffer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::UpdateTransformBuffer( const CRMatrix& Matrix )
{
    TransformBuffer.Update( DirectX::XMMatrixTranspose( Matrix ) );
}

//---------------------------------------------------------------------------------------------------------------------
/// Update view projection buffer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::UpdateViewProjectionBuffer( const CRMatrix& ViewMatrix, const CRMatrix& ProjectionMatrix )
{
    CRViewProjection viewProjection;
    viewProjection.View       = ViewMatrix.Transpose();
    viewProjection.Projection = ProjectionMatrix.Transpose();

    ViewProjectionBuffer.Update( viewProjection );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set ambient light.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::SetAmbientLight( const CRVector4D& ColorAndIntensity )
{
    LightsData.AmbientColor = ColorAndIntensity;
}

//---------------------------------------------------------------------------------------------------------------------
/// Replace active directional lights.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::SetDirectionalLights( const CRArray< CRDirectionalLightData >& Lights )
{
    const u32 clampedCount = ( (u32)Lights.size() < CRMaxDirectionalLights ) ? (u32)Lights.size() : CRMaxDirectionalLights;

    LightsData.DirectionalCount = 0;

    for ( u32 i = 0; i < clampedCount; ++i )
    {
        LightsData.DirectionalLights[ i ] = Lights[ i ];
        ++LightsData.DirectionalCount;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Set one directional light at index.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::SetDirectionalLight( u32 Index, const CRDirectionalLightData& Light )
{
    if ( Index >= CRMaxDirectionalLights ) return;

    LightsData.DirectionalLights[ Index ] = Light;
    if ( LightsData.DirectionalCount <= Index )
    {
        LightsData.DirectionalCount = Index + 1;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Replace active point lights.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::SetPointLights( const CRArray< CRPointLightData >& Lights )
{
    const u32 clampedCount = ( (u32)Lights.size() < CRMaxPointLights ) ? (u32)Lights.size() : CRMaxPointLights;

    LightsData.PointCount = 0;

    for ( u32 i = 0; i < clampedCount; ++i )
    {
        LightsData.PointLights[ i ] = Lights[ i ];
        ++LightsData.PointCount;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Set one point light at index.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::SetPointLight( u32 Index, const CRPointLightData& Light )
{
    if ( Index >= CRMaxPointLights ) return;

    LightsData.PointLights[ Index ] = Light;
    if ( LightsData.PointCount <= Index )
    {
        LightsData.PointCount = Index + 1;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Replace active spot lights.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::SetSpotLights( const CRArray< CRSpotLightData >& Lights )
{
    const u32 clampedCount = ( (u32)Lights.size() < CRMaxSpotLights ) ? (u32)Lights.size() : CRMaxSpotLights;

    LightsData.SpotCount = 0;

    for ( u32 i = 0; i < clampedCount; ++i )
    {
        LightsData.SpotLights[ i ] = Lights[ i ];
        ++LightsData.SpotCount;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Set one spot light at index.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::SetSpotLight( u32 Index, const CRSpotLightData& Light )
{
    if ( Index >= CRMaxSpotLights ) return;

    LightsData.SpotLights[ Index ] = Light;
    if ( LightsData.SpotCount <= Index )
    {
        LightsData.SpotCount = Index + 1;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Clear all active lights.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::ClearLights()
{
    LightsData.DirectionalCount = 0;
    LightsData.PointCount       = 0;
    LightsData.SpotCount        = 0;
}

//---------------------------------------------------------------------------------------------------------------------
/// Draw.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::Draw()
{
    for ( const CRUniquePtr< ICRRHIRenderPass >& renderPass : RenderPasses )
    {
        if ( !renderPass ) continue;
        
        renderPass->OnPreDraw();
    }

    _UpdateCameraBuffers();    
    _FlushLightsBuffer();
    _RemoveStaleRenderElements();

    for ( const CRRenderPassPtr& renderPass : RenderPasses )
    {
        if ( !renderPass ) continue;

        renderPass->OnDrawRenderElements( RenderElements );
    }

    for ( const CRUniquePtr< ICRRHIRenderPass >& renderPass : RenderPasses )
    {
        if ( !renderPass ) continue;
        
        renderPass->OnPostDraw();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Clear render target.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::ClearRenderTarget() const
{
    const float clearColor[ 4 ] = { 0.0f, 0.4f, 0.7f, 1.0f };

    for ( const CRRenderPassPtr& renderPass : RenderPasses )
    {
        if ( !renderPass ) continue;

        renderPass->OnClearRenderTarget( clearColor );
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Initialize viewport.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::_InitializeViewport( f32 Width, f32 Height ) const
{
    D3D11_VIEWPORT viewport;
    ZeroMemory( &viewport, sizeof( D3D11_VIEWPORT ) );

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.Width    = Width;
    viewport.Height   = Height;

    GD11.GetDeviceContext()->RSSetViewports( 1, &viewport );
}

//---------------------------------------------------------------------------------------------------------------------
/// Update camera-related constant buffers from world/camera state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::_UpdateCameraBuffers()
{
    CRMatrix viewMatrix       = CRMatrix::Identity;
    CRMatrix projectionMatrix = CRMatrix::Identity;

    CRCameraProperties cameraProperties;
    cameraProperties.Position = CRVector4D( 0.0f, 0.0f, 0.0f, 1.0f );

    if ( GWorld )
    {
        if ( CRCamera* camera = GWorld->GetCamera() )
        {
            viewMatrix       = camera->GetViewMatrix();
            projectionMatrix = camera->GetProjectionMatrix();

            if ( CRTransformComponent* cameraTransform = camera->GetTransform() )
            {
                const CRVector& camPos = cameraTransform->GetLocation();
                cameraProperties.Position = CRVector4D( camPos.x, camPos.y, camPos.z, 1.0f );
            }
        }
    }

    UpdateViewProjectionBuffer( viewMatrix, projectionMatrix );
    CameraPropertiesBuffer.Update( cameraProperties );
}

//---------------------------------------------------------------------------------------------------------------------
/// Remove stale render elements whose mesh/material resources are no longer valid.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::_RemoveStaleRenderElements()
{
    CRArray< CRRenderElementHandle > staleHandles;

    RenderElements.ForEachActive( [ &staleHandles ] ( const CRRenderElementHandle& Handle, const CRRenderElement& Element )
    {
        if ( Element.Mesh.expired() || Element.Material.expired() )
        {
            staleHandles.push_back( Handle );
        }
    } );

    for ( const CRRenderElementHandle& staleHandle : staleHandles )
    {
        RenderElements.Remove( staleHandle );
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Release and clear render passes.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::_ReleaseRenderPasses()
{
    for ( const CRUniquePtr< ICRRHIRenderPass >& renderPass : RenderPasses )
    {
        if ( renderPass )
        {
            renderPass->Release();
        }
    }

    RenderPasses.clear();
}

//---------------------------------------------------------------------------------------------------------------------
/// Present.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::Present() const
{
    GD11.GetSwapChain()->Present( 0, 0 );
}

//---------------------------------------------------------------------------------------------------------------------
/// Flush CPU-side LightsData to the GPU constant buffer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::_FlushLightsBuffer()
{
    LightsBuffer.Update( LightsData );
}
