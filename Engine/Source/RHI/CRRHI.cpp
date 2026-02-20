#include "CRRHI.h"
#include "ICRRHIRenderer.h"
#include "DX11/CRD11.h"
#include "DX11/CRD11Material.h"
#include "DX11/CRD11Mesh.h"
#include "DX11/CRD11Renderer.h"
#include "DX11/Resource/CRD11CompiledShader.h"
#include "DX11/Resource/CRD11Device.h"
#include "DX11/Resource/CRD11InputLayout.h"
#include "DX11/Resource/CRD11PixelShader.h"
#include "DX11/Resource/CRD11VertexShader.h"
#include "Extras/ImGUI/imgui.h"
#include "Extras/ImGUI/imgui_impl_dx11.h"
#include "Extras/ImGUI/imgui_impl_win32.h"
#include "Source/Utility/Log/CRLog.h"
#include <filesystem>


CRRHI GRHI( ECRRHIType::DirectX11 );


//---------------------------------------------------------------------------------------------------------------------
/// Constructor
//---------------------------------------------------------------------------------------------------------------------
CRRHI::CRRHI( ECRRHIType InRHIType )
: RHIType( InRHIType )
{
    _CreateRenderer();
}

//---------------------------------------------------------------------------------------------------------------------
/// Destructor
//---------------------------------------------------------------------------------------------------------------------
CRRHI::~CRRHI()
{
    Shutdown();
}

