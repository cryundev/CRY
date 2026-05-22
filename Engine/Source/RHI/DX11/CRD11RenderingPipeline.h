#pragma once


#include "CRD11Types.h"
#include "Source/Core/CRTypes.h"
#include <d3d11.h>

//---------------------------------------------------------------------------------------------------------------------
/// CRD11RPStateBundle
//---------------------------------------------------------------------------------------------------------------------
struct CRD11RPStateBundle
{
    ID3D11Buffer* VertexBuffers      [ D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT ]       = { nullptr, };
    UINT          VertexBufferStrides[ D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT ] = { 0, };
    UINT          VertexBufferOffsets[ D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT ] = { 0, };
    
    ID3D11InputLayout* InputLayout = nullptr;
    
    D3D_PRIMITIVE_TOPOLOGY PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;

    ID3D11Buffer* IndexBuffer       = nullptr;
    DXGI_FORMAT   IndexBufferFormat = DXGI_FORMAT_R32_UINT;
    UINT          IndexBufferOffset = 0;

    ID3D11Buffer*             ConstantBuffers    [ D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ][ (int)( ED11RenderingPipelineStage::Max ) ] = { { nullptr }, };
    ID3D11ShaderResourceView* ShaderResourceViews[ D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT      ][ (int)( ED11RenderingPipelineStage::Max ) ] = { { nullptr }, };

    ID3D11SamplerState* SamplerStates[ D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT ] = { nullptr, };

    ID3D11VertexShader* VertexShader = nullptr;
    ID3D11PixelShader*  PixelShader  = nullptr;

    ID3D11RenderTargetView*  RenderTargetView  = nullptr;
    ID3D11DepthStencilView*  DepthStencilView  = nullptr;
    ID3D11DepthStencilState* DepthStencilState = nullptr;
    UINT                     StencilRef        = 0;
    ID3D11RasterizerState*   RasterizerState   = nullptr;
    ID3D11BlendState*        BlendState        = nullptr;
    FLOAT                    BlendFactor[ 4 ]  = { 0.0f, 0.0f, 0.0f, 0.0f };
    UINT                     SampleMask        = D3D11_DEFAULT_SAMPLE_MASK;
};


//---------------------------------------------------------------------------------------------------------------------
/// CRD11RenderingPipeline
//---------------------------------------------------------------------------------------------------------------------
class CRD11RenderingPipeline
{
private:
    CRD11RPStateBundle StateBundle;
    CRD11RPStateBundle CachedStateBundle;
    
    bool bHasCapturedState = false;

public:
    /// Destructor.
    ~CRD11RenderingPipeline();

    /// Set vertex buffer.
    void SetVertexBuffer( ID3D11Buffer* Buffer, u32 Slot, u32 Stride, u32 Offset, D3D_PRIMITIVE_TOPOLOGY TopologyType, bool bSetTopology = true );

    /// Set primitive topology.
    void SetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY TopologyType );

    /// Set index buffer.
    void SetIndexBuffer( ID3D11Buffer* Buffer, u32 Offset, DXGI_FORMAT Format = DXGI_FORMAT_R32_UINT );

    /// Set input layout.
    void SetInputLayout( ID3D11InputLayout* Layout );

    /// Set constant buffer.
    void SetConstantBuffer( ID3D11Buffer* Buffer, u32 Slot, ED11RenderingPipelineStage Stage );

    /// Set shader resource view.
    void SetShaderResourceView( ID3D11ShaderResourceView* View, u32 Slot, ED11RenderingPipelineStage Stage );

    /// Set sampler state.
    void SetSamplerState( ID3D11SamplerState* State, u32 Slot );

    /// Set vertex shader.
    void SetVertexShader( ID3D11VertexShader* Shader );

    /// Set pixel shader.
    void SetPixelShader( ID3D11PixelShader* Shader );

    /// Set render target view.
    void SetRenderTargetView( ID3D11RenderTargetView* View, ID3D11DepthStencilView* DepthStencilView ); 

    /// Set depth stencil state.
    void SetDepthStencilState( ID3D11DepthStencilState* State, UINT StencilRef = 0 );

    /// Set rasterizer state.
    void SetRasterizerState( ID3D11RasterizerState* State );

    /// Set blend state.
    void SetBlendState( ID3D11BlendState* State, const FLOAT BlendFactor[ 4 ] = nullptr, UINT SampleMask = D3D11_DEFAULT_SAMPLE_MASK );

    /// Capture managed pipeline states into internal cache.
    void CapturePipelineStates();

    /// Restore pipeline states from internal cache.
    void RestorePipelineStates();

    /// Clear captured states without restoring.
    void ResetCapturedPipelineStates();

private:
    /// Restore primitive topology from captured state.
    void _RestorePrimitiveTopology();

    /// Restore vertex buffers from captured state.
    void _RestoreVertexBuffers();

    /// Restore input layout from captured state.
    void _RestoreInputLayout();

    /// Restore index buffer from captured state.
    void _RestoreIndexBuffer();

    /// Restore constant buffers from captured state.
    void _RestoreConstantBuffers();

    /// Restore shader resource views from captured state.
    void _RestoreShaderResourceViews();

    /// Restore sampler states from captured state.
    void _RestoreSamplerStates();

    /// Restore shaders from captured state.
    void _RestoreShaders();

    /// Restore render target view from captured state.
    void _RestoreRenderTargetView();

    /// Restore depth stencil state from captured state.
    void _RestoreDepthStencilState();

    /// Restore rasterizer state from captured state.
    void _RestoreRasterizerState();

    /// Restore blend state from captured state.
    void _RestoreBlendState();

    /// Check whether blend factor differs from captured state.
    bool _IsBlendFactorChanged() const;
    
};
