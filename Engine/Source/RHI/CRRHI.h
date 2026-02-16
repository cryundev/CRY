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
    bool            bInitialized = false;

    CRArray< ICRRHIMeshSPtr >     Meshes;
    CRArray< ICRRHIMaterialSPtr > Materials;
    
public:
    /// Constructor
    CRRHI( ECRRHIType InRHIType );

    /// Destructor
    ~CRRHI();
    
    /// Initialize RHI.
    bool Initialize( HWND hWnd, u32 Width, u32 Height );

    /// Initialize shaders.
    void InitializeShaders();

    /// Render frame RHI.
    void RenderFrame() const;

    /// Present.
    void Present() const;

    /// Shutdown RHI and release resources.
    void Shutdown();

    /// Create mesh.
    ICRRHIMeshSPtr CreateMesh();

    /// Create material.
    ICRRHIMaterialSPtr CreateMaterial();

    /// Get renderer.
    ICRRHIRenderer* GetRenderer() const { return Renderer; }

    /// Return true when RHI is fully initialized.
    bool IsInitialized() const { return bInitialized; }

private:
    /// Create renderer. 
    ICRRHIRenderer* _CreateRenderer();
};


extern CRRHI GRHI;
