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
