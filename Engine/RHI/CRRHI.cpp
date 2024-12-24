#include "CRRHI.h"
#include "DX11/CRD11.h"
#include "DX11/CRD11Renderer.h"
#include "DX11/Core/CRD11Device.h"


//---------------------------------------------------------------------------------------------------------------------
/// Initialize RHI.
//---------------------------------------------------------------------------------------------------------------------
void CRRHI::Initialize( HWND hWnd, unsigned int Width, unsigned int Height )
{
    GD11.Create( hWnd );
    GD11Renderer.Initialize( Width, Height );
}

//---------------------------------------------------------------------------------------------------------------------
/// Render frame RHI.
//---------------------------------------------------------------------------------------------------------------------
void CRRHI::RenderFrame()
{
    GD11Renderer.ClearRenderTarget();
    GD11Renderer.Draw();
    GD11Renderer.Present();
}
