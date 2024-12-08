#pragma once


#include "CRD11Include.h"
#include "../../CRTypes.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11PixelShader
//---------------------------------------------------------------------------------------------------------------------
class CRD11PixelShader
{
private:
	ID3D11PixelShader* PixelShaderPtr = nullptr;
	
public:
	/// Create shader.
	void CreateShader( const CWRString& Path, const CRString& EntryPoint, const CRString& ShaderModel );

	/// Get shader pointer.
	ID3D11PixelShader* GetShaderPtr() const { return PixelShaderPtr; }
};
