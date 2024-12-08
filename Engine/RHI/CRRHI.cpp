#include "CRRHI.h"
#include "../Core/CRVertex.h"
#include "DX11/CRD11.h"
#include "DX11/CRD11Device.h"
#include "DX11/CRD11Renderer.h"
#include "DX11/CRD11VertexBuffer.h"

//---------------------------------------------------------------------------------------------------------------------
/// Initialize RHI.
//---------------------------------------------------------------------------------------------------------------------
void CRRHI::Initialize( HWND hWnd, unsigned int Width, unsigned int Height )
{
	GD11.Create( hWnd );
	GD11Renderer.Initialize( Width, Height );

	CRD11VertexBuffer triangle;
	triangle.Create( D3D11_USAGE_DEFAULT, 0, GCRVTriangle, sizeof( CRVertex ), 3 );

	GD11Renderer.SetVertexBuffer( &triangle, 0 );
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
