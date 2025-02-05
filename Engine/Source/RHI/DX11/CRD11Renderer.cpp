#include "CRD11Renderer.h"
#include "CRD11.h"
#include "CRD11RenderingPipeline.h"
#include "CRD11ResourceManager.h"
#include "Engine.h"
#include "Resource/CRD11Device.h"
#include "Resource/CRD11RasterizerState.h"
#include "Resource/CRD11RenderTargetView.h"
#include "Source/RHI/ICRRHIMesh.h"
#include "Source/Utility/Log/CRLog.h"


//---------------------------------------------------------------------------------------------------------------------
/// Initialize renderer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::Initialize( unsigned int Width, unsigned int Height )
{
    ViewportWidth  = Width;
    ViewportHeight = Height;

    DepthStencilBuffer.Create( Width, Height );
    
    _InitializeRenderTarget();
    _InitializeViewport( (float)( Width ), (float)( Height ) );

    TransformBuffer.Create( "Transform", (unsigned int)( ECVS::Transform ), ED11RenderingPipelineStage::VS );
    TransformBuffer.SetInRenderingPipeline();
    
    TransformBuffer.Update( DirectX::XMMatrixTranspose( CRMatrix::Identity ) );

    ViewProjectionBuffer.Create( "ViewProjection", (unsigned int)( ECVS::ViewProjection ), ED11RenderingPipelineStage::VS );
    ViewProjectionBuffer.SetInRenderingPipeline();

    LightDirectionBuffer.Create( "LightDirection", (unsigned int)( ECPS::LightDirection ), ED11RenderingPipelineStage::PS );
    LightDirectionBuffer.SetInRenderingPipeline();
    
    LightColorBuffer.Create( "LightColor", (unsigned int)( ECPS::LightColor ), ED11RenderingPipelineStage::PS );
    LightColorBuffer.SetInRenderingPipeline();

    CRVector lightDir( 1.0f, -1.0f, 1.0f );
    lightDir.Normalize();

    LightDirectionBuffer.Update( CRVector4D( lightDir.x, lightDir.y, lightDir.z, 1.0f ) );
    LightColorBuffer    .Update( CRVector4D( 1.0f, 1.0f,  1.0f, 1.0f ) );

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
/// Add render mesh.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::AddRenderMesh( const ICRRHIMeshWPtr& Mesh )
{
    RenderMeshes.push_back( Mesh );
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
/// Draw.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::Draw()
{
    UpdateViewProjectionBuffer( GCamera->GetViewMatrix(), GCamera->GetProjectionMatrix() );
    
    for ( const ICRRHIMeshWPtr& renderMesh : RenderMeshes )
    {
        if ( renderMesh.expired() ) continue;
        
        renderMesh.lock()->SetInRenderingPipeline();
        renderMesh.lock()->Draw();
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
//---------------------------------------------------------------------------------------------------------------------%
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
void CRD11Renderer::_InitializeViewport( float Width, float Height ) const
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
