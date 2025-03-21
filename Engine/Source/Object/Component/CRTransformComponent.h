#pragma once


#include "CRComponent.h"
#include "Source/Core/Identify/CRIdentity.h"
#include "Source/Core/Math/CRMath.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRTransformComponent
//---------------------------------------------------------------------------------------------------------------------
DECLARE_TYPE_ID( CRTransformId );
class CRTransformComponent : public CRComponent< CRTransformComponent >
{
public:
    CRVector     Location = CRVector::Zero;
    CRQuaternion Rotation = CRQuaternion::Identity;
    CRVector     Scale    = CRVector::One;

    bool         bDirty   = true;
    CRMatrix     LocalMatrix;

public:
    /// Constructor.
    CRTransformComponent() = default;

    /// Update component.
    virtual void UpdateComponent( float DeltaSeconds ) override;

    /// Get local matrix.
    const CRMatrix& GetLocalMatrix() const { return LocalMatrix; }

    /// Get location.
    const CRVector& GetLocation() const { return Location; }

    /// Get Rotation.
    const CRQuaternion& GetRotation() const { return Rotation; }

    /// Get scale.
    const CRVector& GetScale() const { return Scale; }

    /// Translate.
    void Translate( const CRVector& InTranslation ) { Location += InTranslation; _SetDirty(); }

    /// Set location.
    void SetLocation( const CRVector& InLocation ) { Location = InLocation; _SetDirty(); }

    /// Set location.
    void SetLocation( float X, float Y, float Z ) { Location = CRVector( X, Y, Z ); _SetDirty(); }

    /// Set rotation.
    void SetRotation( const CRQuaternion& InRotation ) { Rotation = InRotation; _SetDirty(); }

    /// Rotate.
    void Rotate( const CRQuaternion& InRotation ) { Rotation *= InRotation; _SetDirty(); }

    /// Rotate.
    void Rotate( const CRVector& Axis, float Angle ) { Rotation *= CRQuaternion::CreateFromAxisAngle( Axis, Angle ); _SetDirty(); }

    /// Set scale.
    void SetScale( const CRVector& InScale ) { Scale = InScale; _SetDirty(); }

    /// Get forward vector.
    CRVector GetForward() const;

    /// Get right vector.
    CRVector GetRight() const;

    /// Get up vector.
    CRVector GetUp() const;

private:
    /// Set dirty.
    void _SetDirty() { bDirty = true; }

public:
    static CRTransformComponent Identity;
};
