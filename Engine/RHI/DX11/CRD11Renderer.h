#pragma once


#include "CRD11Include.h"


class CRD11IndexBuffer;
class CRD11InputLayout;
class CRD11PixelShader;
class CRD11VertexShader;
class CRD11VertexBuffer;


//---------------------------------------------------------------------------------------------------------------------
/// CRD11Renderer
//---------------------------------------------------------------------------------------------------------------------
class CRD11Renderer
{
private:
    const CRD11VertexBuffer* VertexBuffers[ D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT ] = { nullptr, };
    const CRD11InputLayout*  InputLayout  [ D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT ] = { nullptr, };
    const CRD11IndexBuffer*  IndexBuffer  = nullptr;
    const CRD11VertexShader* VertexShader = nullptr;
    const CRD11PixelShader*  PixelShader  = nullptr;

    ID3D11RenderTargetView* RenderTargetView = nullptr;
    
public:
    /// Initialize renderer.
    void Initialize( unsigned int Width, unsigned int Height );

    /// Set vertex buffer.
    void SetVertexBuffer( const CRD11VertexBuffer* CRVertexBuffer, unsigned int Slot );

    /// Set index buffer.
    void SetIndexBuffer( const CRD11IndexBuffer* CRIndexBuffer );

    /// Set vertex shader.
    void SetVertexShader( const CRD11VertexShader* CRVertexShader );

    /// Set input layout.
    void SetInputLayout( const CRD11InputLayout* CRInputLayout, unsigned int Slot );

    /// Set pixel shader.
    void SetPixelShader( const CRD11PixelShader* CRPixelShader );

    /// Draw.
    void Draw( unsigned int Slot ) const;

    /// Clear render target.
    void ClearRenderTarget() const;

    /// Present.
    void Present() const;
private:
    /// Initialize render target.
    void _InitializeRenderTarget();
    
    /// Initialize viewport.
    void _InitializeViewport( float Width, float Height ) const;
};
