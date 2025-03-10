#pragma once


#include "ICRComponent.h"
#include "Source/Core/CRGeneric.h"
#include "Source/Object/CRObject.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRComponent
//---------------------------------------------------------------------------------------------------------------------
template< typename T >
class CRComponent : public CRObject, public ICRComponent
{
private:
    /// Components.
    inline static CRArray< T > Components;
    
public:
    /// Add component.
    static T* Add( const CRIdentity::id_t& Id );

    /// Remove component.
    static bool Remove( const CRIdentity::id_t& Id );

    /// Update components.
    static void UpdateComponents( float DeltaSeconds );
};


//---------------------------------------------------------------------------------------------------------------------
/// Add component.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
T* CRComponent< T >::Add( const CRIdentity::id_t& Id )
{
    CRIdentity::id_t index = CRIdentity::IndexOf( Id );
    if ( index >= Components.size() )
    {
        Components.resize( index + 1 );
    }

    return CRCast< T* >( &Components[ index ] );
}

//---------------------------------------------------------------------------------------------------------------------
/// Remove component.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
bool CRComponent< T >::Remove( const CRIdentity::id_t& Id )
{
    if ( Id >= Components.size() ) return false;

    Components.remove( Id );

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Update components.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
void CRComponent< T >::UpdateComponents( float DeltaSeconds )
{
    for ( auto& component : Components )
    {
        if ( !component.IsValid() ) continue;

        component.UpdateComponent( DeltaSeconds );
    }
}
