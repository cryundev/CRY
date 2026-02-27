using System.Runtime.InteropServices;


namespace Editor_WPF.DllWrappers;


//---------------------------------------------------------------------------------------------------------------------
/// EngineAPI Input
//---------------------------------------------------------------------------------------------------------------------
public static partial class EngineAPI
{
    //-----------------------------------------------------------------------------------------------------------------
    /// Input
    //-----------------------------------------------------------------------------------------------------------------
    internal static class Input
    {
        internal static class MouseButton
        {
            public const int Left  = 0;
            public const int Right = 1;
        }

        [DllImport( EngineDllName, EntryPoint = "OnViewportMouseMove" )]
        public static extern void OnViewportMouseMove( int pixelX, int pixelY, int viewportW, int viewportH );

        [DllImport( EngineDllName, EntryPoint = "OnViewportMouseButton" )]
        public static extern void OnViewportMouseButton
        (
            int pixelX,
            int pixelY,
            int viewportW,
            int viewportH,
            int button,
            [MarshalAs( UnmanagedType.I1 )] bool pressed
        );

        [DllImport( EngineDllName, EntryPoint = "OnViewportKeyState" )]
        public static extern void OnViewportKeyState( int virtualKey, [MarshalAs( UnmanagedType.I1 )] bool pressed );

        [DllImport( EngineDllName, EntryPoint = "OnViewportMouseWheel" )]
        public static extern void OnViewportMouseWheel( int wheelDelta );

        [DllImport( EngineDllName, EntryPoint = "PickActorAtScreen" )]
        public static extern long PickActor( int pixelX, int pixelY, int viewportW, int viewportH );
    }
}
