#include "CRD11ShaderResourceView.h"
#include "CRD11Device.h"
#include "Source/RHI/DX11/CRD11.h"


//---------------------------------------------------------------------------------------------------------------------
/// Create shader resource view.
//---------------------------------------------------------------------------------------------------------------------
void CRD11ShaderResourceView::Create( ID3D11Resource* Resource, const D3D11_SHADER_RESOURCE_VIEW_DESC& Description )
{
    ID3D11ShaderResourceView* createdView = nullptr;
    HRESULT hr = GD11.GetDevice()->CreateShaderResourceView( Resource, &Description, &createdView );

    CommitCreatedObject( createdView, hr );
}
