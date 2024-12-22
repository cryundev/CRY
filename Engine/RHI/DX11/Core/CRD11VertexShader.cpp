#include "CRD11VertexShader.h"
#include "../CRD11.h"
#include "CRD11Device.h"
#include "../../../Utility/Generic/CRGeneric.h"
#include <d3dcompiler.h>


//---------------------------------------------------------------------------------------------------------------------
/// Create shader.
//---------------------------------------------------------------------------------------------------------------------
void CRD11VertexShader::Create( ID3DBlob* CompiledShader )
{
    if ( !CompiledShader ) return;

    HRESULT hr = GD11.GetDevice()->CreateVertexShader( CompiledShader->GetBufferPointer(), CompiledShader->GetBufferSize(), nullptr, &ObjectPtr );
    CRGeneric::CheckError( hr );
}
