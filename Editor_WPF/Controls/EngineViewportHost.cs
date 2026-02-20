using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Interop;
using Editor_WPF.DllWrappers;
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
    private HWND _hwnd             = default;
    private bool _engineInitialized = false;

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
                             WINDOW_STYLE.WS_CLIPCHILDREN;

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
}
