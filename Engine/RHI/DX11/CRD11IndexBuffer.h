#pragma once


#include "CRD11Include.h"
#include "../Core/Container/CRArray.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11IndexBuffer
//---------------------------------------------------------------------------------------------------------------------
class CRD11IndexBuffer
{
private:
    ID3D11Buffer* BufferPtr = nullptr;
    unsigned int  Count     = 0;

public:
    /// Create index buffer.
    void Create( D3D11_USAGE Usage, unsigned int CpuAccess, const CRArray< int >& Indice );

    /// Get BufferPtr.
    ID3D11Buffer* GetBufferPtr() const { return BufferPtr; }

    /// Get count.
    unsigned int GetCount() const { return Count; }
};
