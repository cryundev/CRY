#include "CRD11ResourceManager.h"
#include "Core/CRD11CompiledShader.h"
#include "Core/CRD11ConstantBuffer.h"
#include "Core/CRD11DepthStencilState.h"
#include "Core/CRD11DepthStencilView.h"
#include "Core/CRD11IndexBuffer.h"
#include "Core/CRD11InputLayout.h"
#include "Core/CRD11PixelShader.h"
#include "Core/CRD11RenderTargetView.h"
#include "Core/CRD11SamplerState.h"
#include "Core/CRD11ShaderResourceView.h"
#include "Core/CRD11Texture2D.h"
#include "Core/CRD11VertexBuffer.h"
#include "Core/CRD11VertexShader.h"


#define CLEAR_CRD11_RESOURCE_MANAGE_TYPE( ResourceType ) \
    CRD11##ResourceType##s.clear(); \


#define GET_CRD11_RESOURCE_MANAGE_TYPE( ResourceType ) \
    CRD11##ResourceType##SPtr CRD11ResourceManager::Get##ResourceType( const CRName& Name ) \
    { \
        return _GetResource< CRD11##ResourceType >( Name, CRD11##ResourceType##s ); \
    } \


//---------------------------------------------------------------------------------------------------------------------
/// Clear
//---------------------------------------------------------------------------------------------------------------------
void CRD11ResourceManager::Clear()
{
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( CompiledShader     )
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( ConstantBuffer     )
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( DepthStencilState  )
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( DepthStencilView   )
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( IndexBuffer        )
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( InputLayout        )
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( PixelShader        )
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( RenderTargetView   )
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( SamplerState       )
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( ShaderResourceView )
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( Texture2D          )
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( VertexBuffer       )
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( VertexShader       )
}


GET_CRD11_RESOURCE_MANAGE_TYPE( CompiledShader     )
GET_CRD11_RESOURCE_MANAGE_TYPE( ConstantBuffer     )
GET_CRD11_RESOURCE_MANAGE_TYPE( DepthStencilState  )
GET_CRD11_RESOURCE_MANAGE_TYPE( DepthStencilView   )
GET_CRD11_RESOURCE_MANAGE_TYPE( IndexBuffer        )
GET_CRD11_RESOURCE_MANAGE_TYPE( InputLayout        )
GET_CRD11_RESOURCE_MANAGE_TYPE( PixelShader        )
GET_CRD11_RESOURCE_MANAGE_TYPE( RenderTargetView   )
GET_CRD11_RESOURCE_MANAGE_TYPE( SamplerState       )
GET_CRD11_RESOURCE_MANAGE_TYPE( ShaderResourceView )
GET_CRD11_RESOURCE_MANAGE_TYPE( Texture2D          )
GET_CRD11_RESOURCE_MANAGE_TYPE( VertexBuffer       )
GET_CRD11_RESOURCE_MANAGE_TYPE( VertexShader       )