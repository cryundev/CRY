#pragma once


#include "CRD11Object.h"
#include "Source/Core/CRTypes.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11InputLayout
//---------------------------------------------------------------------------------------------------------------------
class CRD11InputLayout : public CRD11Object< ID3D11InputLayout >
{
public:
    /// Create input layout.
    void Create( D3D11_INPUT_ELEMENT_DESC* Elments, u32 Count, ID3DBlob* CompiledShader );
};
