#pragma once


#include "CRD11BindingConstantBuffer.h"
#include "CRD11Include.h"
#include "CRD11Mesh.h"
#include "CRD11Types.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11Renderer
//---------------------------------------------------------------------------------------------------------------------
class CRD11Renderer
{
private:
    CRD11Mesh Mesh;

    CRD11BindingConstantBuffer< CRMatrix > TransformBuffer;
    CRD11RenderTargetViewSPtr RenderTargetView;
    
public:
    /// Initialize renderer.
    void Initialize( unsigned int Width, unsigned int Height );

    /// Draw.
    void Draw();

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
