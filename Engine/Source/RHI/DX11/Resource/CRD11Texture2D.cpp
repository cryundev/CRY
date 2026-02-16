#include "CRD11Texture2D.h"
#include "CRD11Device.h"
#include "Source/RHI/DX11/CRD11.h"
#include "Source/Utility/Generic/CRGeneric.h"


//---------------------------------------------------------------------------------------------------------------------
/// Create texture.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Texture2D::Create( const D3D11_TEXTURE2D_DESC& Description, const D3D11_SUBRESOURCE_DATA* Data )
{
    ID3D11Texture2D* createdTexture = nullptr;
    HRESULT hr = GD11.GetDevice()->CreateTexture2D( &Description, Data, &createdTexture );
    CommitCreatedObject( createdTexture, hr );

    CRGeneric::CheckError( hr );
}
