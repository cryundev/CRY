#pragma once

#include "CRD11Types.h"
#include "Source/Core/CRTypes.h"
#include "Source/Core/Strings/CRStringInc.h"


class CRTextureAsset;

//---------------------------------------------------------------------------------------------------------------------
/// CRD11ShaderResourceTexture
//---------------------------------------------------------------------------------------------------------------------
class CRD11ShaderResourceTexture
{
private:
    CRD11Texture2DWPtr          Texture2D;
    CRD11ShaderResourceViewWPtr ShaderResourceView;
    CRD11SamplerStateWPtr       SamplerState;
    
public:
    /// Constructor
    CRD11ShaderResourceTexture() = default;

    /// Destructor
    virtual ~CRD11ShaderResourceTexture() {}

    /// Create from file.
    void Create( const CRPath& Path );

    /// Create from texture asset.
    void CreateFromAsset( const CRName& ResourceName, const CRTextureAsset& Asset );

    /// Create a 1x1 solid-color texture.
    void CreateSolidColor( const CRName& ResourceName, u8 R, u8 G, u8 B, u8 A );

    /// Get shader resource view.
    const CRD11ShaderResourceViewWPtr& GetShaderResourceView() const { return ShaderResourceView; }

    /// Get sampler state.
    const CRD11SamplerStateWPtr& GetSamplerState() const { return SamplerState; }

private:
    /// Create D3D texture and shader resource view from texture asset data.
    void _CreateTextureResource( const CRTextureAsset& Asset ) const;

    /// Create sampler state.
    void _CreateSamplerState() const;
};
