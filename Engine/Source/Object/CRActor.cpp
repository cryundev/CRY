#include "CRActor.h"


//---------------------------------------------------------------------------------------------------------------------
/// Initialize components.
//---------------------------------------------------------------------------------------------------------------------
void CRActor::InitializeComponents()
{
    if ( !CRIdentity::IsValid( ObjectId ) ) return;

    Transform = AddComponent< CRTransform >();
}
