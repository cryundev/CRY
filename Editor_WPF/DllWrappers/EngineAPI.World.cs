using System.Numerics;
using System.Runtime.InteropServices;
using Editor_WPF.Objects;
using Editor_WPF.Utilities;


namespace Editor_WPF.DllWrappers;


//---------------------------------------------------------------------------------------------------------------------
/// EngineAPI World
//---------------------------------------------------------------------------------------------------------------------
public static partial class EngineAPI
{
    [StructLayout( LayoutKind.Sequential )]
    public struct CrVector3Native
    {
        public float X;
        public float Y;
        public float Z;

        public CrVector3Native( float x, float y, float z )
        {
            X = x;
            Y = y;
            Z = z;
        }

        public CrVector3Native( Vector3 value ) : this( value.X, value.Y, value.Z )
        {
        }

        public Vector3 ToVector3() => new Vector3( X, Y, Z );
    }

    [StructLayout( LayoutKind.Sequential )]
    public struct CrTransformNative
    {
        public CrVector3Native Position;
        public CrVector3Native RotationEuler;
        public CrVector3Native Scale;

        public CrTransformNative( Vector3 position, Vector3 rotationEuler, Vector3 scale )
        {
            Position      = new CrVector3Native( position );
            RotationEuler = new CrVector3Native( rotationEuler );
            Scale         = new CrVector3Native( scale );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// World
    //-----------------------------------------------------------------------------------------------------------------
    internal static class World
    {
        [DllImport( EngineDllName, EntryPoint = "SpawnActor" )]
        private static extern Int64 SpawnActorNative();

        [DllImport( EngineDllName, EntryPoint = "DespawnActor" )]
        private static extern void DespawnActorNative( Int64 actorId );

        public static Int64 SpawnActor( CrActorViewModel actor )
        {
            return SpawnActorNative();
        }

        public static void DespawnActor( CrActorViewModel actor )
        {
            DespawnActorNative( actor.ActorId );
        }
    }
}
