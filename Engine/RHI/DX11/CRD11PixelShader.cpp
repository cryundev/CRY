#include "CRD11PixelShader.h"
#include "CRD11.h"
#include "CRD11Device.h"
#include "../../Utility/CRLog.h"
#include <d3dcompiler.h>


//---------------------------------------------------------------------------------------------------------------------
/// Destructor
//---------------------------------------------------------------------------------------------------------------------
CRD11PixelShader::~CRD11PixelShader()
{
	if ( PixelShaderPtr )
	{
		PixelShaderPtr->Release();
		PixelShaderPtr = nullptr;
	}
}

//---------------------------------------------------------------------------------------------------------------------
/// Create shader.
//---------------------------------------------------------------------------------------------------------------------
void CRD11PixelShader::Create( ID3DBlob* CompiledShader )
{
	if ( !CompiledShader ) return;
	
	HRESULT hr = GD11.GetDevice()->CreatePixelShader( CompiledShader->GetBufferPointer(), CompiledShader->GetBufferSize(), nullptr, &PixelShaderPtr );
	
	if ( FAILED( hr ) )
	{
		GLog.AddErrorLog( hr );
		
		return;
	}
}
