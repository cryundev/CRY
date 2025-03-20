#pragma once


#include "CRD11Types.h"
#include "Source/Core/CRTypes.h"

struct ID3D11DepthStencilState;
struct ID3D11DepthStencilView;


//---------------------------------------------------------------------------------------------------------------------
/// CRD11DepthStencilBuffer
//---------------------------------------------------------------------------------------------------------------------
class CRD11DepthStencilBuffer
{
private:
    CRD11Texture2DWPtr         BufferPtr;
    CRD11DepthStencilViewWPtr  ViewPtr;
    CRD11DepthStencilStateWPtr StatePtr;

public:
    /// Create depth stencil buffer.
    void Create( u32 Width, u32 Height );

    /// Get depth stencil view.
    ID3D11DepthStencilView* GetView() const;

    /// Get depth stencil state.
    ID3D11DepthStencilState* GetState() const;

    /// Clear buffer.
    void ClearBuffer() const;
};
