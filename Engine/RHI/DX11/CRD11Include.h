#pragma once


#include "../../CRTypes.h"
#include <d3d11.h>
#include <DirectXTK/inc/SimpleMath.h>


#pragma comment ( lib, "d3d11.lib" )
#pragma comment ( lib, "d3dcompiler.lib" )
#pragma comment ( lib, "DirectXTK.lib" )


#define DECLARE_SMART_PTR( ClassName ) \
	class ClassName; \
	using ClassName##SPtr = CRSharedPtr< ##ClassName >; \
	using ClassName##WPtr = CRWeakPtr  < ##ClassName >; \


DECLARE_SMART_PTR( CRD11VertexBuffer       )
DECLARE_SMART_PTR( CRD11IndexBuffer        )
DECLARE_SMART_PTR( CRD11InputLayout        )
DECLARE_SMART_PTR( CRD11CompiledShader     )
DECLARE_SMART_PTR( CRD11VertexShader       )
DECLARE_SMART_PTR( CRD11PixelShader        )
DECLARE_SMART_PTR( CRD11Texture2D          )
DECLARE_SMART_PTR( CRD11SamplerState       )
DECLARE_SMART_PTR( CRD11ShaderResourceView )