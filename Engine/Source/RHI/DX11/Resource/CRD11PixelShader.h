#pragma once


#include "CRD11Object.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11PixelShader
//---------------------------------------------------------------------------------------------------------------------
class CRD11PixelShader : public CRD11Object< ID3D11PixelShader >
{
public:
    /// Create shader.
    void Create( ID3DBlob* CompiledShader );
};
