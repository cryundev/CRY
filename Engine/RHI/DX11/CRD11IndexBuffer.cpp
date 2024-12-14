#include "CRD11IndexBuffer.h"
#include "CRD11.h"
#include "CRD11Device.h"
#include "../../Utility/CRLog.h"


//---------------------------------------------------------------------------------------------------------------------
/// Destructor
//---------------------------------------------------------------------------------------------------------------------
CRD11IndexBuffer::~CRD11IndexBuffer()
{
	if ( BufferPtr )
	{
		BufferPtr->Release();
		BufferPtr = nullptr;
	}
}

//---------------------------------------------------------------------------------------------------------------------
/// Create index buffer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11IndexBuffer::Create( D3D11_USAGE Usage, unsigned int CpuAccess, const CRArray< int >& Indice )
{
    Count = (unsigned int)( Indice.size() );
    
    D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof( D3D11_BUFFER_DESC ) );
	
    bd.Usage          = Usage;  
    bd.ByteWidth      = sizeof( int ) * Count;  
    bd.BindFlags      = D3D11_BIND_INDEX_BUFFER;  
    bd.CPUAccessFlags = CpuAccess;
  
    D3D11_SUBRESOURCE_DATA sd;
    ZeroMemory( &sd, sizeof( D3D11_SUBRESOURCE_DATA ) );
	
    sd.pSysMem = Indice.data();
  
    HRESULT hr = GD11.GetDevice()->CreateBuffer( &bd, &sd, &BufferPtr );

    if ( FAILED( hr ) )
    {
        GLog.AddErrorLog( hr );
		
        return;
    }
}
