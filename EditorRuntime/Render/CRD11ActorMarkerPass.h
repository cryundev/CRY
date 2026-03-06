#pragma once


#include "CRD11OverlayPass.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11ActorMarkerPass
//---------------------------------------------------------------------------------------------------------------------
class CRD11ActorMarkerPass : public CRD11OverlayPass
{
public:
    /// Get pass name.
    virtual CRName GetPassName() const override { return "ActorMarkerPass"; }

protected:
    /// Get overlay render config for actor marker pass.
    virtual CROverlayPassConfig GetOverlayConfig() const override;

    /// Check whether actor marker draw should run.
    virtual bool CanDrawOverlay() const override;

    /// Draw marker elements from actor marker system.
    virtual void DrawOverlayElements() override;
};
