#include "CRD11Renderer.h"
#include "CRD11.h"
#include "CRD11Include.h"
#include "CRD11ResourceManager.h"
#include "../../Core/CRVertex.h"
#include "../../Utility/Log/CRLog.h"
#include "Core/CRD11ConstantBuffer.h"
#include "Core/CRD11Device.h"
#include "Core/CRD11IndexBuffer.h"
#include "Core/CRD11InputLayout.h"
#include "Core/CRD11PixelShader.h"
#include "Core/CRD11RenderTargetView.h"
#include "Core/CRD11SamplerState.h"
#include "Core/CRD11ShaderResourceView.h"
#include "Core/CRD11VertexBuffer.h"
#include "Core/CRD11VertexShader.h"


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
    GD11.GetDeviceContext()->ClearRenderTargetView( RenderTargetView.lock()->GetObjectPtr(), color );
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
/// Set constant buffer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Renderer::SetConstantBuffer( const CRD11ConstantBufferSPtr& CRConstantBuffer, unsigned int Slot, ED11RenderingPipelineStage Stage )
{
    if ( Slot >= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ) return;
    if ( !CRConstantBuffer.get() ) return;

    ConstantBuffers[ Slot ][ (int)( Stage ) ] = CRConstantBuffer;

    ID3D11Buffer* bufferPtr = CRConstantBuffer->GetObjectPtr();
    switch ( Stage )
    {
    case ED11RenderingPipelineStage::VS: GD11.GetDeviceContext()->VSSetConstantBuffers( Slot, 1, &bufferPtr ); break;
    case ED11RenderingPipelineStage::GS: GD11.GetDeviceContext()->GSSetConstantBuffers( Slot, 1, &bufferPtr ); break;
    case ED11RenderingPipelineStage::DS: GD11.GetDeviceContext()->DSSetConstantBuffers( Slot, 1, &bufferPtr ); break;
    case ED11RenderingPipelineStage::HS: GD11.GetDeviceContext()->HSSetConstantBuffers( Slot, 1, &bufferPtr ); break;
    case ED11RenderingPipelineStage::CS: GD11.GetDeviceContext()->CSSetConstantBuffers( Slot, 1, &bufferPtr ); break;
    case ED11RenderingPipelineStage::PS: GD11.GetDeviceContext()->PSSetConstantBuffers( Slot, 1, &bufferPtr ); break;
    default: break;
    }
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

    RenderTargetView = GD11RM.GetRenderTargetView( "BackBuffer" );
    RenderTargetView.lock()->Create( texture, nullptr );

    ID3D11RenderTargetView* rtv = RenderTargetView.lock()->GetObjectPtr();
    GD11.GetDeviceContext()->OMSetRenderTargets( 1, &rtv, nullptr );

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
