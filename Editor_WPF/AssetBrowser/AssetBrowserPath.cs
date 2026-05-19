using System.IO;


namespace Editor_WPF.AssetBrowser;


//---------------------------------------------------------------------------------------------------------------------
/// AssetBrowserPath
//---------------------------------------------------------------------------------------------------------------------
internal static class AssetBrowserPath
{
    internal const string ContentFolderName = "Content";

    internal static readonly StringComparer Comparer = StringComparer.OrdinalIgnoreCase;

    //-----------------------------------------------------------------------------------------------------------------
    /// GetProjectRelativePath
    //-----------------------------------------------------------------------------------------------------------------
    internal static string GetProjectRelativePath( string fullPath, string contentPath )
    {
        if ( string.IsNullOrWhiteSpace( contentPath ) ) return fullPath;

        string relativePath = Path.GetRelativePath( contentPath, fullPath );

        return relativePath == "." ? ContentFolderName : Path.Combine( ContentFolderName, relativePath );
    }

}
