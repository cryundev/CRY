#pragma once


#include "CRD11.h"
#include "CRD11RenderingPipeline.h"
#include "CRD11ResourceManager.h"
#include "CRD11Types.h"
#include "Core/CRD11ConstantBuffer.h"
#include "Core/CRD11Device.h"
#include "Utility/Generic/CRGeneric.h"
#include "Utility/Log/CRLog.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11BindingConstantBuffer
//---------------------------------------------------------------------------------------------------------------------
template< typename T >
class CRD11BindingConstantBuffer
{
private:
    CRName                     Name;
    CRD11ConstantBufferWPtr    ConstantBufferPtr;
    ED11RenderingPipelineStage Stage = ED11RenderingPipelineStage::Max;
    unsigned int               Slot  = 0;

public:
    /// Constructor.
    CRD11BindingConstantBuffer() = default;
    
    /// Create constant buffer.
    void Create( const CRName& InName, unsigned int InSlot, ED11RenderingPipelineStage InStage, const T* Data = nullptr );

    /// Update constant buffer.
    void Update( const T& Data );

    /// Get constant buffer weak pointer.
    const CRD11ConstantBufferWPtr& GetConstantBuffer() const { return ConstantBufferPtr; }

    /// Get rendering pipeline stage.
    ED11RenderingPipelineStage GetStage() const { return Stage; }

    /// Get slot.
    unsigned int GetSlot() const { return Slot; }

    /// Set in rendering pipeline.
    void SetInRenderingPipeline() const;
};


//---------------------------------------------------------------------------------------------------------------------
/// Create constant buffer.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
void CRD11BindingConstantBuffer<T>::Create( const CRName& InName, unsigned int InSlot, ED11RenderingPipelineStage InStage, const T* Data )
{
    Name  = InName;
    Slot  = InSlot;
    Stage = InStage;
    
    ConstantBufferPtr = GD11RM.GetConstantBuffer( Name );
    ConstantBufferPtr.lock()->Create( D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, sizeof( T ) );

    if ( Data )
    {
        Update( *Data );
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Update constant buffer.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
void CRD11BindingConstantBuffer<T>::Update( const T& Data )
{
    if ( ConstantBufferPtr.expired() ) return;

    ID3D11Buffer* bufferPtr = ConstantBufferPtr.lock()->GetObjectPtr();
    if ( !bufferPtr ) return;

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = GD11.GetDeviceContext()->Map( bufferPtr, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
    CRGeneric::CheckError( hr );
    {
        memcpy_s( mappedResource.pData, sizeof( T ), &Data, sizeof( T ) );
    }
    GD11.GetDeviceContext()->Unmap( bufferPtr, 0 );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set in rendering pipeline.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
void CRD11BindingConstantBuffer<T>::SetInRenderingPipeline() const
{
    if ( ConstantBufferPtr.expired() ) return;
    
    GD11RP.SetConstantBuffer( ConstantBufferPtr.lock()->GetObjectPtr(), Slot, Stage );
} 
