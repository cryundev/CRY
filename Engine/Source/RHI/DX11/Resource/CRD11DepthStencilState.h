#pragma once


#include "CRD11Object.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11DepthStencilState
//---------------------------------------------------------------------------------------------------------------------
class CRD11DepthStencilState : public CRD11Object< ID3D11DepthStencilState >
{
public:
    /// Create depth stencil state.
    void Create( const D3D11_DEPTH_STENCIL_DESC& Description );
};
