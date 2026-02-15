#pragma once


#include <cassert>
#include <utility>


//---------------------------------------------------------------------------------------------------------------------
/// Get next generation. 0 is reserved for uninitialized slots.
//---------------------------------------------------------------------------------------------------------------------
template< typename TValue >
u32 CRPackedArray< TValue >::_NextGeneration( u32 Generation )
{
    ++Generation;

    if ( Generation == 0 )
    {
        Generation = 1;
    }

    return Generation;
}

//---------------------------------------------------------------------------------------------------------------------
/// Clear all entries.
//---------------------------------------------------------------------------------------------------------------------
template< typename TValue >
void CRPackedArray< TValue >::Clear()
{
    Slots.clear();
    FreeSlotIndices.clear();
    DenseValues.clear();
    DenseToSlotIndices.clear();
}

//---------------------------------------------------------------------------------------------------------------------
/// Insert value (copy).
//---------------------------------------------------------------------------------------------------------------------
template< typename TValue >
typename CRPackedArray< TValue >::Handle CRPackedArray< TValue >::Insert( const TValue& Value )
{
    return Emplace( Value );
}

//---------------------------------------------------------------------------------------------------------------------
/// Insert value (move).
//---------------------------------------------------------------------------------------------------------------------
template< typename TValue >
typename CRPackedArray< TValue >::Handle CRPackedArray< TValue >::Insert( TValue&& Value )
{
    return Emplace( std::move( Value ) );
}

//---------------------------------------------------------------------------------------------------------------------
/// Emplace value in packed array.
//---------------------------------------------------------------------------------------------------------------------
template< typename TValue >
template< typename... TArgs >
typename CRPackedArray< TValue >::Handle CRPackedArray< TValue >::Emplace( TArgs&&... Args )
{
    u32 slotIndex = 0;

    if ( !FreeSlotIndices.empty() )
    {
        slotIndex = FreeSlotIndices.back();
        FreeSlotIndices.pop_back();
    }
    else
    {
        assert( Slots.size() < (size_t)( Handle::InvalidIndex ) );

        slotIndex = (u32)( Slots.size() );
        Slots.emplace_back();
    }

    CRSlot& slot    = Slots[ slotIndex ];
    slot.Generation = _NextGeneration( slot.Generation );
    slot.DenseIndex = (u32)( DenseValues.size() );

    DenseValues.emplace_back( std::forward< TArgs >( Args )... );
    DenseToSlotIndices.push_back( slotIndex );

    return Handle{ slotIndex, slot.Generation };
}

//---------------------------------------------------------------------------------------------------------------------
/// Remove value by handle.
//---------------------------------------------------------------------------------------------------------------------
template< typename TValue >
bool CRPackedArray< TValue >::Remove( const Handle& InHandle )
{
    if ( !IsValid( InHandle ) ) return false;

    CRSlot& slot         = Slots[ InHandle.Index ];
    const u32 denseIndex = slot.DenseIndex;
    const u32 lastIndex  = (u32)( DenseValues.size() - 1 );

    if ( denseIndex != lastIndex )
    {
        DenseValues[ denseIndex ] = std::move( DenseValues[ lastIndex ] );

        const u32 movedSlotIndex = DenseToSlotIndices[ lastIndex ];
        DenseToSlotIndices[ denseIndex ] = movedSlotIndex;
        Slots[ movedSlotIndex ].DenseIndex = denseIndex;
    }

    DenseValues.pop_back();
    DenseToSlotIndices.pop_back();

    slot.DenseIndex = CRSlot::InvalidDenseIndex;

    FreeSlotIndices.push_back( InHandle.Index );

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Check whether handle is valid and points to an active slot.
//---------------------------------------------------------------------------------------------------------------------
template< typename TValue >
bool CRPackedArray< TValue >::IsValid( const Handle& InHandle ) const
{
    if ( !InHandle.IsValid() ) return false;
    if ( InHandle.Index >= Slots.size() ) return false;

    const CRSlot& slot = Slots[ InHandle.Index ];
    if ( slot.DenseIndex == CRSlot::InvalidDenseIndex ) return false;

    return slot.Generation == InHandle.Generation;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get value by handle.
//---------------------------------------------------------------------------------------------------------------------
template< typename TValue >
TValue* CRPackedArray< TValue >::Get( const Handle& InHandle )
{
    if ( !IsValid( InHandle ) ) return nullptr;

    return &DenseValues[ Slots[ InHandle.Index ].DenseIndex ];
}

//---------------------------------------------------------------------------------------------------------------------
/// Get value by handle (const).
//---------------------------------------------------------------------------------------------------------------------
template< typename TValue >
const TValue* CRPackedArray< TValue >::Get( const Handle& InHandle ) const
{
    if ( !IsValid( InHandle ) ) return nullptr;

    return &DenseValues[ Slots[ InHandle.Index ].DenseIndex ];
}

//---------------------------------------------------------------------------------------------------------------------
/// Iterate active entries.
//---------------------------------------------------------------------------------------------------------------------
template< typename TValue >
template< typename TFunc >
void CRPackedArray< TValue >::ForEachActive( TFunc&& Func )
{
    for ( u32 denseIndex = 0; denseIndex < (u32)( DenseValues.size() ); ++denseIndex )
    {
        const u32 slotIndex = DenseToSlotIndices[ denseIndex ];
        CRSlot& slot = Slots[ slotIndex ];

        Func( Handle{ slotIndex, slot.Generation }, DenseValues[ denseIndex ] );
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Iterate active entries (const).
//---------------------------------------------------------------------------------------------------------------------
template< typename TValue >
template< typename TFunc >
void CRPackedArray< TValue >::ForEachActive( TFunc&& Func ) const
{
    for ( u32 denseIndex = 0; denseIndex < (u32)( DenseValues.size() ); ++denseIndex )
    {
        const u32 slotIndex = DenseToSlotIndices[ denseIndex ];
        const CRSlot& slot = Slots[ slotIndex ];

        Func( Handle{ slotIndex, slot.Generation }, DenseValues[ denseIndex ] );
    }
}
