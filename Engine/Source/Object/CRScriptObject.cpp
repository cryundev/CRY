#include "CRScriptObject.h"


namespace CRScript
{
    using ScriptRegistery = CRMap< CRName, CRScriptCreator >;

    ScriptRegistery& Registery()
    {
        static ScriptRegistery reg;
        return reg;
    }

    bool RegisterScript( CRName Name, CRScriptCreator Creator )
    {
        bool result = Registery().insert( ScriptRegistery::value_type( Name, Creator ) ).second;

        assert( result );

        return result;
    }
};