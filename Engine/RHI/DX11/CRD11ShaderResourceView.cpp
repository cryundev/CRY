#include "CRD11ShaderResourceView.h"
#include "CRD11.h"
#include "CRD11Device.h"


//---------------------------------------------------------------------------------------------------------------------
/// Create shader resource view.
//---------------------------------------------------------------------------------------------------------------------
void CRD11ShaderResourceView::Create( ID3D11Resource* Resource, const D3D11_SHADER_RESOURCE_VIEW_DESC& Description )
{
	HRESULT hr = GD11.GetDevice()->CreateShaderResourceView( Resource, &Description, &ObjectPtr );
}
