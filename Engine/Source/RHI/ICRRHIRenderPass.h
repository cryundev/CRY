#pragma once


#include "CRRenderElement.h"
#include "Source/Core/CRTypes.h"
#include "Source/Core/Strings/CRStringInc.h"


//---------------------------------------------------------------------------------------------------------------------
/// ICRRHIRenderPass
//---------------------------------------------------------------------------------------------------------------------
class ICRRHIRenderPass
{
public:
    /// Destructor.
    virtual ~ICRRHIRenderPass() = default;

    /// Get pass name.
    virtual CRName GetPassName() const = 0;

    /// Initialize pass resources.
    virtual void Initialize( u32 Width, u32 Height ) = 0;

    /// Resize pass resources.
    virtual void Resize( u32 Width, u32 Height ) = 0;

    /// Clear pass render targets.
    virtual void OnClearRenderTarget( const float ClearColor[ 4 ] ) {}

    /// Prepare pass state before main draw.
    virtual void OnPreDraw() {}

    /// Run main draw for render elements.
    virtual void OnDrawRenderElements( const CRPackedArray< CRRenderElement >& RenderElements ) {}

    /// Run pass work after main draw.
    virtual void OnPostDraw() {}

    /// Release pass resources.
    virtual void Release() = 0;
};
