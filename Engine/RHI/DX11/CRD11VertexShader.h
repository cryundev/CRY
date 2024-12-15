#pragma once


#include "CRD11Object.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11VertexShader
//---------------------------------------------------------------------------------------------------------------------
class CRD11VertexShader : public CRD11Object< ID3D11VertexShader >
{
public:
	/// Create shader.
	void Create( ID3DBlob* CompiledShader );
};
