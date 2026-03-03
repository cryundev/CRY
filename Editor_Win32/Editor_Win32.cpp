#include "framework.h"
#include "Editor_Win32.h"
#include "../EditorRuntime/EditorRuntime.h"
#include "../EditorRuntime/Input/CRInputProcessorCamera.h"
#include "../EditorRuntime/Input/CRInputProcessorPicking.h"
#include "UI/CREditorUI.h"
#include "UI/CRUIManager.h"
#include <Extras/ImGUI/imgui.h>
#include <Source/Core/Math/CRMath.h>
#include <Source/RHI/CRRHI.h>
#include <Source/World/CRWorld.h>
#include <Engine.h>


#define MAX_LOADSTRING 100


//---------------------------------------------------------------------------------------------------------------------
/// global variables
//---------------------------------------------------------------------------------------------------------------------
HINSTANCE hInst;                           // Current instance.
WCHAR     szTitle      [ MAX_LOADSTRING ]; // Title bar text.
WCHAR     szWindowClass[ MAX_LOADSTRING ]; // Main window class name.


CRUIManager GUIManager;


//---------------------------------------------------------------------------------------------------------------------
/// function forward declaration
//---------------------------------------------------------------------------------------------------------------------
ATOM MyRegisterClass( HINSTANCE hInstance );
BOOL InitInstance   ( HINSTANCE, int );

LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
INT_PTR CALLBACK About  ( HWND, UINT, WPARAM, LPARAM );


//---------------------------------------------------------------------------------------------------------------------
/// wWinMain
//---------------------------------------------------------------------------------------------------------------------
int APIENTRY wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    LoadStringW( hInstance, IDS_APP_TITLE, szTitle,       MAX_LOADSTRING );
    LoadStringW( hInstance, IDC_ENGINE,    szWindowClass, MAX_LOADSTRING );

    MyRegisterClass( hInstance );

    if ( !InitInstance( hInstance, nCmdShow ) ) return false;

    HACCEL hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_ENGINE ) );
    MSG    msg;

    float deltaSeconds = 0.f;

    while( true )
    {
        if ( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
	    {
		    TranslateMessage( &msg );
		    DispatchMessage ( &msg );

		    if ( msg.message == WM_QUIT || msg.message == WM_DESTROY ) break;
	    }


        GFrameTime.Start();

        CREditorRuntimeWin32Input::TickCamera( deltaSeconds );

        CREngine::Tick( deltaSeconds );

        if ( CREngine::PreRender( deltaSeconds ) )
        {
            CREngine::Render( deltaSeconds );

            GUIManager.Draw();

            CREngine::PostRender( deltaSeconds );
        }

        deltaSeconds = GFrameTime.Finish();
    }

    return (int)msg.wParam;
}


//---------------------------------------------------------------------------------------------------------------------
/// MyRegisterClass
//---------------------------------------------------------------------------------------------------------------------
ATOM MyRegisterClass( HINSTANCE hInstance )
{
    WNDCLASSEXW wcex;
    wcex.cbSize        = sizeof( WNDCLASSEX );
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_ENGINE ) );
    wcex.hCursor       = LoadCursor( nullptr, IDC_ARROW );
    wcex.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
    wcex.lpszMenuName  = MAKEINTRESOURCEW( IDC_ENGINE );
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm       = LoadIcon( wcex.hInstance, MAKEINTRESOURCE( IDI_SMALL ) );

    return RegisterClassExW( &wcex );
}


