#include "CRRHI.h"
#include "Core/ImGUI/imgui.h"
#include "Core/ImGUI/imgui_impl_dx11.h"
#include "Core/ImGUI/imgui_impl_win32.h"
#include "DX11/CRD11.h"
#include "DX11/CRD11Mesh.h"
#include "DX11/CRD11Renderer.h"
#include "DX11/Core/CRD11CompiledShader.h"
#include "DX11/Core/CRD11Device.h"
#include "DX11/Core/CRD11InputLayout.h"
#include "DX11/Core/CRD11PixelShader.h"
#include "DX11/Core/CRD11VertexShader.h"
#include "UI/CRUIManager.h"


CRUIManager GUIManager;


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
/// Initialize RHI.
//---------------------------------------------------------------------------------------------------------------------
void CRRHI::Initialize( HWND hWnd, unsigned int Width, unsigned int Height )
{
    GD11.Create( hWnd );
    Renderer->Initialize( Width, Height );

    InitializeShaders();

    /// Initialize ImGUI
    {
        IMGUI_CHECKVERSION();

        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui::StyleColorsDark();

        ImGui_ImplWin32_Init( hWnd );
        ImGui_ImplDX11_Init( GD11.GetDevice(), GD11.GetDeviceContext() );
    }

    GUIManager.Initialize();
}

//---------------------------------------------------------------------------------------------------------------------
/// Initialize shaders.
//---------------------------------------------------------------------------------------------------------------------
void CRRHI::InitializeShaders()
{
    CRD11CompiledShader compiledVS;
    compiledVS.Create( L"RHI/DX11/HLSL/shader.hlsl", "VS", "vs_5_0" );

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
    compiledPS.Create( L"RHI/DX11/HLSL/shader.hlsl", "PS", "ps_5_0" );

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
    Renderer->ClearRenderTarget();
    Renderer->Draw();
    
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();
    
    /// 이곳부터 내가 그리고 싶은 GUI들을 구성한다.
    {
        GUIManager.Draw(); 
    }

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );
    
    Renderer->Present();
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
