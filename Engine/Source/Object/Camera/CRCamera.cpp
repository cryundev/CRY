#include "CRCamera.h"


//---------------------------------------------------------------------------------------------------------------------
/// Initialize.
//---------------------------------------------------------------------------------------------------------------------
void CRCamera::Initialize( EProjectionType InProjectionType, f32 InFieldOfView, f32 InViewWidth, f32 InViewHeight, f32 InNearDistance, f32 InFarDistance )
{
    ProjectionType = InProjectionType;
    FieldOfView    = InFieldOfView;
    ViewWidth      = InViewWidth;
    ViewHeight     = InViewHeight;
    NearDistance   = InNearDistance;
    FarDistance    = InFarDistance;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get view matrix.
//---------------------------------------------------------------------------------------------------------------------
CRMatrix CRCamera::GetViewMatrix() const
{
    CRTransformComponent* transform = GetTransform();
    if ( !transform ) return CRMatrix::Identity;

    const CRVector& location = transform->GetLocation();
    const CRVector& lookAt   = GetLookDirection();
    const CRVector& up       = GetUpDirection();
    
    return DirectX::XMMatrixLookAtLH( location, location + lookAt, up );
}

//---------------------------------------------------------------------------------------------------------------------
/// Get projection matrix.
//---------------------------------------------------------------------------------------------------------------------
CRMatrix CRCamera::GetProjectionMatrix() const
{
    switch ( ProjectionType )
    {
        case EProjectionType::Orthographic: return DirectX::XMMatrixOrthographicLH  ( ViewWidth, ViewHeight, NearDistance, FarDistance );
        case EProjectionType::Perspective:  return DirectX::XMMatrixPerspectiveFovLH( DirectX::XMConvertToRadians( FieldOfView ), ViewWidth / ViewHeight, NearDistance, FarDistance );
    }

    return CRMatrix::Identity;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get normalized world-space look direction.
//---------------------------------------------------------------------------------------------------------------------
CRVector CRCamera::GetLookDirection() const
{
    if ( CRTransformComponent* transform = GetTransform() )
    {
        CRVector lookDirection = CRVector::Transform( LookAtDirection, transform->GetRotation() );
        if ( lookDirection.LengthSquared() > CRMath::Epsilon )
        {
            lookDirection.Normalize();
            return lookDirection;
        }
    }

    CRVector fallbackLook = LookAtDirection;
    if ( fallbackLook.LengthSquared() <= CRMath::Epsilon )
    {
        fallbackLook = CRVector::Backward;
    }
    fallbackLook.Normalize();
    return fallbackLook;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get normalized world-space right direction.
//---------------------------------------------------------------------------------------------------------------------
CRVector CRCamera::GetRightDirection() const
{
    CRVector upDirection = Up;
    if ( upDirection.LengthSquared() <= CRMath::Epsilon )
    {
        upDirection = CRVector::Up;
    }
    upDirection.Normalize();

    CRVector rightDirection = upDirection.Cross( GetLookDirection() );
    if ( rightDirection.LengthSquared() <= CRMath::Epsilon )
    {
        if ( CRTransformComponent* transform = GetTransform() )
        {
            rightDirection = transform->GetRight();
        }
    }

    if ( rightDirection.LengthSquared() <= CRMath::Epsilon )
    {
        rightDirection = CRVector::Right;
    }

    rightDirection.Normalize();
    return rightDirection;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get normalized world-space up direction.
//---------------------------------------------------------------------------------------------------------------------
CRVector CRCamera::GetUpDirection() const
{
    CRVector upDirection = GetLookDirection().Cross( GetRightDirection() );
    if ( upDirection.LengthSquared() <= CRMath::Epsilon )
    {
        upDirection = CRVector::Up;
    }

    upDirection.Normalize();
    return upDirection;
}

//---------------------------------------------------------------------------------------------------------------------
/// Try unproject NDC coordinate to world position at clip-space z.
//---------------------------------------------------------------------------------------------------------------------
bool CRCamera::TryConvertNdcToWorld( f32 NdcX, f32 NdcY, f32 ClipZ, CRVector& OutWorldPosition ) const
{
    CRMatrix inverseViewProjection;
    if ( !_TryCreateInverseViewProjection( inverseViewProjection ) ) return false;
    
    CRVector4D clipPoint( NdcX, NdcY, ClipZ, 1.0f );
    CRVector4D worldPoint = CRVector4D::Transform( clipPoint, inverseViewProjection );

    if ( CRMath::IsNearlyZero( worldPoint.w ) ) return false;

    f32 inverseW = 1.0f / worldPoint.w;
    OutWorldPosition = CRVector( worldPoint.x * inverseW, worldPoint.y * inverseW, worldPoint.z * inverseW );

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Set view size.
//---------------------------------------------------------------------------------------------------------------------
void CRCamera::SetViewSize( f32 InViewWidth, f32 InViewHeight )
{
    ViewWidth  = InViewWidth;
    ViewHeight = InViewHeight;
}

//---------------------------------------------------------------------------------------------------------------------
/// Try create inverse view-projection matrix.
//---------------------------------------------------------------------------------------------------------------------
bool CRCamera::_TryCreateInverseViewProjection( CRMatrix& OutInverseViewProjection ) const
{
    CRMatrix viewProjection = GetViewMatrix() * GetProjectionMatrix();

    DirectX::XMVECTOR determinant = DirectX::XMMatrixDeterminant( viewProjection );
    
    f32 determinantValue = DirectX::XMVectorGetX( determinant );
    if ( CRMath::IsNearlyZero( determinantValue ) ) return false;

    OutInverseViewProjection = viewProjection.Invert();
    
    return true;
}
