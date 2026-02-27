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
    CRVector        LookAtDirection = CRVector( 0.f, 0.f, 1.f );
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

    /// Get normalized world-space look direction.
    CRVector GetLookDirection() const;

    /// Get normalized world-space right direction.
    CRVector GetRightDirection() const;

    /// Get normalized world-space up direction.
    CRVector GetUpDirection() const;
    
    /// Try unproject NDC coordinate to world position at clip-space z.
    bool TryConvertNdcToWorld( f32 NdcX, f32 NdcY, f32 ClipZ, CRVector& OutWorldPosition ) const;

    /// Set view size.
    void SetViewSize( f32 InViewWidth, f32 InViewHeight );

    /// Set look at direction.
    void SetLookAtDirection( const CRVector& InLookAtDirection ) { LookAtDirection = InLookAtDirection; }

    /// Set look at direction.
    void SetLookAtDirection( f32 X, f32 Y, f32 Z ) { LookAtDirection = CRVector( X, Y, Z ); }
    
private:
    /// Try create inverse view-projection matrix.
    bool _TryCreateInverseViewProjection( CRMatrix& OutInverseViewProjection ) const;
};
