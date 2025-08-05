#include "CRScript.h"


using ScriptRegistry = CRMap< CRName, CRScriptCreator >;


CRArray< CRString > CRScript::ScriptNames;


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
/// Add a script name.
//---------------------------------------------------------------------------------------------------------------------
bool CRScript::AddScriptName( const CRName& Name )
{
    ScriptNames.emplace_back( Name );
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get script names.
//---------------------------------------------------------------------------------------------------------------------
const CRArray<CRName>& CRScript::GetScriptNames()
{
    return ScriptNames;
}

#endif


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
    const u32 size = CRScript::GetScriptNames().size();
    if ( size == 0 ) return nullptr;

    CComSafeArray< BSTR > names( size );
    for ( u32 i = 0; i < size; ++i )
    {
        names.SetAt( i, A2BSTR_EX( CRScript::GetScriptNames()[ i ].c_str() ), false );
    }

    return names.Detach();
}

#endif


