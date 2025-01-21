#include "CRD11RenderingPipeline.h"
#include "CRD11.h"
#include "Core/CRD11Device.h"


//---------------------------------------------------------------------------------------------------------------------
/// Set vertex buffer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderingPipeline::SetVertexBuffer( ID3D11Buffer* Buffer, unsigned int Slot, unsigned int Stride, unsigned int Offset, D3D_PRIMITIVE_TOPOLOGY TopologyType )
{
    if ( Slot >= D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT ) return;
    if ( !Buffer ) return;

    VertexBuffers[ Slot ] = Buffer;

    GD11.GetDeviceContext()->IASetVertexBuffers( Slot, 1, &Buffer, &Stride, &Offset );
    GD11.GetDeviceContext()->IASetPrimitiveTopology( TopologyType );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set index buffer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderingPipeline::SetIndexBuffer( ID3D11Buffer* Buffer, unsigned int Offset )
{
    if ( !Buffer ) return;

    IndexBuffer = Buffer;

    GD11.GetDeviceContext()->IASetIndexBuffer( Buffer, DXGI_FORMAT_R32_UINT, Offset );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set input layout.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderingPipeline::SetInputLayout( ID3D11InputLayout* Layout, unsigned int Slot )
{
    if ( Slot >= D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT ) return;
    if ( !Layout ) return;

    InputLayouts[ Slot ] = Layout;

    GD11.GetDeviceContext()->IASetInputLayout( Layout );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set constant buffer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderingPipeline::SetConstantBuffer( ID3D11Buffer* Buffer, unsigned int Slot, ED11RenderingPipelineStage Stage )
{
    if ( Slot >= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ) return;
    if ( !Buffer ) return;

    ConstantBuffers[ Slot ][ (int)( Stage ) ] = Buffer;

    switch ( Stage )
    {
    case ED11RenderingPipelineStage::VS: GD11.GetDeviceContext()->VSSetConstantBuffers( Slot, 1, &Buffer ); break;
    case ED11RenderingPipelineStage::GS: GD11.GetDeviceContext()->GSSetConstantBuffers( Slot, 1, &Buffer ); break;
    case ED11RenderingPipelineStage::DS: GD11.GetDeviceContext()->DSSetConstantBuffers( Slot, 1, &Buffer ); break;
    case ED11RenderingPipelineStage::HS: GD11.GetDeviceContext()->HSSetConstantBuffers( Slot, 1, &Buffer ); break;
    case ED11RenderingPipelineStage::CS: GD11.GetDeviceContext()->CSSetConstantBuffers( Slot, 1, &Buffer ); break;
    case ED11RenderingPipelineStage::PS: GD11.GetDeviceContext()->PSSetConstantBuffers( Slot, 1, &Buffer ); break;
    default: break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Set shader resource view.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderingPipeline::SetShaderResourceView( ID3D11ShaderResourceView* View, unsigned int Slot, ED11RenderingPipelineStage Stage )
{
    if ( Slot >= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT ) return;
    if ( !View ) return;

    ShaderResourceViews[ Slot ][ (int)( Stage ) ] = View;

    switch ( Stage )
    {
    case ED11RenderingPipelineStage::VS: GD11.GetDeviceContext()->VSSetShaderResources( Slot, 1, &View ); break;
    case ED11RenderingPipelineStage::GS: GD11.GetDeviceContext()->GSSetShaderResources( Slot, 1, &View ); break;
    case ED11RenderingPipelineStage::DS: GD11.GetDeviceContext()->DSSetShaderResources( Slot, 1, &View ); break;
    case ED11RenderingPipelineStage::HS: GD11.GetDeviceContext()->HSSetShaderResources( Slot, 1, &View ); break;
    case ED11RenderingPipelineStage::CS: GD11.GetDeviceContext()->CSSetShaderResources( Slot, 1, &View ); break;
    case ED11RenderingPipelineStage::PS: GD11.GetDeviceContext()->PSSetShaderResources( Slot, 1, &View ); break;
    default: break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Set sampler state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderingPipeline::SetSamplerState( ID3D11SamplerState* State, unsigned int Slot )
{
    if ( Slot >= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT ) return;
    if ( !State ) return;

    SamplerStates[ Slot ] = State;

    GD11.GetDeviceContext()->PSSetSamplers( Slot, 1, &State );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set vertex shader.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderingPipeline::SetVertexShader( ID3D11VertexShader* Shader )
{
    if ( !Shader ) return;

    VertexShader = Shader;

    GD11.GetDeviceContext()->VSSetShader( Shader, nullptr, 0 );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set pixel shader.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderingPipeline::SetPixelShader( ID3D11PixelShader* Shader )
{
    if ( !Shader ) return;

    PixelShader = Shader;

    GD11.GetDeviceContext()->PSSetShader( Shader, nullptr, 0 );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set render target view.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderingPipeline::SetRenderTargetView( ID3D11RenderTargetView* View, ID3D11DepthStencilView* DepthStencilView )
{
    if ( !View ) return;

    RenderTargetView = View;

    GD11.GetDeviceContext()->OMSetRenderTargets( 1, &View, DepthStencilView );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set depth stencil state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderingPipeline::SetDepthStencilState( ID3D11DepthStencilState* State )
{
    if ( !State ) return;
    
    DepthStencilState = State;

    GD11.GetDeviceContext()->OMSetDepthStencilState( State, 0 );
}
