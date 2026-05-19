#pragma once


#include "Source/Core/CRTypes.h"
#include "Extras/DirectXTK/Inc/SimpleMath.h"
#include <cmath>


#pragma comment ( lib, "DirectXTK.lib" )


//---------------------------------------------------------------------------------------------------------------------
/// Common math epsilon and helpers.
//---------------------------------------------------------------------------------------------------------------------
namespace CRMath
{
    static constexpr f32 f32_min = -3.402823466e+38f;
    static constexpr f32 f32_max =  3.402823466e+38f;
    static constexpr i32 i32_min = -2147483647 - 1;
    static constexpr i32 i32_max =  2147483647;
    static constexpr f32 Epsilon = 1.0e-6f;


    inline bool IsNearlyZero( f32 Value, f32 Tolerance = Epsilon )
    {
        return Value > -Tolerance && Value < Tolerance;
    }

    inline bool IsFinite( f32 Value )
    {
        return std::isfinite( Value );
    }

    inline bool IsFinite( f64 Value )
    {
        return std::isfinite( Value );
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


namespace CRMath
{
    inline bool IsFinite( const CRVector2D& Value )
    {
        return IsFinite( Value.x ) && IsFinite( Value.y );
    }

    inline bool IsFinite( const CRVector& Value )
    {
        return IsFinite( Value.x ) && IsFinite( Value.y ) && IsFinite( Value.z );
    }

    inline bool IsFinite( const CRVector4D& Value )
    {
        return IsFinite( Value.x ) && IsFinite( Value.y ) && IsFinite( Value.z ) && IsFinite( Value.w );
    }
}
