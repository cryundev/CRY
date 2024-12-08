#include "CRD11VertexBuffer.h"
#include "CRD11.h"
#include "CRD11Device.h"


//---------------------------------------------------------------------------------------------------------------------
/// Create vertex buffer
//---------------------------------------------------------------------------------------------------------------------
void CRD11VertexBuffer::Create( D3D11_USAGE Usage, unsigned int CpuAccess, const void* BlobPtr, unsigned int Stride, unsigned int Count )
{
	D3D11_BUFFER_DESC bd;      
	bd.Usage          = Usage;  
	bd.ByteWidth      = Stride * Count;  
	bd.BindFlags      = D3D11_BIND_VERTEX_BUFFER;  
	bd.CPUAccessFlags = CpuAccess;
  
	D3D11_SUBRESOURCE_DATA sd;  
	sd.pSysMem     = BlobPtr;
	sd.SysMemPitch = Stride;
  
	HRESULT hr = GD11.GetDevice()->CreateBuffer( &bd, &sd, &BufferPtr );
	if ( FAILED( hr ) )
	{
		return;
	}
}
