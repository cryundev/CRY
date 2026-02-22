using System.Runtime.InteropServices;


namespace Editor_WPF.DllWrappers;


//---------------------------------------------------------------------------------------------------------------------
/// EngineAPI
//---------------------------------------------------------------------------------------------------------------------
public static partial class EngineAPI
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
}
