#pragma once


#include "CRD11Object.h"
#include "../../Core/Strings/CRStringInc.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11CompiledShader
//---------------------------------------------------------------------------------------------------------------------
class CRD11CompiledShader : public CRD11Object< ID3DBlob >
{
public:
    /// Create compiled shader.
    void Create( const CRWString& Path, const CRString& EntryPoint, const CRString& ShaderModel );
};
