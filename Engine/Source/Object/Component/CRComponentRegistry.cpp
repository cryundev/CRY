#include "CRComponentRegistry.h"
#include <algorithm>


//---------------------------------------------------------------------------------------------------------------------
/// Register tick function.
//---------------------------------------------------------------------------------------------------------------------
void CRComponentRegistry::RegisterTick( StageFn Fn, ECRComponentPriority Priority )
{
    TickEntries.push_back( { Priority, Fn } );
    bTickSorted = false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Register pre-render function.
//---------------------------------------------------------------------------------------------------------------------
void CRComponentRegistry::RegisterPreRender( StageFn Fn, ECRComponentPriority Priority )
{
    PreRenderEntries.push_back( { Priority, Fn } );
    bPreRenderSorted = false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Tick all registered components.
//---------------------------------------------------------------------------------------------------------------------
void CRComponentRegistry::Tick( float DeltaSeconds )
{
    if ( !bTickSorted )
    {
        std::ranges::sort( TickEntries, {}, [] ( const Entry& InEntry )
        {
            return static_cast< i32 >( InEntry.Priority );
        } );
        
        bTickSorted = true;
    }

    for ( const auto& entry : TickEntries )
    {
        entry.Fn( DeltaSeconds );
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Pre-render all registered components.
//---------------------------------------------------------------------------------------------------------------------
void CRComponentRegistry::PreRender( float DeltaSeconds )
{
    if ( !bPreRenderSorted )
    {
        std::ranges::sort( PreRenderEntries, {}, [] ( const Entry& InEntry )
        {
            return static_cast< i32 >( InEntry.Priority );
        } );
        
        bPreRenderSorted = true;
    }

    for ( const auto& entry : PreRenderEntries )
    {
        entry.Fn( DeltaSeconds );
    }
}
