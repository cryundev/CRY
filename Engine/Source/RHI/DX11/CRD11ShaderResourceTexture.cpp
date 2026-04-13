#include "CRD11ShaderResourceTexture.h"
#include "CRD11.h"
#include "CRD11ResourceManager.h"
#include "Resource/CRD11SamplerState.h"
#include "Resource/CRD11ShaderResourceView.h"
#include "Resource/CRD11Texture2D.h"
#include "Extras/DirectXTK/Inc/WICTextureLoader.h"
#include "Source/Utility/Log/CRLog.h"
#include "Source/Utility/WIC/CRWICTextureLoader.h"


//---------------------------------------------------------------------------------------------------------------------
/// Create from file.
//---------------------------------------------------------------------------------------------------------------------
void CRD11ShaderResourceTexture::Create( const CRPath& Path )
{
    const CRString resourceName = Path.lexically_normal().string();
    
	Texture2D          = GD11RM.GetTexture2D         ( resourceName );
    ShaderResourceView = GD11RM.GetShaderResourceView( resourceName );
    SamplerState       = GD11RM.GetSamplerState      ( resourceName );
	
	if ( Texture2D.expired() || ShaderResourceView.expired() || SamplerState.expired() ) return;

    _CreateSamplerState();

    CRWICTextureLoader loader;
    if ( !loader.LoadFromFile( Path ) ) return;
    
    D3D11_TEXTURE2D_DESC td;
    ZeroMemory( &td, sizeof( D3D11_TEXTURE2D_DESC ) );  
    
    td.Width              = loader.GetWidth();  
    td.Height             = loader.GetHeight();
    td.MipLevels          = 1;  
    td.ArraySize          = 1;  
    td.Format             = loader.GetDxgiFormat();  
    td.SampleDesc.Count   = 1;
    td.SampleDesc.Quality = 0;  
    td.Usage              = D3D11_USAGE_DEFAULT;  
    td.BindFlags          = D3D11_BIND_SHADER_RESOURCE;  
    td.CPUAccessFlags     = 0;  
    td.MiscFlags          = 0;
    
    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory( &data, sizeof( D3D11_SUBRESOURCE_DATA ) );
    
    data.pSysMem          = loader.GetPixels();
    data.SysMemPitch      = loader.GetRowPitch();
    data.SysMemSlicePitch = loader.GetImageSize();
    
    Texture2D.lock()->Create( td, &data );
    
    D3D11_SHADER_RESOURCE_VIEW_DESC srvd;  
    ZeroMemory( &srvd, sizeof( D3D11_SHADER_RESOURCE_VIEW_DESC ) );  
    
    srvd.Format                    = loader.GetDxgiFormat();  
    srvd.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;  
    srvd.Texture2D.MostDetailedMip = 0;  
    srvd.Texture2D.MipLevels       = 1;
    
    ID3D11Resource* texture2D = Texture2D.lock()->GetObjectPtr();
    ShaderResourceView.lock()->Create( texture2D, srvd );
}

//---------------------------------------------------------------------------------------------------------------------
/// Create a 1x1 solid-color texture.
//---------------------------------------------------------------------------------------------------------------------
void CRD11ShaderResourceTexture::CreateSolidColor( const CRName& ResourceName, u8 R, u8 G, u8 B, u8 A )
{
	Texture2D          = GD11RM.GetTexture2D         ( ResourceName );
    ShaderResourceView = GD11RM.GetShaderResourceView( ResourceName );
    SamplerState       = GD11RM.GetSamplerState      ( ResourceName );

	if ( Texture2D.expired() || ShaderResourceView.expired() || SamplerState.expired() ) return;

    _CreateSamplerState();

    const u8 pixel[ 4 ] = { R, G, B, A };

    D3D11_TEXTURE2D_DESC td;
    ZeroMemory( &td, sizeof( D3D11_TEXTURE2D_DESC ) );

    td.Width              = 1;
    td.Height             = 1;
    td.MipLevels          = 1;
    td.ArraySize          = 1;
    td.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
    td.SampleDesc.Count   = 1;
    td.SampleDesc.Quality = 0;
    td.Usage              = D3D11_USAGE_DEFAULT;
    td.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
    td.CPUAccessFlags     = 0;
    td.MiscFlags          = 0;

    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory( &data, sizeof( D3D11_SUBRESOURCE_DATA ) );

    data.pSysMem          = pixel;
    data.SysMemPitch      = sizeof( pixel );
    data.SysMemSlicePitch = sizeof( pixel );

    Texture2D.lock()->Create( td, &data );

    D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
    ZeroMemory( &srvd, sizeof( D3D11_SHADER_RESOURCE_VIEW_DESC ) );

    srvd.Format                    = td.Format;
    srvd.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvd.Texture2D.MostDetailedMip = 0;
    srvd.Texture2D.MipLevels       = 1;

    ID3D11Resource* texture2D = Texture2D.lock()->GetObjectPtr();
    ShaderResourceView.lock()->Create( texture2D, srvd );
}

//---------------------------------------------------------------------------------------------------------------------
/// Create sampler state.
//---------------------------------------------------------------------------------------------------------------------
void CRD11ShaderResourceTexture::_CreateSamplerState() const
{
    D3D11_SAMPLER_DESC sd;
    ZeroMemory( &sd, sizeof( D3D11_SAMPLER_DESC ) );

    sd.Filter           = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.AddressU         = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressV         = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressW         = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.MipLODBias       = 0.0f;
    sd.MaxAnisotropy    = 1;
    sd.ComparisonFunc   = D3D11_COMPARISON_ALWAYS;
    sd.BorderColor[ 0 ] = 0;
    sd.BorderColor[ 1 ] = 0;
    sd.BorderColor[ 2 ] = 0;
    sd.BorderColor[ 3 ] = 0;
    sd.MinLOD           = 0;
    sd.MaxLOD           = D3D11_FLOAT32_MAX;

    SamplerState.lock()->Create( sd );
}
