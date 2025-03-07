#include "CRD11IndexBuffer.h"
#include "CRD11Device.h"
#include "Source/RHI/DX11/CRD11.h"
#include "Source/Utility/Generic/CRGeneric.h"
#include "Source/Utility/Log/CRLog.h"


//---------------------------------------------------------------------------------------------------------------------
/// Create index buffer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11IndexBuffer::Create( D3D11_USAGE Usage, u32 CpuAccess, const CRArray< i32 >& Indice )
{
    Count = (u32)( Indice.size() );

    D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof( D3D11_BUFFER_DESC ) );

    bd.Usage          = Usage;  
    bd.ByteWidth      = sizeof( i32 ) * Count;  
    bd.BindFlags      = D3D11_BIND_INDEX_BUFFER;  
    bd.CPUAccessFlags = CpuAccess;

    D3D11_SUBRESOURCE_DATA sd;
    ZeroMemory( &sd, sizeof( D3D11_SUBRESOURCE_DATA ) );

    sd.pSysMem = Indice.data();

    HRESULT hr = GD11.GetDevice()->CreateBuffer( &bd, &sd, &ObjectPtr );
    CRGeneric::CheckError( hr );
}
