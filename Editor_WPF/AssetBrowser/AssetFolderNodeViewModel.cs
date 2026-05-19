using System.Collections.ObjectModel;
using System.IO;
using Editor_WPF.Common;


namespace Editor_WPF.AssetBrowser;


//---------------------------------------------------------------------------------------------------------------------
/// AssetFolderNodeViewModel
//---------------------------------------------------------------------------------------------------------------------
public sealed class AssetFolderNodeViewModel : ViewModelBase
{
    private bool _isExpanded;
    private bool _childrenLoaded;
    private readonly bool _isPlaceholder;
    private readonly string _contentPath;

    public string Name                { get; }
    public string FullPath            { get; }
    public string ProjectRelativePath { get; }

    public ObservableCollection< AssetFolderNodeViewModel > Children { get; } = [];

    public bool IsExpanded
    {
        get => _isExpanded;
        set
        {
            if ( _isExpanded == value ) return;

            _isExpanded = value;
            if ( _isExpanded )
            {
                LoadChildren();
            }

            OnPropertyChanged( nameof( IsExpanded ) );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// AssetFolderNodeViewModel
    //-----------------------------------------------------------------------------------------------------------------
    private AssetFolderNodeViewModel( string fullPath, string contentPath )
    {
        FullPath = fullPath;
        _contentPath = contentPath;

        Name = Path.GetFileName( fullPath );
        if ( string.IsNullOrWhiteSpace( Name ) )
        {
            Name = AssetBrowserPath.ContentFolderName;
        }

        ProjectRelativePath = AssetBrowserPath.GetProjectRelativePath( fullPath, contentPath );

        if ( HasChildDirectories( fullPath ) )
        {
            Children.Add( CreatePlaceholder( contentPath ) );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// AssetFolderNodeViewModel
    //-----------------------------------------------------------------------------------------------------------------
    private AssetFolderNodeViewModel( string contentPath )
    {
        FullPath            = string.Empty;
        Name                = string.Empty;
        ProjectRelativePath = string.Empty;
        
        _contentPath = contentPath;
        
        _isPlaceholder  = true;
        _childrenLoaded = true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// CreateTree
    //-----------------------------------------------------------------------------------------------------------------
    public static AssetFolderNodeViewModel CreateTree( string fullPath, string contentPath )
    {
        return new AssetFolderNodeViewModel( fullPath, contentPath );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// LoadChildren
    //-----------------------------------------------------------------------------------------------------------------
    public void LoadChildren()
    {
        if ( _childrenLoaded || _isPlaceholder ) return;

        _childrenLoaded = true;
        Children.Clear();

        if ( !Directory.Exists( FullPath ) ) return;

        foreach ( string directory in Directory.EnumerateDirectories( FullPath ).OrderBy( Path.GetFileName ) )
        {
            Children.Add( CreateTree( directory, _contentPath ) );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// HasChildDirectories
    //-----------------------------------------------------------------------------------------------------------------
    private static bool HasChildDirectories( string path )
    {
        try
        {
            return Directory.EnumerateDirectories( path ).Any();
        }
        catch
        {
            return false;
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// CreatePlaceholder
    //-----------------------------------------------------------------------------------------------------------------
    private static AssetFolderNodeViewModel CreatePlaceholder( string contentPath )
    {
        return new AssetFolderNodeViewModel( contentPath );
    }
}
