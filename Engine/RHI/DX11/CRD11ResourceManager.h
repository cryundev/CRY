#pragma once


#include "CRD11Types.h"
#include "../../Core/Strings/CRStringInc.h"
#include "../../Core/Containers/CRContainerInc.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11ResourceManager
//---------------------------------------------------------------------------------------------------------------------
class CRD11ResourceManager
{
public:
	using CRD11VertexBufferMap       = CRMap< CRName, CRD11VertexBufferSPtr       >;
	using CRD11IndexBufferMap        = CRMap< CRName, CRD11IndexBufferSPtr        >;
	using CRD11InputLayoutMap        = CRMap< CRName, CRD11InputLayoutSPtr        >;
	using CRD11compiledShaderMap     = CRMap< CRName, CRD11CompiledShaderSPtr     >;
	using CRD11PixelShaderMap        = CRMap< CRName, CRD11PixelShaderSPtr        >;
	using CRD11VertexShaderMap       = CRMap< CRName, CRD11VertexShaderSPtr       >;
	using CRD11Texture2DMap          = CRMap< CRName, CRD11Texture2DSPtr          >;
	using CRD11ShaderResourceViewMap = CRMap< CRName, CRD11ShaderResourceViewSPtr >;

private:
	CRD11VertexBufferMap       VertexBuffers;
	CRD11IndexBufferMap        IndexBuffers;
	CRD11InputLayoutMap        InputLayouts;
	CRD11compiledShaderMap     CompiledShaders;
	CRD11VertexShaderMap       VertexShaders;
	CRD11PixelShaderMap        PixelShaders;
	CRD11Texture2DMap	       Texture2Ds;
	CRD11ShaderResourceViewMap ShaderResourceViews;
	
public:
	/// Constructor
	CRD11ResourceManager() = default;

	/// Clear
	void Clear();
	
	/// Add vertex buffer.
	CRD11VertexBufferSPtr GetVertexBuffer( const CRName& Name );

	/// Add index buffer.
	CRD11IndexBufferSPtr GetIndexBuffer( const CRName& Name );

	/// Add input layout.
	CRD11InputLayoutSPtr GetInputLayout( const CRName& Name );

	/// Add compiled shader.
	CRD11CompiledShaderSPtr GetCompiledShader( const CRName& Name );

	/// Add vertex shader.
	CRD11VertexShaderSPtr GetVertexShader( const CRName& Name );

	/// Add pixel shader.
	CRD11PixelShaderSPtr GetPixelShader( const CRName& Name );

	/// Add texture2D.
	CRD11Texture2DSPtr GetTexture2D( const CRName& Name );

	/// Add shader resource view.
	CRD11ShaderResourceViewSPtr GetShaderResourceView( const CRName& Name );

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
