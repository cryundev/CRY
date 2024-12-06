#pragma once


class ID3D11RenderTargetView;


//---------------------------------------------------------------------------------------------------------------------
/// CRD11Renderer
//---------------------------------------------------------------------------------------------------------------------
class CRD11Renderer
{
private:
    ID3D11RenderTargetView* RenderTargetView = nullptr;
    
public:
    /// Initialize renderer.
    void Initialize( unsigned int Width, unsigned int Height );

    // Clear render target.
    void ClearRenderTarget() const;

    // Present.
    void Present() const;

private:
    // Initialize render target.
    void _InitializeRenderTarget();
    
    // Initialize viewport.
    void _InitializeViewport( float Width, float Height ) const;
};
