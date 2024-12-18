#include "CRD11Texture2D.h"
#include "CRD11.h"
#include "CRD11Device.h"
#include "../../Utility/Log/CRLog.h"


//---------------------------------------------------------------------------------------------------------------------
/// Create texture.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Texture2D::Create( const D3D11_TEXTURE2D_DESC& Description, const D3D11_SUBRESOURCE_DATA* Data )
{
    HRESULT hr = GD11.GetDevice()->CreateTexture2D( &Description, Data, &ObjectPtr );
    if ( FAILED( hr ) )
    {
        GLog.AddErrorLog( hr );
        return;
    }
}