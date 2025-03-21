#pragma once


#include "Source/Core/Containers/CRContainerInc.h"
#include "Source/Core/Identify/CRIdentity.h"
#include "Source/Object/Camera/CRCamera.h"


class CRActor;


//---------------------------------------------------------------------------------------------------------------------
/// CRWorld
//---------------------------------------------------------------------------------------------------------------------
class CRWorld
{
protected:
    CRList< CRActor* > Actors;
    CRCamera* Camera = nullptr;

public:
    /// Constructor.
    CRWorld();

    /// Get camera.
    CRCamera* GetCamera() const { return Camera; }
    
    /// Spawn actor.
    template< ActorType T >
    T* SpawnActor();

    /// Despawn actor.
    void DespawnActor( const CRObjectId& ActorId );
};


//---------------------------------------------------------------------------------------------------------------------
/// Spawn actor.
//---------------------------------------------------------------------------------------------------------------------
template < ActorType T >
T* CRWorld::SpawnActor()
{
    CRObjectId actorId = CRIdentity::CRIDGenerator< CRObjectId >::Get().Create();

    T* newActor = new T();

    newActor->ObjectId = actorId;
    newActor->World    = this;
    newActor->InitializeComponents();

    Actors.push_back( newActor );

    return newActor;
};

