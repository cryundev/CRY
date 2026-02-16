#include "CRD11RasterizerState.h"
#include "CRD11Device.h"
#include "Source/RHI/DX11/CRD11.h"


//---------------------------------------------------------------------------------------------------------------------
/// Create rasterizer state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RasterizerState::Create( const D3D11_RASTERIZER_DESC& Description )
{
    ID3D11RasterizerState* createdState = nullptr;
    HRESULT hr = GD11.GetDevice()->CreateRasterizerState( &Description, &createdState );

    CommitCreatedObject( createdState, hr );
}
