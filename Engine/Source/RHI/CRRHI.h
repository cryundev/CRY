#pragma once


#include "CRRHITypes.h"
#include "Source/Core/CRTypes.h"
#include "Source/Core/Containers/CRContainerInc.h"
#include <windows.h>


class CRRHI
{
private:
    ECRRHIType      RHIType  = ECRRHIType::DirectX11;
    ICRRHIRenderer* Renderer = nullptr;

    CRArray< ICRRHIMeshSPtr >     Meshes;
    CRArray< ICRRHIMaterialSPtr > Materials;
    
public:
    /// Constructor
    CRRHI( ECRRHIType InRHIType );

    /// Destructor
    ~CRRHI();
    
    /// Initialize RHI.
    void Initialize( HWND hWnd, u32 Width, u32 Height );

    /// Initialize shaders.
    void InitializeShaders();

    /// Render frame RHI.
    void RenderFrame() const;

    /// Present.
    void Present() const;

    /// Shutdown RHI.
    void Shutdown();

    /// Create mesh.
    ICRRHIMeshSPtr CreateMesh();

    /// Create material.
    ICRRHIMaterialSPtr CreateMaterial();

    /// Get renderer.
    ICRRHIRenderer* GetRenderer() const { return Renderer; }

private:
    /// Create renderer. 
    ICRRHIRenderer* _CreateRenderer();
};


extern CRRHI GRHI;
