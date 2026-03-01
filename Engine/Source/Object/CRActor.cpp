#include "CRActor.h"


//---------------------------------------------------------------------------------------------------------------------
/// Destory
//---------------------------------------------------------------------------------------------------------------------
void CRActor::Destroy()
{
    const CRArray< CRComponentRemover > removers = ComponentRemovers;
    ComponentRemovers.clear();
    ComponentRemoverTypes.clear();

    for ( const CRComponentRemover& remover : removers )
    {
        remover();
    }

    CRObject::Destroy();
}

//---------------------------------------------------------------------------------------------------------------------
/// Initialize components.
//---------------------------------------------------------------------------------------------------------------------
void CRActor::InitializeComponents()
{
    if ( !CRIdentity::IsValid( ObjectId ) ) return;

    Add< CRTransformComponent >();
}
