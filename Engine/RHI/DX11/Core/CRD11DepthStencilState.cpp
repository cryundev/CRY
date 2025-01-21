#include "CRD11DepthStencilState.h"
#include "CRD11Device.h"
#include "RHI/DX11/CRD11.h"
#include "Utility/Generic/CRGeneric.h"


//---------------------------------------------------------------------------------------------------------------------
/// Create depth stencil state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11DepthStencilState::Create( const D3D11_DEPTH_STENCIL_DESC& Description )
{
    HRESULT hr = GD11.GetDevice()->CreateDepthStencilState( &Description, &ObjectPtr );
    CRGeneric::CheckError( hr );
}
