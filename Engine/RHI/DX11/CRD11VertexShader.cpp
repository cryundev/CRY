#include "CRD11VertexShader.h"
#include "CRD11.h"
#include "CRD11Device.h"
#include "../../Utility/CRLog.h"
#include <d3dcompiler.h>


//---------------------------------------------------------------------------------------------------------------------
/// Destructor
//---------------------------------------------------------------------------------------------------------------------
CRD11VertexShader::~CRD11VertexShader()
{
	if ( VertexShaderPtr )
	{
		VertexShaderPtr->Release();
		VertexShaderPtr = nullptr;
	}
}

//---------------------------------------------------------------------------------------------------------------------
/// Create shader.
//---------------------------------------------------------------------------------------------------------------------
void CRD11VertexShader::Create( ID3DBlob* CompiledShader )
{
	if ( !CompiledShader ) return;

	HRESULT hr = GD11.GetDevice()->CreateVertexShader( CompiledShader->GetBufferPointer(), CompiledShader->GetBufferSize(), nullptr, &VertexShaderPtr );
	
	if ( FAILED( hr ) )
	{
		GLog.AddErrorLog( hr );
		
		return;
	}
}
