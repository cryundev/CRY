#pragma once


#include "CRD11Object.h"
#include "Source/Core/CRTypes.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11ConstantBuffer
//---------------------------------------------------------------------------------------------------------------------
class CRD11ConstantBuffer : public CRD11Object< ID3D11Buffer >
{
public:
    /// Create constant buffer.
    void Create( D3D11_USAGE Usage, u32 CpuAccess, u32 Size );
};
