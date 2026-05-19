#include "CRD11TextureFormat.h"


//---------------------------------------------------------------------------------------------------------------------
/// Convert CRY texture format to DXGI.
//---------------------------------------------------------------------------------------------------------------------
DXGI_FORMAT CRD11TextureFormat::ToDXGI( ECRTextureFormat Format )
{
    switch ( Format )
    {
        case ECRTextureFormat::R32G32B32A32_Float:        return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case ECRTextureFormat::R32G32B32_Float:           return DXGI_FORMAT_R32G32B32_FLOAT;
        case ECRTextureFormat::R16G16B16A16_Float:        return DXGI_FORMAT_R16G16B16A16_FLOAT;
        case ECRTextureFormat::R16G16B16A16_UNorm:        return DXGI_FORMAT_R16G16B16A16_UNORM;
        case ECRTextureFormat::R10G10B10_XR_Bias_A2_UNorm: return DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
        case ECRTextureFormat::R10G10B10A2_UNorm:         return DXGI_FORMAT_R10G10B10A2_UNORM;
        case ECRTextureFormat::R9G9B9E5_SharedExp:        return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
        case ECRTextureFormat::R8G8B8A8_UNorm:            return DXGI_FORMAT_R8G8B8A8_UNORM;
        case ECRTextureFormat::B8G8R8A8_UNorm:            return DXGI_FORMAT_B8G8R8A8_UNORM;
        case ECRTextureFormat::B8G8R8X8_UNorm:            return DXGI_FORMAT_B8G8R8X8_UNORM;
        case ECRTextureFormat::B5G5R5A1_UNorm:            return DXGI_FORMAT_B5G5R5A1_UNORM;
        case ECRTextureFormat::B5G6R5_UNorm:              return DXGI_FORMAT_B5G6R5_UNORM;
        case ECRTextureFormat::R32_Float:                 return DXGI_FORMAT_R32_FLOAT;
        case ECRTextureFormat::R16_Float:                 return DXGI_FORMAT_R16_FLOAT;
        case ECRTextureFormat::R16_UNorm:                 return DXGI_FORMAT_R16_UNORM;
        case ECRTextureFormat::R8_UNorm:                  return DXGI_FORMAT_R8_UNORM;
        case ECRTextureFormat::A8_UNorm:                  return DXGI_FORMAT_A8_UNORM;
        case ECRTextureFormat::BC1_UNorm:                 return DXGI_FORMAT_BC1_UNORM;
        case ECRTextureFormat::BC1_UNorm_SRGB:            return DXGI_FORMAT_BC1_UNORM_SRGB;
        case ECRTextureFormat::BC3_UNorm:                 return DXGI_FORMAT_BC3_UNORM;
        case ECRTextureFormat::BC3_UNorm_SRGB:            return DXGI_FORMAT_BC3_UNORM_SRGB;
        case ECRTextureFormat::BC5_UNorm:                 return DXGI_FORMAT_BC5_UNORM;
        case ECRTextureFormat::BC7_UNorm:                 return DXGI_FORMAT_BC7_UNORM;
        case ECRTextureFormat::BC7_UNorm_SRGB:            return DXGI_FORMAT_BC7_UNORM_SRGB;

        default: break;
    }

    return DXGI_FORMAT_UNKNOWN;
}

//---------------------------------------------------------------------------------------------------------------------
/// Convert DXGI texture format to CRY texture format.
//---------------------------------------------------------------------------------------------------------------------
ECRTextureFormat CRD11TextureFormat::FromDXGI( DXGI_FORMAT Format )
{
    switch ( Format )
    {
        case DXGI_FORMAT_R32G32B32A32_FLOAT:         return ECRTextureFormat::R32G32B32A32_Float;
        case DXGI_FORMAT_R32G32B32_FLOAT:            return ECRTextureFormat::R32G32B32_Float;
        case DXGI_FORMAT_R16G16B16A16_FLOAT:         return ECRTextureFormat::R16G16B16A16_Float;
        case DXGI_FORMAT_R16G16B16A16_UNORM:         return ECRTextureFormat::R16G16B16A16_UNorm;
        case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM: return ECRTextureFormat::R10G10B10_XR_Bias_A2_UNorm;
        case DXGI_FORMAT_R10G10B10A2_UNORM:          return ECRTextureFormat::R10G10B10A2_UNorm;
        case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:         return ECRTextureFormat::R9G9B9E5_SharedExp;
        case DXGI_FORMAT_R8G8B8A8_UNORM:             return ECRTextureFormat::R8G8B8A8_UNorm;
        case DXGI_FORMAT_B8G8R8A8_UNORM:             return ECRTextureFormat::B8G8R8A8_UNorm;
        case DXGI_FORMAT_B8G8R8X8_UNORM:             return ECRTextureFormat::B8G8R8X8_UNorm;
        case DXGI_FORMAT_B5G5R5A1_UNORM:             return ECRTextureFormat::B5G5R5A1_UNorm;
        case DXGI_FORMAT_B5G6R5_UNORM:               return ECRTextureFormat::B5G6R5_UNorm;
        case DXGI_FORMAT_R32_FLOAT:                  return ECRTextureFormat::R32_Float;
        case DXGI_FORMAT_R16_FLOAT:                  return ECRTextureFormat::R16_Float;
        case DXGI_FORMAT_R16_UNORM:                  return ECRTextureFormat::R16_UNorm;
        case DXGI_FORMAT_R8_UNORM:                   return ECRTextureFormat::R8_UNorm;
        case DXGI_FORMAT_A8_UNORM:                   return ECRTextureFormat::A8_UNorm;
        case DXGI_FORMAT_BC1_UNORM:                  return ECRTextureFormat::BC1_UNorm;
        case DXGI_FORMAT_BC1_UNORM_SRGB:             return ECRTextureFormat::BC1_UNorm_SRGB;
        case DXGI_FORMAT_BC3_UNORM:                  return ECRTextureFormat::BC3_UNorm;
        case DXGI_FORMAT_BC3_UNORM_SRGB:             return ECRTextureFormat::BC3_UNorm_SRGB;
        case DXGI_FORMAT_BC5_UNORM:                  return ECRTextureFormat::BC5_UNorm;
        case DXGI_FORMAT_BC7_UNORM:                  return ECRTextureFormat::BC7_UNorm;
        case DXGI_FORMAT_BC7_UNORM_SRGB:             return ECRTextureFormat::BC7_UNorm_SRGB;

        default: break;
    }

    return ECRTextureFormat::Unknown;
}
