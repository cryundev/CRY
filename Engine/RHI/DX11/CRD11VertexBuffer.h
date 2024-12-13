#pragma once


#include "CRD11Include.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11VertexBuffer
//---------------------------------------------------------------------------------------------------------------------
class CRD11VertexBuffer
{
private:
	ID3D11Buffer* BufferPtr = nullptr;
	unsigned int  Count     = 0;

public:
	/// Create vertex buffer.
	void Create( D3D11_USAGE Usage, unsigned int CpuAccess, const void* BlobPtr, unsigned int Stride, unsigned int Count );

	/// Get BufferPtr.
	ID3D11Buffer* GetBufferPtr() const { return BufferPtr; }

	/// Get count.
	unsigned int GetCount() const { return Count; }
};
