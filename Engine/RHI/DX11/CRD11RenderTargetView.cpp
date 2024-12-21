#include "CRD11RenderTargetView.h"
#include "CRD11.h"
#include "CRD11Device.h"


//---------------------------------------------------------------------------------------------------------------------
/// Create render target view.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderTargetView::Create( ID3D11Resource* Resource, const D3D11_RENDER_TARGET_VIEW_DESC* Description )
{
    HRESULT hr = GD11.GetDevice()->CreateRenderTargetView( Resource, Description, &ObjectPtr );
}
