#pragma once


#include <utility>


template < typename To, typename From >
To* CRCast( From&& Value )
{
    return static_cast< To* >( std::forward< From >( Value ) );
}

template < typename T >
void CRSwap( T& Left, T& Right )
{
    T temp = std::move( Left );
    Left   = std::move( Right );
    Right  = std::move( temp );
}
