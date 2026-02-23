#pragma once


#include "Source/Core/CRTypes.h"
#include "Extras/DirectXTK/Inc/SimpleMath.h"


#pragma comment ( lib, "DirectXTK.lib" )


//---------------------------------------------------------------------------------------------------------------------
/// Common math epsilon and helpers.
//---------------------------------------------------------------------------------------------------------------------
namespace CRMath
{
    static constexpr f32 Epsilon = 1.0e-6f;


    inline bool IsNearlyZero( f32 Value, f32 Tolerance = Epsilon )
    {
        return Value > -Tolerance && Value < Tolerance;
    }

    template< typename T >
    constexpr T Abs( const T& Value )
    {
        return ( Value < static_cast< T >( 0 ) ) ? -Value : Value;
    }

    template< typename T >
    constexpr T Min( const T& A, const T& B )
    {
        return ( B < A ) ? B : A;
    }

    template< typename T >
    constexpr T Max( const T& A, const T& B )
    {
        return ( A < B ) ? B : A;
    }

    template< typename T >
    constexpr T Clamp( const T& Value, const T& MinValue, const T& MaxValue )
    {
        return Min( Max( Value, MinValue ), MaxValue );
    }
}


//---------------------------------------------------------------------------------------------------------------------
/// Variants
//---------------------------------------------------------------------------------------------------------------------
using CRVector     = DirectX::SimpleMath::Vector3;
using CRVector2D   = DirectX::SimpleMath::Vector2;
using CRVector4D   = DirectX::SimpleMath::Vector4;
using CRQuaternion = DirectX::SimpleMath::Quaternion;
using CRMatrix     = DirectX::SimpleMath::Matrix;
