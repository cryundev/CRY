#pragma once


#include "CRObject.h"
#include "Source/Core/CRSmartPtrMacro.h"


namespace CRScript
{
    //---------------------------------------------------------------------------------------------------------------------
    /// CRScriptObject
    //---------------------------------------------------------------------------------------------------------------------
    class CRScriptObject : public CRObject
    {
    public:
        /// Destructor
        virtual ~CRScriptObject() = default;

        /// Begin play.
        virtual void BeginPlay() {}

        /// Update.
        virtual void Update( float DeltaSeconds ) {}

    protected:
        constexpr explicit CRScriptObject( CRObject Object )
        : CRObject( Object.GetObjectId() )
        {}
    };


    using CRScriptPtr = CRUniquePtr< CRScriptObject >;
    using CRScriptCreator = CRScriptPtr(*)( CRObject Object );

    bool RegisterScript( CRName Name, CRScriptCreator Creator );

    template< class T >
    CRScriptPtr CreateScript( CRObject Object )
    {
        assert( Object.IsValid() );

        return std::make_unique< T >( Object );
    }

#define REGISTER_SCRIPT( T ) \
class T; \
namespace \
{ \
    const bool _reg##T \
    { \
    CRScript::RegisterScript( CRName( #T ), &CRScript::CreateScript< T > ) \
    }; \
}
};