#include "CRD11Device.h"
#include "Source/Utility/Generic/CRGeneric.h"


//---------------------------------------------------------------------------------------------------------------------
/// Destructor
//---------------------------------------------------------------------------------------------------------------------
CRD11Device::~CRD11Device()
{
    if ( SwapChainPtr )
    {
	    SwapChainPtr->Release();
	    SwapChainPtr = nullptr;
    }

    if ( DeviceContextPtr )
    {
	    DeviceContextPtr->Release();
	    DeviceContextPtr = nullptr;
    }

    if ( DevicePtr )
    {
	    DevicePtr->Release();
	    DevicePtr = nullptr;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Create
//---------------------------------------------------------------------------------------------------------------------
bool CRD11Device::Create( HWND hWnd )
{
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory( &scd, sizeof( DXGI_SWAP_CHAIN_DESC ) );

    scd.BufferCount       = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow      = hWnd;
    scd.SampleDesc.Count  = 1;
    scd.Windowed          = true;

    HRESULT hr = D3D11CreateDeviceAndSwapChain
    (
	    nullptr,
	    D3D_DRIVER_TYPE_HARDWARE,
	    nullptr,
	    0,
	    nullptr,
	    0,
	    D3D11_SDK_VERSION,
	    &scd,
	    &SwapChainPtr,
	    &DevicePtr,
	    nullptr,
	    &DeviceContextPtr
    );
    
    if ( CRGeneric::CheckError( hr ) ) return false;
	    
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get maximum texture size.
//---------------------------------------------------------------------------------------------------------------------
u32 CRD11Device::GetMaxTextureSize() const
{
    if ( !DevicePtr ) return 0;

    switch( DevicePtr->GetFeatureLevel() )
    {
    case D3D_FEATURE_LEVEL_9_1:
    case D3D_FEATURE_LEVEL_9_2:  return 2048;
    case D3D_FEATURE_LEVEL_9_3:  return 4096;

    case D3D_FEATURE_LEVEL_10_0:
    case D3D_FEATURE_LEVEL_10_1: return 8192;

    default: return D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION;
    }

    return 0;
}
