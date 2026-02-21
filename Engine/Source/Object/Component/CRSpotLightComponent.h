#pragma once


#include "CRComponent.h"
#include "CRTransformComponent.h"
#include "Source/RHI/CRConstBufferStructures.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRSpotLightComponent
//---------------------------------------------------------------------------------------------------------------------
class CRSpotLightComponent : public CRComponent< CRSpotLightComponent >
{
public:
    static constexpr ECRComponentPriority Priority = ECRComponentPriority::Light; /// Component priority.

public:
    CRVector4D Color      = { 1.0f, 1.0f, 1.0f, 1.0f }; /// RGB color + intensity (w).
    f32        Range      = 100.0f;                     /// Light range.
    f32        InnerAngle = 15.0f;                      /// Inner cone angle in degrees.
    f32        OuterAngle = 30.0f;                      /// Outer cone angle in degrees.

public:
    /// Constructor.
    CRSpotLightComponent() = default;

    /// Destroy component.
    virtual void DestroyComponent() override;

    /// Called when component is disabled.
    virtual void OnDisabled() override;

    /// Update component.
    virtual void UpdateComponent( float DeltaSeconds ) override {}

    /// Pre-render component (flush all active spot lights to renderer).
    virtual void PreRenderComponent( float DeltaSeconds ) override;

    /// Build GPU light data (reads position and direction from TransformComponent).
    CRSpotLightData BuildLightData() const;

private:
    /// Collect all active components and send to renderer.
    static void _FlushToRenderer();
};
