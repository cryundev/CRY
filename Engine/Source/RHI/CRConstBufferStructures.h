#pragma once


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
/// CRLightProperties
//---------------------------------------------------------------------------------------------------------------------
struct alignas( 16 ) CRLightProperties
{
    CRVector4D Direction = { 0.0f, 0.0f, 1.0f, 1.0f };
    CRVector4D Color     = { 1.0f, 1.0f, 1.0f, 1.0f };
};

static_assert( sizeof( CRLightProperties ) % 16 == 0, "CRLightProperties size must be 16-byte aligned." );
