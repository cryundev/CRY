#pragma once


#include "CRD11Object.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11RasterizerState
//---------------------------------------------------------------------------------------------------------------------
class CRD11RasterizerState : public CRD11Object< ID3D11RasterizerState >
{
public:
    /// Create rasterizer state.
    void Create( const D3D11_RASTERIZER_DESC& Description );
    
};
