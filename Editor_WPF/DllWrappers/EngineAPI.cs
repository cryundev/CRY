using System.Runtime.InteropServices;
using Editor_WPF.Objects;


namespace Editor_WPF.DllWrappers;


//---------------------------------------------------------------------------------------------------------------------
/// EngineAPI
//---------------------------------------------------------------------------------------------------------------------
public static class EngineAPI
{
    private const string DllName = "EngineDLL.dll";

    [DllImport( DllName )]
    private static extern Int64 SpawnActor();

    public static Int64 SpawnActor( CrActor crActor )
    {
        return SpawnActor();
    }

    [DllImport( DllName )]
    private static extern void DespawnActor( Int64 id );
    
    public static void DespawnActor( CrActor crActor )
    {
        DespawnActor( crActor.ActorId );
    }
}