#include "CRWorld.h"


//---------------------------------------------------------------------------------------------------------------------
/// Constructor.
//---------------------------------------------------------------------------------------------------------------------
CRWorld::CRWorld()
{
    Camera = SpawnActor< CRCamera >();
}

//---------------------------------------------------------------------------------------------------------------------
/// Destructor.
//---------------------------------------------------------------------------------------------------------------------
CRWorld::~CRWorld()
{
    for ( CRActor* actor : Actors )
    {
        if ( !actor ) continue;

        actor->Destroy();
        delete actor;
    }

    Actors.clear();
    Camera = nullptr;
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
        if ( *itr == Camera )
        {
            Camera = nullptr;
        }

        (*itr)->Destroy();

        delete *itr;
        Actors.erase( itr );
    }
}
