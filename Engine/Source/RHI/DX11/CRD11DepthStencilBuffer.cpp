#include "CRD11DepthStencilBuffer.h"
#include "CRD11.h"
#include "CRD11RenderingPipeline.h"
#include "CRD11ResourceManager.h"
#include "Resource/CRD11DepthStencilState.h"
#include "Resource/CRD11DepthStencilView.h"
#include "Resource/CRD11Device.h"
#include "Resource/CRD11Texture2D.h"
#include <d3d11.h>


//---------------------------------------------------------------------------------------------------------------------
/// Create depth stencil buffer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11DepthStencilBuffer::Create( u32 Width, u32 Height )
{
    BufferPtr = GD11RM.GetTexture2D( "DepthStencilBuffer" );
    if ( !BufferPtr.expired() )
    {
        D3D11_TEXTURE2D_DESC td;
        ZeroMemory( &td, sizeof( D3D11_TEXTURE2D_DESC ) );

        td.Width            = Width;
        td.Height           = Height;
        td.MipLevels        = 1;
        td.ArraySize        = 1;
        td.Format           = DXGI_FORMAT_D24_UNORM_S8_UINT;
        td.SampleDesc.Count = 1;
        td.Usage            = D3D11_USAGE_DEFAULT;
        td.BindFlags        = D3D11_BIND_DEPTH_STENCIL;

        BufferPtr.lock()->Create( td );
    }

    ViewPtr = GD11RM.GetDepthStencilView( "DepthStencilView" );
    if ( !ViewPtr.expired() )
    {
        D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
        ZeroMemory( &dsvd, sizeof( D3D11_DEPTH_STENCIL_VIEW_DESC ) );

        dsvd.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
        dsvd.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
        dsvd.Texture2D.MipSlice = 0;

        ViewPtr.lock()->Create( BufferPtr.lock()->GetObjectPtr(), dsvd );
    }

    StatePtr = GD11RM.GetDepthStencilState( "DepthStencilState" );
    if ( !StatePtr.expired() )
    {
        D3D11_DEPTH_STENCIL_DESC dsd;
        ZeroMemory( &dsd, sizeof( D3D11_DEPTH_STENCIL_DESC ) );

        dsd.DepthEnable    = true;
        dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        dsd.DepthFunc      = D3D11_COMPARISON_LESS;

        StatePtr.lock()->Create( dsd );
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Get depth stencil view.
//---------------------------------------------------------------------------------------------------------------------
ID3D11DepthStencilView* CRD11DepthStencilBuffer::GetView() const
{
    if ( ViewPtr.expired() ) return nullptr;

    return ViewPtr.lock()->GetObjectPtr();
}

//---------------------------------------------------------------------------------------------------------------------
/// Get depth stencil state.
//---------------------------------------------------------------------------------------------------------------------
ID3D11DepthStencilState* CRD11DepthStencilBuffer::GetState() const
{
    if ( StatePtr.expired() ) return nullptr;

    return StatePtr.lock()->GetObjectPtr();
}

//---------------------------------------------------------------------------------------------------------------------
/// Clear buffer.
//---------------------------------------------------------------------------------------------------------------------
void CRD11DepthStencilBuffer::ClearBuffer() const
{
    if ( ViewPtr.expired() ) return;
    
    GD11.GetDeviceContext()->ClearDepthStencilView( ViewPtr.lock()->GetObjectPtr(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0.f );
}
