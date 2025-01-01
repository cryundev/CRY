#include "CRCamera.h"


//---------------------------------------------------------------------------------------------------------------------
/// Initialize.
//---------------------------------------------------------------------------------------------------------------------
void CRCamera::Initialize( EProjectionType InProjectionType, float InFieldOfView, float InViewWidth, float InViewHeight, float InNearDistance, float InFarDistance )
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
    const CRVector& location = Transform.GetLocation();
    const CRVector& lookAt   = CRVector::Transform( LookAtDirection, Transform.GetRotation() );
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