#pragma once


#include "CRD11Object.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11VertexBuffer
//---------------------------------------------------------------------------------------------------------------------
class CRD11VertexBuffer : public CRD11Object< ID3D11Buffer >
{
private:
    unsigned int Count = 0;

public:
    /// Create vertex buffer.
    void Create( D3D11_USAGE Usage, unsigned int CpuAccess, const void* BlobPtr, unsigned int Stride, unsigned int Count );

    /// Get count.
    unsigned int GetCount() const { return Count; }
};
