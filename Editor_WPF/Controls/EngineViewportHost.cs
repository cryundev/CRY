using System.ComponentModel;
using System.Numerics;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Input;
using Editor_WPF.Components;
using Editor_WPF.DllWrappers;
using Editor_WPF.Editors;
using Editor_WPF.GameProject;
using Editor_WPF.Objects;
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
    private HWND _hwnd              = default;
    private bool _engineInitialized = false;
    private bool _leftButtonDown    = false;
    private bool _rightButtonDown   = false;
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
                             (WINDOW_STYLE)(uint)Windows.Win32.System.SystemServices.STATIC_STYLES.SS_NOTIFY;

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
        _ReleaseViewportInputState();
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
            case PInvoke.WM_MOUSEMOVE:   return _OnMouseMove      ( lParam, ref handled );
            case PInvoke.WM_LBUTTONDOWN: return _OnLeftButtonDown ( lParam, ref handled );
            case PInvoke.WM_LBUTTONUP:   return _OnLeftButtonUp   ( lParam, ref handled );
            case PInvoke.WM_RBUTTONDOWN: return _OnRightButtonDown( lParam, ref handled );
            case PInvoke.WM_RBUTTONUP:   return _OnRightButtonUp  ( lParam, ref handled );
            case PInvoke.WM_MOUSEWHEEL:  return _OnMouseWheel     ( wParam, ref handled );
            case PInvoke.WM_KEYDOWN:     return _OnKeyDown        ( wParam, lParam, ref handled );
            case PInvoke.WM_SYSKEYDOWN:  return _OnSysKeyDown     ( hwnd, msg, wParam, lParam, ref handled );
            case PInvoke.WM_KEYUP:       return _OnKeyUp          ( wParam, ref handled );
            case PInvoke.WM_SYSKEYUP:    return _OnSysKeyUp       ( hwnd, msg, wParam, lParam, ref handled );
            case PInvoke.WM_KILLFOCUS:   return _OnKillFocus      ( ref handled );
        }

        return base.WndProc( hwnd, msg, wParam, lParam, ref handled );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _OnMouseMove
    //-----------------------------------------------------------------------------------------------------------------
    private IntPtr _OnMouseMove( IntPtr lParam, ref bool handled )
    {
        _ForwardMouseMove( lParam );
        handled = true;
        return IntPtr.Zero;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _OnLeftButtonDown
    //-----------------------------------------------------------------------------------------------------------------
    private IntPtr _OnLeftButtonDown( IntPtr lParam, ref bool handled )
    {
        _leftButtonDown = true;
        _CaptureMouse();
        _FocusViewport();

        _TryPickActor( lParam );
        _ForwardMouseButton( lParam, EngineAPI.Input.MouseButton.Left, true );

        handled = true;
        return IntPtr.Zero;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _OnLeftButtonUp
    //-----------------------------------------------------------------------------------------------------------------
    private IntPtr _OnLeftButtonUp( IntPtr lParam, ref bool handled )
    {
        _leftButtonDown = false;

        _ForwardMouseButton( lParam, EngineAPI.Input.MouseButton.Left, false );
        _ReleaseMouseCaptureIfNeeded();

        handled = true;
        return IntPtr.Zero;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _OnRightButtonDown
    //-----------------------------------------------------------------------------------------------------------------
    private IntPtr _OnRightButtonDown( IntPtr lParam, ref bool handled )
    {
        _rightButtonDown = true;
        _CaptureMouse();
        _FocusViewport();

        _ForwardMouseButton( lParam, EngineAPI.Input.MouseButton.Right, true );

        handled = true;
        return IntPtr.Zero;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _OnRightButtonUp
    //-----------------------------------------------------------------------------------------------------------------
    private IntPtr _OnRightButtonUp( IntPtr lParam, ref bool handled )
    {
        _rightButtonDown = false;

        _ForwardMouseButton( lParam, EngineAPI.Input.MouseButton.Right, false );
        _ReleaseMouseCaptureIfNeeded();

        handled = true;
        return IntPtr.Zero;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _OnMouseWheel
    //-----------------------------------------------------------------------------------------------------------------
    private IntPtr _OnMouseWheel( IntPtr wParam, ref bool handled )
    {
        _ForwardMouseWheel( wParam );
        handled = true;
        return IntPtr.Zero;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _OnKeyDown
    //-----------------------------------------------------------------------------------------------------------------
    private IntPtr _OnKeyDown( IntPtr wParam, IntPtr lParam, ref bool handled )
    {
        int virtualKey = _GetVirtualKeyFromWParam( wParam );

        if ( _IsRepeatKey( lParam ) )
        {
            handled = true;
            return IntPtr.Zero;
        }

        if ( _TryRelayEditorShortcut( virtualKey ) )
        {
            handled = true;
            return IntPtr.Zero;
        }

        _ForwardKeyState( virtualKey, true );

        handled = true;
        return IntPtr.Zero;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _OnSysKeyDown
    //-----------------------------------------------------------------------------------------------------------------
    private IntPtr _OnSysKeyDown( IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled )
    {
        return base.WndProc( hwnd, msg, wParam, lParam, ref handled );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _OnKeyUp
    //-----------------------------------------------------------------------------------------------------------------
    private IntPtr _OnKeyUp( IntPtr wParam, ref bool handled )
    {
        int virtualKey = _GetVirtualKeyFromWParam( wParam );
        
        _ForwardKeyState( virtualKey, false );

        handled = true;
        return IntPtr.Zero;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _OnSysKeyUp
    //-----------------------------------------------------------------------------------------------------------------
    private IntPtr _OnSysKeyUp( IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled )
    {
        return base.WndProc( hwnd, msg, wParam, lParam, ref handled );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _OnKillFocus
    //-----------------------------------------------------------------------------------------------------------------
    private IntPtr _OnKillFocus( ref bool handled )
    {
        _ReleaseViewportInputState();
        _ReleaseMouseCapture();

        handled = true;
        return IntPtr.Zero;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// RenderFrame
    //-----------------------------------------------------------------------------------------------------------------
    public void RenderFrame( float deltaSeconds )
    {
        if ( !_engineInitialized ) return;

        EngineAPI.Viewport.Tick  ( deltaSeconds );
        EngineAPI.Viewport.Render( deltaSeconds );
        _SyncSelectedActorTransformFromEngine();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// ApplyWorldCamera
    //-----------------------------------------------------------------------------------------------------------------
    public void ApplyWorldCamera( WorldViewModel? world = null )
    {
        if ( !_engineInitialized ) return;

        WorldViewModel? cameraWorld = world ?? ProjectViewModel.Current?.ActiveWorld;
        if ( cameraWorld == null ) return;

        Vector3 position  = cameraWorld.ViewportCameraPosition;
        Vector3 direction = cameraWorld.ViewportCameraDirection;

        EngineAPI.Viewport.SetCamera( new EngineAPI.CrVector3Native( position ), new EngineAPI.CrVector3Native( direction ) );
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
                _ReloadActiveWorldActors();
                ApplyWorldCamera();
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
    /// _FocusViewport
    //-----------------------------------------------------------------------------------------------------------------
    private void _FocusViewport()
    {
        if ( _hwnd == default ) return;

        PInvoke.SetFocus( _hwnd );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _ReleaseMouseCaptureIfNeeded
    //-----------------------------------------------------------------------------------------------------------------
    private void _ReleaseMouseCaptureIfNeeded()
    {
        if ( _leftButtonDown || _rightButtonDown ) return;

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
    /// _ReleaseViewportInputState
    //-----------------------------------------------------------------------------------------------------------------
    private void _ReleaseViewportInputState()
    {
        _leftButtonDown  = false;
        _rightButtonDown = false;

        if ( !_engineInitialized ) return;

        if ( _TryGetClientSize( out int viewportW, out int viewportH ) )
        {
            EngineAPI.Input.OnViewportMouseButton( 0, 0, viewportW, viewportH, EngineAPI.Input.MouseButton.Left,  false );
            EngineAPI.Input.OnViewportMouseButton( 0, 0, viewportW, viewportH, EngineAPI.Input.MouseButton.Right, false );
        }

        _ForwardKeyState( KeyInterop.VirtualKeyFromKey( Key.W ), false );
        _ForwardKeyState( KeyInterop.VirtualKeyFromKey( Key.A ), false );
        _ForwardKeyState( KeyInterop.VirtualKeyFromKey( Key.S ), false );
        _ForwardKeyState( KeyInterop.VirtualKeyFromKey( Key.D ), false );
        _ForwardKeyState( KeyInterop.VirtualKeyFromKey( Key.Q ), false );
        _ForwardKeyState( KeyInterop.VirtualKeyFromKey( Key.E ), false );
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
    /// _ForwardMouseWheel
    //-----------------------------------------------------------------------------------------------------------------
    private void _ForwardMouseWheel( IntPtr wParam )
    {
        if ( !_engineInitialized ) return;

        int wheelDelta = _GetWheelDeltaFromWParam( wParam );
        if ( wheelDelta == 0 ) return;

        EngineAPI.Input.OnViewportMouseWheel( wheelDelta );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _ForwardKeyState
    //-----------------------------------------------------------------------------------------------------------------
    private void _ForwardKeyState( int virtualKey, bool pressed )
    {
        if ( !_engineInitialized ) return;

        EngineAPI.Input.OnViewportKeyState( virtualKey, pressed );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _TryRelayEditorShortcut
    //-----------------------------------------------------------------------------------------------------------------
    private static bool _TryRelayEditorShortcut( int virtualKey )
    {
        ProjectViewModel? project = ProjectViewModel.Current;
        if ( project == null ) return false;

        Key pressedKey = KeyInterop.KeyFromVirtualKey( virtualKey );
        
        bool isControlDown = ( Keyboard.Modifiers & ModifierKeys.Control ) == ModifierKeys.Control;
        bool isShiftDown   = ( Keyboard.Modifiers & ModifierKeys.Shift   ) == ModifierKeys.Shift;

        if ( pressedKey == Key.F7 ) 
        {
            return _TryExecuteCommand( project.BuildCommand, true );
        }

        if ( !isControlDown ) return false;

        if ( pressedKey == Key.S && !isShiftDown ) return _TryExecuteCommand( project.SaveCommand );
        if ( pressedKey == Key.Z && !isShiftDown ) return _TryExecuteCommand( project.UndoCommand );
        if ( pressedKey == Key.Y )                 return _TryExecuteCommand( project.RedoCommand );

        return false;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _TryExecuteCommand
    //-----------------------------------------------------------------------------------------------------------------
    private static bool _TryExecuteCommand( ICommand? command, object? parameter = null )
    {
        if ( command == null ) return false;
        if ( !command.CanExecute( parameter ) ) return false;

        command.Execute( parameter );
        
        return true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _ReloadActiveWorldActors
    //-----------------------------------------------------------------------------------------------------------------
    private static void _ReloadActiveWorldActors()
    {
        WorldViewModel? activeWorld = ProjectViewModel.Current?.ActiveWorld;
        if ( activeWorld == null ) return;

        foreach ( CrActorViewModel actor in activeWorld.Actors )
        {
            if ( !ID.IsValid( actor.ActorId ) ) continue;

            EngineAPI.World.ApplyActorRuntimeState( actor.ActorId, actor );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _SyncSelectedActorTransformFromEngine
    //-----------------------------------------------------------------------------------------------------------------
    private static void _SyncSelectedActorTransformFromEngine()
    {
        if ( ActorView.Instance?.DataContext is not MultiSelectionActor selection ) return;
        if ( selection.SelectedActors.Count != 1 ) return;

        CrActorViewModel actor = selection.SelectedActors[ 0 ];
        if ( !ID.IsValid( actor.ActorId ) ) return;

        CrTransformComponentViewModel? transform = actor.GetComponent< CrTransformComponentViewModel >();
        if ( transform == null ) return;
        if ( !transform.RefreshFromEngine() ) return;

        selection.GetComponent< MultiSelectionTransform >()?.Refresh();
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
        
        long actorId = EngineAPI.Input.PickActor( pixelX, pixelY, viewportW, viewportH );

        _ = Dispatcher.BeginInvoke( () => ProjectLayoutView.Instance?.SelectActorFromViewport( actorId ) );
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
        long value = lParam.ToInt64();
        
        return unchecked( (short)value );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _GetYFromLParam
    //-----------------------------------------------------------------------------------------------------------------
    private static int _GetYFromLParam( IntPtr lParam )
    {
        long value = lParam.ToInt64();
        
        return unchecked( (short)( value >> 16 ) );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _GetWheelDeltaFromWParam
    //-----------------------------------------------------------------------------------------------------------------
    private static int _GetWheelDeltaFromWParam( IntPtr wParam )
    {
        long value = wParam.ToInt64();
        
        return unchecked( (short)( value >> 16 ) );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _GetVirtualKeyFromWParam
    //-----------------------------------------------------------------------------------------------------------------
    private static int _GetVirtualKeyFromWParam( IntPtr wParam )
    {
        long value = wParam.ToInt64();
        
        return unchecked( (ushort)value );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _IsRepeatKey
    //-----------------------------------------------------------------------------------------------------------------
    private static bool _IsRepeatKey( IntPtr lParam )
    {
        long value = lParam.ToInt64();
        
        return ( value & ( 1L << 30 ) ) != 0;
    }
}
