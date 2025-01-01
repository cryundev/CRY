#pragma once


#include <memory>


template< typename T >
using CRSharedPtr = std::shared_ptr< T >;

template< typename T >
using CRWeakPtr = std::weak_ptr< T >;

template< typename T >
CRSharedPtr< T > CRMakeShared( T* New )
{
	return std::shared_ptr< T >( New );
}


#define DECLARE_SMART_PTR( ClassName ) \
class ClassName; \
using ClassName##SPtr = CRSharedPtr< ##ClassName >; \
using ClassName##WPtr = CRWeakPtr  < ##ClassName >; \


DECLARE_SMART_PTR( CRD11VertexBuffer       )
DECLARE_SMART_PTR( CRD11IndexBuffer        )
DECLARE_SMART_PTR( CRD11ConstantBuffer     )
DECLARE_SMART_PTR( CRD11InputLayout        )
DECLARE_SMART_PTR( CRD11CompiledShader     )
DECLARE_SMART_PTR( CRD11VertexShader       )
DECLARE_SMART_PTR( CRD11PixelShader        )
DECLARE_SMART_PTR( CRD11Texture2D          )
DECLARE_SMART_PTR( CRD11SamplerState       )
DECLARE_SMART_PTR( CRD11ShaderResourceView )
DECLARE_SMART_PTR( CRD11RenderTargetView   )


//---------------------------------------------------------------------------------------------------------------------
/// ED11RenderingPipelineStage
//---------------------------------------------------------------------------------------------------------------------
enum class ED11RenderingPipelineStage
{
    IA, // Input Assembler
    VS, // Vertex Shader
    GS, // Geometry Shader
    DS, // Domain Shader
    HS, // Hull Shader
    CS, // Compute Shader
    PS, // Pixel Shader
    OM, // Output Merger
    Max,
};

//---------------------------------------------------------------------------------------------------------------------
/// Enum Constantbuffers Vertexshader Stage
//---------------------------------------------------------------------------------------------------------------------
enum class ECVS
{
    Transform,
    ViewProjection,
    Max,
};

//---------------------------------------------------------------------------------------------------------------------
/// Enum Constantbuffers Pertexshader Stage
//---------------------------------------------------------------------------------------------------------------------
enum class ECPS
{
    LightDirection,
    LightColor,
    Max,
};


