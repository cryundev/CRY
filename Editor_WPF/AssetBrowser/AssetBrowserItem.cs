using System.IO;


namespace Editor_WPF.AssetBrowser;


//---------------------------------------------------------------------------------------------------------------------
/// AssetBrowserItem
//---------------------------------------------------------------------------------------------------------------------
public sealed class AssetBrowserItem
{
    public string Name                { get; private init; } = string.Empty;
    public string FullPath            { get; private init; } = string.Empty;
    public string ProjectRelativePath { get; private init; } = string.Empty;
    public string DetailText          { get; private init; } = string.Empty;

    public AssetBrowserItemKind Kind { get; private init; }

    //-----------------------------------------------------------------------------------------------------------------
    /// CreateFolder
    //-----------------------------------------------------------------------------------------------------------------
    public static AssetBrowserItem CreateFolder( string fullPath, string contentPath )
    {
        return new AssetBrowserItem
        {
            Kind                = AssetBrowserItemKind.Folder,
            Name                = Path.GetFileName( fullPath ),
            FullPath            = fullPath,
            ProjectRelativePath = AssetBrowserPath.GetProjectRelativePath( fullPath, contentPath ),
            DetailText          = "Folder",
        };
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// CreateFile
    //-----------------------------------------------------------------------------------------------------------------
    public static AssetBrowserItem CreateFile( string fullPath, string contentPath )
    {
        string extension = Path.GetExtension( fullPath );
        bool isAsset = string.Equals( extension, ".cra", StringComparison.OrdinalIgnoreCase );

        string detailText = isAsset ? "CRY Asset" : extension.TrimStart( '.' ).ToUpperInvariant();

        return new AssetBrowserItem
        {
            Kind                = isAsset ? AssetBrowserItemKind.Asset : AssetBrowserItemKind.File,
            Name                = Path.GetFileName( fullPath ),
            FullPath            = fullPath,
            ProjectRelativePath = AssetBrowserPath.GetProjectRelativePath( fullPath, contentPath ),
            DetailText          = string.IsNullOrWhiteSpace( detailText ) ? "File" : detailText,
        };
    }
}
