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


//---------------------------------------------------------------------------------------------------------------------
/// Statics 
//---------------------------------------------------------------------------------------------------------------------

CRArray< CRTransform > CRTransform::Transforms;

//---------------------------------------------------------------------------------------------------------------------
/// Get
//---------------------------------------------------------------------------------------------------------------------
CRTransform& CRTransform::Get( const CRIdentity::id_t& Id )
{
    CRIdentity::id_t index = CRIdentity::IndexOf( Id );

    if ( Transforms.size() <= index )
    {
        Transforms.resize( index + 1 );
        return Transforms[ index ];
    }
    else
    {
        return Transforms[ index ];
    }
}
