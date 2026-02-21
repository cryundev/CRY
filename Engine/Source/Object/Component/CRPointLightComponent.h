#pragma once


#include "CRComponent.h"
#include "CRTransformComponent.h"
#include "Source/RHI/CRConstBufferStructures.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRPointLightComponent
//---------------------------------------------------------------------------------------------------------------------
class CRPointLightComponent : public CRComponent< CRPointLightComponent >
{
public:
    static constexpr ECRComponentPriority Priority = ECRComponentPriority::Light; /// Component priority.

public:
    CRVector4D Color = { 1.0f, 1.0f, 1.0f, 1.0f }; /// RGB color + intensity (w).
    f32        Range = 100.0f;                     /// Light range.

public:
    /// Constructor.
    CRPointLightComponent() = default;

    /// Destroy component.
    virtual void DestroyComponent() override;

    /// Called when component is disabled.
    virtual void OnDisabled() override;

    /// Update component.
    virtual void UpdateComponent( float DeltaSeconds ) override {}

    /// Pre-render component (flush all active point lights to renderer).
    virtual void PreRenderComponent( float DeltaSeconds ) override;

    /// Build GPU light data (reads position from TransformComponent).
    CRPointLightData BuildLightData() const;

private:
    /// Collect all active components and send to renderer.
    static void _FlushToRenderer();
};
