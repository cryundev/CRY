#pragma once


#include "CRD11Types.h"
#include "../../Core/Strings/CRStringInc.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11Texture2DSampler
//---------------------------------------------------------------------------------------------------------------------
class CRD11Texture2DSampler
{
private:
	CRD11Texture2DWPtr          Texture2D;
	CRD11ShaderResourceViewWPtr ShaderResourceView;
	
public:
	/// Constructor
	CRD11Texture2DSampler() = default;

	/// Destructor
	virtual ~CRD11Texture2DSampler() {}

	/// Create from file.
	void Create( const CRString& Path );

	/// Get shader resource view.
	const CRD11ShaderResourceViewWPtr& GetShaderResourceView() const { return ShaderResourceView; }
};
