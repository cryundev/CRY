#include "EngineDLL.h"
#include "WorldAPI.ActorInternal.h"


namespace CRActorApiInternal
{
//---------------------------------------------------------------------------------------------------------------------
/// Get actor by id.
//---------------------------------------------------------------------------------------------------------------------
CRActor* GetActorById( CRIdentity::id_t Id )
{
    if ( !GWorld || !CRIdentity::IsValid( Id ) ) return nullptr;

    return GWorld->GetActor( CRObjectId( Id ) );
}

//---------------------------------------------------------------------------------------------------------------------
/// Get transform by actor id.
//---------------------------------------------------------------------------------------------------------------------
CRTransformComponent* GetTransform( CRIdentity::id_t Id )
{
    if ( CRActor* actor = GetActorById( Id ) ) return actor->GetTransform();

    return nullptr;
}
}
