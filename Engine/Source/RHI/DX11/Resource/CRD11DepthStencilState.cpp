#include "CRD11DepthStencilState.h"
#include "CRD11Device.h"
#include "Source/RHI/DX11/CRD11.h"
#include "Source/Utility/Generic/CRGeneric.h"


//---------------------------------------------------------------------------------------------------------------------
/// Create depth stencil state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11DepthStencilState::Create( const D3D11_DEPTH_STENCIL_DESC& Description )
{
    ID3D11DepthStencilState* createdState = nullptr;
    HRESULT hr = GD11.GetDevice()->CreateDepthStencilState( &Description, &createdState );
    CommitCreatedObject( createdState, hr );

    CRGeneric::CheckError( hr );
}
