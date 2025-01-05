#pragma once


#include "DirectXTK/Inc/SimpleMath.h"
#include <d3d11.h>


#pragma comment ( lib, "d3d11.lib" )
#pragma comment ( lib, "d3dcompiler.lib" )
#pragma comment ( lib, "Libs/DirectXTK.lib" )


//---------------------------------------------------------------------------------------------------------------------
/// DirectX
//---------------------------------------------------------------------------------------------------------------------
using CRVector     = DirectX::SimpleMath::Vector3;
using CRVector2D   = DirectX::SimpleMath::Vector2;
using CRVector4D   = DirectX::SimpleMath::Vector4;
using CRQuaternion = DirectX::SimpleMath::Quaternion;
using CRMatrix     = DirectX::SimpleMath::Matrix;