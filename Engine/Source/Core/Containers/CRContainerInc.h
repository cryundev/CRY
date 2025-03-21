#pragma once

#include <algorithm>
#include <deque>
#include <vector>
#include <unordered_map>


//---------------------------------------------------------------------------------------------------------------------
/// Containers
//---------------------------------------------------------------------------------------------------------------------
template< typename T >
using CRArray = std::vector< T >;

template< typename T >
using CRList = std::list< T >;

template< typename K, typename V >
using CRMap = std::unordered_map< K, V >;

template< typename T >
using CRDeque = std::deque< T >;


//---------------------------------------------------------------------------------------------------------------------
/// UtilContainer
//---------------------------------------------------------------------------------------------------------------------
namespace UtilContainer
{
    template< typename T >
    void EraseUnordered( CRArray< T >& Array, size_t Index )
    {
        std::iter_swap( Array.begin() + Index, Array.end() - 1 );
        Array.pop_back();
    }
};