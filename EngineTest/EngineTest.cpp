#pragma comment( lib, "Engine.lib" )


#include "TestActor.h"
#include <crtdbg.h>


#define TEST_ENTITY 1


#if TEST_ENTITY

#else
#error Empty test.
#endif


//---------------------------------------------------------------------------------------------------------------------
/// main
//---------------------------------------------------------------------------------------------------------------------
int main()
{
#if _DEBUG
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    EngineTest test;

    if( test.Initialize() )
    {
        test.Run();
    }

    test.Shutdown();
    
    return 0;
}
