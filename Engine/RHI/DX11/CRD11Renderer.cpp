#include "CRD11Renderer.h"
#include "CRD11.h"
#include "CRD11Device.h"
#include "CRD11Include.h"
#include "CRD11IndexBuffer.h"
#include "CRD11InputLayout.h"
#include "CRD11PixelShader.h"
#include "CRD11SamplerState.h"
#include "CRD11ShaderResourceView.h"
#include "CRD11VertexBuffer.h"
#include "CRD11VertexShader.h"
#include "../../Core/CRVertex.h"
#include "../../Utility/Log/CRLog.h"


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
    if ( InputLayout[ Slot ].expired() ) return;

    if ( IndexBuffer.expired() )
    {
        if ( !VertexBuffers[ Slot ].expired() )
        {
            GD11.GetDeviceContext()->Draw( VertexBuffers[ Slot ].lock()->GetCount(), 0 );
        }
    }
    else
    {
        GD11.GetDeviceContext()->DrawIndexed( IndexBuffer.lock()->GetCount(), 0, 0 );
    }
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
void CRD11Renderer::SetVertexBuffer( const CRD11VertexBufferSPtr& CRVertexBuffer, unsigned int Slot )
{
    if ( Slot >= D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT ) return;
    if ( !CRVertexBuffer.get() ) return;

    VertexBuffers[ Slot ] = CRVertexBuffer;

    unsigned int stride = sizeof( CRVertex );
    unsigned int offset = 0;

    ID3D11Buffer* bufferPtr = CRVertexBuffer->GetObjectPtr();

    GD11.GetDeviceContext()->IASetVertexBuffers( Slot, 1, &bufferPtr, &stride, &offset );
    GD11.GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set index buffer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::SetIndexBuffer( const CRD11IndexBufferSPtr& CRIndexBuffer )
{
    if ( !CRIndexBuffer.get() ) return;
    
    IndexBuffer = CRIndexBuffer;
    
    GD11.GetDeviceContext()->IASetIndexBuffer( CRIndexBuffer->GetObjectPtr(), DXGI_FORMAT_R32_UINT, 0 );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set vertex shader.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::SetVertexShader( const CRD11VertexShaderSPtr& CRVertexShader )
{
    if ( !CRVertexShader.get() ) return;

    VertexShader = CRVertexShader;
    
    GD11.GetDeviceContext()->VSSetShader( CRVertexShader->GetObjectPtr(), nullptr, 0 );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set input layout.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::SetInputLayout( const CRD11InputLayoutSPtr& CRInputLayout, unsigned int Slot )
{
    if ( Slot >= D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT ) return;
    if ( !CRInputLayout.get() ) return;

    InputLayout[ Slot ] = CRInputLayout;
    
    GD11.GetDeviceContext()->IASetInputLayout( CRInputLayout->GetObjectPtr() );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set pixel shader.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::SetPixelShader( const CRD11PixelShaderSPtr& CRPixelShader )
{
    if ( !CRPixelShader.get() ) return;

    PixelShader = CRPixelShader;
    
    GD11.GetDeviceContext()->PSSetShader( CRPixelShader->GetObjectPtr(), nullptr, 0 );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set shader resource.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::SetShaderResource( const CRD11ShaderResourceViewSPtr& CRShaderResourceView, unsigned int Slot )
{
    if ( Slot >= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT ) return;
    if ( !CRShaderResourceView.get() ) return;

    ShaderResourceViews[ Slot ] = CRShaderResourceView;

    ID3D11ShaderResourceView* srv = CRShaderResourceView->GetObjectPtr();
    GD11.GetDeviceContext()->PSSetShaderResources( Slot, 1, &srv );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set sampler state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::SetSamplerState( const CRD11SamplerStateSPtr& CRSamplerState, unsigned int Slot )
{
	if ( Slot >= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT ) return;
	if ( !CRSamplerState.get() ) return;

	SamplerStates[ Slot ] = CRSamplerState;
	
	ID3D11SamplerState* ss = CRSamplerState->GetObjectPtr();
	GD11.GetDeviceContext()->PSSetSamplers( Slot, 1, &ss );
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
