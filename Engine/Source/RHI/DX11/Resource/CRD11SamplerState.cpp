#include "CRD11SamplerState.h"
#include "CRD11Device.h"
#include "Source/RHI/DX11//CRD11.h"
#include "Source/Utility/Generic/CRGeneric.h"


//---------------------------------------------------------------------------------------------------------------------
/// Create sampler state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11SamplerState::Create( const D3D11_SAMPLER_DESC& Description )
{
    HRESULT hr = GD11.GetDevice()->CreateSamplerState( &Description, &ObjectPtr );
    CRGeneric::CheckError( hr );
}
