#pragma once


#include "CRObject.h"
#include "Source/Core/CRSmartPtrMacro.h"


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
};


//---------------------------------------------------------------------------------------------------------------------
/// REGISTER_SCRIPT Macro
//---------------------------------------------------------------------------------------------------------------------
#define REGISTER_SCRIPT( T ) \
class T; \
namespace \
{ \
    const bool register##T \
    { \
        CRScript::RegisterScript( CRName( #T ), &CRScript::CreateScript< T > ) \
    }; \
}
