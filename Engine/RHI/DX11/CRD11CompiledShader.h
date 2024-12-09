#pragma once


#include "CRD11Include.h"
#include "../../CRTypes.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11CompiledShader
//---------------------------------------------------------------------------------------------------------------------
class CRD11CompiledShader
{
private:
    ID3DBlob* CompiledShader = nullptr;

public:
    /// Destructor
    virtual ~CRD11CompiledShader();

    /// Create compiled shader.
    void Create( const CWRString& Path, const CRString& EntryPoint, const CRString& ShaderModel );

    /// Get compiled shader.
    ID3DBlob* GetCompiledShader() const { return CompiledShader; }
};
