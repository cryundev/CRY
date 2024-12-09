#pragma once


#include "CRD11Include.h"


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
    ID3D11Buffer*           VertexBuffers[ D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT ] = { nullptr, };
    ID3D11VertexShader*     VertexShader     = nullptr;
    ID3D11InputLayout*      InputLayout      = nullptr;
    ID3D11PixelShader*      PixelShader      = nullptr;
    ID3D11RenderTargetView* RenderTargetView = nullptr;
    
public:
    /// Initialize renderer.
    void Initialize( unsigned int Width, unsigned int Height );

    /// Set vertex buffer.
    void SetVertexBuffer( const CRD11VertexBuffer* CRVertexBuffer, unsigned int Slot );

    /// Set vertex shader.
    void SetVertexShader( const CRD11VertexShader* CRVertexShader );

    /// Set input layout.
    void SetInputLayout( const CRD11InputLayout* CRInputLayout );

    /// Set pixel shader.
    void SetPixelShader( const CRD11PixelShader* CRPixelShader );

    /// Draw.
    void Draw() const;

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
