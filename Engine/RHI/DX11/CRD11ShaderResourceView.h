#pragma once


#include "CRD11Object.h"


class CRD11ShaderResourceView : public CRD11Object< ID3D11ShaderResourceView >
{
public:
	/// Create shader resource view.
	void Create( ID3D11Resource* Resource, const D3D11_SHADER_RESOURCE_VIEW_DESC& Description );
	
};
