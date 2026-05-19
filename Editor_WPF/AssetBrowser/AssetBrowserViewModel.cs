using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Windows.Data;
using Editor_WPF.Common;
using Editor_WPF.GameProject;
using Editor_WPF.Utilities;


namespace Editor_WPF.AssetBrowser;


//---------------------------------------------------------------------------------------------------------------------
/// AssetBrowserViewModel
//---------------------------------------------------------------------------------------------------------------------
internal sealed class AssetBrowserViewModel : ViewModelBase
{
    private string _searchText = string.Empty;
    
    private AssetFolderNodeViewModel? _selectedFolder;

    public ObservableCollection< AssetFolderNodeViewModel > RootFolders  { get; } = [];
    public ObservableCollection< AssetBrowserItem         > CurrentItems { get; } = [];

    public ICollectionView FilteredItems { get; }

    public string ContentPath { get; private set; } = string.Empty;
    public string SelectedFolderFullPath => _selectedFolder?.FullPath ?? ContentPath;
    public string SelectedPathText => _selectedFolder?.ProjectRelativePath ?? AssetBrowserPath.ContentFolderName;
    public string StatusText { get; private set; } = "No project loaded.";

    public string SearchText
    {
        get => _searchText;
        set
        {
            if ( _searchText == value ) return;

            _searchText = value ?? string.Empty;
            FilteredItems.Refresh();
            UpdateStatusText();
            OnPropertyChanged( nameof( SearchText ) );
        }
    }

