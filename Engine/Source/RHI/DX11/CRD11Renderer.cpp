#include "CRD11Renderer.h"
#include "CRD11.h"
#include "CRD11RenderingPipeline.h"
#include "CRD11ResourceManager.h"
#include "Engine.h"
#include "Resource/CRD11Device.h"
#include "Resource/CRD11RasterizerState.h"
#include "Resource/CRD11RenderTargetView.h"
#include "Source/RHI/ICRRHIMaterial.h"
#include "Source/RHI/ICRRHIMesh.h"
#include "Source/Utility/Log/CRLog.h"
#include "Source/World/CRWorld.h"


//---------------------------------------------------------------------------------------------------------------------
/// Initialize renderer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::Initialize( u32 Width, u32 Height )
{
    ViewportWidth  = Width;
    ViewportHeight = Height;

    RenderElements.Clear();

    DepthStencilBuffer.Create( Width, Height );

    _InitializeRenderTarget();
    _InitializeViewport( (f32)( Width ), (f32)( Height ) );

    TransformBuffer.Create( "Transform", (u32)( EConstBufferSlotVS::Transform ), ED11RenderingPipelineStage::VS );
    TransformBuffer.SetInRenderingPipeline();

    TransformBuffer.Update( DirectX::XMMatrixTranspose( CRMatrix::Identity ) );

    ViewProjectionBuffer.Create( "ViewProjection", (u32)( EConstBufferSlotVS::ViewProjection ), ED11RenderingPipelineStage::VS );
    ViewProjectionBuffer.SetInRenderingPipeline();

    LightsBuffer.Create( "LightsBuffer", (u32)( EConstBufferSlotPS::LightProperties ), ED11RenderingPipelineStage::PS );
    LightsBuffer.SetInRenderingPipeline();

    CRDirectionalLightData sun;
    CRVector sunDir( 1.0f, -1.0f, 1.0f );
    sunDir.Normalize();
    sun.Direction = CRVector4D( sunDir.x, sunDir.y, sunDir.z, 0.0f );
    sun.Color     = CRVector4D( 1.0f, 1.0f, 1.0f, 1.0f );

    LightsData.AmbientColor           = CRVector4D( 0.1f, 0.1f, 0.1f, 1.0f );
    LightsData.DirectionalCount       = 1;
    LightsData.PointCount             = 0;
    LightsData.SpotCount              = 0;
    LightsData.DirectionalLights[ 0 ] = sun;

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
/// Add render element.
//---------------------------------------------------------------------------------------------------------------------
CRRenderElementHandle CRD11Renderer::AddRenderElement( const CRRenderElement& RenderElement )
{
    if ( RenderElement.Mesh.expired() ) return {};
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
    if ( !GWorld ) return;

    CRCamera* camera = GWorld->GetCamera();
    if ( !camera ) return;

    UpdateViewProjectionBuffer( camera->GetViewMatrix(), camera->GetProjectionMatrix() );

    if ( CRTransformComponent* cameraTransform = camera->GetTransform() )
    {
        const CRVector& camPos = cameraTransform->GetLocation();

        CRCameraProperties cameraProperties;
        cameraProperties.Position = CRVector4D( camPos.x, camPos.y, camPos.z, 1.0f );

        CameraPropertiesBuffer.Update( cameraProperties );
    }
\
    _FlushLightsBuffer();

    CRArray< CRRenderElementHandle > staleHandles;

    RenderElements.ForEachActive( [ &staleHandles ] ( const CRRenderElementHandle& Handle, CRRenderElement& Element )
    {
        ICRRHIMeshSPtr     mesh     = Element.Mesh.lock();
        ICRRHIMaterialSPtr material = Element.Material.lock();
        if ( !mesh || !material )
        {
            staleHandles.push_back( Handle );
            return;
        }

        mesh->SetInRenderingPipeline();
        material->SetInRenderingPipeline();
        mesh->Draw();
    } );

    for ( const CRRenderElementHandle& handle : staleHandles )
    {
        RenderElements.Remove( handle );
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Clear render target.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::ClearRenderTarget() const
{
    float color[ 4 ] = { 0.0f, 0.4f, 0.7f, 1.0f };
    GD11.GetDeviceContext()->ClearRenderTargetView( RenderTargetView->GetObjectPtr(), color );

    DepthStencilBuffer.ClearBuffer();
}

//---------------------------------------------------------------------------------------------------------------------
/// Present.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::Present() const
{
    GD11.GetSwapChain()->Present( 0, 0 );
}

//---------------------------------------------------------------------------------------------------------------------
/// Initialize render target.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::_InitializeRenderTarget()
{
    ID3D11Texture2D* texture = nullptr;
    GD11.GetSwapChain()->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&texture );

    if ( !texture ) return;

    RenderTargetView = GD11RM.GetRenderTargetView( "BackBuffer" );
    RenderTargetView->Create( texture, nullptr );

    GD11RP.SetRenderTargetView( RenderTargetView->GetObjectPtr(), DepthStencilBuffer.GetView() );

    texture->Release();
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
/// Flush CPU-side LightsData to the GPU constant buffer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::_FlushLightsBuffer()
{
    LightsBuffer.Update( LightsData );
}
