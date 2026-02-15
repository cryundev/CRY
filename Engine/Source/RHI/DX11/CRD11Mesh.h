#pragma once

#include "CRD11Types.h"
#include "Source/RHI/ICRRHIMesh.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11Mesh
//---------------------------------------------------------------------------------------------------------------------
class CRD11Mesh : public ICRRHIMesh
{
private:
    CRD11VertexBufferWPtr VertexBuffer;
    CRD11IndexBufferWPtr  IndexBuffer;
    
public:
    /// Constructor
    CRD11Mesh() = default;

    /// Destructor
    virtual ~CRD11Mesh() override {}

    /// Initialize primitive.
    virtual void InitializePrimitive( const CRName& Name, const CRPrimitiveAsset& PrimitiveData ) override;

    /// Set in the rendering pipeline.
    virtual void SetInRenderingPipeline() const override;

    /// Draw.
    virtual void Draw() const override;
};
