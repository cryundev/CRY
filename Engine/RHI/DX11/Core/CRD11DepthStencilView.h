#pragma once


#include "CRD11Object.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11DepthStencilView
//---------------------------------------------------------------------------------------------------------------------
class CRD11DepthStencilView : public CRD11Object< ID3D11DepthStencilView >
{
public:
    /// Create depth stencil view.
    void Create( ID3D11Resource* Resource, const D3D11_DEPTH_STENCIL_VIEW_DESC& Description );
};
