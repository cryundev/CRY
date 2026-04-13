#pragma once


#include "Source/Core/Math/CRMath.h"
#include "Source/Core/Strings/CRStringInc.h"
#include "Source/RHI/CRConstBufferStructures.h"


//---------------------------------------------------------------------------------------------------------------------
/// ECRMaterialTextureSemantic
//---------------------------------------------------------------------------------------------------------------------
enum class ECRMaterialTextureSemantic
{
    Diffuse  = 0,
    Normal   = 1,
    Specular = 2,
    Emissive = 3,
    Max,
};

using ECRMaterialTextureSlot = ECRMaterialTextureSemantic;


//---------------------------------------------------------------------------------------------------------------------
/// ICRRHIMaterial
//---------------------------------------------------------------------------------------------------------------------
class ICRRHIMaterial
{
public:
    /// Constructor
    ICRRHIMaterial() = default;

    /// Destructor
    virtual ~ICRRHIMaterial() = 0 {}

public:
    /// Initialize material.
    virtual void Initialize( const CRName& Name ) = 0;

    /// Set the shader used by this material.
    virtual bool SetShader( const CRName& ShaderName ) = 0;

    /// Set material texture for a specific semantic.
    virtual void SetTexture( ECRMaterialTextureSemantic Semantic, const CRPath& TexturePath ) = 0;

    /// Set in the rendering pipeline.
    virtual void SetInRenderingPipeline() const = 0;

    /// Set diffuse color (RGB) and alpha (A).
    virtual void SetDiffuseColor( const CRVector4D& Color ) = 0;

    /// Set specular color (RGB) and shininess (A).
    virtual void SetSpecularColor( const CRVector4D& Color ) = 0;

    /// Get material properties.
    virtual const CRMaterialProperties& GetMaterialProperties() const = 0;
};
