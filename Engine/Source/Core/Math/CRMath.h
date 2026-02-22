#pragma once


#include "Source/Core/CRTypes.h"
#include "Extras/DirectXTK/Inc/SimpleMath.h"


#pragma comment ( lib, "DirectXTK.lib" )


//---------------------------------------------------------------------------------------------------------------------
/// Common math epsilon and helpers.
//---------------------------------------------------------------------------------------------------------------------
static constexpr f32 CRMathEpsilon = 1.0e-6f;


inline bool CRIsNearlyZero( f32 Value, f32 Epsilon = CRMathEpsilon )
{
    return Value > -Epsilon && Value < Epsilon;
}


//---------------------------------------------------------------------------------------------------------------------
/// Variants
//---------------------------------------------------------------------------------------------------------------------
using CRVector     = DirectX::SimpleMath::Vector3;
using CRVector2D   = DirectX::SimpleMath::Vector2;
using CRVector4D   = DirectX::SimpleMath::Vector4;
using CRQuaternion = DirectX::SimpleMath::Quaternion;
using CRMatrix     = DirectX::SimpleMath::Matrix;
