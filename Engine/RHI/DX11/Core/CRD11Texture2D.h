#pragma once


#include "CRD11Object.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11Texture2D
//---------------------------------------------------------------------------------------------------------------------
class CRD11Texture2D : public CRD11Object< ID3D11Texture2D >
{
public:
	/// Create texture.
	void Create( const D3D11_TEXTURE2D_DESC& Description, const D3D11_SUBRESOURCE_DATA* Data = nullptr );
};
