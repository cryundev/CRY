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
public:
    friend class CRActor;

private:
    /// Components.
    inline static CRArray< T > Components = {};

    /// Id map.
    inline static CRArray< CRIdentity::id_t > IdMap = {};
    
public:
    /// Add component.
    static T* Add( const CRIdentity::id_t& Id );

    /// Remove component.
    static bool Remove( const CRIdentity::id_t& Id );

    /// Get component.
    static T* Get( const CRIdentity::id_t& Id );

    /// Update components.
    static void UpdateComponents( float DeltaSeconds );
};


template< typename T >
concept ComponentType = std::is_base_of_v< CRComponent< T >, T >;


//---------------------------------------------------------------------------------------------------------------------
/// Add component.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
T* CRComponent< T >::Add( const CRIdentity::id_t& Id )
{
    assert( CRIdentity::IsValid( Id ) );

    CRIdentity::id_t index = CRIdentity::IndexOf( Id );

    Components.emplace_back();

    if ( index >= IdMap.size() )
    {
        IdMap.resize( index + 1 );
    }

    IdMap[ index ] = Components.size() - 1;

    return CRCast< T >( &Components[ IdMap[ index ] ] );
}

//---------------------------------------------------------------------------------------------------------------------
/// Remove component.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
bool CRComponent< T >::Remove( const CRIdentity::id_t& Id )
{
    CRIdentity::id_t index = CRIdentity::IndexOf( Id );

    if ( index >= IdMap.size() ) return false;
    if ( IdMap[ index ] == CRIdentity::InvalidId ) return false;
    if ( IdMap[ index ] >= Components.size() ) return false;

    const CRIdentity::id_t removedComponentIndex = IdMap[ index ];

    Components[ removedComponentIndex ].DestroyComponent();

    CRIdentity::id_t lastedIndex = CRIdentity::IndexOf( Components.back().GetObjectId() );

    IdMap[ lastedIndex ] = removedComponentIndex;

    UtilContainer::EraseUnordered( Components, removedComponentIndex );

    IdMap[ index ] = CRIdentity::InvalidId;

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get component.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
T* CRComponent< T >::Get( const CRIdentity::id_t& Id )
{
    assert( CRIdentity::IsValid( Id ) );
    
    CRIdentity::id_t index = CRIdentity::IndexOf( Id );
    
    assert( index < IdMap.size() );
    assert( IdMap[ index ] < Components.size() );

    if ( IdMap[ index ] == CRIdentity::InvalidId ) return nullptr;

    return CRCast< T >( &Components[ IdMap[ index ] ] );
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

        if ( !component.IsEnabled() )
        {
            if ( component.bWasEnabled )
            {
                component.OnDisabled();
                component.bWasEnabled = false;
            }
            continue;
        }

        component.bWasEnabled = true;
        component.UpdateComponent( DeltaSeconds );
    }
}
