#pragma once


#include "CRObject.h"
#include "Source/Core/CRSmartPtrMacro.h"


#ifdef WITH_EDITOR
    #include <atlsafe.h>
#endif


class CRScript;


using CRScriptPtr = CRUniquePtr< CRScript >;
using CRScriptCreator = CRScriptPtr(*)( CRObject Object );


//---------------------------------------------------------------------------------------------------------------------
/// CRScript
//---------------------------------------------------------------------------------------------------------------------
class CRScript : public CRObject
{
public:
    /// Destructor
    virtual ~CRScript() = default;

protected:
    /// Constructor
    constexpr explicit CRScript( CRObject Object )
    : CRObject( Object.GetObjectId() )
    {}

private:
#ifdef WITH_EDITOR
    static CRArray< CRName > ScriptNames;
#endif

public:
    /// Register script.
    static bool RegisterScript( const CRName& Name, const CRScriptCreator& Creator );

    /// Create script.
    template< class T >
    static CRScriptPtr CreateScript( CRObject Object )
    {
        assert( Object.IsValid() );

        return std::make_unique< T >( Object );
    }

    /// Get script creator.
    static CRScriptCreator GetScriptCreator( const CRName& Name );

#ifdef WITH_EDITOR
    /// Add a script name.
    static bool AddScriptName( const CRName& Name );

    /// Get script names.
    static const CRArray< CRName >& GetScriptNames();
#endif
};


//---------------------------------------------------------------------------------------------------------------------
/// REGISTER_SCRIPT Macro
//---------------------------------------------------------------------------------------------------------------------
#define REGISTER_SCRIPT( T ) \
namespace \
{ \
    const bool register##T \
    { \
        CRScript::RegisterScript( CRName( #T ), &CRScript::CreateScript< T > ) \
    }; \
}


#ifdef WITH_EDITOR
/// Get script creator.
extern "C" __declspec( dllexport ) CRScriptCreator GetScriptCreator( const CRName& Name );

/// Get script names.
extern "C" __declspec( dllexport ) LPSAFEARRAY GetScriptNames();
#endif