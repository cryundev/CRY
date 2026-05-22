#pragma once


#include "CRD11Object.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11BlendState
//---------------------------------------------------------------------------------------------------------------------
class CRD11BlendState : public CRD11Object< ID3D11BlendState >
{
public:
    /// Create blend state.
    void Create( const D3D11_BLEND_DESC& Description );
};
