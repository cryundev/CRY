#include "CRD11Texture2DSampler.h"
#include "CRD11.h"
#include "CRD11ResourceManager.h"
#include "CRD11ShaderResourceView.h"
#include "CRD11Texture2D.h"


//---------------------------------------------------------------------------------------------------------------------
/// Create from file.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Texture2DSampler::Create( const CRString& Path )
{
	Texture2D          = GD11RM.GetTexture2D         ( Path );
	ShaderResourceView = GD11RM.GetShaderResourceView( Path );
	
	if ( Texture2D.expired() || ShaderResourceView.expired() ) return;
	
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory( &td, sizeof( D3D11_TEXTURE2D_DESC ) );  
	
	td.Height             = 512;  
	td.Width              = 512;  
	td.MipLevels          = 1;  
	td.ArraySize          = 1;  
	td.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;  
	td.SampleDesc.Count   = 1;
	td.SampleDesc.Quality = 0;  
	td.Usage              = D3D11_USAGE_DEFAULT;  
	td.BindFlags          = D3D11_BIND_SHADER_RESOURCE;  
	td.CPUAccessFlags     = 0;  
	td.MiscFlags          = 0;
	
	Texture2D.lock()->Create( td );
	
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;  
	ZeroMemory( &srvd, sizeof( D3D11_SHADER_RESOURCE_VIEW_DESC ) );  
  
	srvd.Format                    = DXGI_FORMAT_R8G8B8A8_UNORM;  
	srvd.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;  
	srvd.Texture2D.MostDetailedMip = 0;  
	srvd.Texture2D.MipLevels       = 1;
	
	ShaderResourceView.lock()->Create( Texture2D.lock()->GetObjectPtr(), srvd );
}
