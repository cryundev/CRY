#pragma once


#include "CRD11CompiledShader.h"
#include "CRD11Include.h"
#include "CRD11IndexBuffer.h"
#include "CRD11InputLayout.h"
#include "CRD11PixelShader.h"
#include "CRD11ShaderResourceView.h"
#include "CRD11Texture2D.h"
#include "CRD11VertexBuffer.h"
#include "CRD11VertexShader.h"
#include "../../CRTypes.h"


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
	CRD11VertexBufferSPtr GetVertexBuffer( const CRName& Name ) { return _GetResource< CRD11VertexBuffer >( Name, VertexBuffers ); }

	/// Add index buffer.
	CRD11IndexBufferSPtr GetIndexBuffer( const CRName& Name ) { return _GetResource< CRD11IndexBuffer >( Name, IndexBuffers ); }

	/// Add input layout.
	CRD11InputLayoutSPtr GetInputLayout( const CRName& Name ) { return _GetResource< CRD11InputLayout >( Name, InputLayouts ); }

	/// Add compiled shader.
	CRD11CompiledShaderSPtr GetCompiledShader( const CRName& Name ) { return _GetResource< CRD11CompiledShader >( Name, CompiledShaders ); }

	/// Add vertex shader.
	CRD11VertexShaderSPtr GetVertexShader( const CRName& Name ) { return _GetResource< CRD11VertexShader >( Name, VertexShaders ); }

	/// Add pixel shader.
	CRD11PixelShaderSPtr GetPixelShader( const CRName& Name ) { return _GetResource< CRD11PixelShader >( Name, PixelShaders ); }

	/// Add texture2D.
	CRD11Texture2DSPtr GetTexture2D( const CRName& Name ) { return _GetResource< CRD11Texture2D >( Name, Texture2Ds ); }

	/// Add shader resource view.
	CRD11ShaderResourceViewSPtr GetShaderResourceView( const CRName& Name ) { return _GetResource< CRD11ShaderResourceView >( Name, ShaderResourceViews ); }

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
