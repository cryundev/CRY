#pragma once


#include "CRD11BindingConstantBuffer.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRD11PendingUpdateConstantBuffer
//---------------------------------------------------------------------------------------------------------------------
template< typename T >
class CRD11PendingUpdateConstantBuffer
{
private:
    T Data = {};

    mutable bool                              bPendingUpdate = true;
    mutable CRD11BindingConstantBuffer< T >   Buffer;

public:
    /// Constructor.
    CRD11PendingUpdateConstantBuffer() = default;

    /// Create constant buffer and upload initial data.
    void Create( const CRName& InName, u32 InSlot, ED11RenderingPipelineStage InStage, const T* InData = nullptr );

    /// Get data.
    const T& Get() const { return Data; }

    /// Edit data and mark pending update.
    T& Edit();

    /// Set data and mark pending update.
    void Set( const T& InData );

    /// Returns true if a GPU upload is pending.
    bool IsPendingUpdate() const { return bPendingUpdate; }

    /// Upload pending data to GPU.
    void UploadPending() const;

    /// Upload pending data and bind to rendering pipeline.
    void SetInRenderingPipeline() const;
};


//---------------------------------------------------------------------------------------------------------------------
/// Create constant buffer and upload initial data.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
void CRD11PendingUpdateConstantBuffer< T >::Create( const CRName& InName, u32 InSlot, ED11RenderingPipelineStage InStage, const T* InData )
{
    if ( InData )
    {
        Data = *InData;
    }

    Buffer.Create( InName, InSlot, InStage, &Data );
    bPendingUpdate = false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Edit data and mark pending update.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
T& CRD11PendingUpdateConstantBuffer< T >::Edit()
{
    bPendingUpdate = true;
    return Data;
}

//---------------------------------------------------------------------------------------------------------------------
/// Set data and mark pending update.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
void CRD11PendingUpdateConstantBuffer< T >::Set( const T& InData )
{
    Data = InData;
    bPendingUpdate = true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Upload pending data to GPU.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
void CRD11PendingUpdateConstantBuffer< T >::UploadPending() const
{
    if ( !bPendingUpdate ) return;

    Buffer.Update( Data );
    bPendingUpdate = false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Upload pending data and bind to rendering pipeline.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
void CRD11PendingUpdateConstantBuffer< T >::SetInRenderingPipeline() const
{
    UploadPending();
    Buffer.SetInRenderingPipeline();
}
