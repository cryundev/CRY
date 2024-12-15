#include "CRD11Texture2D.h"
#include "CRD11.h"
#include "CRD11Device.h"
#include "../../Utility/CRLog.h"


//---------------------------------------------------------------------------------------------------------------------
/// Create texture.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Texture2D::Create( const D3D11_TEXTURE2D_DESC& Description )
{
	HRESULT hr = GD11.GetDevice()->CreateTexture2D( &Description, nullptr, &ObjectPtr );
	if ( FAILED( hr ) )
	{
		GLog.AddErrorLog( hr );
		return;
	}
}