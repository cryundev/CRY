#pragma once


#include "CRComponent.h"
#include "CRTransformComponent.h"
#include "Source/RHI/CRConstBufferStructures.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRDirectionalLightComponent
//---------------------------------------------------------------------------------------------------------------------
class CRDirectionalLightComponent : public CRComponent< CRDirectionalLightComponent >
{
public:
    static constexpr ECRComponentPriority Priority = ECRComponentPriority::Light; /// Component priority.

public:
    CRVector4D Direction            = { 0.577f, -0.577f, 0.577f, 0.0f }; /// Light direction (normalized).
    CRVector4D Color                = { 1.0f, 1.0f, 1.0f, 1.0f };        /// RGB color + intensity (w).
    bool       bUseTransformForward = false;                             /// If true, use Transform forward as direction.

public:
    /// Constructor.
    CRDirectionalLightComponent() = default;

    /// Destroy component.
    virtual void DestroyComponent() override;

    /// Called when component is disabled.
    virtual void OnDisabled() override;

    /// Update component (Tick: sync direction from Transform if bUseTransformForward).
    virtual void UpdateComponent( float DeltaSeconds ) override;

    /// Pre-render component (flush all active directional lights to renderer).
    virtual void PreRenderComponent( float DeltaSeconds ) override;

    /// Build GPU light data.
    CRDirectionalLightData BuildLightData() const;

private:
    /// Collect all active components and send to renderer.
    static void _FlushToRenderer();
};
