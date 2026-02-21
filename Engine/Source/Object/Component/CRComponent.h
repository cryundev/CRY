#pragma once


#include "CRComponentRegistry.h"
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

    /// Component priority.
    static constexpr ECRComponentPriority Priority = ECRComponentPriority::None;

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
    CRComponentRegistry::RegisterTick( [] ( float DeltaSeconds )
    {
        CRComponent< T >::UpdateComponents( DeltaSeconds );
    }, T::Priority );
    
    CRComponentRegistry::RegisterPreRender( [] ( float DeltaSeconds )
    {
        CRComponent< T >::UpdateComponents( DeltaSeconds );
    }, T::Priority );
    
    assert( CRIdentity::IsValid( Id ) );

    CRIdentity::id_t index = CRIdentity::IndexOf( Id );

    if ( index < IdMap.size() )
    {
        const CRIdentity::id_t componentIndex = IdMap[ index ];
        if ( componentIndex != CRIdentity::InvalidId )
        {
            if ( componentIndex < Components.size() )
            {
                return CRCast< T >( &Components[ componentIndex ] );
            }

            assert( false && "Invalid component index in IdMap." );
            return nullptr;
        }
    }

    Components.emplace_back();

    if ( index >= IdMap.size() )
    {
        const size_t oldSize = IdMap.size();
        IdMap.resize( index + 1 );

        for ( size_t i = oldSize; i < IdMap.size(); ++i )
        {
            IdMap[ i ] = CRIdentity::InvalidId;
        }
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

    if ( index >= IdMap.size() ) return nullptr;
    if ( IdMap[ index ] == CRIdentity::InvalidId ) return nullptr;

    assert( IdMap[ index ] < Components.size() );
    if ( IdMap[ index ] >= Components.size() ) return nullptr;

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
