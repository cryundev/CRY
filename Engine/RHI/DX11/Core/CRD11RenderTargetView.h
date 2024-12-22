#pragma once


#include "CRD11Object.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11RenderTargetView
//---------------------------------------------------------------------------------------------------------------------
class CRD11RenderTargetView : public CRD11Object< ID3D11RenderTargetView >
{
public:
    /// Create render target view.
    void Create( ID3D11Resource* Resource, const D3D11_RENDER_TARGET_VIEW_DESC* Description );
};
