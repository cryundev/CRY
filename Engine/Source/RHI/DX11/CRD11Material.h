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
    CRD11MaterialShaderWPtr MaterialShader;

    CRArray< CRD11ShaderResourceTexture > Textures;

    CRD11PendingUpdateConstantBuffer< CRMaterialProperties > PropertiesBuffer;

public:
    /// Constructor
    CRD11Material() = default;

    /// Destructor
    virtual ~CRD11Material() override {}

    /// Initialize material.
    virtual void Initialize( const CRName& Name ) override;

    /// Set the shader used by this material.
    virtual bool SetShader( const CRName& ShaderName ) override;

    /// Set texture for a specific material semantic.
    virtual void SetTexture( ECRMaterialTextureSemantic Semantic, const CRPath& TexturePath ) override;

    /// Set in the rendering pipeline.
    virtual void SetInRenderingPipeline() const override;

    /// Set diffuse color (RGB) and alpha (A).
    virtual void SetDiffuseColor( const CRVector4D& Color ) override;

    /// Set specular color (RGB) and shininess (A).
    virtual void SetSpecularColor( const CRVector4D& Color ) override;

    /// Get material properties.
    virtual const CRMaterialProperties& GetMaterialProperties() const override { return PropertiesBuffer.Get(); }

private:
    /// Set a semantic back to its default texture.
    void _SetDefaultTexture( ECRMaterialTextureSemantic Semantic );
};
