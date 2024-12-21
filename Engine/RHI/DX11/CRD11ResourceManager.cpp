#include "CRD11ResourceManager.h"
#include "CRD11CompiledShader.h"
#include "CRD11ConstantBuffer.h"
#include "CRD11IndexBuffer.h"
#include "CRD11InputLayout.h"
#include "CRD11PixelShader.h"
#include "CRD11RenderTargetView.h"
#include "CRD11SamplerState.h"
#include "CRD11ShaderResourceView.h"
#include "CRD11Texture2D.h"
#include "CRD11VertexBuffer.h"
#include "CRD11VertexShader.h"


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
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( VertexBuffer       )
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( IndexBuffer        )
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( ConstantBuffer     )
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( InputLayout        )
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( CompiledShader     )
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( VertexShader       )
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( PixelShader        )
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( Texture2D          )
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( ShaderResourceView )
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( SamplerState       )
    CLEAR_CRD11_RESOURCE_MANAGE_TYPE( RenderTargetView   )
}


GET_CRD11_RESOURCE_MANAGE_TYPE( VertexBuffer       )
GET_CRD11_RESOURCE_MANAGE_TYPE( IndexBuffer        )
GET_CRD11_RESOURCE_MANAGE_TYPE( ConstantBuffer     )
GET_CRD11_RESOURCE_MANAGE_TYPE( InputLayout        )
GET_CRD11_RESOURCE_MANAGE_TYPE( CompiledShader     )
GET_CRD11_RESOURCE_MANAGE_TYPE( VertexShader       )
GET_CRD11_RESOURCE_MANAGE_TYPE( PixelShader        )
GET_CRD11_RESOURCE_MANAGE_TYPE( Texture2D          )
GET_CRD11_RESOURCE_MANAGE_TYPE( ShaderResourceView )
GET_CRD11_RESOURCE_MANAGE_TYPE( SamplerState       )
GET_CRD11_RESOURCE_MANAGE_TYPE( RenderTargetView   )