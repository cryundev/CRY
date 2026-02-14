using Microsoft.Win32;
using System;

namespace Editor_WPF.Utilities;


//---------------------------------------------------------------------------------------------------------------------
/// RegistryHelper
//---------------------------------------------------------------------------------------------------------------------
public static class RegistryHelper
{
    //-----------------------------------------------------------------------------------------------------------------
    /// GetString
    //-----------------------------------------------------------------------------------------------------------------
    public static string GetString( string keyPath, string valueName, string defaultValue = null )
    {
        try
        {
            using RegistryKey key = Registry.CurrentUser.OpenSubKey( keyPath );

            if ( key != null )
            {
                object value = key.GetValue( valueName );

                if ( value != null )
                {
                    return value.ToString();
                }
            }
        }
        catch ( Exception ex )
        {
            Logger.Log( MessageType.Warning, $"Failed to read registry [{keyPath}\\{valueName}]: {ex.Message}" );
        }

        return defaultValue;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// SetString
    //-----------------------------------------------------------------------------------------------------------------
    public static bool SetString( string keyPath, string valueName, string value )
    {
        try
        {
            using RegistryKey key = Registry.CurrentUser.CreateSubKey( keyPath );

            if ( key != null )
            {
                key.SetValue( valueName, value, RegistryValueKind.String );

                return true;
            }
        }
        catch ( Exception ex )
        {
            Logger.Log( MessageType.Error, $"Failed to write registry [{keyPath}\\{valueName}]: {ex.Message}" );
        }

        return false;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetInt
    //-----------------------------------------------------------------------------------------------------------------
    public static int GetInt( string keyPath, string valueName, int defaultValue = 0 )
    {
        try
        {
            using RegistryKey key = Registry.CurrentUser.OpenSubKey( keyPath );

            if ( key != null )
            {
                object value = key.GetValue( valueName );

                if ( value != null && int.TryParse( value.ToString(), out int result ) )
                {
                    return result;
                }
            }
        }
        catch ( Exception ex )
        {
            Logger.Log( MessageType.Warning, $"Failed to read registry [{keyPath}\\{valueName}]: {ex.Message}" );
        }

        return defaultValue;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// SetInt
    //-----------------------------------------------------------------------------------------------------------------
    public static bool SetInt( string keyPath, string valueName, int value )
    {
        try
        {
            using RegistryKey key = Registry.CurrentUser.CreateSubKey( keyPath );

            if ( key != null )
            {
                key.SetValue( valueName, value, RegistryValueKind.DWord );

                return true;
            }
        }
        catch ( Exception ex )
        {
            Logger.Log( MessageType.Error, $"Failed to write registry [{keyPath}\\{valueName}]: {ex.Message}" );
        }

        return false;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// DeleteValue
    //-----------------------------------------------------------------------------------------------------------------
    public static bool DeleteValue( string keyPath, string valueName )
    {
        try
        {
            using RegistryKey key = Registry.CurrentUser.OpenSubKey( keyPath, true );

            if ( key != null )
            {
                key.DeleteValue( valueName, false );

                return true;
            }
        }
        catch ( Exception ex )
        {
            Logger.Log( MessageType.Error, $"Failed to delete registry value [{keyPath}\\{valueName}]: {ex.Message}" );
        }

        return false;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// DeleteKey
    //-----------------------------------------------------------------------------------------------------------------
    public static bool DeleteKey( string keyPath )
    {
        try
        {
            Registry.CurrentUser.DeleteSubKeyTree( keyPath, false );

            return true;
        }
        catch ( Exception ex )
        {
            Logger.Log( MessageType.Error, $"Failed to delete registry key [{keyPath}]: {ex.Message}" );
        }

        return false;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// KeyExists
    //-----------------------------------------------------------------------------------------------------------------
    public static bool KeyExists( string keyPath )
    {
        try
        {
            using RegistryKey key = Registry.CurrentUser.OpenSubKey( keyPath );

            return key != null;
        }
        catch
        {
            return false;
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// ValueExists
    //-----------------------------------------------------------------------------------------------------------------
    public static bool ValueExists( string keyPath, string valueName )
    {
        try
        {
            using RegistryKey key = Registry.CurrentUser.OpenSubKey( keyPath );

            if ( key != null )
            {
                return key.GetValue( valueName ) != null;
            }
        }
        catch
        {
            return false;
        }

        return false;
    }
}