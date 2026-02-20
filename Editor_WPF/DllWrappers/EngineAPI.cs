using System.Runtime.InteropServices;
using Editor_WPF.Objects;


namespace Editor_WPF.DllWrappers;


//---------------------------------------------------------------------------------------------------------------------
/// EngineAPI
//---------------------------------------------------------------------------------------------------------------------
public static class EngineAPI
{
    private const string EngineDllName = "EngineDLL.dll";
    
    [ DllImport( EngineDllName, CharSet = CharSet.Ansi ) ]
    public static extern int LoadGameCodeDLL( string dllPath );
    
    [ DllImport( EngineDllName ) ]
    public static extern int UnloadGameCodeDLL();
    
    [ DllImport( EngineDllName ) ]
    public static extern int GetScriptCreator( string name );
    
    [ DllImport( EngineDllName, EntryPoint = "ScriptNames" ) ]
    [return: MarshalAs( UnmanagedType.SafeArray )]
    public static extern string[] GetScriptNames();

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
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// World
    //-----------------------------------------------------------------------------------------------------------------
    internal static class World
    {
        [DllImport( EngineDllName )]
        private static extern Int64 SpawnActor();

        public static Int64 SpawnActor( CrActorViewModel crActor )
        {
            return SpawnActor();
        }

        [DllImport( EngineDllName )]
        private static extern void DespawnActor( Int64 id );
        
        public static void DespawnActor( CrActorViewModel crActor )
        {
            DespawnActor( crActor.ActorId );
        }
    }
}