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
	/// Constructor
	CRD11VertexShader() = default;

	/// Destructor
	virtual ~CRD11VertexShader();
	
	/// Create shader.
	void Create( ID3DBlob* CompiledShader );

	/// Get shader pointer.
	ID3D11VertexShader* GetShaderPtr() const { return VertexShaderPtr; }
};
