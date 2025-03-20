namespace Editor_WPF.Utilities;


public static class UtilObject
{
    //-----------------------------------------------------------------------------------------------------------------
    /// GetMixedValue
    //-----------------------------------------------------------------------------------------------------------------
    public static float? GetMixedValue< T >( List< T > objects, Func< T, float > getProperty )
    {
        float value = getProperty( objects.First() );

        return objects.Skip( 1 ).Any( x => !getProperty( x ).IsTheSameAs( value ) ) ? (float?)null : value;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetMixedValue
    //-----------------------------------------------------------------------------------------------------------------
    public static bool? GetMixedValue< T >( List< T > objects, Func< T, bool > getProperty )
    {
        bool value = getProperty( objects.First() );

        return objects.Skip( 1 ).Any( x => value != getProperty( x ) ) ? (bool?)null : value;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetMixedValue
    //-----------------------------------------------------------------------------------------------------------------
    public static string? GetMixedValue< T >( List< T > objects, Func< T, string > getProperty )
    {
        string value = getProperty( objects.First() );

        return objects.Skip( 1 ).Any( x => value != getProperty( x ) ) ? (string?)null : value;
    }
}