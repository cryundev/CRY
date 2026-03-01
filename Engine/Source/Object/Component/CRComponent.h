#pragma once


#include "CRComponentRegistry.h"
#include "ICRComponent.h"
#include "Source/Core/CRGeneric.h"
#include "Source/Object/CRObject.h"
#include <algorithm>


//---------------------------------------------------------------------------------------------------------------------
/// CRComponent
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
class CRComponent : public CRObject, public ICRComponent
{
public:
    friend class CRActor;

    /// Component priority.
    static constexpr ECRComponentPriority Priority = ECRComponentPriority::None;

private:
    inline static CRArray< T >                Components        = {}; /// Components.
    inline static CRArray< CRIdentity::id_t > IdMap             = {}; /// Id map.
    inline static CRArray< CRIdentity::id_t > NextInstanceLinks = {}; /// Next instance links.

    inline static bool bStageRegistered = false; /// Whether stage functions have been registered.

public:
    /// Add component.
    static T* Add( const CRIdentity::id_t& Id );

    /// Remove component.
    static bool Remove( const CRIdentity::id_t& Id );

    /// Get component.
    static T* Get( const CRIdentity::id_t& Id );

    /// Get all components.
    static CRArray< T* > GetAll( const CRIdentity::id_t& Id );

    /// Update components.
    static void UpdateComponents( float DeltaSeconds );

    /// Pre-render components.
    static void PreRenderComponents( float DeltaSeconds );

    /// Get components array.
    static CRArray< T >& GetComponents() { return Components; }

private:
    /// Ensure stage registered.
    static void _EnsureStageRegistered();

    /// Is exclusive.
    static constexpr bool _IsExclusive();

    /// Ensure actor slot in id map.
    static void _EnsureActorSlot( CRIdentity::id_t actorIdx );

    /// Add internal.
    static T* _AddInternal( const CRIdentity::id_t& Id );

    /// Fix moved element links.
    static void _FixupMovedElement( CRIdentity::id_t compIdx );
};


template < typename T >
concept ComponentType = std::is_base_of_v< CRComponent< T >, T >;


