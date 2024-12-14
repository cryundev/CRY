#pragma once


#include "../../CRTypes.h"
#include <d3d11.h>
#include <DirectXTK/inc/SimpleMath.h>


#pragma comment ( lib, "d3d11.lib" )
#pragma comment ( lib, "d3dcompiler.lib" )
#pragma comment ( lib, "DirectXTK.lib" )


class CRD11VertexBuffer;
using CRD11VertexBufferSPtr = CRSharedPtr< CRD11VertexBuffer >;
using CRD11VertexBufferWPtr = CRWeakPtr  < CRD11VertexBuffer >;


class CRD11IndexBuffer;
using CRD11IndexBufferSPtr = CRSharedPtr< CRD11IndexBuffer >;
using CRD11IndexBufferWPtr = CRWeakPtr  < CRD11IndexBuffer >;


class CRD11InputLayout;
using CRD11InputLayoutSPtr = CRSharedPtr< CRD11InputLayout >;
using CRD11InputLayoutWPtr = CRWeakPtr  < CRD11InputLayout >;


class CRD11CompiledShader;
using CRD11CompiledShaderSPtr = CRSharedPtr< CRD11CompiledShader >;
using CRD11CompiledShaderWPtr = CRWeakPtr  < CRD11CompiledShader >;


class CRD11VertexShader;
using CRD11VertexShaderSPtr = CRSharedPtr< CRD11VertexShader >;
using CRD11VertexShaderWPtr = CRWeakPtr  < CRD11VertexShader >;


class CRD11PixelShader;
using CRD11PixelShaderSPtr = CRSharedPtr< CRD11PixelShader >;
using CRD11PixelShaderWPtr = CRWeakPtr  < CRD11PixelShader >;