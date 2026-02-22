#pragma once


#include "Engine.h"
#include "Source/Core/Identify/CRIdentity.h"
#include "Source/Object/Component/CRTransformComponent.h"
#include "Source/World/CRWorld.h"


namespace CRActorApiInternal
{
    CRActor* GetActorById( CRIdentity::id_t Id );
    CRTransformComponent* GetTransform( CRIdentity::id_t Id );
}
