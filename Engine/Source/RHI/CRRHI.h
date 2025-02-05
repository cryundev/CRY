#pragma once


#include "CRRHITypes.h"
#include "Source/Core/Containers/CRContainerInc.h"
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

    /// Initialize shaders.
    void InitializeShaders();

    /// Render frame RHI.
    void RenderFrame() const;

    /// Present.
    void Present() const;

    /// Create mesh.
    ICRRHIMeshSPtr CreateMesh();

    /// Get renderer.
    ICRRHIRenderer* GetRenderer() const { return Renderer; }

private:
    /// Create renderer. 
    ICRRHIRenderer* _CreateRenderer();
};


extern CRRHI GRHI;
