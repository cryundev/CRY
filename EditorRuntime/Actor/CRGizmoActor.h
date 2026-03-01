#pragma once


#include "Source/Object/CRActor.h"
#include "Source/Core/Math/CRMath.h"
#include "Source/RHI/CRRHITypes.h"


class CRPrimitiveAsset;


//---------------------------------------------------------------------------------------------------------------------
/// CRGizmoRenderElement
//---------------------------------------------------------------------------------------------------------------------
struct CRGizmoRenderElement
{
    ICRRHIMeshSPtr Mesh      = nullptr;
    CRMatrix       Transform = CRMatrix::Identity;
    CRVector4D     Color     = CRVector4D( 1.0f, 1.0f, 1.0f, 1.0f );
    f32            Type      = 0.0f;
};


//---------------------------------------------------------------------------------------------------------------------
/// CRGizmoActor
//---------------------------------------------------------------------------------------------------------------------
class CRGizmoActor : public CRActor
{
public:
    /// Initialize gizmo actor.
    virtual bool InitializeGizmo( const CRString& AssetPath, const CRPrimitiveAsset& PrimitiveAsset );

    /// Get render element count.
    virtual u32 GetRenderElementCount() const;

    /// Get render element.
    virtual bool GetRenderElement( u32 ElementIndex, CRGizmoRenderElement& OutRenderElement ) const;

    /// Set gizmo world position.
    void SetPivot( const CRVector& InPivot );

    /// Get current gizmo world position.
    CRVector GetPivot() const;
};
