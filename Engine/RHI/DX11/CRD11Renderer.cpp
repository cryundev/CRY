#include "CRD11Renderer.h"
#include "CRD11.h"
#include "CRD11Device.h"
#include "CRD11Include.h"
#include "CRD11InputLayout.h"
#include "CRD11PixelShader.h"
#include "CRD11VertexBuffer.h"
#include "CRD11VertexShader.h"
#include "../../Core/CRVertex.h"
#include "../../Utility/CRLog.h"


//---------------------------------------------------------------------------------------------------------------------
/// Initialize renderer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::Initialize( unsigned int Width, unsigned int Height )
{
    _InitializeRenderTarget();
    _InitializeViewport( (float)( Width ), (float)( Height ) );
}

//---------------------------------------------------------------------------------------------------------------------
/// Draw.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::Draw( unsigned int Slot ) const
{
    if ( Slot >= D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT ) return;
    if ( !InputLayout[ Slot ] ) return;
    
    GD11.GetDeviceContext()->IASetInputLayout( InputLayout[ Slot ] );
    GD11.GetDeviceContext()->Draw( 3, 0 );
}

//---------------------------------------------------------------------------------------------------------------------
/// Clear render target.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::ClearRenderTarget() const
{
    float color[ 4 ] = { 0.0f, 0.4f, 0.7f, 1.0f };
    GD11.GetDeviceContext()->ClearRenderTargetView( RenderTargetView, color );
}

//---------------------------------------------------------------------------------------------------------------------
/// Present.
//---------------------------------------------------------------------------------------------------------------------%
void CRD11Renderer::Present() const
{
    GD11.GetSwapChain()->Present( 0, 0 );
}

//---------------------------------------------------------------------------------------------------------------------
/// SetVertexBuffer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::SetVertexBuffer( const CRD11VertexBuffer* CRVertexBuffer, unsigned int Slot )
{
    if ( Slot >= D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT ) return;
    if ( !CRVertexBuffer ) return;

    VertexBuffers[ Slot ] = CRVertexBuffer->GetBufferPtr();

    unsigned int stride = sizeof( CRVertex );
    unsigned int offset = 0;

    GD11.GetDeviceContext()->IASetVertexBuffers( Slot, 1, &VertexBuffers[ Slot ], &stride, &offset );
    GD11.GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set vertex shader.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::SetVertexShader( const CRD11VertexShader* CRVertexShader )
{
    if ( !CRVertexShader ) return;

    VertexShader = CRVertexShader->GetShaderPtr();
    
    GD11.GetDeviceContext()->VSSetShader( CRVertexShader->GetShaderPtr(), nullptr, 0 );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set input layout.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::SetInputLayout( const CRD11InputLayout* CRInputLayout, unsigned int Slot )
{
    if ( Slot >= D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT ) return;
    if ( !CRInputLayout ) return;

    InputLayout[ Slot ] = CRInputLayout->GetInputLayoutPtr();
}

//---------------------------------------------------------------------------------------------------------------------
/// Set pixel shader.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::SetPixelShader( const CRD11PixelShader* CRPixelShader )
{
    if ( !CRPixelShader ) return;

    PixelShader = CRPixelShader->GetShaderPtr();
    
    GD11.GetDeviceContext()->PSSetShader( CRPixelShader->GetShaderPtr(), nullptr, 0 );
}

//---------------------------------------------------------------------------------------------------------------------
/// Initialize render target.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::_InitializeRenderTarget()
{
    ID3D11Texture2D* texture = nullptr;
    GD11.GetSwapChain()->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&texture );

    if ( !texture ) return;

    HRESULT hr = GD11.GetDevice()->CreateRenderTargetView( texture, nullptr, &RenderTargetView );
    if ( FAILED( hr ) )
    {
        GLog.AddErrorLog( hr );
        return;
    }

    GD11.GetDeviceContext()->OMSetRenderTargets( 1, &RenderTargetView, nullptr );

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
