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
    const CRVector& lookAt   = CRVector::Transform( LookAtDirection, transform->GetRotation() );
    
    return DirectX::XMMatrixLookAtLH( location, location + lookAt, Up );
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