    public AssetFolderNodeViewModel? SelectedFolder
    {
        get => _selectedFolder;
        set
        {
            if ( value == null || ReferenceEquals( _selectedFolder, value ) ) return;

            SetSelectedFolder( value );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// AssetBrowserViewModel
    //-----------------------------------------------------------------------------------------------------------------
    public AssetBrowserViewModel()
    {
        FilteredItems = CollectionViewSource.GetDefaultView( CurrentItems );
        FilteredItems.Filter = FilterAssetItem;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Refresh
    //-----------------------------------------------------------------------------------------------------------------
    public void Refresh()
    {
        string? projectPath = ProjectViewModel.Current?.Path;
        if ( string.IsNullOrWhiteSpace( projectPath ) )
        {
            ClearProjectState();
            return;
        }

        ReloadContentRoot( projectPath );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// ClearProjectState
    //-----------------------------------------------------------------------------------------------------------------
    private void ClearProjectState()
    {
        RootFolders.Clear();
        CurrentItems.Clear();

        ContentPath = string.Empty;
        _selectedFolder = null;

        StatusText = "No project loaded.";
        RaiseFolderStateChanged();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// ReloadContentRoot
    //-----------------------------------------------------------------------------------------------------------------
    private void ReloadContentRoot( string projectPath )
    {
        ContentPath = Path.GetFullPath( Path.Combine( projectPath, AssetBrowserPath.ContentFolderName ) );
        Directory.CreateDirectory( ContentPath );

        string? selectedPath = _selectedFolder?.FullPath;

        RootFolders.Clear();

        AssetFolderNodeViewModel rootFolder = AssetFolderNodeViewModel.CreateTree( ContentPath, ContentPath );
        rootFolder.LoadChildren();
        RootFolders.Add( rootFolder );

        SetSelectedFolder( FindFolderByPath( rootFolder, selectedPath ) ?? rootFolder );
        RaiseFolderStateChanged();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OpenFolder
    //-----------------------------------------------------------------------------------------------------------------
    public void OpenFolder( AssetBrowserItem item )
    {
        if ( item.Kind != AssetBrowserItemKind.Folder ) return;
        if ( RootFolders.FirstOrDefault() is not { } rootFolder ) return;

        AssetFolderNodeViewModel? folder = FindFolderByPath( rootFolder, item.FullPath );
        if ( folder == null ) return;

        SetSelectedFolder( folder );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// TryCreateFolder
    //-----------------------------------------------------------------------------------------------------------------
    public bool TryCreateFolder( string folderName, string? parentFolderPath, out string errorMessage )
    {
        errorMessage = string.Empty;

        if ( string.IsNullOrWhiteSpace( ContentPath ) )
        {
            errorMessage = "No project is loaded.";
            return false;
        }

        string requestedParentPath = string.IsNullOrWhiteSpace( parentFolderPath ) ? SelectedFolderFullPath : parentFolderPath;

        PathValidation.FileNameRequest folderNameRequest = new PathValidation.FileNameRequest()
        {
            Value                        = folderName,
            EmptyErrorMessage            = "Type in a folder name.",
            InvalidCharacterErrorMessage = "Invalid character(s) used in folder name.",
        };

        PathValidation.FullPathRequest parentPathRequest = new PathValidation.FullPathRequest()
        {
            Value                        = requestedParentPath,
            EmptyErrorMessage            = "Select a valid content folder.",
            InvalidCharacterErrorMessage = "Invalid character(s) used in folder path.",
        };

        PathValidation.NewDirectoryRequest request = new PathValidation.NewDirectoryRequest()
        {
            NameRequest                = folderNameRequest,
            ParentPathRequest          = parentPathRequest,
            RootPath                   = ContentPath,
            InvalidParentErrorMessage  = "Select a valid content folder.",
            OutsideRootErrorMessage    = "Folder must be created inside Content.",
            AlreadyExistsMessageFormat = "Folder '{0}' already exists.",
        };

        if ( !PathValidation.TryGetNewDirectoryPath( request, out string newFolderPath, out errorMessage ) )
        {
            return false;
        }

        try
        {
            Directory.CreateDirectory( newFolderPath );
        }
        catch ( Exception exception )
        {
            errorMessage = exception.Message;
            return false;
        }

        Refresh();

        AssetFolderNodeViewModel? rootFolder = RootFolders.FirstOrDefault();
        AssetFolderNodeViewModel? newFolder  = rootFolder != null ? FindFolderByPath( rootFolder, newFolderPath ) : null;
        if ( newFolder != null )
        {
            SetSelectedFolder( newFolder );
        }

        return true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// LoadCurrentFolderItems
    //-----------------------------------------------------------------------------------------------------------------
    private void LoadCurrentFolderItems()
    {
        CurrentItems.Clear();

        if ( _selectedFolder != null && Directory.Exists( _selectedFolder.FullPath ) )
        {
            foreach ( string directory in Directory.EnumerateDirectories( _selectedFolder.FullPath ).OrderBy( Path.GetFileName ) )
            {
                CurrentItems.Add( AssetBrowserItem.CreateFolder( directory, ContentPath ) );
            }

            foreach ( string file in Directory.EnumerateFiles( _selectedFolder.FullPath ).OrderBy( Path.GetFileName ) )
            {
                CurrentItems.Add( AssetBrowserItem.CreateFile( file, ContentPath ) );
            }
        }

        UpdateStatusText();
        
        OnPropertyChanged( nameof( SelectedPathText ) );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// FilterAssetItem
    //-----------------------------------------------------------------------------------------------------------------
    private bool FilterAssetItem( object value )
    {
        if ( value is not AssetBrowserItem item ) return false;
        if ( string.IsNullOrWhiteSpace( _searchText ) ) return true;

        return item.Name      .Contains( _searchText, StringComparison.OrdinalIgnoreCase )
            || item.DetailText.Contains( _searchText, StringComparison.OrdinalIgnoreCase );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// UpdateStatusText
    //-----------------------------------------------------------------------------------------------------------------
    private void UpdateStatusText()
    {
        int folderCount = 0;
        int assetCount  = 0;
        int fileCount   = 0;

        foreach ( object item in FilteredItems )
        {
            if ( item is not AssetBrowserItem assetItem ) continue;

            switch ( assetItem.Kind )
            {
            case AssetBrowserItemKind.Folder: ++folderCount; break;
            case AssetBrowserItemKind.Asset:  ++assetCount;  break;
            default:                          ++fileCount;   break;
            }
        }

        StatusText = $"{folderCount} folders | {assetCount} assets | {fileCount} files";
        OnPropertyChanged( nameof( StatusText ) );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// RaiseFolderStateChanged
    //-----------------------------------------------------------------------------------------------------------------
    private void RaiseFolderStateChanged()
    {
        OnPropertyChanged( nameof( ContentPath ) );
        OnPropertyChanged( nameof( SelectedPathText ) );
        OnPropertyChanged( nameof( StatusText ) );
        OnPropertyChanged( nameof( SelectedFolder ) );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// FindFolderByPath
    //-----------------------------------------------------------------------------------------------------------------
    private static AssetFolderNodeViewModel? FindFolderByPath( AssetFolderNodeViewModel folder, string? path )
    {
        if ( string.IsNullOrWhiteSpace( path ) ) return null;

        string targetPath = Path.GetFullPath( path );
        if ( AssetBrowserPath.Comparer.Equals( folder.FullPath, targetPath ) ) return folder;
        if ( !PathValidation.IsPathInsideFolder( folder.FullPath, targetPath ) ) return null;

        folder.LoadChildren();

        foreach ( AssetFolderNodeViewModel child in folder.Children )
        {
            AssetFolderNodeViewModel? match = FindFolderByPath( child, targetPath );
            if ( match != null ) return match;
        }

        return null;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// SetSelectedFolder
    //-----------------------------------------------------------------------------------------------------------------
    private void SetSelectedFolder( AssetFolderNodeViewModel folder )
    {
        _selectedFolder = folder;

        AssetFolderNodeViewModel? rootFolder = RootFolders.FirstOrDefault();
        if ( rootFolder != null )
        {
            ExpandAncestors( rootFolder, folder );
        }

        _selectedFolder.IsExpanded = true;

        LoadCurrentFolderItems();

        OnPropertyChanged( nameof( SelectedFolder ) );
        OnPropertyChanged( nameof( SelectedPathText ) );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// ExpandAncestors
    //-----------------------------------------------------------------------------------------------------------------
    private static bool ExpandAncestors( AssetFolderNodeViewModel current, AssetFolderNodeViewModel target )
    {
        if ( ReferenceEquals( current, target ) )
        {
            current.IsExpanded = true;
            return true;
        }

        current.LoadChildren();

        foreach ( AssetFolderNodeViewModel child in current.Children )
        {
            if ( !ExpandAncestors( child, target ) ) continue;

            current.IsExpanded = true;
            return true;
        }

        return false;
    }
}
