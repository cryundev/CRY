#pragma once


#include "CRD11PendingUpdateConstantBuffer.h"
#include "CRD11ShaderResourceTexture.h"
#include "CRD11Types.h"
#include "Source/Core/Containers/CRContainerInc.h"
#include "Source/RHI/ICRRHIMaterial.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11Material
//---------------------------------------------------------------------------------------------------------------------
class CRD11Material : public ICRRHIMaterial
{
private:
    CRD11VertexShaderWPtr VertexShader;
    CRD11PixelShaderWPtr  PixelShader;
    CRD11InputLayoutWPtr  InputLayout;

    CRArray< CRD11ShaderResourceTexture > Textures;

    CRD11PendingUpdateConstantBuffer< CRMaterialProperties > PropertiesBuffer;

public:
    /// Constructor
    CRD11Material() = default;

    /// Destructor
    virtual ~CRD11Material() override {}

    /// Initialize material.
    virtual void Initialize( const CRName& Name ) override;

    /// Set in the rendering pipeline.
    virtual void SetInRenderingPipeline() const override;

    /// Set diffuse color (RGB) and alpha (A).
    virtual void SetDiffuseColor( const CRVector4D& Color ) override;

    /// Set specular color (RGB) and shininess (A).
    virtual void SetSpecularColor( const CRVector4D& Color ) override;

    /// Get material properties.
    virtual const CRMaterialProperties& GetMaterialProperties() const override { return PropertiesBuffer.Get(); }
};
