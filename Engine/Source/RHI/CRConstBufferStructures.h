#pragma once


#include "Source/Core/CRTypes.h"
#include "Source/Core/Math/CRMath.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRViewProjection
//---------------------------------------------------------------------------------------------------------------------
struct alignas( 16 ) CRViewProjection
{
    CRMatrix View;
    CRMatrix Projection;
};

static_assert( sizeof( CRViewProjection ) % 16 == 0, "CRViewProjection size must be 16-byte aligned." );


//---------------------------------------------------------------------------------------------------------------------
/// CRMaterialProperties
//---------------------------------------------------------------------------------------------------------------------
struct alignas( 16 ) CRMaterialProperties
{
    CRVector4D DiffuseColor  = { 1.0f, 1.0f, 1.0f, 1.0f };
    CRVector4D SpecularColor = { 1.0f, 1.0f, 1.0f, 32.0f };
};

static_assert( sizeof( CRMaterialProperties ) % 16 == 0, "CRMaterialProperties size must be 16-byte aligned." );


//---------------------------------------------------------------------------------------------------------------------
/// Type-specific GPU light entries
//---------------------------------------------------------------------------------------------------------------------
struct alignas( 16 ) CRDirectionalLightData
{
    CRVector4D Direction = { 0.0f, -1.0f, 0.0f, 0.0f };  // xyz = light travel direction
    CRVector4D Color     = { 1.0f,  1.0f, 1.0f, 1.0f };  // xyz = RGB, w = intensity
};

static_assert( sizeof( CRDirectionalLightData ) == 32, "CRDirectionalLightData must be exactly 32 bytes." );
static_assert( sizeof( CRDirectionalLightData ) % 16 == 0, "CRDirectionalLightData size must be 16-byte aligned." );


//---------------------------------------------------------------------------------------------------------------------
/// CRPointLightData
//---------------------------------------------------------------------------------------------------------------------
struct alignas( 16 ) CRPointLightData
{
    CRVector4D Position = { 0.0f, 0.0f, 0.0f, 0.0f };  // xyz = world position
    CRVector4D Color    = { 1.0f, 1.0f, 1.0f, 1.0f };  // xyz = RGB, w = intensity
    f32        Range    = 100.0f;
    f32        _Pad0    = 0.0f;
    f32        _Pad1    = 0.0f;
    f32        _Pad2    = 0.0f;
};

static_assert( sizeof( CRPointLightData ) == 48, "CRPointLightData must be exactly 48 bytes." );
static_assert( sizeof( CRPointLightData ) % 16 == 0, "CRPointLightData size must be 16-byte aligned." );


//---------------------------------------------------------------------------------------------------------------------
/// CRSpotLightData
//---------------------------------------------------------------------------------------------------------------------
struct alignas( 16 ) CRSpotLightData
{
    CRVector4D Position     = { 0.0f,  0.0f, 0.0f, 0.0f };
    CRVector4D Direction    = { 0.0f, -1.0f, 0.0f, 0.0f };
    CRVector4D Color        = { 1.0f,  1.0f, 1.0f, 1.0f };
    f32        Range        = 100.0f;
    f32        SpotInnerCos = 0.966f;
    f32        SpotOuterCos = 0.866f;
    f32        _Pad0        = 0.0f;
};

static_assert( sizeof( CRSpotLightData ) == 64, "CRSpotLightData must be exactly 64 bytes." );
static_assert( sizeof( CRSpotLightData ) % 16 == 0, "CRSpotLightData size must be 16-byte aligned." );


//---------------------------------------------------------------------------------------------------------------------
/// CRLightsBuffer
//---------------------------------------------------------------------------------------------------------------------
static constexpr u32 CRMaxDirectionalLights = 8;
static constexpr u32 CRMaxPointLights       = 8;
static constexpr u32 CRMaxSpotLights        = 8;

struct alignas( 16 ) CRLightsBuffer
{
    CRVector4D             AmbientColor = { 0.1f, 0.1f, 0.1f, 1.0f };
    u32                    DirectionalCount = 0;
    u32                    PointCount       = 0;
    u32                    SpotCount        = 0;
    u32                    _Pad0            = 0;
    CRDirectionalLightData DirectionalLights[ CRMaxDirectionalLights ] = {};
    CRPointLightData       PointLights      [ CRMaxPointLights       ] = {};
    CRSpotLightData        SpotLights       [ CRMaxSpotLights        ] = {};
};

static_assert( sizeof( CRLightsBuffer ) % 16 == 0, "CRLightsBuffer size must be 16-byte aligned." );


//---------------------------------------------------------------------------------------------------------------------
/// CRCameraProperties
//---------------------------------------------------------------------------------------------------------------------
struct alignas( 16 ) CRCameraProperties
{
    CRVector4D Position = { 0.0f, 0.0f, 0.0f, 1.0f };  // xyz = world position, w = unused
};

static_assert( sizeof( CRCameraProperties ) % 16 == 0, "CRCameraProperties size must be 16-byte aligned." );
