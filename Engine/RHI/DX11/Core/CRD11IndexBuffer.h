#pragma once


#include "CRD11Object.h"
#include "../Core/Containers/CRContainerInc.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11IndexBuffer
//---------------------------------------------------------------------------------------------------------------------
class CRD11IndexBuffer : public CRD11Object< ID3D11Buffer >
{
private:
    unsigned int Count = 0;

public:
    /// Create index buffer.
    void Create( D3D11_USAGE Usage, unsigned int CpuAccess, const CRArray< int >& Indice );

    /// Get count.
    unsigned int GetCount() const { return Count; }
};
