using System.Runtime.InteropServices;


namespace Editor_WPF.DllWrappers;


//---------------------------------------------------------------------------------------------------------------------
/// EngineAPI Viewport
//---------------------------------------------------------------------------------------------------------------------
public static partial class EngineAPI
{
    //-----------------------------------------------------------------------------------------------------------------
    /// Viewport
    //-----------------------------------------------------------------------------------------------------------------
    internal static class Viewport
    {
        [ DllImport( EngineDllName, EntryPoint = "InitializeViewport" ) ]
        [ return: MarshalAs( UnmanagedType.Bool ) ]
        public static extern bool Initialize( IntPtr hwnd, int width, int height );

        [ DllImport( EngineDllName, EntryPoint = "ShutdownViewport" ) ]
        public static extern void Shutdown();

        [ DllImport( EngineDllName, EntryPoint = "ResizeViewport" ) ]
        public static extern void Resize( int width, int height );

        [ DllImport( EngineDllName, EntryPoint = "TickViewport" ) ]
        public static extern void Tick( float deltaSeconds );

        [ DllImport( EngineDllName, EntryPoint = "RenderViewport" ) ]
        public static extern void Render( float deltaSeconds );

        [ DllImport( EngineDllName, EntryPoint = "SetViewportCamera" ) ]
        private static extern void SetViewportCameraNative( ref CrVector3Native position, ref CrVector3Native direction );

        public static void SetCamera( CrVector3Native position, CrVector3Native direction )
        {
            SetViewportCameraNative( ref position, ref direction );
        }
    }
}
