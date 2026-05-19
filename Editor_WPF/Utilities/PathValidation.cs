using System.IO;


namespace Editor_WPF.Utilities;


//---------------------------------------------------------------------------------------------------------------------
/// PathValidation
//---------------------------------------------------------------------------------------------------------------------
public static class PathValidation
{
    //-----------------------------------------------------------------------------------------------------------------
    /// FileNameRequest
    //-----------------------------------------------------------------------------------------------------------------
    public readonly struct FileNameRequest
    {
        public required string Value                        { get; init; }
        public required string EmptyErrorMessage            { get; init; }
        public required string InvalidCharacterErrorMessage { get; init; }
        public bool            ForbidWhiteSpace             { get; init; }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// FullPathRequest
    //-----------------------------------------------------------------------------------------------------------------
    public readonly struct FullPathRequest
    {
        public required string Value                        { get; init; }
        public required string EmptyErrorMessage            { get; init; }
        public required string InvalidCharacterErrorMessage { get; init; }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// NewDirectoryRequest
    //-----------------------------------------------------------------------------------------------------------------
    public readonly struct NewDirectoryRequest
    {
        public required FileNameRequest NameRequest                { get; init; }
        public required FullPathRequest ParentPathRequest          { get; init; }
        public required string          RootPath                   { get; init; }
        public required string          InvalidParentErrorMessage  { get; init; }
        public required string          OutsideRootErrorMessage    { get; init; }
        public required string          AlreadyExistsMessageFormat { get; init; }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// TryGetRequiredFileName
    //-----------------------------------------------------------------------------------------------------------------
    public static bool TryGetRequiredFileName( FileNameRequest request, out string fileName, out string errorMessage )
    {
        fileName = request.Value.Trim();
        errorMessage = string.Empty;

        if ( string.IsNullOrWhiteSpace( fileName ) )
        {
            errorMessage = request.EmptyErrorMessage;
            return false;
        }

        bool hasInvalidCharacter  = fileName.IndexOfAny( Path.GetInvalidFileNameChars() ) != -1;
        bool hasInvalidWhiteSpace = request.ForbidWhiteSpace && fileName.Any( char.IsWhiteSpace );

        if ( hasInvalidCharacter || hasInvalidWhiteSpace )
        {
            errorMessage = request.InvalidCharacterErrorMessage;
            return false;
        }

        return true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// TryGetRequiredFullPath
    //-----------------------------------------------------------------------------------------------------------------
    public static bool TryGetRequiredFullPath( FullPathRequest request, out string fullPath, out string errorMessage )
    {
        fullPath = string.Empty;
        errorMessage = string.Empty;

        string path = request.Value.Trim();
        if ( string.IsNullOrWhiteSpace( path ) )
        {
            errorMessage = request.EmptyErrorMessage;
            return false;
        }

        if ( path.IndexOfAny( Path.GetInvalidPathChars() ) != -1 )
        {
            errorMessage = request.InvalidCharacterErrorMessage;
            return false;
        }

        try
        {
            fullPath = Path.GetFullPath( path );
        }
        catch ( Exception exception ) when ( IsInvalidPathException( exception ) )
        {
            errorMessage = request.InvalidCharacterErrorMessage;
            return false;
        }

        return true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// TryGetNewDirectoryPath
    //-----------------------------------------------------------------------------------------------------------------
    public static bool TryGetNewDirectoryPath( NewDirectoryRequest request, out string newDirectoryPath, out string errorMessage )
    {
        newDirectoryPath = string.Empty;

        if ( !TryGetRequiredFileName( request.NameRequest, out string directoryName, out errorMessage ) )
        {
            return false;
        }

        if ( !TryGetRequiredFullPath( request.ParentPathRequest, out string parentPath, out errorMessage ) )
        {
            return false;
        }

        string rootPath = Path.GetFullPath( request.RootPath );
        if ( !Directory.Exists( parentPath ) || !IsPathInsideFolder( rootPath, parentPath, true ) )
        {
            errorMessage = request.InvalidParentErrorMessage;
            return false;
        }

        newDirectoryPath = Path.GetFullPath( Path.Combine( parentPath, directoryName ) );
        if ( !IsPathInsideFolder( rootPath, newDirectoryPath ) )
        {
            errorMessage = request.OutsideRootErrorMessage;
            return false;
        }

        if ( Directory.Exists( newDirectoryPath ) )
        {
            errorMessage = string.Format( request.AlreadyExistsMessageFormat, directoryName );
            return false;
        }

        return true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// IsInvalidPathException
    //-----------------------------------------------------------------------------------------------------------------
    private static bool IsInvalidPathException( Exception exception )
    {
        return exception is ArgumentException
            || exception is NotSupportedException
            || exception is PathTooLongException;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// IsPathInsideFolder
    //-----------------------------------------------------------------------------------------------------------------
    public static bool IsPathInsideFolder( string folderPath, string path, bool allowSamePath = false )
    {
        string relativePath = Path.GetRelativePath( folderPath, path );
        if ( relativePath == "." ) return allowSamePath;

        return !relativePath.StartsWith( "..", StringComparison.Ordinal )
            && !Path.IsPathRooted( relativePath );
    }
}
