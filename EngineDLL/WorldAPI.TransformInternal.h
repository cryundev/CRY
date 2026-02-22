#pragma once


#include "Source/Core/CRTypes.h"
#include "Source/Object/Component/CRTransformComponent.h"


struct CRDllVector3
{
    f32 X = 0.0f;
    f32 Y = 0.0f;
    f32 Z = 0.0f;
};

struct CRDllTransform
{
    CRDllVector3 Position;
    CRDllVector3 RotationEuler;
    CRDllVector3 Scale = { 1.0f, 1.0f, 1.0f };
};


namespace CRTransformApiInternal
{
    void WriteTransform( const CRTransformComponent& Transform, CRDllTransform& OutTransform );
    void ApplyTransform( CRTransformComponent& Transform, const CRDllTransform& InTransform );
}
