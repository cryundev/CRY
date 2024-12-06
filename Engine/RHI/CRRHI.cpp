#include "CRRHI.h"
#include "DX11/CRD11.h"
#include "DX11/CRD11Device.h"
#include "DX11/CRD11Renderer.h"


//---------------------------------------------------------------------------------------------------------------------
/// Initialize RHI.
//---------------------------------------------------------------------------------------------------------------------
void InitializeRHI( HWND hWnd, unsigned int Width, unsigned int Height )
{
	GD11.Create( hWnd );
	GD11Renderer.Initialize( Width, Height );
}

//---------------------------------------------------------------------------------------------------------------------
/// Render frame RHI.
//---------------------------------------------------------------------------------------------------------------------
void RenderFrameRHI()
{
	GD11Renderer.ClearRenderTarget();
	GD11Renderer.Present();
}
