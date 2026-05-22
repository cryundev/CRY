#include "CRD11BlendState.h"
#include "CRD11Device.h"
#include "Source/RHI/DX11/CRD11.h"


//---------------------------------------------------------------------------------------------------------------------
/// Create blend state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11BlendState::Create( const D3D11_BLEND_DESC& Description )
{
    ID3D11BlendState* createdState = nullptr;
    HRESULT hr = GD11.GetDevice()->CreateBlendState( &Description, &createdState );

    CommitCreatedObject( createdState, hr );
}
