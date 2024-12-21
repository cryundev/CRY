#include "CRRHI.h"
#include "../Core/CRVertex.h"
#include "DX11/CRD11.h"
#include "DX11/CRD11Renderer.h"
#include "DX11/CRD11ResourceManager.h"
#include "DX11/CRD11ShaderResourceTexture.h"
#include "DX11/Core/CRD11CompiledShader.h"
#include "DX11/Core/CRD11Device.h"
#include "DX11/Core/CRD11IndexBuffer.h"
#include "DX11/Core/CRD11InputLayout.h"
#include "DX11/Core/CRD11PixelShader.h"
#include "DX11/Core/CRD11VertexBuffer.h"
#include "DX11/Core/CRD11VertexShader.h"


//---------------------------------------------------------------------------------------------------------------------
/// Initialize RHI.
//---------------------------------------------------------------------------------------------------------------------
void CRRHI::Initialize( HWND hWnd, unsigned int Width, unsigned int Height )
{
	GD11.Create( hWnd );
	GD11Renderer.Initialize( Width, Height );

	CRD11VertexBufferSPtr vb = GD11RM.GetVertexBuffer( "Rect" );
	vb->Create( D3D11_USAGE_DEFAULT, 0, GCRVRect, sizeof( CRVertex ), ARRAYSIZE( GCRVRect ) );

	GD11Renderer.SetVertexBuffer( vb, 0 );
	
	CRD11IndexBufferSPtr ib = GD11RM.GetIndexBuffer( "Rect" );
	
	ib->Create( D3D11_USAGE_DEFAULT, 0, { 0, 1, 2, 1, 3, 2 } );

	GD11Renderer.SetIndexBuffer( ib );
	
	CRD11CompiledShader compiledVS;
	compiledVS.Create( L"RHI/DX11/HLSL/shader.hlsl", "VS", "vs_5_0" );
	
	CRD11VertexShaderSPtr vertexShader = GD11RM.GetVertexShader( "Diffuse" );
	vertexShader->Create( compiledVS.GetObjectPtr() );

	GD11Renderer.SetVertexShader( vertexShader );

	D3D11_INPUT_ELEMENT_DESC elements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	CRD11InputLayoutSPtr inputLayout = GD11RM.GetInputLayout( "Diffuse" );
	inputLayout->Create( elements, ARRAYSIZE( elements ), compiledVS.GetObjectPtr() );

	GD11Renderer.SetInputLayout( inputLayout, 0 );

	CRD11CompiledShader compiledPS;
	compiledPS.Create( L"RHI/DX11/HLSL/shader.hlsl", "PS", "ps_5_0" );

	CRD11PixelShaderSPtr pixelShader = CRMakeShared( new CRD11PixelShader() );
	pixelShader->Create( compiledPS.GetObjectPtr() );

	GD11Renderer.SetPixelShader( pixelShader );

	CRD11ShaderResourceTexture shaderResourceTexture;
	shaderResourceTexture.Create( "../Asset/cryun_icon.png" );
	
	GD11Renderer.SetShaderResource( CRD11ShaderResourceViewSPtr( shaderResourceTexture.GetShaderResourceView() ), 0 );
	GD11Renderer.SetSamplerState  ( CRD11SamplerStateSPtr      ( shaderResourceTexture.GetSamplerState()       ), 0 );
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
