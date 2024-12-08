#pragma once


#include "CRD11Include.h"


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
    ID3D11RenderTargetView* RenderTargetView = nullptr;
    
public:
    /// Initialize renderer.
    void Initialize( unsigned int Width, unsigned int Height );

    /// Set vertex buffer.
    void SetVertexBuffer( const CRD11VertexBuffer* VertexBuffer, unsigned int Slot );

    /// Set vertex shader.
    void SetVertexShader( CRD11VertexShader* VertexShader ) const;

    /// Set pixel shader.
    void SetPixelShader( CRD11PixelShader* PixelShader ) const;

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
