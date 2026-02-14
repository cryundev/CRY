#include "CRActor.h"


//---------------------------------------------------------------------------------------------------------------------
/// Destory
//---------------------------------------------------------------------------------------------------------------------
void CRActor::Destroy()
{
    for ( const CRComponentRemover& remover : ComponentRemovers )
    {
        remover( ObjectId );
    }

    ComponentRemovers.clear();

    CRObject::Destroy();
}

//---------------------------------------------------------------------------------------------------------------------
/// Initialize components.
//---------------------------------------------------------------------------------------------------------------------
void CRActor::InitializeComponents()
{
    if ( !CRIdentity::IsValid( ObjectId ) ) return;

    AddComponent< CRTransformComponent >();
}
