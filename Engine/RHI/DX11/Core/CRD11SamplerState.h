#pragma once


#include "CRD11Object.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11SamplerState
//---------------------------------------------------------------------------------------------------------------------
class CRD11SamplerState : public CRD11Object< ID3D11SamplerState >
{
public:
    /// Create sampler state.
    void Create( const D3D11_SAMPLER_DESC& Description );
};
