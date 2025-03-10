#pragma once


#include <utility>


template < typename To, typename From >
To CRCast( From&& Value )
{
    return static_cast< To >( std::forward< From >( Value ) );
}
