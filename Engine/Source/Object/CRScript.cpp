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
bool CRScript::RegisterScript( const CRName& Name, const CRScriptCreator& Creator )
{
    bool result = Registry().insert( ScriptRegistry::value_type( Name, Creator ) ).second;

    assert( result );

    return result;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get script creator.
//---------------------------------------------------------------------------------------------------------------------
CRScriptCreator CRScript::GetScriptCreator( const CRName& Name )
{
    auto find = Registry().find( Name );

    assert( find != Registry().end() );

    return find->second;
}


#ifdef WITH_EDITOR

//---------------------------------------------------------------------------------------------------------------------
/// Get script creator.
//---------------------------------------------------------------------------------------------------------------------
CRScriptCreator GetScriptCreator( const CRName& Name )
{
    return CRScript::GetScriptCreator( Name );
}

//---------------------------------------------------------------------------------------------------------------------
/// Get script names.
//---------------------------------------------------------------------------------------------------------------------
LPSAFEARRAY GetScriptNames()
{
    const u32 size = Registry().size();
    if ( size == 0 ) return nullptr;

    CComSafeArray< BSTR > names( size );
    i32 i = 0;
    for ( auto itr = Registry().begin(); itr != Registry().end(); ++itr, ++i )
    {
        names.SetAt( i, A2BSTR_EX( itr->first.c_str() ), false );
    }

    return names.Detach();
}

#endif


