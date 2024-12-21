#pragma once


#include "CRD11Object.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11ConstantBuffer
//---------------------------------------------------------------------------------------------------------------------
class CRD11ConstantBuffer : public CRD11Object< ID3D11Buffer >
{
public:
    /// Create constant buffer.
    void Create( D3D11_USAGE Usage, unsigned int CpuAccess, unsigned int Size );
};
