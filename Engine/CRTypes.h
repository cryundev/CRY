#pragma once


#include "DirectXTK/Inc/SimpleMath.h"
#include <string>
#include <vector>


using CRVector   = DirectX::SimpleMath::Vector3;
using CRVector2D = DirectX::SimpleMath::Vector2;


template< typename T >
using CRArray = std::vector< T >;

using CRString  = std::string;
using CWRString = std::wstring;