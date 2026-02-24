#include "CRD11RenderingPipeline.h"
#include "CRD11.h"
#include "Resource/CRD11Device.h"


//---------------------------------------------------------------------------------------------------------------------
/// Destructor.
//---------------------------------------------------------------------------------------------------------------------
CRD11RenderingPipeline::~CRD11RenderingPipeline()
{
    ResetCapturedPipelineStates();
}

 
//---------------------------------------------------------------------------------------------------------------------
/// Set vertex buffer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderingPipeline::SetVertexBuffer( ID3D11Buffer* Buffer, u32 Slot, u32 Stride, u32 Offset, D3D_PRIMITIVE_TOPOLOGY TopologyType, bool bSetTopology )
{
    if ( Slot >= D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT ) return;

    StateBundle.VertexBuffers      [ Slot ] = Buffer;
    StateBundle.VertexBufferStrides[ Slot ] = Stride;
    StateBundle.VertexBufferOffsets[ Slot ] = Offset;

    if ( bSetTopology )
    {
        SetPrimitiveTopology( TopologyType );
    }
    
    GD11.GetDeviceContext()->IASetVertexBuffers( Slot, 1, &Buffer, &Stride, &Offset );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set primitive topology.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderingPipeline::SetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY TopologyType )
{
    StateBundle.PrimitiveTopology = TopologyType;

    GD11.GetDeviceContext()->IASetPrimitiveTopology( TopologyType );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set index buffer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderingPipeline::SetIndexBuffer( ID3D11Buffer* Buffer, u32 Offset, DXGI_FORMAT Format )
{
    StateBundle.IndexBuffer       = Buffer;
    StateBundle.IndexBufferFormat = Format;
    StateBundle.IndexBufferOffset = Offset;

    GD11.GetDeviceContext()->IASetIndexBuffer( Buffer, Format, Offset );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set input layout.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderingPipeline::SetInputLayout( ID3D11InputLayout* Layout )
{
    StateBundle.InputLayout = Layout;

    GD11.GetDeviceContext()->IASetInputLayout( Layout );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set constant buffer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderingPipeline::SetConstantBuffer( ID3D11Buffer* Buffer, u32 Slot, ED11RenderingPipelineStage Stage )
{
    if ( Slot >= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ) return;

    StateBundle.ConstantBuffers[ Slot ][ (int)( Stage ) ] = Buffer;

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
void CRD11RenderingPipeline::SetShaderResourceView( ID3D11ShaderResourceView* View, u32 Slot, ED11RenderingPipelineStage Stage )
{
    if ( Slot >= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT ) return;

    StateBundle.ShaderResourceViews[ Slot ][ (int)( Stage ) ] = View;

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
void CRD11RenderingPipeline::SetSamplerState( ID3D11SamplerState* State, u32 Slot )
{
    if ( Slot >= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT ) return;

    StateBundle.SamplerStates[ Slot ] = State;

    GD11.GetDeviceContext()->PSSetSamplers( Slot, 1, &State );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set vertex shader.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderingPipeline::SetVertexShader( ID3D11VertexShader* Shader )
{
    StateBundle.VertexShader = Shader;

    GD11.GetDeviceContext()->VSSetShader( Shader, nullptr, 0 );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set pixel shader.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderingPipeline::SetPixelShader( ID3D11PixelShader* Shader )
{
    StateBundle.PixelShader = Shader;

    GD11.GetDeviceContext()->PSSetShader( Shader, nullptr, 0 );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set render target view.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderingPipeline::SetRenderTargetView( ID3D11RenderTargetView* View, ID3D11DepthStencilView* DepthStencilView )
{
    StateBundle.RenderTargetView = View;
    StateBundle.DepthStencilView = DepthStencilView;

    GD11.GetDeviceContext()->OMSetRenderTargets( 1, &View, DepthStencilView );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set depth stencil state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderingPipeline::SetDepthStencilState( ID3D11DepthStencilState* State, UINT StencilRef )
{
    StateBundle.DepthStencilState = State;
    StateBundle.StencilRef = StencilRef;

    GD11.GetDeviceContext()->OMSetDepthStencilState( State, StencilRef );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set rasterizer state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderingPipeline::SetRasterizerState( ID3D11RasterizerState* State )
{
    StateBundle.RasterizerState = State;

    GD11.GetDeviceContext()->RSSetState( State );
}

//---------------------------------------------------------------------------------------------------------------------
/// Capture managed pipeline states.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderingPipeline::CapturePipelineStates()
{
    CachedStateBundle = StateBundle;
    bHasCapturedState = true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Restore pipeline states from backup.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderingPipeline::RestorePipelineStates()
{
    if ( !bHasCapturedState ) return;

    if ( StateBundle.PrimitiveTopology != CachedStateBundle.PrimitiveTopology )
    {
        SetPrimitiveTopology( CachedStateBundle.PrimitiveTopology );
    }

    for ( u32 slot = 0; slot < D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT; ++slot )
    {
        if ( StateBundle.VertexBuffers      [ slot ] != CachedStateBundle.VertexBuffers      [ slot ] ||
             StateBundle.VertexBufferStrides[ slot ] != CachedStateBundle.VertexBufferStrides[ slot ] ||
             StateBundle.VertexBufferOffsets[ slot ] != CachedStateBundle.VertexBufferOffsets[ slot ] )
        {
            SetVertexBuffer( CachedStateBundle.VertexBuffers[ slot ], slot, CachedStateBundle.VertexBufferStrides[ slot ], CachedStateBundle.VertexBufferOffsets[ slot ], CachedStateBundle.PrimitiveTopology, false );
        }
    }

    if ( StateBundle.InputLayout != CachedStateBundle.InputLayout )
    {
        SetInputLayout( CachedStateBundle.InputLayout );
    }

    if ( StateBundle.IndexBuffer       != CachedStateBundle.IndexBuffer       ||
         StateBundle.IndexBufferOffset != CachedStateBundle.IndexBufferOffset ||
         StateBundle.IndexBufferFormat != CachedStateBundle.IndexBufferFormat )
    {
        SetIndexBuffer( CachedStateBundle.IndexBuffer, CachedStateBundle.IndexBufferOffset, CachedStateBundle.IndexBufferFormat );
    }

    for ( int stage = (int)( ED11RenderingPipelineStage::VS ); stage <= (int)( ED11RenderingPipelineStage::PS ); ++stage )
    {
        const ED11RenderingPipelineStage currentStage = (ED11RenderingPipelineStage)( stage );

        for ( u32 slot = 0; slot < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; ++slot )
        {
            if ( StateBundle.ConstantBuffers[ slot ][ stage ] != CachedStateBundle.ConstantBuffers[ slot ][ stage ] )
            {
                SetConstantBuffer( CachedStateBundle.ConstantBuffers[ slot ][ stage ], slot, currentStage );
            }
        }

        for ( u32 slot = 0; slot < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; ++slot )
        {
            if ( StateBundle.ShaderResourceViews[ slot ][ stage ] != CachedStateBundle.ShaderResourceViews[ slot ][ stage ] )
            {
                SetShaderResourceView( CachedStateBundle.ShaderResourceViews[ slot ][ stage ], slot, currentStage );
            }
        }
    }

    for ( u32 slot = 0; slot < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; ++slot )
    {
        if ( StateBundle.SamplerStates[ slot ] != CachedStateBundle.SamplerStates[ slot ] )
        {
            SetSamplerState( CachedStateBundle.SamplerStates[ slot ], slot );
        }
    }

    if ( StateBundle.VertexShader != CachedStateBundle.VertexShader )
    {
        SetVertexShader( CachedStateBundle.VertexShader );
    }

    if ( StateBundle.PixelShader != CachedStateBundle.PixelShader )
    {
        SetPixelShader( CachedStateBundle.PixelShader );
    }

    if ( StateBundle.RenderTargetView != CachedStateBundle.RenderTargetView ||
         StateBundle.DepthStencilView != CachedStateBundle.DepthStencilView )
    {
        SetRenderTargetView( CachedStateBundle.RenderTargetView, CachedStateBundle.DepthStencilView );
    }

    if ( StateBundle.DepthStencilState != CachedStateBundle.DepthStencilState ||
         StateBundle.StencilRef        != CachedStateBundle.StencilRef         )
    {
        SetDepthStencilState( CachedStateBundle.DepthStencilState, CachedStateBundle.StencilRef );
    }

    if ( StateBundle.RasterizerState != CachedStateBundle.RasterizerState )
    {
        SetRasterizerState( CachedStateBundle.RasterizerState );
    }

    ResetCapturedPipelineStates();
}

//---------------------------------------------------------------------------------------------------------------------
/// Clear captured states.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderingPipeline::ResetCapturedPipelineStates()
{
    CachedStateBundle = CRD11RPStateBundle();
    bHasCapturedState = false;
}
