#include "CRWorld.h"


//---------------------------------------------------------------------------------------------------------------------
/// Constructor.
//---------------------------------------------------------------------------------------------------------------------
CRWorld::CRWorld()
{
    Camera = SpawnActor< CRCamera >();
}

//---------------------------------------------------------------------------------------------------------------------
/// Despawn actor.
//---------------------------------------------------------------------------------------------------------------------
void CRWorld::DespawnActor( const CRObjectId& ActorId )
{
    auto itr = std::ranges::find_if( Actors, [ ActorId ] ( const CRActor* Actor )
    {
        return Actor->GetObjectId() == ActorId;
    } );

    if ( itr != Actors.end() )
    {
        CRTransformComponent::Remove( (*itr)->GetObjectId() );

        (*itr)->Destroy();

        delete *itr;
        Actors.erase( itr );
    }
}
