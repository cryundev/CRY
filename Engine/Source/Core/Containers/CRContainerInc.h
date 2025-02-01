#pragma once


#include <vector>
#include <unordered_map>


//---------------------------------------------------------------------------------------------------------------------
/// Containers
//---------------------------------------------------------------------------------------------------------------------
template< typename T >
using CRArray = std::vector< T >;

template< typename K, typename V >
using CRMap = std::unordered_map< K, V >;