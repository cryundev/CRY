#pragma once


#include "CRD11Object.h"
#include "Source/Core/CRTypes.h"
#include "Source/Core/Containers/CRContainerInc.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11IndexBuffer
//---------------------------------------------------------------------------------------------------------------------
class CRD11IndexBuffer : public CRD11Object< ID3D11Buffer >
{
private:
    unsigned int Count = 0;

public:
    /// Create index buffer.
    void Create( D3D11_USAGE Usage, u32 CpuAccess, const CRArray< i32 >& Indice );

    /// Get count.
    u32 GetCount() const { return Count; }
};
