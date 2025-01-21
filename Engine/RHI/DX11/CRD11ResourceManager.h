#pragma once


#include "CRD11Types.h"
#include "../../Core/Strings/CRStringInc.h"
#include "../../Core/Containers/CRContainerInc.h"


#define DECLARE_CRD11_RESOURCE_MANAGE_TYPE( ResourceType ) \
    public: using CRD11##ResourceType##Map = CRMap< CRName, CRD11##ResourceType##SPtr >; \
    private: CRD11##ResourceType##Map CRD11##ResourceType##s; \
    public: CRD11##ResourceType##SPtr Get##ResourceType( const CRName& Name ); \


//---------------------------------------------------------------------------------------------------------------------
/// CRD11ResourceManager
//---------------------------------------------------------------------------------------------------------------------
class CRD11ResourceManager
{
    DECLARE_CRD11_RESOURCE_MANAGE_TYPE( CompiledShader     )
    DECLARE_CRD11_RESOURCE_MANAGE_TYPE( ConstantBuffer     )
    DECLARE_CRD11_RESOURCE_MANAGE_TYPE( DepthStencilState  )
    DECLARE_CRD11_RESOURCE_MANAGE_TYPE( DepthStencilView   )
    DECLARE_CRD11_RESOURCE_MANAGE_TYPE( IndexBuffer        )
    DECLARE_CRD11_RESOURCE_MANAGE_TYPE( InputLayout        )
    DECLARE_CRD11_RESOURCE_MANAGE_TYPE( PixelShader        )
    DECLARE_CRD11_RESOURCE_MANAGE_TYPE( RenderTargetView   )
    DECLARE_CRD11_RESOURCE_MANAGE_TYPE( SamplerState       )
    DECLARE_CRD11_RESOURCE_MANAGE_TYPE( ShaderResourceView )
    DECLARE_CRD11_RESOURCE_MANAGE_TYPE( Texture2D          )
    DECLARE_CRD11_RESOURCE_MANAGE_TYPE( VertexBuffer       )
    DECLARE_CRD11_RESOURCE_MANAGE_TYPE( VertexShader       )
	
public:
	/// Constructor
	CRD11ResourceManager() = default;

	/// Clear
	void Clear();

private:
	/// Get resource.
	template< typename T >
	CRSharedPtr< T > _GetResource( const CRName& Name, CRMap< CRName, CRSharedPtr< T > >& Resources );
};


//---------------------------------------------------------------------------------------------------------------------
/// Get resource.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
CRSharedPtr< T > CRD11ResourceManager::_GetResource( const CRName& Name, CRMap< CRName, CRSharedPtr< T > >& Resources )
    {
	auto find = Resources.find( Name );

	if ( find == Resources.end() )
	{
		Resources[ Name ] = CRMakeShared( new T() );

		return Resources[ Name ];
	}

	return find->second;
}
