#include "CRRHI.h"
#include "Core/ImGUI/imgui.h"
#include "Core/ImGUI/imgui_impl_dx11.h"
#include "Core/ImGUI/imgui_impl_win32.h"
#include "DX11/CRD11.h"
#include "DX11/CRD11Mesh.h"
#include "DX11/CRD11Renderer.h"
#include "DX11/Core/CRD11Device.h"


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
        ImGui::Button( "Test Button" ); 
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
