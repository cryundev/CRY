using System.Numerics;
using System.Runtime.InteropServices;
using Editor_WPF.Utilities;


namespace Editor_WPF.DllWrappers;


//---------------------------------------------------------------------------------------------------------------------
/// EngineAPI Actor
//---------------------------------------------------------------------------------------------------------------------
public static partial class EngineAPI
{
    //-----------------------------------------------------------------------------------------------------------------
    /// Actor
    //-----------------------------------------------------------------------------------------------------------------
    internal static class Actor
    {
        [DllImport( EngineDllName, EntryPoint = "GetActorTransform" )]
        private static extern bool GetTransformNative( Int64 id, out CrTransformNative outTransform );

        [DllImport( EngineDllName, EntryPoint = "SetActorTransform" )]
        private static extern bool SetTransformNative( Int64 id, ref CrTransformNative inTransform );

        [DllImport( EngineDllName, EntryPoint = "SetActorName", CharSet = CharSet.Ansi )]
        private static extern bool SetActorNameNative( Int64 id, string name );

        [DllImport( EngineDllName, EntryPoint = "ApplyNamedActorDefaults", CharSet = CharSet.Ansi )]
        private static extern bool ApplyNamedActorDefaultsNative( Int64 id, string name, string? projectPath );

        public static bool TryGetTransform( Int64 actorId, out CrTransformNative transform )
        {
            transform = default;
            if ( !ID.IsValid( actorId ) ) return false;

            return GetTransformNative( actorId, out transform );
        }

        public static bool TrySetTransform( Int64 actorId, Vector3 position, Vector3 rotationEuler, Vector3 scale )
        {
            if ( !ID.IsValid( actorId ) ) return false;

            CrTransformNative transform = new CrTransformNative( position, rotationEuler, scale );
            return SetTransformNative( actorId, ref transform );
        }

        public static bool TrySetName( Int64 actorId, string name )
        {
            if ( !ID.IsValid( actorId ) ) return false;

            return SetActorNameNative( actorId, name ?? string.Empty );
        }

        public static bool TryApplyNamedActorDefaults( Int64 actorId, string name, string? projectPath )
        {
            if ( !ID.IsValid( actorId ) ) return false;

            return ApplyNamedActorDefaultsNative( actorId, name ?? string.Empty, projectPath );
        }
    }
}
