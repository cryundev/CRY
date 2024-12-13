#include "CRRHI.h"
#include "../Core/CRVertex.h"
#include "DX11/CRD11.h"
#include "DX11/CRD11CompiledShader.h"
#include "DX11/CRD11Device.h"
#include "DX11/CRD11IndexBuffer.h"
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

	CRD11VertexBuffer* vb = new CRD11VertexBuffer();
	vb->Create( D3D11_USAGE_DEFAULT, 0, GCRVRect, sizeof( CRVertex ), ARRAYSIZE( GCRVRect ) );

	GD11Renderer.SetVertexBuffer( vb, 0 );

	CRD11IndexBuffer* ib = new CRD11IndexBuffer();

	CRArray< int > indices;
	indices.Add( 0 );
	indices.Add( 1 );
	indices.Add( 2 );
	indices.Add( 1 );
	indices.Add( 3 );
	indices.Add( 2 );
	
	ib->Create( D3D11_USAGE_DEFAULT, 0, indices );

	GD11Renderer.SetIndexBuffer( ib );
	
	CRD11CompiledShader* compiledVS = new CRD11CompiledShader();
	compiledVS->Create( L"RHI/DX11/HLSL/shader.hlsl", "VS", "vs_5_0" );
	
	CRD11VertexShader* vertexShader = new CRD11VertexShader();
	vertexShader->Create( compiledVS->GetCompiledShader() );

	GD11Renderer.SetVertexShader( vertexShader );

	D3D11_INPUT_ELEMENT_DESC elements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	CRD11InputLayout* inputLayout = new CRD11InputLayout();
	inputLayout->Create( elements, ARRAYSIZE( elements ), compiledVS->GetCompiledShader() );

	GD11Renderer.SetInputLayout( inputLayout, 0 );

	CRD11CompiledShader* compiledPS = new CRD11CompiledShader();
	compiledPS->Create( L"RHI/DX11/HLSL/shader.hlsl", "PS", "ps_5_0" );

	CRD11PixelShader* pixelShader = new CRD11PixelShader();
	pixelShader->Create( compiledPS->GetCompiledShader() );

	GD11Renderer.SetPixelShader( pixelShader );
}

//---------------------------------------------------------------------------------------------------------------------
/// Render frame RHI.
//---------------------------------------------------------------------------------------------------------------------
void CRRHI::RenderFrame()
{
	GD11Renderer.ClearRenderTarget();
	GD11Renderer.Draw( 0 );
	GD11Renderer.Present();
}
