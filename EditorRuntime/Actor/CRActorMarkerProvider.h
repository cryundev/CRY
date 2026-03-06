#pragma once


#include "Source/Core/CRTypes.h"
#include "Source/Core/Containers/CRContainerInc.h"
#include "Source/Core/Identify/CRIdentity.h"
#include "Source/Core/Math/CRMath.h"
#include "Source/Core/Strings/CRStringInc.h"


class CRWorld;


//---------------------------------------------------------------------------------------------------------------------
/// ECRActorMarkerVisualKind
//---------------------------------------------------------------------------------------------------------------------
enum class ECRActorMarkerVisualKind
{
    Mesh3D,
    Billboard2D, // Reserved for later implementation.
};


//---------------------------------------------------------------------------------------------------------------------
/// ECRLightMarkerComponentKind
//---------------------------------------------------------------------------------------------------------------------
enum class ECRLightMarkerComponentKind
{
    Directional,
    Spot,
    Point,
};


//---------------------------------------------------------------------------------------------------------------------
/// CRActorMarkerDescriptor
//---------------------------------------------------------------------------------------------------------------------
struct CRActorMarkerDescriptor
{
    CRIdentity::id_t         ActorId          = CRIdentity::InvalidId;
    CRName                   MarkerType       = {};
    CRVector                 Position         = CRVector::Zero;
    CRQuaternion             Rotation         = CRQuaternion::Identity;
    CRVector4D               Color            = CRVector4D( 1.0f, 1.0f, 1.0f, 1.0f );
    f32                      PixelSize        = 72.0f;
    f32                      PickRadiusPixels = 18.0f;
    i32                      Priority         = 0;
    ECRActorMarkerVisualKind VisualKind       = ECRActorMarkerVisualKind::Mesh3D;
};


//---------------------------------------------------------------------------------------------------------------------
/// CRLightMarkerRule
//---------------------------------------------------------------------------------------------------------------------
struct CRLightMarkerRule
{
    ECRLightMarkerComponentKind ComponentKind     = ECRLightMarkerComponentKind::Directional;
    CRName                      MarkerType        = {};
    CRVector4D                  Color             = CRVector4D( 1.0f, 1.0f, 1.0f, 1.0f );
    f32                         PixelSize         = 72.0f;
    f32                         PickRadiusPixels  = 18.0f;
    i32                         Priority          = 0;
    bool                        bUseActorRotation = true;
};


//---------------------------------------------------------------------------------------------------------------------
/// ICRActorMarkerProvider
//---------------------------------------------------------------------------------------------------------------------
class ICRActorMarkerProvider
{
public:
    virtual ~ICRActorMarkerProvider() = default;

    /// Get provider name.
    virtual CRName GetProviderName() const = 0;

    /// Collect actor markers from world.
    virtual void CollectMarkers( const CRWorld& World, CRArray< CRActorMarkerDescriptor >& OutMarkers ) const = 0;
};
