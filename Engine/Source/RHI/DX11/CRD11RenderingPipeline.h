#pragma once


#include "CRD11Types.h"
#include <d3d11.h>


//---------------------------------------------------------------------------------------------------------------------
/// CRD11RenderingPipeline
//---------------------------------------------------------------------------------------------------------------------
class CRD11RenderingPipeline
{
private:
    //-----------------------------------------------------------------------------------------------------------------
    /// IA buffers
    //-----------------------------------------------------------------------------------------------------------------
    ID3D11Buffer*      VertexBuffers[ D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT ] = { nullptr, };
    ID3D11InputLayout* InputLayouts [ D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT ] = { nullptr, };
    ID3D11Buffer*      IndexBuffer = nullptr;

    //-----------------------------------------------------------------------------------------------------------------
    /// Shader stages
    //-----------------------------------------------------------------------------------------------------------------
    ID3D11Buffer*             ConstantBuffers    [ D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ][ (int)( ED11RenderingPipelineStage::Max ) ] = { { nullptr }, };
    ID3D11ShaderResourceView* ShaderResourceViews[ D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT      ][ (int)( ED11RenderingPipelineStage::Max ) ] = { { nullptr }, };

    //-----------------------------------------------------------------------------------------------------------------
    /// Sampler states
    //-----------------------------------------------------------------------------------------------------------------
    ID3D11SamplerState* SamplerStates[ D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT ] = { nullptr, };

    //-----------------------------------------------------------------------------------------------------------------
    /// Shaders
    //-----------------------------------------------------------------------------------------------------------------
    ID3D11VertexShader* VertexShader = nullptr;
    ID3D11PixelShader*  PixelShader  = nullptr;

    //-----------------------------------------------------------------------------------------------------------------
    /// Render target
    //-----------------------------------------------------------------------------------------------------------------
    ID3D11RenderTargetView* RenderTargetView = nullptr;

    //-----------------------------------------------------------------------------------------------------------------
    /// DepthStencil state
    //-----------------------------------------------------------------------------------------------------------------
    ID3D11DepthStencilState* DepthStencilState = nullptr;

    //-----------------------------------------------------------------------------------------------------------------
    /// Rasterizer state
    //-----------------------------------------------------------------------------------------------------------------
    ID3D11RasterizerState* RasterizerState = nullptr;

public:
    /// Set vertex buffer.
    void SetVertexBuffer( ID3D11Buffer* Buffer, unsigned int Slot, unsigned int Stride, unsigned int Offset, D3D_PRIMITIVE_TOPOLOGY TopologyType );

    /// Set index buffer.
    void SetIndexBuffer( ID3D11Buffer* Buffer, unsigned int Offset );

    /// Set input layout.
    void SetInputLayout( ID3D11InputLayout* Layout, unsigned int Slot );

    /// Set constant buffer.
    void SetConstantBuffer( ID3D11Buffer* Buffer, unsigned int Slot, ED11RenderingPipelineStage Stage );

    /// Set shader resource view.
    void SetShaderResourceView( ID3D11ShaderResourceView* View, unsigned int Slot, ED11RenderingPipelineStage Stage );

    /// Set sampler state.
    void SetSamplerState( ID3D11SamplerState* State, unsigned int Slot );

    /// Set vertex shader.
    void SetVertexShader( ID3D11VertexShader* Shader );

    /// Set pixel shader.
    void SetPixelShader( ID3D11PixelShader* Shader );

    /// Set render target view.
    void SetRenderTargetView( ID3D11RenderTargetView* View, ID3D11DepthStencilView* DepthStencilView ); 

    /// Set depth stencil state.
    void SetDepthStencilState( ID3D11DepthStencilState* State );

    /// Set rasterizer state.
    void SetRasterizerState( ID3D11RasterizerState* State );
    
};
