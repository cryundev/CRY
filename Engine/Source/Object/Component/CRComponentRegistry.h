#pragma once


#include "Source/Core/CRGeneric.h"
#include "Source/Core/CRTypes.h"
#include "Source/Core/Containers/CRContainerInc.h"


//---------------------------------------------------------------------------------------------------------------------
/// ECRComponentPriority
//---------------------------------------------------------------------------------------------------------------------
enum class ECRComponentPriority : i32
{
    None,
    Transform,
    Primitive,
};


//---------------------------------------------------------------------------------------------------------------------
/// CRComponentRegistry
//---------------------------------------------------------------------------------------------------------------------
class CRComponentRegistry
{
public:
    /// Stage function type.
    using StageFn = void(*)( float );

    /// Registry entry.
    struct Entry
    {
        ECRComponentPriority Priority;
        StageFn              Fn;
    };

private:
    inline static CRArray< Entry > TickEntries      = {};    /// Tick entries.    
    inline static CRArray< Entry > PreRenderEntries = {};    /// Pre-render entries.
    inline static bool             bTickSorted      = false; /// Tick sorted flag.
    inline static bool             bPreRenderSorted = false; /// Pre-render sorted flag.

public:
    /// Register tick function.
    static void RegisterTick( StageFn Fn, ECRComponentPriority Priority );

    /// Register pre-render function.
    static void RegisterPreRender( StageFn Fn, ECRComponentPriority Priority );

    /// Tick all registered components.
    static void Tick( float DeltaSeconds );

    /// Pre-render all registered components.
    static void PreRender( float DeltaSeconds );
};
