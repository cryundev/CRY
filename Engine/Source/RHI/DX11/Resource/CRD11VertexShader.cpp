#include "CRD11VertexShader.h"
#include "CRD11Device.h"
#include "Source/RHI/DX11/CRD11.h"
#include "Source/Utility/Generic/CRGeneric.h"
#include <d3dcompiler.h>


//---------------------------------------------------------------------------------------------------------------------
/// Create shader.
//---------------------------------------------------------------------------------------------------------------------
void CRD11VertexShader::Create( ID3DBlob* CompiledShader )
{
    if ( !CompiledShader ) return;

    ID3D11VertexShader* createdShader = nullptr;
    HRESULT hr = GD11.GetDevice()->CreateVertexShader( CompiledShader->GetBufferPointer(), CompiledShader->GetBufferSize(), nullptr, &createdShader );
    CommitCreatedObject( createdShader, hr );

    CRGeneric::CheckError( hr );
}
