#pragma once


#include "CRD11Types.h"
#include "../../Core/Strings/CRStringInc.h"


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
	void Create( const CRString& Path );

	/// Get shader resource view.
	const CRD11ShaderResourceViewWPtr& GetShaderResourceView() const { return ShaderResourceView; }

	/// Get sampler state.
	const CRD11SamplerStateWPtr& GetSamplerState() const { return SamplerState; }

private:
    /// Create sampler state.
    void _CreateSamplerState() const;
};
