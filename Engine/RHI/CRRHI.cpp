#include "CRRHI.h"
#include "../Core/CRVertex.h"
#include "DX11/CRD11.h"
#include "DX11/CRD11CompiledShader.h"
#include "DX11/CRD11Device.h"
#include "DX11/CRD11InputLayout.h"
#include "DX11/CRD11PixelShader.h"
#include "DX11/CRD11Renderer.h"
#include "DX11/CRD11VertexBuffer.h"
#include "DX11/CRD11VertexShader.h"


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

	CRD11VertexBuffer triangle2;
	triangle2.Create( D3D11_USAGE_DEFAULT, 0, GCRVTriangle2, sizeof( CRVertex ), 3 );

	GD11Renderer.SetVertexBuffer( &triangle2, 1 );
	
	CRD11CompiledShader compiledVS;
	compiledVS.Create( L"RHI/DX11/HLSL/shader.hlsl", "VS", "vs_5_0" );
	
	CRD11VertexShader vertexShader;
	vertexShader.Create( compiledVS.GetCompiledShader() );

	GD11Renderer.SetVertexShader( &vertexShader );

	D3D11_INPUT_ELEMENT_DESC elements[] =
	{
		{ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",       0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	CRD11InputLayout inputLayout;
	inputLayout.Create( elements, ARRAYSIZE( elements ), compiledVS.GetCompiledShader() );

	GD11Renderer.SetInputLayout( &inputLayout, 0 );

	D3D11_INPUT_ELEMENT_DESC elements2[] =
	{
		{ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",       0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	CRD11InputLayout inputLayout2;
	inputLayout2.Create( elements2, ARRAYSIZE( elements2 ), compiledVS.GetCompiledShader() );

	GD11Renderer.SetInputLayout( &inputLayout2, 1 );

	CRD11CompiledShader compiledPS;
	compiledPS.Create( L"RHI/DX11/HLSL/shader.hlsl", "PS", "ps_5_0" );

	CRD11PixelShader pixelShader;
	pixelShader.Create( compiledPS.GetCompiledShader() );

	GD11Renderer.SetPixelShader( &pixelShader );
}

//---------------------------------------------------------------------------------------------------------------------
/// Render frame RHI.
//---------------------------------------------------------------------------------------------------------------------
void CRRHI::RenderFrame()
{
	GD11Renderer.ClearRenderTarget();
	GD11Renderer.Draw( 0 );
	GD11Renderer.Draw( 1 );
	GD11Renderer.Present();
}
