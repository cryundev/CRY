#include "CRScript.h"


using ScriptRegistry = CRMap< CRName, CRScriptCreator >;


//---------------------------------------------------------------------------------------------------------------------
/// Registry
//---------------------------------------------------------------------------------------------------------------------
ScriptRegistry& Registry()
{
    static ScriptRegistry reg;
    return reg;
}

//---------------------------------------------------------------------------------------------------------------------
/// RegisterScript
//---------------------------------------------------------------------------------------------------------------------
bool RegisterScript( const CRName& Name, const CRScriptCreator& Creator )
{
    bool result = Registry().insert( ScriptRegistry::value_type( Name, Creator ) ).second;

    assert( result );

    return result;
}