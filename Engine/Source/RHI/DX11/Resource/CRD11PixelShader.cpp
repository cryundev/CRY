#include "CRD11PixelShader.h"
#include "CRD11Device.h"
#include "Source/RHI/DX11/CRD11.h"
#include "Source/Utility/Generic/CRGeneric.h"
#include <d3dcompiler.h>


//---------------------------------------------------------------------------------------------------------------------
/// Create shader.
//---------------------------------------------------------------------------------------------------------------------
void CRD11PixelShader::Create( ID3DBlob* CompiledShader )
{
    if ( !CompiledShader ) return;

    ID3D11PixelShader* createdShader = nullptr;
    HRESULT hr = GD11.GetDevice()->CreatePixelShader( CompiledShader->GetBufferPointer(), CompiledShader->GetBufferSize(), nullptr, &createdShader );
    CommitCreatedObject( createdShader, hr );

    CRGeneric::CheckError( hr );
}
