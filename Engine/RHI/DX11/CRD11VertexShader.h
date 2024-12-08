#pragma once


#include "CRD11Include.h"
#include "../../CRTypes.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11VertexShader
//---------------------------------------------------------------------------------------------------------------------
class CRD11VertexShader
{
private:
	ID3D11VertexShader* VertexShaderPtr = nullptr;

public:
	/// Create shader.
	void CreateShader( const CWRString& Path, const CRString& EntryPoint, const CRString& ShaderModel );

	/// Get shader pointer.
	ID3D11VertexShader* GetShaderPtr() const { return VertexShaderPtr; }
};
