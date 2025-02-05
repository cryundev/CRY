#include "CRD11RasterizerState.h"
#include "CRD11Device.h"
#include "Source/RHI/DX11/CRD11.h"
#include "Source/Utility/Generic/CRGeneric.h"


//---------------------------------------------------------------------------------------------------------------------
/// Create rasterizer state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RasterizerState::Create( const D3D11_RASTERIZER_DESC& Description )
{
    HRESULT hr = GD11.GetDevice()->CreateRasterizerState( &Description, &ObjectPtr );
    CRGeneric::CheckError( hr );
}
