#include "CRD11PixelShader.h"
#include "CRD11.h"
#include "CRD11Device.h"
#include "../../Utility/CRLog.h"
#include <d3dcompiler.h>

//---------------------------------------------------------------------------------------------------------------------
/// Create shader.
//---------------------------------------------------------------------------------------------------------------------
void CRD11PixelShader::CreateShader( const CWRString& Path, const CRString& EntryPoint, const CRString& ShaderModel )
{
	ID3DBlob* compiledShader = nullptr;
	ID3DBlob* error          = nullptr;
	
	HRESULT hr = D3DCompileFromFile( Path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryPoint.c_str(), ShaderModel.c_str(), D3DCOMPILE_ENABLE_STRICTNESS, 0, &compiledShader, &error );
	if ( FAILED( hr ) )
	{
		if ( error )
		{
			GLog.AddLog( (char*)error->GetBufferPointer() );
			error->Release();
		}
		return;
	}

	if ( !compiledShader ) return;

	hr = GD11.GetDevice()->CreatePixelShader( compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), nullptr, &PixelShaderPtr );
	
	compiledShader->Release();

	if ( error )
	{
		error->Release();
	}

	if ( FAILED( hr ) )
	{
		return;
	}
}
