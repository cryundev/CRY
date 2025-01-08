#pragma once


#include "CRRHITypes.h"
#include <windows.h>


class ICRRHIMesh;
class ICRRHIRenderer;


class CRRHI
{
private:
    ECRRHIType      RHIType  = ECRRHIType::DirectX11;
    ICRRHIRenderer* Renderer = nullptr;
    
public:
    /// Constructor
    CRRHI( ECRRHIType InRHIType );
    
    /// Initialize RHI.
    void Initialize( HWND hWnd, unsigned int Width, unsigned int Height );

    /// Render frame RHI.
    void RenderFrame() const;

    /// Create renderer. 
    ICRRHIRenderer* CreateRenderer();

    /// Create mesh.
    ICRRHIMesh* CreateMesh() const;

    /// Get renderer.
    ICRRHIRenderer* GetRenderer() const { return Renderer; }
};


extern CRRHI GRHI;
