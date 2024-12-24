#include "CRD11Renderer.h"
#include "CRD11.h"
#include "CRD11RenderingPipeline.h"
#include "CRD11ResourceManager.h"
#include "../../Utility/Log/CRLog.h"
#include "Core/CRD11Device.h"
#include "Core/CRD11RenderTargetView.h"


//---------------------------------------------------------------------------------------------------------------------
/// Initialize renderer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::Initialize( unsigned int Width, unsigned int Height )
{
    _InitializeRenderTarget();
    _InitializeViewport( (float)( Width ), (float)( Height ) );

    TransformBuffer.Create( "Transform", 0, ED11RenderingPipelineStage::VS );
    GD11RP.SetConstantBuffer( TransformBuffer.GetConstantBuffer().lock()->GetObjectPtr(), TransformBuffer.GetSlot(), TransformBuffer.GetStage() );

    Mesh.Initialize();
    Mesh.SetInRenderingPipeline();
}

//---------------------------------------------------------------------------------------------------------------------
/// Draw.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::Draw()
{
    CRMatrix matrix = CRMatrix::Identity;

    matrix = CRMatrix::CreateScale( 0.5f, 0.5f, 1.f ) * CRMatrix::CreateTranslation( -0.5f, 0.5f, 0.0f );

    TransformBuffer.Update( DirectX::XMMatrixTranspose( matrix ) );
    Mesh.Draw();

    matrix = CRMatrix::CreateScale( 0.5f, 0.5f, 1.f ) * CRMatrix::CreateTranslation( 0.5f, -0.5f, 0.0f );

    TransformBuffer.Update( DirectX::XMMatrixTranspose( matrix ) );
    Mesh.Draw();
}

//---------------------------------------------------------------------------------------------------------------------
/// Clear render target.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::ClearRenderTarget() const
{
    float color[ 4 ] = { 0.0f, 0.4f, 0.7f, 1.0f };
    GD11.GetDeviceContext()->ClearRenderTargetView( RenderTargetView->GetObjectPtr(), color );
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

    GD11RP.SetRenderTargetView( RenderTargetView->GetObjectPtr() );

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
    viewport.Width    = Width;
    viewport.Height   = Height;

    GD11.GetDeviceContext()->RSSetViewports( 1, &viewport );
}
