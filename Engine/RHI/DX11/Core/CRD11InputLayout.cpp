#include "CRD11InputLayout.h"
#include "../CRD11.h"
#include "CRD11Device.h"
#include "../../../Utility/Log/CRLog.h"


/// Create input layout.
//---------------------------------------------------------------------------------------------------------------------
void CRD11InputLayout::Create( D3D11_INPUT_ELEMENT_DESC* Elments, unsigned int Count, ID3DBlob* CompiledShader )
{
    if ( !CompiledShader ) return;

    HRESULT hr = GD11.GetDevice()->CreateInputLayout( Elments, Count, CompiledShader->GetBufferPointer(), CompiledShader->GetBufferSize(), &ObjectPtr );

    if ( FAILED( hr ) )
    {
        GLog.AddErrorLog( hr );
        
        return;
    }
}
