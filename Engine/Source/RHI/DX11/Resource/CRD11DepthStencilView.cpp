#include "CRD11DepthStencilView.h"
#include "CRD11Device.h"
#include "Source/RHI/DX11/CRD11.h"


//---------------------------------------------------------------------------------------------------------------------
/// Create depth stencil view.
//---------------------------------------------------------------------------------------------------------------------
void CRD11DepthStencilView::Create( ID3D11Resource* Resource, const D3D11_DEPTH_STENCIL_VIEW_DESC& Description )
{
    ID3D11DepthStencilView* createdView = nullptr;
    HRESULT hr = GD11.GetDevice()->CreateDepthStencilView( Resource, &Description, &createdView );

    CommitCreatedObject( createdView, hr );
}