//---------------------------------------------------------------------------------------------------------------------
/// Initialize RHI.
//---------------------------------------------------------------------------------------------------------------------
bool CRRHI::Initialize( HWND hWnd, u32 Width, u32 Height )
{
    Shutdown();

    if ( !Renderer )
    {
        _CreateRenderer();
    }

    if ( !Renderer )
    {
        GLog.AddLog( "[CRRHI::Initialize] Failed to create renderer." );
        return false;
    }

    if ( !GD11.Create( hWnd ) )
    {
        GLog.AddLog( "[CRRHI::Initialize] Failed to create D3D11 device." );
        Shutdown();
        return false;
    }

    Renderer->Initialize( Width, Height );

    InitializeShaders();

    /// Initialize ImGUI
    {
        if ( ImGui::GetCurrentContext() )
        {
            ImGui_ImplDX11_Shutdown();
            ImGui_ImplWin32_Shutdown();
            ImGui::DestroyContext();
        }

        IMGUI_CHECKVERSION();

        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui::StyleColorsDark();

        if ( !ImGui_ImplWin32_Init( hWnd ) )
        {
            GLog.AddLog( "[CRRHI::Initialize] Failed to initialize ImGui Win32 backend." );
            Shutdown();
            return false;
        }

        if ( !ImGui_ImplDX11_Init( GD11.GetDevice(), GD11.GetDeviceContext() ) )
        {
            GLog.AddLog( "[CRRHI::Initialize] Failed to initialize ImGui DX11 backend." );
            Shutdown();
            return false;
        }
    }

    bInitialized = true;
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Resize viewport without reinitializing the device or clearing render elements.
//---------------------------------------------------------------------------------------------------------------------
void CRRHI::Resize( u32 Width, u32 Height ) const
{
    if ( !bInitialized ) return;
    if ( !Renderer     ) return;
    
    if ( IDXGISwapChain* swapChain = GD11.GetSwapChain() ) 
    {
        HRESULT hr = swapChain->ResizeBuffers( 0, Width, Height, DXGI_FORMAT_UNKNOWN, 0 );
        
        if ( !CRGeneric::CheckError( hr ) ) return;
    }

    Renderer->Resize( Width, Height );
}

//---------------------------------------------------------------------------------------------------------------------
/// Initialize shaders.
//---------------------------------------------------------------------------------------------------------------------
void CRRHI::InitializeShaders() const
{
    std::filesystem::path hlslFilePath = std::filesystem::path( __FILE__ ).parent_path() / "DX11/HLSL/shader.hlsl";
    if ( !std::filesystem::exists( hlslFilePath ) )
    {
        GLog.AddLog( "[CRRHI::InitializeShaders] Failed to find shader file." );
        
        return;
    }

    CRD11CompiledShader compiledVS;
    compiledVS.Create( hlslFilePath.wstring(), "VS", "vs_5_0" );
    if ( !compiledVS.GetObjectPtr() )
    {
        GLog.AddLog( "[CRRHI::InitializeShaders] Failed to compile vertex shader." );
        
        return;
    }

    CRD11VertexShaderWPtr vertexShader = GD11RM.GetVertexShader( "Diffuse" );
    if ( !vertexShader.expired() )
    {
        vertexShader.lock()->Create( compiledVS.GetObjectPtr() );
    }

    D3D11_INPUT_ELEMENT_DESC elements[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    CRD11InputLayoutWPtr inputLayout = GD11RM.GetInputLayout( "Diffuse" );
    if ( !inputLayout.expired() )
    {
        inputLayout.lock()->Create( elements, ARRAYSIZE( elements ), compiledVS.GetObjectPtr() );
    }
    
    CRD11CompiledShader compiledPS;
    compiledPS.Create( hlslFilePath.wstring(), "PS", "ps_5_0" );
    if ( !compiledPS.GetObjectPtr() )
    {
        GLog.AddLog( "[CRRHI::InitializeShaders] Failed to compile pixel shader." );
        
        return;
    }

    CRD11PixelShaderWPtr pixelShader = GD11RM.GetPixelShader( "Diffuse" );
    if ( !pixelShader.expired() )
    {
        pixelShader.lock()->Create( compiledPS.GetObjectPtr() );
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Render frame RHI.
//---------------------------------------------------------------------------------------------------------------------
void CRRHI::RenderFrame() const
{
    if ( !bInitialized ) return;
    if ( !Renderer ) return;
    if ( !GD11.GetDeviceContext() ) return;
    if ( !ImGui::GetCurrentContext() ) return;

    Renderer->ClearRenderTarget();
    Renderer->Draw();
    
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();
}

//---------------------------------------------------------------------------------------------------------------------
/// Present.
//---------------------------------------------------------------------------------------------------------------------
void CRRHI::Present() const
{
    if ( !bInitialized ) return;
    if ( !Renderer ) return;
    if ( !GD11.GetSwapChain() ) return;
    if ( !ImGui::GetCurrentContext() ) return;

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );
    
    Renderer->Present();
}

//---------------------------------------------------------------------------------------------------------------------
/// Shutdown RHI and release resources.
//---------------------------------------------------------------------------------------------------------------------
void CRRHI::Shutdown()
{
    bInitialized = false;

    // ImGui shutdown is idempotent-safe when backend/context is not initialized.
    if ( ImGui::GetCurrentContext() )
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    Meshes.clear();
    Materials.clear();

    GD11RM.Clear();

    if ( Renderer )
    {
        delete Renderer;
        Renderer = nullptr;
    }

    GD11.Clear();
}

//---------------------------------------------------------------------------------------------------------------------
/// Create mesh.
//---------------------------------------------------------------------------------------------------------------------
ICRRHIMeshSPtr CRRHI::CreateMesh()
{
    ICRRHIMesh* newMesh = nullptr;

    switch ( RHIType )
    {
    case ECRRHIType::DirectX11: newMesh = new CRD11Mesh(); break;
    }

    Meshes.push_back( CRMakeShared( newMesh ) );

    return Meshes.back();
}

//---------------------------------------------------------------------------------------------------------------------
/// Create material.
//---------------------------------------------------------------------------------------------------------------------
ICRRHIMaterialSPtr CRRHI::CreateMaterial()
{
    ICRRHIMaterial* newMaterial = nullptr;

    switch ( RHIType )
    {
    case ECRRHIType::DirectX11: newMaterial = new CRD11Material(); break;
    }

    Materials.push_back( CRMakeShared( newMaterial ) );

    return Materials.back();
}

//---------------------------------------------------------------------------------------------------------------------
/// Create renderer.
//---------------------------------------------------------------------------------------------------------------------
ICRRHIRenderer* CRRHI::_CreateRenderer()
{
    switch ( RHIType )
    {
    case ECRRHIType::DirectX11: Renderer = new CRD11Renderer(); break;
    }

    return Renderer;
}
