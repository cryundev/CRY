#pragma once


#include "CRD11ShaderResourceTexture.h"
#include "CRD11Types.h"
#include "RHI/ICRRHIMesh.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11Mesh
//---------------------------------------------------------------------------------------------------------------------
class CRD11Mesh : public ICRRHIMesh
{
private:
    CRD11VertexBufferWPtr VertexBuffer;
    CRD11IndexBufferWPtr  IndexBuffer;
    CRD11InputLayoutWPtr  InputLayout;

    CRD11ShaderResourceTexture Texture;
    CRD11SamplerStateWPtr SamplerState;

    CRD11VertexShaderWPtr VertexShader;
    CRD11PixelShaderWPtr  PixelShader;
    
public:
    /// Constructor
    CRD11Mesh() = default;

    /// Destructor
    virtual ~CRD11Mesh() override {};

    /// Initialize primitive.
    virtual void InitializePrimitive( const CRName& Name, const CRPrimitiveData& PrimitiveData ) override;

    /// Initialize material.
    virtual void InitializeMaterial() override;

    /// Set in the rendering pipeline.
    virtual void SetInRenderingPipeline() const override;

    /// Draw.
    virtual void Draw() const override;
};
