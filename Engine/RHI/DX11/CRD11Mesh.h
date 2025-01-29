#pragma once


#include "CRD11Material.h"
#include "CRD11Types.h"
#include "Core/Containers/CRContainerInc.h"
#include "RHI/ICRRHIMesh.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11Mesh
//---------------------------------------------------------------------------------------------------------------------
class CRD11Mesh : public ICRRHIMesh
{
private:
    CRD11VertexBufferWPtr VertexBuffer;
    CRD11IndexBufferWPtr  IndexBuffer;

    CRArray< CRD11Material > Materials;
    
public:
    /// Constructor
    CRD11Mesh() = default;

    /// Destructor
    virtual ~CRD11Mesh() override {}

    /// Initialize primitive.
    virtual void InitializePrimitive( const CRName& Name, const CRPrimitiveAsset& PrimitiveData ) override;

    /// Initialize material.
    virtual void InitializeMaterial() override;

    /// Set in the rendering pipeline.
    virtual void SetInRenderingPipeline() const override;

    /// Draw.
    virtual void Draw() const override;
};
