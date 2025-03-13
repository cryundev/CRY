#pragma once


#include "CRTest.h"
#include "TestActor.h"
#include "../Engine/Extras/ImGUI/imgui_internal.h"
#include "Source/Core/CRTypes.h"
#include "Source/Core/Containers/CRContainerInc.h"
#include "Source/Object/CRActor.h"
#include "Source/World/CRWorld.h"
#include <iostream>
#include <ctime>


//---------------------------------------------------------------------------------------------------------------------
/// EngineTest
//---------------------------------------------------------------------------------------------------------------------
class EngineTest : public CRTest
{
private:
    CRWorld World;
    CRArray< CRActor* > Actors;

    u32 Added     = 0;
    u32 Removed   = 0;
    u32 NumActors = 0;
    
public:
    //-----------------------------------------------------------------------------------------------------------------
    /// Initialize
    //-----------------------------------------------------------------------------------------------------------------
    bool Initialize() override
    {
        srand( (u32)time( nullptr ) );
        return true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Run
    //-----------------------------------------------------------------------------------------------------------------
    void Run() override
    {
        do
        {
            for ( u32 i = 0; i < 10000; ++i )
            {
                CreateRandom();
                RemoveRandom();
                
                NumActors = (u32)Actors.size();
            }

            PrintResult();
        }
        while ( getchar() != 'q' );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Shutdown
    //-----------------------------------------------------------------------------------------------------------------
    void Shutdown() override
    {
        
    }

private:
    //-----------------------------------------------------------------------------------------------------------------
    /// CreateRandom
    //-----------------------------------------------------------------------------------------------------------------
    void CreateRandom()
    {
        u32 count = rand() % 20;

        if ( Actors.empty() ) count = 10000;

        while ( count )
        {
            ++Added;

            CRActor* actor = World.SpawnActor< CRActor >();
            assert( actor->IsValid() );
            assert( CRIdentity::IsValid( actor->GetObjectId() ) );

            Actors.push_back( actor );
            
            --count;
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// RemoveRandom
    //-----------------------------------------------------------------------------------------------------------------
    void RemoveRandom()
    {
        u32 count = rand() % 20;

        if ( Actors.size() < 10000 ) return;

        while ( count > 0 )
        {
            const u32 index = (u32)rand() % (u32)Actors.size();

            CRActor* actor = Actors[ index ];
            assert( actor->IsValid() );
            assert( CRIdentity::IsValid( actor->GetObjectId() ) );

            actor->Destroy();

            Actors.erase( Actors.begin() + index );
            ++Removed;

            --count;
        }
    }

    void PrintResult()
    {
        std::cout << "Actors created: " << Added   << std::endl;
        std::cout << "Actors removed: " << Removed << std::endl;
    }
};