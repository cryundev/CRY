#pragma once


#include "CRD11Include.h"
#include "CRD11IndexBuffer.h"
#include "CRD11InputLayout.h"
#include "CRD11PixelShader.h"
#include "CRD11VertexBuffer.h"
#include "CRD11VertexShader.h"
#include "../../CRTypes.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11ResourceManager
//---------------------------------------------------------------------------------------------------------------------
class CRD11ResourceManager
{
public:
	using CRD11VertexBufferMap = CRMap< CRName, CRD11VertexBufferSPtr >;
	using CRD11IndexBufferMap  = CRMap< CRName, CRD11IndexBufferSPtr  >;
	using CRD11InputLayoutMap  = CRMap< CRName, CRD11InputLayoutSPtr  >;
	using CRD11VertexShaderMap = CRMap< CRName, CRD11VertexShaderSPtr >;
	using CRD11PixelShaderMap  = CRMap< CRName, CRD11PixelShaderSPtr  >;

private:
	CRD11VertexBufferMap VertexBuffers;
	CRD11IndexBufferMap  IndexBuffers;
	CRD11InputLayoutMap  InputLayouts;
	CRD11VertexShaderMap VertexShaders;
	CRD11PixelShaderMap  PixelShaders;
	
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

	/// Add vertex shader.
	CRD11VertexShaderSPtr GetVertexShader( const CRName& Name ) { return _GetResource< CRD11VertexShader >( Name, VertexShaders ); }

	/// Add pixel shader.
	CRD11PixelShaderSPtr GetPixelShader( const CRName& Name ) { return _GetResource< CRD11PixelShader >( Name, PixelShaders ); }

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
