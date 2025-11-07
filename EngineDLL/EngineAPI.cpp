#include "Engine.h"
#include "EngineDLL.h"
#include "Source/Object/CRScript.h"


namespace
{
    HMODULE GameCodeDLL = nullptr;

    using GetScriptCreatorT = CRScriptCreator(*)( CRName );
    using GetScriptNamesT   = LPSAFEARRAY    (*)( void );

    GetScriptCreatorT GetScriptCreator = nullptr;
    GetScriptNamesT   GetScriptNames   = nullptr; 
}


//---------------------------------------------------------------------------------------------------------------------
/// LoadGameCodeDLL
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API i32 LoadGameCodeDLL( const char* DllPath )
{
    if ( GameCodeDLL ) return 0;

    GameCodeDLL = LoadLibraryA( DllPath );
    assert( GameCodeDLL );

    GetScriptCreator = (GetScriptCreatorT)GetProcAddress( GameCodeDLL, "GetScriptCreator" );
    GetScriptNames   = (GetScriptNamesT  )GetProcAddress( GameCodeDLL, "GetScriptNames"   );
    
    return ( GameCodeDLL && GetScriptCreator && GetScriptNames ) ? 1 : 0;
}

//---------------------------------------------------------------------------------------------------------------------
/// UnloadGameCodeDLL
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API i32 UnloadGameCodeDLL()
{
    if ( !GameCodeDLL ) return 0;

    assert( GameCodeDLL );
    i32 result = FreeLibrary( GameCodeDLL );
    assert( result );

    GameCodeDLL = nullptr;
    
    return 1;
}

//---------------------------------------------------------------------------------------------------------------------
/// ScriptCreator
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API CRScriptCreator ScriptCreator( const CRName& Name )
{
    return ( GameCodeDLL && GetScriptCreator ) ? GetScriptCreator( Name ) : nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/// ScriptNames 
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API LPSAFEARRAY ScriptNames()
{
    return ( GameCodeDLL && GetScriptNames ) ? GetScriptNames() : nullptr;
}
