#include "CRD11ResourceManager.h"
#include "CRD11CompiledShader.h"
#include "CRD11IndexBuffer.h"
#include "CRD11InputLayout.h"
#include "CRD11PixelShader.h"
#include "CRD11SamplerState.h"
#include "CRD11ShaderResourceView.h"
#include "CRD11Texture2D.h"
#include "CRD11VertexBuffer.h"
#include "CRD11VertexShader.h"


//---------------------------------------------------------------------------------------------------------------------
/// Clear
//---------------------------------------------------------------------------------------------------------------------
void CRD11ResourceManager::Clear()
{
	VertexBuffers.clear();
	IndexBuffers .clear();
	InputLayouts .clear();
	VertexShaders.clear();
	PixelShaders .clear();
}

CRD11VertexBufferSPtr CRD11ResourceManager::GetVertexBuffer( const CRName& Name )
{
	return _GetResource< CRD11VertexBuffer >( Name, VertexBuffers );
}

CRD11IndexBufferSPtr CRD11ResourceManager::GetIndexBuffer( const CRName& Name )
{
	return _GetResource< CRD11IndexBuffer >( Name, IndexBuffers );
}

CRD11InputLayoutSPtr CRD11ResourceManager::GetInputLayout( const CRName& Name )
{
	return _GetResource< CRD11InputLayout >( Name, InputLayouts );
}

CRD11CompiledShaderSPtr CRD11ResourceManager::GetCompiledShader( const CRName& Name )
{
	return _GetResource< CRD11CompiledShader >( Name, CompiledShaders );
}

CRD11VertexShaderSPtr CRD11ResourceManager::GetVertexShader( const CRName& Name )
{
	return _GetResource< CRD11VertexShader >( Name, VertexShaders );
}

CRD11PixelShaderSPtr CRD11ResourceManager::GetPixelShader( const CRName& Name )
{
	return _GetResource< CRD11PixelShader >( Name, PixelShaders );
}

CRD11Texture2DSPtr CRD11ResourceManager::GetTexture2D( const CRName& Name )
{
	return _GetResource< CRD11Texture2D >( Name, Texture2Ds );
}

CRD11ShaderResourceViewSPtr CRD11ResourceManager::GetShaderResourceView( const CRName& Name )
{
	return _GetResource< CRD11ShaderResourceView >( Name, ShaderResourceViews );
}

CRD11SamplerStateSPtr CRD11ResourceManager::GetSamplerState( const CRName& Name )
{
	return _GetResource< CRD11SamplerState >( Name, SamplerStates );
}
