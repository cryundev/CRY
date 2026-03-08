using System;
using System.IO;

namespace Editor_WPF.Utilities;


//---------------------------------------------------------------------------------------------------------------------
/// EnginePathManager
//---------------------------------------------------------------------------------------------------------------------
public static class EnginePathManager
{
    private const string EnginePathVariableName = "CRYE_PATH";

    private static string _cachedEnginePath = null;


    //-----------------------------------------------------------------------------------------------------------------
    /// GetEnginePath
    //-----------------------------------------------------------------------------------------------------------------
    public static string GetEnginePath()
    {
        if ( _cachedEnginePath != null )
        {
            return _cachedEnginePath;
        }

        string path = Environment.GetEnvironmentVariable( EnginePathVariableName, EnvironmentVariableTarget.User );
        if ( string.IsNullOrEmpty( path ) || !Directory.Exists( Path.Combine( path, @"Engine\Source" ) ) )
        {
            return null;
        }

        _cachedEnginePath = NormalizePath( path );
        return _cachedEnginePath;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// SetEnginePath
    //-----------------------------------------------------------------------------------------------------------------
    public static bool SetEnginePath( string enginePath )
    {
        if ( string.IsNullOrWhiteSpace( enginePath ) )
        {
            Logger.Log( MessageType.Error, "Engine path is empty" );
            return false;
        }

        if ( !Directory.Exists( Path.Combine( enginePath, @"Engine\Source" ) ) )
        {
            Logger.Log( MessageType.Error, $"Engine path does not exist: {enginePath}" );
            return false;
        }

        string normalizedPath = NormalizePath( enginePath );
        Environment.SetEnvironmentVariable( EnginePathVariableName, normalizedPath, EnvironmentVariableTarget.User );

        _cachedEnginePath = normalizedPath;
        Logger.Log( MessageType.Info, $"Engine path registered: {normalizedPath}" );

        return true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// IsRegistered
    //-----------------------------------------------------------------------------------------------------------------
    public static bool IsRegistered()
    {
        return !string.IsNullOrEmpty( GetEnginePath() );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Unregister
    //-----------------------------------------------------------------------------------------------------------------
    public static bool Unregister()
    {
        Environment.SetEnvironmentVariable( EnginePathVariableName, null, EnvironmentVariableTarget.User );
        _cachedEnginePath = null;

        Logger.Log( MessageType.Info, "Engine path unregistered" );
        return true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// NormalizePath
    //-----------------------------------------------------------------------------------------------------------------
    private static string NormalizePath( string path )
    {
        path = Path.GetFullPath( path );

        if ( !path.EndsWith( "\\" ) )
        {
            path += "\\";
        }

        return path;
    }
}
