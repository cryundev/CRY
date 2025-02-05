#include "CRD11InputLayout.h"
#include "CRD11Device.h"
#include "Source/RHI/DX11/CRD11.h"
#include "Source/Utility/Generic/CRGeneric.h"


//---------------------------------------------------------------------------------------------------------------------
/// Create input layout.
//---------------------------------------------------------------------------------------------------------------------
void CRD11InputLayout::Create( D3D11_INPUT_ELEMENT_DESC* Elments, unsigned int Count, ID3DBlob* CompiledShader )
{
    if ( !CompiledShader ) return;

    HRESULT hr = GD11.GetDevice()->CreateInputLayout( Elments, Count, CompiledShader->GetBufferPointer(), CompiledShader->GetBufferSize(), &ObjectPtr );
    CRGeneric::CheckError( hr );
}
