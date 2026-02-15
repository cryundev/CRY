#include "CRD11Mesh.h"
#include "CRD11.h"
#include "CRD11RenderingPipeline.h"
#include "CRD11ResourceManager.h"
#include "Resource/CRD11Device.h"
#include "Resource/CRD11IndexBuffer.h"
#include "Resource/CRD11VertexBuffer.h"
#include "Source/Core/CRVertex.h"


//---------------------------------------------------------------------------------------------------------------------
/// Initialize primitive.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Mesh::InitializePrimitive( const CRName& Name, const CRPrimitiveAsset& PrimitiveData )
{
    CRArray< CRVertex > vertice;
    CRVertex::LoadFromPrimitiveData( PrimitiveData, vertice );

    VertexBuffer = GD11RM.GetVertexBuffer( Name );
    VertexBuffer.lock()->Create( D3D11_USAGE_DEFAULT, 0, vertice.data(), sizeof( CRVertex ), (u32)vertice.size() );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set in the rendering pipeline.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Mesh::SetInRenderingPipeline() const
{
    if ( !VertexBuffer.expired() )
    {
        GD11RP.SetVertexBuffer( VertexBuffer.lock()->GetObjectPtr(), 0, VertexBuffer.lock()->GetStride(), 0, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    }

    if ( !IndexBuffer.expired() )
    {
        GD11RP.SetIndexBuffer( IndexBuffer.lock()->GetObjectPtr(), 0 );
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Draw.
//---------------------------------------------------------------------------------------------------------------------
void CRD11Mesh::Draw() const
{
    if ( VertexBuffer.expired() ) return;

    if ( IndexBuffer.expired() )
    {
        GD11.GetDeviceContext()->Draw( VertexBuffer.lock()->GetCount(), 0 );
    }
    else if ( !VertexBuffer.expired() )
    {
        GD11.GetDeviceContext()->DrawIndexed( IndexBuffer.lock()->GetCount(), 0, 0 );
    }
}
