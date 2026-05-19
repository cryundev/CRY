#pragma once


#include "Source/Core/CRTextureFormat.h"
#include <dxgiformat.h>


//---------------------------------------------------------------------------------------------------------------------
/// CRD11TextureFormat
//---------------------------------------------------------------------------------------------------------------------
namespace CRD11TextureFormat
{
    /// Convert CRY texture format to DXGI.
    DXGI_FORMAT ToDXGI( ECRTextureFormat Format );

    /// Convert DXGI texture format to CRY texture format.
    ECRTextureFormat FromDXGI( DXGI_FORMAT Format );
}
