#include "CRD11RenderTarget2D.h"
#include "CRD11.h"
#include "CRD11ResourceManager.h"
#include "Resource/CRD11RenderTargetView.h"
#include "Resource/CRD11ShaderResourceView.h"
#include "Resource/CRD11Texture2D.h"


//---------------------------------------------------------------------------------------------------------------------
/// Create render target.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderTarget2D::Create( const CRName& BaseName, u32 InWidth, u32 InHeight, DXGI_FORMAT InFormat )
{
    Width  = InWidth;
    Height = InHeight;
    Format = InFormat;

    const CRName textureName = BaseName + "_Texture2D";
    const CRName rtvName     = BaseName + "_RTV";
    const CRName srvName     = BaseName + "_SRV";

    Texture = GD11RM.GetTexture2D( textureName );
    if ( !Texture.expired() )
    {
        Texture.lock()->SetObjectPtr( nullptr );

        D3D11_TEXTURE2D_DESC td;
        ZeroMemory( &td, sizeof( D3D11_TEXTURE2D_DESC ) );

        td.Width            = Width;
        td.Height           = Height;
        td.MipLevels        = 1;
        td.ArraySize        = 1;
        td.Format           = Format;
        td.SampleDesc.Count = 1;
        td.Usage            = D3D11_USAGE_DEFAULT;
        td.BindFlags        = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

        Texture.lock()->Create( td );
    }

    RenderTargetView = GD11RM.GetRenderTargetView( rtvName );
    if ( !RenderTargetView.expired() )
    {
        RenderTargetView.lock()->SetObjectPtr( nullptr );
        RenderTargetView.lock()->Create( Texture.lock()->GetObjectPtr(), nullptr );
    }

    ShaderResourceView = GD11RM.GetShaderResourceView( srvName );
    if ( !ShaderResourceView.expired() )
    {
        ShaderResourceView.lock()->SetObjectPtr( nullptr );

        D3D11_SHADER_RESOURCE_VIEW_DESC sd;
        ZeroMemory( &sd, sizeof( D3D11_SHADER_RESOURCE_VIEW_DESC ) );

        sd.Format                    = Format;
        sd.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
        sd.Texture2D.MipLevels       = 1;
        sd.Texture2D.MostDetailedMip = 0;

        ShaderResourceView.lock()->Create( Texture.lock()->GetObjectPtr(), sd );
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Release resources.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderTarget2D::Release()
{
    if ( !ShaderResourceView.expired() ) ShaderResourceView.lock()->SetObjectPtr( nullptr );
    if ( !RenderTargetView.expired() )  RenderTargetView.lock()->SetObjectPtr( nullptr );
    if ( !Texture.expired() )           Texture.lock()->SetObjectPtr( nullptr );
}

//---------------------------------------------------------------------------------------------------------------------
/// Resize.
//---------------------------------------------------------------------------------------------------------------------
void CRD11RenderTarget2D::Resize( const CRName& BaseName, u32 InWidth, u32 InHeight, DXGI_FORMAT InFormat )
{
    Release();
    Create( BaseName, InWidth, InHeight, InFormat );
}

//---------------------------------------------------------------------------------------------------------------------
/// Get render target view.
//---------------------------------------------------------------------------------------------------------------------
ID3D11RenderTargetView* CRD11RenderTarget2D::GetRTV() const
{
    if ( RenderTargetView.expired() ) return nullptr;

    return RenderTargetView.lock()->GetObjectPtr();
}

//---------------------------------------------------------------------------------------------------------------------
/// Get shader resource view.
//---------------------------------------------------------------------------------------------------------------------
ID3D11ShaderResourceView* CRD11RenderTarget2D::GetSRV() const
{
    if ( ShaderResourceView.expired() ) return nullptr;

    return ShaderResourceView.lock()->GetObjectPtr();
}

//---------------------------------------------------------------------------------------------------------------------
/// Is valid.
//---------------------------------------------------------------------------------------------------------------------
bool CRD11RenderTarget2D::IsValid() const
{
    return GetRTV() && GetSRV();
}
