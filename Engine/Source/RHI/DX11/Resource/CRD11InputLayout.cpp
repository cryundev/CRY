#include "CRD11InputLayout.h"
#include "CRD11Device.h"
#include "Source/RHI/DX11/CRD11.h"
#include "Source/Utility/Generic/CRGeneric.h"


//---------------------------------------------------------------------------------------------------------------------
/// Create input layout.
//---------------------------------------------------------------------------------------------------------------------
void CRD11InputLayout::Create( D3D11_INPUT_ELEMENT_DESC* Elments, u32 Count, ID3DBlob* CompiledShader )
{
    if ( !CompiledShader ) return;

    ID3D11InputLayout* createdInputLayout = nullptr;
    HRESULT hr = GD11.GetDevice()->CreateInputLayout( Elments, Count, CompiledShader->GetBufferPointer(), CompiledShader->GetBufferSize(), &createdInputLayout );
    CommitCreatedObject( createdInputLayout, hr );

    CRGeneric::CheckError( hr );
}
