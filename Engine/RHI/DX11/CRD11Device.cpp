#include "CRD11Device.h"


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

	if ( FAILED( hr ) ) return false;
		
	return true;
}
