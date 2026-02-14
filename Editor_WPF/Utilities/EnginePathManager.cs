using System.IO;

namespace Editor_WPF.Utilities;


//---------------------------------------------------------------------------------------------------------------------
/// EnginePathManager
//---------------------------------------------------------------------------------------------------------------------
public static class EnginePathManager
{
    private const string RegistryKeyPath = @"SOFTWARE\CRY Engine";
    private const string InstallPathValueName = "InstallPath";

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

        string path = RegistryHelper.GetString( RegistryKeyPath, InstallPathValueName );

        if ( !string.IsNullOrEmpty( path ) && Directory.Exists( path ) )
        {
            _cachedEnginePath = NormalizePath( path );

            return _cachedEnginePath;
        }

        return null;
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

        if ( !Directory.Exists( enginePath ) )
        {
            Logger.Log( MessageType.Error, $"Engine path does not exist: {enginePath}" );

            return false;
        }

        string normalizedPath = NormalizePath( enginePath );

        if ( RegistryHelper.SetString( RegistryKeyPath, InstallPathValueName, normalizedPath ) )
        {
            _cachedEnginePath = normalizedPath;

            Logger.Log( MessageType.Info, $"Engine path registered: {normalizedPath}" );

            return true;
        }

        return false;
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
        if ( RegistryHelper.DeleteValue( RegistryKeyPath, InstallPathValueName ) )
        {
            _cachedEnginePath = null;

            Logger.Log( MessageType.Info, "Engine path unregistered" );

            return true;
        }

        return false;
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