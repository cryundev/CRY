#pragma once


#include "CRTypes.h"


//---------------------------------------------------------------------------------------------------------------------
/// ECRTextureFormat
//---------------------------------------------------------------------------------------------------------------------
enum class ECRTextureFormat : u32
{
    Unknown = 0,

    R32G32B32A32_Float,
    R32G32B32_Float,
    R16G16B16A16_Float,
    R16G16B16A16_UNorm,
    R10G10B10_XR_Bias_A2_UNorm,
    R10G10B10A2_UNorm,
    R9G9B9E5_SharedExp,
    R8G8B8A8_UNorm,
    B8G8R8A8_UNorm,
    B8G8R8X8_UNorm,
    B5G5R5A1_UNorm,
    B5G6R5_UNorm,
    R32_Float,
    R16_Float,
    R16_UNorm,
    R8_UNorm,
    A8_UNorm,

    BC1_UNorm,
    BC1_UNorm_SRGB,
    BC3_UNorm,
    BC3_UNorm_SRGB,
    BC5_UNorm,
    BC7_UNorm,
    BC7_UNorm_SRGB,
};
