#ifndef CR_ENGINE_API
#define CR_ENGINE_API extern "C" __declspec( dllexport )
#endif


#include "Engine.h"
#include "Source/Core/Identify/CRIdentity.h"
#include "Source/World/CRWorld.h"


//---------------------------------------------------------------------------------------------------------------------
/// SpawnActor
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API CRIdentity::id_t SpawnActor()

{
    if ( CRActor* actor = GWorld.get()->SpawnActor< CRActor >() )
    {
        return actor->GetObjectId();
    }

    return CRIdentity::IdMask;
}

//---------------------------------------------------------------------------------------------------------------------
/// DespawnActor
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API void DespawnActor( CRIdentity::id_t Id )
{
    GWorld.get()->DespawnActor( CRObjectId( Id ) );
}