//---------------------------------------------------------------------------------------------------------------------
/// InitInstance
//---------------------------------------------------------------------------------------------------------------------
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
    hInst = hInstance; // Store the instance handle in the global variable.

    constexpr int width  = 1400;
    constexpr int height = 900;

    HWND hWnd = CreateWindowW( szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, width, height, nullptr, nullptr, hInstance, nullptr );
    if ( !hWnd ) return false;

    RECT clientRect = {};
    GetClientRect( hWnd, &clientRect );

    const i32 viewportW = CRMath::Max< i32 >( 1, (i32)( clientRect.right  - clientRect.left ) );
    const i32 viewportH = CRMath::Max< i32 >( 1, (i32)( clientRect.bottom - clientRect.top  ) );

    if ( !CREngine::Initialize( hWnd, (u32)viewportW, (u32)viewportH ) )
    {
        MessageBoxW( hWnd, L"Engine initialization failed.", L"CRY Engine", MB_OK | MB_ICONERROR );
        DestroyWindow( hWnd );
        return FALSE;
    }

    CREditorRuntime::InitializeRuntime();

    GUIManager.AddUI( new CREditorUI() );

    ShowWindow( hWnd, nCmdShow );
    UpdateWindow( hWnd );

    return TRUE;
}


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );


namespace
{
//---------------------------------------------------------------------------------------------------------------------
/// Resize camera and renderer viewport to current client size.
//---------------------------------------------------------------------------------------------------------------------
void _ResizeViewportToClientRect( HWND hWnd )
{
    if ( !GRHI.IsInitialized() ) return;

    RECT clientRect = {};
    if ( !GetClientRect( hWnd, &clientRect ) ) return;

    const i32 viewportW = CRMath::Max< i32 >( 1, (i32)( clientRect.right  - clientRect.left ) );
    const i32 viewportH = CRMath::Max< i32 >( 1, (i32)( clientRect.bottom - clientRect.top  ) );

    if ( GWorld )
    {
        if ( CRCamera* camera = GWorld->GetCamera() )
        {
            camera->SetViewSize( (f32)viewportW, (f32)viewportH );
        }
    }

    GRHI.Resize( (u32)viewportW, (u32)viewportH );
}

//---------------------------------------------------------------------------------------------------------------------
/// WM_ACTIVATEAPP
//---------------------------------------------------------------------------------------------------------------------
LRESULT _OnActivateApp( HWND hWnd, WPARAM wParam )
{
    if ( !wParam )
    {
        CREditorRuntimeWin32Input::OnKillFocus( hWnd );
    }

    return 0;
}

//---------------------------------------------------------------------------------------------------------------------
/// WM_SIZE
//---------------------------------------------------------------------------------------------------------------------
LRESULT _OnSize( HWND hWnd, WPARAM wParam )
{
    if ( wParam == SIZE_MINIMIZED ) return 0;

    _ResizeViewportToClientRect( hWnd );

    return 0;
}

//---------------------------------------------------------------------------------------------------------------------
/// Mouse related messages.
//---------------------------------------------------------------------------------------------------------------------
LRESULT _OnMouseMessage( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    CREditorRuntimeWin32Input::OnMouseMessage       ( hWnd, message, wParam, lParam );
    CREditorRuntimeWin32Input::OnPickingMouseMessage( hWnd, message, wParam, lParam );

    return 0;
}

//---------------------------------------------------------------------------------------------------------------------
/// WM_MOUSEACTIVATE
//---------------------------------------------------------------------------------------------------------------------
LRESULT _OnMouseActivate()
{
    return MA_ACTIVATE;
}

//---------------------------------------------------------------------------------------------------------------------
/// WM_KEYDOWN
//---------------------------------------------------------------------------------------------------------------------
LRESULT _OnKeyDown( WPARAM wParam, LPARAM lParam )
{
    CREditorRuntimeWin32Input::OnKeyboardMessage( wParam, lParam, true );
    return 0;
}

//---------------------------------------------------------------------------------------------------------------------
/// WM_KEYUP
//---------------------------------------------------------------------------------------------------------------------
LRESULT _OnKeyUp( WPARAM wParam, LPARAM lParam )
{
    CREditorRuntimeWin32Input::OnKeyboardMessage( wParam, lParam, false );
    return 0;
}

//---------------------------------------------------------------------------------------------------------------------
/// WM_SYSKEYDOWN
//---------------------------------------------------------------------------------------------------------------------
LRESULT _OnSysKeyDown( WPARAM wParam, LPARAM lParam )
{
    CREditorRuntimeWin32Input::OnKeyboardMessage( wParam, lParam, true );

    if ( wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000 )
    {
    }

    return 0;
}

//---------------------------------------------------------------------------------------------------------------------
/// WM_SYSKEYUP
//---------------------------------------------------------------------------------------------------------------------
LRESULT _OnSysKeyUp( WPARAM wParam, LPARAM lParam )
{
    CREditorRuntimeWin32Input::OnKeyboardMessage( wParam, lParam, false );

    return 0;
}

//---------------------------------------------------------------------------------------------------------------------
/// WM_KILLFOCUS
//---------------------------------------------------------------------------------------------------------------------
LRESULT _OnKillFocus( HWND hWnd )
{
    CREditorRuntimeWin32Input::OnKillFocus( hWnd );

    return 0;
}

//---------------------------------------------------------------------------------------------------------------------
/// WM_COMMAND
//---------------------------------------------------------------------------------------------------------------------
LRESULT _OnCommand( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch ( LOWORD( wParam ) )
    {
    case IDM_ABOUT: DialogBox( hInst, MAKEINTRESOURCE( IDD_ABOUTBOX ), hWnd, About ); return 0;
    case IDM_EXIT:  DestroyWindow( hWnd ); return 0;
    default:        return DefWindowProc( hWnd, message, wParam, lParam );
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// WM_PAINT
//---------------------------------------------------------------------------------------------------------------------
LRESULT _OnPaint( HWND hWnd )
{
    PAINTSTRUCT ps;

    BeginPaint( hWnd, &ps );
    EndPaint  ( hWnd, &ps );

    return 0;
}

//---------------------------------------------------------------------------------------------------------------------
/// WM_DESTROY
//---------------------------------------------------------------------------------------------------------------------
LRESULT _OnDestroy( HWND hWnd )
{
    CREditorRuntimeWin32Input::OnKillFocus( hWnd );
    CREditorRuntime::ShutdownRuntime();
    CREngine::Shutdown();

    PostQuitMessage( 0 );

    return 0;
}
}


//---------------------------------------------------------------------------------------------------------------------
/// WndProc
//---------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    if ( ImGui_ImplWin32_WndProcHandler( hWnd, message, wParam, lParam ) )
        return true;

    switch ( message )
    {
    case WM_ACTIVATEAPP:   return _OnActivateApp   ( hWnd, wParam );
    case WM_SIZE:          return _OnSize          ( hWnd, wParam );
    case WM_ACTIVATE:
    case WM_INPUT:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MOUSEWHEEL:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
    case WM_MOUSEHOVER:    return _OnMouseMessage  ( hWnd, message, wParam, lParam );
    case WM_MOUSEACTIVATE: return _OnMouseActivate ();
    case WM_KEYDOWN:       return _OnKeyDown       ( wParam, lParam );
    case WM_KEYUP:         return _OnKeyUp         ( wParam, lParam );
    case WM_SYSKEYDOWN:    return _OnSysKeyDown    ( wParam, lParam );
    case WM_SYSKEYUP:      return _OnSysKeyUp      ( wParam, lParam );
    case WM_KILLFOCUS:     return _OnKillFocus     ( hWnd );
    case WM_COMMAND:       return _OnCommand       ( hWnd, message, wParam, lParam );
    case WM_PAINT:         return _OnPaint         ( hWnd );
    case WM_DESTROY:       return _OnDestroy       ( hWnd );
    default:               return DefWindowProc    ( hWnd, message, wParam, lParam );
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// About
//---------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK About( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
    UNREFERENCED_PARAMETER( lParam );
    switch ( message )
    {
    case WM_INITDIALOG: return (INT_PTR)TRUE;
    case WM_COMMAND:
	    {
		    if ( LOWORD( wParam ) == IDOK || LOWORD( wParam ) == IDCANCEL )
		    {
			    EndDialog( hDlg, LOWORD( wParam ) );
			    return (INT_PTR)TRUE;
		    }
		    break;
	    }
    }
    return (INT_PTR)FALSE;
}
