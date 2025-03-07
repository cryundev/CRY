#pragma once


#include "CRD11Object.h"
#include "Source/Core/CRTypes.h"

//---------------------------------------------------------------------------------------------------------------------
/// CRD11VertexBuffer
//---------------------------------------------------------------------------------------------------------------------
class CRD11VertexBuffer : public CRD11Object< ID3D11Buffer >
{
private:
    unsigned int Count  = 0;
    unsigned int Stride = 0;

public:
    /// Create vertex buffer.
    void Create( D3D11_USAGE Usage, u32 CpuAccess, const void* BlobPtr, u32 Stride, u32 Count );

    /// Get count.
    unsigned int GetCount() const { return Count; }

    /// Get stride.
    unsigned int GetStride() const { return Stride; }
};
