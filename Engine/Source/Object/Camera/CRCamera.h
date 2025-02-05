#pragma once


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
    float           FieldOfView     = 90.0f;
    float           ViewWidth       = 1920.f;
    float           ViewHeight      = 1080.f;
    float           NearDistance    = 0.1f;
    float           FarDistance     = 10000.f;
    
public:
    /// Constructor
    CRCamera() = default;

    /// Initialize.
    void Initialize( EProjectionType InProjectionType, float InFieldOfView, float InViewWidth, float InViewHeight, float InNearDistance, float InFarDistance );

    /// Get view matrix.
    CRMatrix GetViewMatrix() const;

    /// Get projection matrix.
    CRMatrix GetProjectionMatrix() const;

    /// Set look at direction.
    void SetLookAtDirection( const CRVector& InLookAtDirection ) { LookAtDirection = InLookAtDirection; }

    /// Set look at direction.
    void SetLookAtDirection( float X, float Y, float Z ) { LookAtDirection = CRVector( X, Y, Z ); }
};
