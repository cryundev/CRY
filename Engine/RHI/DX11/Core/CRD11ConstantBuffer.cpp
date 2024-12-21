#include "CRD11ConstantBuffer.h"
#include "../CRD11.h"
#include "CRD11Device.h"
#include "../../../Utility/Log/CRLog.h"


//---------------------------------------------------------------------------------------------------------------------
/// Create constant buffer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11ConstantBuffer::Create( D3D11_USAGE Usage, unsigned int CpuAccess, unsigned int Size )
{
    D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof( D3D11_BUFFER_DESC ) );
	
    bd.Usage          = Usage;  
    bd.ByteWidth      = Size;  
    bd.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;  
    bd.CPUAccessFlags = CpuAccess;

    HRESULT hr = GD11.GetDevice()->CreateBuffer( &bd, nullptr, &ObjectPtr );

    if ( FAILED( hr ) )
    {
        GLog.AddErrorLog( hr );
		
        return;
    }
}
