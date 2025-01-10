#pragma once


#include "CRRHITypes.h"
#include "Core/Containers/CRContainerInc.h"
#include <windows.h>


class CRRHI
{
private:
    ECRRHIType      RHIType  = ECRRHIType::DirectX11;
    ICRRHIRenderer* Renderer = nullptr;

    CRArray< ICRRHIMeshSPtr > Meshes;
    
public:
    /// Constructor
    CRRHI( ECRRHIType InRHIType );
    
    /// Initialize RHI.
    void Initialize( HWND hWnd, unsigned int Width, unsigned int Height );

    /// Render frame RHI.
    void RenderFrame() const;

    /// Create mesh.
    ICRRHIMeshSPtr CreateMesh();

    /// Get renderer.
    ICRRHIRenderer* GetRenderer() const { return Renderer; }

private:
    /// Create renderer. 
    ICRRHIRenderer* _CreateRenderer();
};


extern CRRHI GRHI;
