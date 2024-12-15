#include "CRD11VertexBuffer.h"
#include "CRD11.h"
#include "CRD11Device.h"
#include "../../Utility/CRLog.h"


//---------------------------------------------------------------------------------------------------------------------
/// Create vertex buffer
//---------------------------------------------------------------------------------------------------------------------
void CRD11VertexBuffer::Create( D3D11_USAGE Usage, unsigned int CpuAccess, const void* BlobPtr, unsigned int Stride, unsigned int InCount )
{
	Count = InCount;
	
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof( D3D11_BUFFER_DESC ) );
	
	bd.Usage          = Usage;  
	bd.ByteWidth      = Stride * Count;  
	bd.BindFlags      = D3D11_BIND_VERTEX_BUFFER;  
	bd.CPUAccessFlags = CpuAccess;
  
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory( &sd, sizeof( D3D11_SUBRESOURCE_DATA ) );
	
	sd.pSysMem = BlobPtr;
  
	HRESULT hr = GD11.GetDevice()->CreateBuffer( &bd, &sd, &ObjectPtr );

	if ( FAILED( hr ) )
	{
		GLog.AddErrorLog( hr );
		
		return;
	}
}