//---------------------------------------------------------------------------------------------------------------------
/// Ensure stage registered.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
void CRComponent< T >::_EnsureStageRegistered()
{
    if ( bStageRegistered ) return;

    CRComponentRegistry::RegisterTick( [] ( float DeltaSeconds )
    {
        CRComponent< T >::UpdateComponents( DeltaSeconds );
    }, T::Priority );

    CRComponentRegistry::RegisterPreRender( [] ( float DeltaSeconds )
    {
        CRComponent< T >::PreRenderComponents( DeltaSeconds );
    }, T::Priority );

    bStageRegistered = true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Is exclusive.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
constexpr bool CRComponent< T >::_IsExclusive()
{
    if constexpr ( requires { T::IsExclusive; } )
    {
        return (bool)T::IsExclusive;
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Ensure actor slot in id map.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
void CRComponent< T >::_EnsureActorSlot( CRIdentity::id_t actorIdx )
{
    if ( actorIdx < IdMap.size() ) return;

    const size_t oldSize = IdMap.size();
    IdMap.resize( actorIdx + 1 );

    for ( size_t i = oldSize; i < IdMap.size(); ++i )
    {
        IdMap[ i ] = CRIdentity::InvalidId;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Add internal.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
T* CRComponent< T >::_AddInternal( const CRIdentity::id_t& Id )
{
    _EnsureStageRegistered();

    assert( CRIdentity::IsValid( Id ) );

    const CRIdentity::id_t actorIdx = CRIdentity::IndexOf( Id );
    const bool bReuseExisting = _IsExclusive();

    _EnsureActorSlot( actorIdx );

    if ( bReuseExisting )
    {
        const CRIdentity::id_t componentIndex = IdMap[ actorIdx ];
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
    NextInstanceLinks.push_back( CRIdentity::InvalidId );

    const CRIdentity::id_t newComponentIndex = (CRIdentity::id_t)( Components.size() - 1 );

    if ( IdMap[ actorIdx ] == CRIdentity::InvalidId )
    {
        IdMap[ actorIdx ] = newComponentIndex;
        return CRCast< T >( &Components[ newComponentIndex ] );
    }

    CRIdentity::id_t cursor = IdMap[ actorIdx ];
    while ( NextInstanceLinks[ cursor ] != CRIdentity::InvalidId )
    {
        cursor = NextInstanceLinks[ cursor ];
    }

    NextInstanceLinks[ cursor ] = newComponentIndex;

    return CRCast< T >( &Components[ newComponentIndex ] );
}

//---------------------------------------------------------------------------------------------------------------------
/// Fix moved element links.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
void CRComponent< T >::_FixupMovedElement( CRIdentity::id_t compIdx )
{
    if ( compIdx >= (CRIdentity::id_t)Components.size() ) return;

    const CRIdentity::id_t oldLastIdx = (CRIdentity::id_t)Components.size();
    const CRIdentity::id_t actorIdx   = CRIdentity::IndexOf( Components[ compIdx ].GetObjectId() );

    if ( actorIdx < IdMap.size() && IdMap[ actorIdx ] == oldLastIdx )
    {
        IdMap[ actorIdx ] = compIdx;
        return;
    }

    if ( actorIdx < IdMap.size() && IdMap[ actorIdx ] != CRIdentity::InvalidId )
    {
        CRIdentity::id_t cursor = IdMap[ actorIdx ];
       
        while ( cursor != CRIdentity::InvalidId )
        {
            if ( NextInstanceLinks[ cursor ] == oldLastIdx )
            {
                NextInstanceLinks[ cursor ] = compIdx;
                return;
            }
            cursor = NextInstanceLinks[ cursor ];
        }
    }
}


//---------------------------------------------------------------------------------------------------------------------
/// Add component.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
T* CRComponent< T >::Add( const CRIdentity::id_t& Id )
{
    return _AddInternal( Id );
}

//---------------------------------------------------------------------------------------------------------------------
/// Remove component.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
bool CRComponent< T >::Remove( const CRIdentity::id_t& Id )
{
    const CRIdentity::id_t actorIdx = CRIdentity::IndexOf( Id );

    if ( actorIdx >= IdMap.size() ) return false;
    if ( IdMap[ actorIdx ] == CRIdentity::InvalidId ) return false;

    CRArray< CRIdentity::id_t > toRemove;
    {
        CRIdentity::id_t cursor = IdMap[ actorIdx ];
        while ( cursor != CRIdentity::InvalidId )
        {
            assert( cursor < Components.size() );
            toRemove.push_back( cursor );
            cursor = NextInstanceLinks[ cursor ];
        }
    }

    for ( CRIdentity::id_t compIdx : toRemove )
    {
        Components[ compIdx ].DestroyComponent();
    }

    std::sort( toRemove.begin(), toRemove.end(), std::greater< CRIdentity::id_t >() );

    for ( CRIdentity::id_t compIdx : toRemove )
    {
        UtilContainer::EraseUnordered( Components,        compIdx );
        UtilContainer::EraseUnordered( NextInstanceLinks, compIdx );
        _FixupMovedElement( compIdx );
    }

    IdMap[ actorIdx ] = CRIdentity::InvalidId;

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
/// Get all components.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
CRArray< T* > CRComponent< T >::GetAll( const CRIdentity::id_t& Id )
{
    CRArray< T* > result;
    const CRIdentity::id_t actorIdx = CRIdentity::IndexOf( Id );

    if ( actorIdx >= IdMap.size() ) return result;

    CRIdentity::id_t cursor = IdMap[ actorIdx ];
    while ( cursor != CRIdentity::InvalidId )
    {
        assert( cursor < Components.size() );
        result.push_back( CRCast< T >( &Components[ cursor ] ) );
        cursor = NextInstanceLinks[ cursor ];
    }

    return result;
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

//---------------------------------------------------------------------------------------------------------------------
/// Pre-render components.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
void CRComponent< T >::PreRenderComponents( float DeltaSeconds )
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
        component.PreRenderComponent( DeltaSeconds );
    }
}
