#pragma once


#include "CRActorMarkerProvider.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRLightActorMarkerProvider
//---------------------------------------------------------------------------------------------------------------------
class CRLightActorMarkerProvider : public ICRActorMarkerProvider
{
private:
    CRArray< CRLightMarkerRule > Rules;

public:
    /// Get provider name.
    virtual CRName GetProviderName() const override { return "LightActorMarkerProvider"; }

    /// Initialize marker rules.
    bool InitializeRules( const CRArray< CRLightMarkerRule >& InRules );

    /// Collect actor markers from world.
    virtual void CollectMarkers( const CRWorld& World, CRArray< CRActorMarkerDescriptor >& OutMarkers ) const override;
};
