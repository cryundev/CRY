using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Interop;
using Editor_WPF.DllWrappers;
using Editor_WPF.Editors;
using Editor_WPF.Utilities;
using Windows.Win32;
using Windows.Win32.Foundation;
using Windows.Win32.UI.WindowsAndMessaging;


namespace Editor_WPF.Controls;


//---------------------------------------------------------------------------------------------------------------------
/// EngineViewportHost
//---------------------------------------------------------------------------------------------------------------------
public class EngineViewportHost : HwndHost
{
    // "static" window class does not report mouse clicks unless SS_NOTIFY is enabled.
    private const uint SS_NOTIFY = 0x00000100;

    private HWND _hwnd              = default;
    private bool _engineInitialized = false;
    private bool _leftButtonDown    = false;
    private bool _mouseCaptured     = false;

    //-----------------------------------------------------------------------------------------------------------------
    /// BuildWindowCore
    //-----------------------------------------------------------------------------------------------------------------
    protected override HandleRef BuildWindowCore( HandleRef hwndParent )
    {
        int width  = Math.Max( 1, (int)ActualWidth  );
        int height = Math.Max( 1, (int)ActualHeight );

        WINDOW_STYLE style = WINDOW_STYLE.WS_CHILD        |
                             WINDOW_STYLE.WS_VISIBLE      |
                             WINDOW_STYLE.WS_CLIPSIBLINGS |
                             WINDOW_STYLE.WS_CLIPCHILDREN |
                             (WINDOW_STYLE)SS_NOTIFY;

        unsafe
        {
            _hwnd = PInvoke.CreateWindowEx
            (
                0,
                "static",
                string.Empty,
                style,
                0,
                0,
                width,
                height,
                new HWND( hwndParent.Handle ),
                default,
                default,
                null
            );
        }

        return new HandleRef( this, (IntPtr)_hwnd.Value );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// DestroyWindowCore
    //-----------------------------------------------------------------------------------------------------------------
    protected override void DestroyWindowCore( HandleRef hwnd )
    {
        _ReleaseMouseCapture();

        if ( _engineInitialized )
        {
            EngineAPI.Viewport.Shutdown();
            _engineInitialized = false;
        }

        if ( _hwnd != default )
        {
            PInvoke.DestroyWindow( _hwnd );
        }

        _hwnd = default;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnWindowPositionChanged
    //-----------------------------------------------------------------------------------------------------------------
    protected override void OnWindowPositionChanged( Rect rcBoundingBox )
    {
        base.OnWindowPositionChanged( rcBoundingBox );

        if ( _hwnd == default ) return;

        int width  = Math.Max( 1, (int)rcBoundingBox.Width  );
        int height = Math.Max( 1, (int)rcBoundingBox.Height );

        PInvoke.SetWindowPos
        (
            _hwnd,
            default,
            0,
            0,
            width,
            height,
            SET_WINDOW_POS_FLAGS.SWP_NOZORDER | SET_WINDOW_POS_FLAGS.SWP_NOACTIVATE
        );

        if ( !_engineInitialized )
        {
            _TryInitialize( width, height );
        }
        else
        {
            EngineAPI.Viewport.Resize( width, height );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// WndProc
    //-----------------------------------------------------------------------------------------------------------------
    protected override IntPtr WndProc( IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled )
    {
        uint message = unchecked( (uint)msg );

        switch ( message )
        {
            case PInvoke.WM_MOUSEMOVE:
            {
                _ForwardMouseMove( lParam );
                handled = true;
                return IntPtr.Zero;
            }

            case PInvoke.WM_LBUTTONDOWN:
            {
                _leftButtonDown = true;
                _CaptureMouse();

                _TryPickActor( lParam );
                _ForwardMouseButton( lParam, EngineAPI.Input.MouseButton.Left, true );

                handled = true;
                return IntPtr.Zero;
            }

            case PInvoke.WM_LBUTTONUP:
            {
                _leftButtonDown = false;

                _ForwardMouseButton( lParam, EngineAPI.Input.MouseButton.Left, false );
                _ReleaseMouseCaptureIfNeeded();

                handled = true;
                return IntPtr.Zero;
            }
        }

        return base.WndProc( hwnd, msg, wParam, lParam, ref handled );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// RenderFrame
    //-----------------------------------------------------------------------------------------------------------------
    public void RenderFrame( float deltaSeconds )
    {
        if ( !_engineInitialized ) return;

        EngineAPI.Viewport.Tick( deltaSeconds );
        EngineAPI.Viewport.Render( deltaSeconds );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _TryInitialize
    //-----------------------------------------------------------------------------------------------------------------
    private void _TryInitialize( int width, int height )
    {
        if ( DesignerProperties.GetIsInDesignMode( this ) ) return;
        if ( _hwnd == default ) return;

        try
        {
            _engineInitialized = EngineAPI.Viewport.Initialize( (IntPtr)_hwnd.Value, width, height );

            if ( _engineInitialized )
            {
                Logger.Log( MessageType.Info, "Engine viewport initialized." );
            }
            else
            {
                Logger.Log( MessageType.Warning, "Engine viewport initialization failed." );
            }
        }
        catch ( Exception e )
        {
            Logger.Log( MessageType.Error, $"Engine viewport exception: {e.Message}" );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _CaptureMouse
    //-----------------------------------------------------------------------------------------------------------------
    private void _CaptureMouse()
    {
        if ( _mouseCaptured ) return;
        if ( _hwnd == default ) return;

        PInvoke.SetCapture( _hwnd );
        _mouseCaptured = true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _ReleaseMouseCaptureIfNeeded
    //-----------------------------------------------------------------------------------------------------------------
    private void _ReleaseMouseCaptureIfNeeded()
    {
        if ( _leftButtonDown ) return;

        _ReleaseMouseCapture();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _ReleaseMouseCapture
    //-----------------------------------------------------------------------------------------------------------------
    private void _ReleaseMouseCapture()
    {
        if ( !_mouseCaptured ) return;

        PInvoke.ReleaseCapture();
        _mouseCaptured = false;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _ForwardMouseMove
    //-----------------------------------------------------------------------------------------------------------------
    private void _ForwardMouseMove( IntPtr lParam )
    {
        if ( !_engineInitialized ) return;
        if ( !_TryGetClientSize( out int viewportW, out int viewportH ) ) return;

        int pixelX = _GetXFromLParam( lParam );
        int pixelY = _GetYFromLParam( lParam );

        EngineAPI.Input.OnViewportMouseMove( pixelX, pixelY, viewportW, viewportH );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _ForwardMouseButton
    //-----------------------------------------------------------------------------------------------------------------
    private void _ForwardMouseButton( IntPtr lParam, int button, bool pressed )
    {
        if ( !_engineInitialized ) return;
        if ( !_TryGetClientSize( out int viewportW, out int viewportH ) ) return;

        int pixelX = _GetXFromLParam( lParam );
        int pixelY = _GetYFromLParam( lParam );

        EngineAPI.Input.OnViewportMouseButton( pixelX, pixelY, viewportW, viewportH, button, pressed );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _TryPickActor
    //-----------------------------------------------------------------------------------------------------------------
    private void _TryPickActor( IntPtr lParam )
    {
        if ( !_engineInitialized ) return;
        if ( !_TryGetClientSize( out int viewportW, out int viewportH ) ) return;

        int pixelX = _GetXFromLParam( lParam );
        int pixelY = _GetYFromLParam( lParam );

        EngineAPI.Input.PickActor( pixelX, pixelY, viewportW, viewportH );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _TryGetClientSize
    //-----------------------------------------------------------------------------------------------------------------
    private bool _TryGetClientSize( out int width, out int height )
    {
        width = 0;
        height = 0;

        if ( _hwnd == default ) return false;
        if ( !PInvoke.GetClientRect( _hwnd, out RECT rect ) ) return false;

        width  = Math.Max( 1, rect.right  - rect.left );
        height = Math.Max( 1, rect.bottom - rect.top  );

        return true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _GetXFromLParam
    //-----------------------------------------------------------------------------------------------------------------
    private static int _GetXFromLParam( IntPtr lParam )
    {
        int value = unchecked( (int)(long)lParam );
        return unchecked( (short)( value & 0xFFFF ) );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _GetYFromLParam
    //-----------------------------------------------------------------------------------------------------------------
    private static int _GetYFromLParam( IntPtr lParam )
    {
        int value = unchecked( (int)(long)lParam );
        return unchecked( (short)( ( value >> 16 ) & 0xFFFF ) );
    }
}
