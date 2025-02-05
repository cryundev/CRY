#include "CRTransform.h"


CRTransform CRTransform::Identity = CRTransform();


//---------------------------------------------------------------------------------------------------------------------
/// Return converted matrix.
//---------------------------------------------------------------------------------------------------------------------
CRMatrix CRTransform::ToLocalMatrix() const
{
    return CRMatrix::CreateScale         ( Scale     ) *
           CRMatrix::CreateFromQuaternion( Rotation  ) *
           CRMatrix::CreateTranslation   ( Location  );
}

//---------------------------------------------------------------------------------------------------------------------
/// Get forward vector.
//---------------------------------------------------------------------------------------------------------------------
CRVector CRTransform::GetForward() const
{
    return CRVector::Transform( CRVector::Forward, Rotation );
}

//---------------------------------------------------------------------------------------------------------------------
/// Get right vector.
//---------------------------------------------------------------------------------------------------------------------
CRVector CRTransform::GetRight() const
{
    return CRVector::Transform( CRVector::Right, Rotation );
}

//---------------------------------------------------------------------------------------------------------------------
/// Get up vector.
//---------------------------------------------------------------------------------------------------------------------
CRVector CRTransform::GetUp() const
{
    return CRVector::Transform( CRVector::Up, Rotation );
}
