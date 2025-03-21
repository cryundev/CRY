#include "CRTransformComponent.h"


CRTransformComponent CRTransformComponent::Identity = CRTransformComponent();


//---------------------------------------------------------------------------------------------------------------------
/// Update component.
//---------------------------------------------------------------------------------------------------------------------
void CRTransformComponent::UpdateComponent( float DeltaSeconds )
{
    if ( bDirty )
    {
        LocalMatrix = CRMatrix::CreateScale         ( Scale     ) *
                      CRMatrix::CreateFromQuaternion( Rotation  ) *
                      CRMatrix::CreateTranslation   ( Location  );

        bDirty = false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Get forward vector.
//---------------------------------------------------------------------------------------------------------------------
CRVector CRTransformComponent::GetForward() const
{
    return CRVector::Transform( CRVector::Forward, Rotation );
}

//---------------------------------------------------------------------------------------------------------------------
/// Get right vector.
//---------------------------------------------------------------------------------------------------------------------
CRVector CRTransformComponent::GetRight() const
{
    return CRVector::Transform( CRVector::Right, Rotation );
}

//---------------------------------------------------------------------------------------------------------------------
/// Get up vector.
//---------------------------------------------------------------------------------------------------------------------
CRVector CRTransformComponent::GetUp() const
{
    return CRVector::Transform( CRVector::Up, Rotation );
}