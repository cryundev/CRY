#pragma once


#include "CRD11Types.h"
#include "Source/Core/CRTypes.h"
#include "Source/Core/Strings/CRStringInc.h"


#include <d3d11.h>


//---------------------------------------------------------------------------------------------------------------------
/// CRD11RenderTarget2D
//---------------------------------------------------------------------------------------------------------------------
class CRD11RenderTarget2D
{
private:
    CRD11Texture2DWPtr          Texture;
    CRD11RenderTargetViewWPtr   RenderTargetView;
    CRD11ShaderResourceViewWPtr ShaderResourceView;

    u32         Width  = 0;
    u32         Height = 0;
    DXGI_FORMAT Format = DXGI_FORMAT_UNKNOWN;

public:
    /// Create render target.
    void Create( const CRName& BaseName, u32 InWidth, u32 InHeight, DXGI_FORMAT InFormat );

    /// Release resources.
    void Release();

    /// Resize.
    void Resize( const CRName& BaseName, u32 InWidth, u32 InHeight, DXGI_FORMAT InFormat );

    /// Get render target view.
    ID3D11RenderTargetView* GetRTV() const;

    /// Get shader resource view.
    ID3D11ShaderResourceView* GetSRV() const;

    /// Is valid.
    bool IsValid() const;
};
