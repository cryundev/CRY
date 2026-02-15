#pragma once


#include "CRContainerInc.h"
#include "Source/Core/CRTypes.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRPackedHandle
//---------------------------------------------------------------------------------------------------------------------
struct CRPackedHandle
{
    inline static constexpr u32 InvalidIndex = (u32)( -1 );

    u32 Index      = InvalidIndex;
    u32 Generation = 0;

    bool IsValid() const { return Index != InvalidIndex; }
};


//---------------------------------------------------------------------------------------------------------------------
/// CRPackedArray
//---------------------------------------------------------------------------------------------------------------------
template< typename TValue >
class CRPackedArray
{
public:
    using Handle = CRPackedHandle;

private:
    struct CRSlot
    {
        inline static constexpr u32 InvalidDenseIndex = (u32)( -1 );

        u32 Generation = 0;
        u32 DenseIndex = InvalidDenseIndex;
    };

    CRArray< CRSlot > Slots;
    CRArray< u32 >    FreeSlotIndices;
    CRArray< TValue > DenseValues;
    CRArray< u32 >    DenseToSlotIndices;

private:
    //-----------------------------------------------------------------------------------------------------------------
    /// Get next generation. 0 is reserved for uninitialized slots.
    //-----------------------------------------------------------------------------------------------------------------
    static u32 _NextGeneration( u32 Generation );

public:
    /// Clear all entries.
    void Clear();

    /// Insert value (copy).
    Handle Insert( const TValue& Value );

    /// Insert value (move).
    Handle Insert( TValue&& Value );

    /// Emplace value in packed array.
    template< typename... TArgs >
    Handle Emplace( TArgs&&... Args );

    /// Remove value by handle.
    bool Remove( const Handle& InHandle );

    /// Check whether handle is valid and points to an active slot.
    bool IsValid( const Handle& InHandle ) const;

    /// Get value by handle.
    TValue* Get( const Handle& InHandle );

    /// Get value by handle (const).
    const TValue* Get( const Handle& InHandle ) const;

    /// Iterate active entries.
    template< typename TFunc >
    void ForEachActive( TFunc&& Func );

    /// Iterate active entries (const).
    template< typename TFunc >
    void ForEachActive( TFunc&& Func ) const;
};


#include "CRPackedArray.hpp"
