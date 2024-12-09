#pragma once


#include "CRD11Include.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11InputLayout
//---------------------------------------------------------------------------------------------------------------------
class CRD11InputLayout
{
public:
	ID3D11InputLayout* InputLayoutPtr = nullptr;

public:
	/// Create input layout.
	void Create( D3D11_INPUT_ELEMENT_DESC* Elments, unsigned int Count, ID3DBlob* CompiledShader );

	/// Get input layout.
	ID3D11InputLayout* GetInputLayoutPtr() const { return InputLayoutPtr; }
};
