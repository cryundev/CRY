#pragma once


#include "Source/Core/CRTypes.h"
#include "Source/Core/Math/CRMath.h"
#include "Source/Object/CRActor.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRCamera
//---------------------------------------------------------------------------------------------------------------------
class CRCamera : public CRActor
{
public:
    enum class EProjectionType
    {
        Perspective,
        Orthographic
    };

private:
    EProjectionType ProjectionType  = EProjectionType::Perspective;
    CRVector        LookAtDirection = CRVector::Forward;
    CRVector        Up              = CRVector::Up;
    f32             FieldOfView     = 90.0f;
    f32             ViewWidth       = 1920.f;
    f32             ViewHeight      = 1080.f;
    f32             NearDistance    = 0.1f;
    f32             FarDistance     = 10000.f;
    
public:
    /// Constructor
    CRCamera() = default;

    /// Initialize.
    void Initialize( EProjectionType InProjectionType, f32 InFieldOfView, f32 InViewWidth, f32 InViewHeight, f32 InNearDistance, f32 InFarDistance );

    /// Get view matrix.
    CRMatrix GetViewMatrix() const;

    /// Get projection matrix.
    CRMatrix GetProjectionMatrix() const;

    /// Set look at direction.
    void SetLookAtDirection( const CRVector& InLookAtDirection ) { LookAtDirection = InLookAtDirection; }

    /// Set look at direction.
    void SetLookAtDirection( f32 X, f32 Y, f32 Z ) { LookAtDirection = CRVector( X, Y, Z ); }
};
