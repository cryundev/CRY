using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using Editor_WPF.AssetImport;
using Editor_WPF.DllWrappers;
using Editor_WPF.Utilities;
using Microsoft.Win32;


namespace Editor_WPF.AssetBrowser;


//---------------------------------------------------------------------------------------------------------------------
/// AssetFolderView
//---------------------------------------------------------------------------------------------------------------------
public partial class AssetFolderView : UserControl
{
    private readonly AssetBrowserViewModel _viewModel = new AssetBrowserViewModel();
    private string? _contextFolderPath;

    public static AssetFolderView? Instance { get; private set; }

    //-----------------------------------------------------------------------------------------------------------------
    /// AssetFolderView
    //-----------------------------------------------------------------------------------------------------------------
    public AssetFolderView()
    {
        InitializeComponent();

        DataContext = _viewModel;
        Instance = this;

        Loaded  += OnAssetFolderViewLoaded;
        Unloaded += OnAssetFolderViewUnloaded;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Refresh
    //-----------------------------------------------------------------------------------------------------------------
    public void Refresh()
    {
        _viewModel.Refresh();
        QueueSelectedFolderTreeItemSync();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnAssetFolderViewLoaded
    //-----------------------------------------------------------------------------------------------------------------
    private void OnAssetFolderViewLoaded( object sender, RoutedEventArgs e )
    {
        Refresh();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnAssetFolderViewUnloaded
    //-----------------------------------------------------------------------------------------------------------------
    private void OnAssetFolderViewUnloaded( object sender, RoutedEventArgs e )
    {
        if ( ReferenceEquals( Instance, this ) )
        {
            Instance = null;
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnFolderTreeSelectedItemChanged
    //-----------------------------------------------------------------------------------------------------------------
    private void OnFolderTreeSelectedItemChanged( object sender, RoutedPropertyChangedEventArgs< object > e )
    {
        if ( e.NewValue is AssetFolderNodeViewModel folder )
        {
            _viewModel.SelectedFolder = folder;
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnAssetBrowserPreviewMouseRightButtonDown
    //-----------------------------------------------------------------------------------------------------------------
    private void OnAssetBrowserPreviewMouseRightButtonDown( object sender, MouseButtonEventArgs e )
    {
        _contextFolderPath = _viewModel.SelectedFolderFullPath;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnFolderTreePreviewMouseRightButtonDown
    //-----------------------------------------------------------------------------------------------------------------
    private void OnFolderTreePreviewMouseRightButtonDown( object sender, MouseButtonEventArgs e )
    {
        if ( e.OriginalSource is not DependencyObject source )
        {
            _contextFolderPath = _viewModel.SelectedFolderFullPath;
            return;
        }

        TreeViewItem? treeViewItem = FindAncestor< TreeViewItem >( source );
        if ( treeViewItem?.DataContext is not AssetFolderNodeViewModel folder )
        {
            _contextFolderPath = _viewModel.SelectedFolderFullPath;
            return;
        }

        treeViewItem.Focus();
        treeViewItem.IsSelected = true;
        _contextFolderPath = folder.FullPath;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnAssetListPreviewMouseRightButtonDown
    //-----------------------------------------------------------------------------------------------------------------
    private void OnAssetListPreviewMouseRightButtonDown( object sender, MouseButtonEventArgs e )
    {
        _contextFolderPath = _viewModel.SelectedFolderFullPath;

        if ( e.OriginalSource is not DependencyObject source ) return;

        ListBoxItem? listBoxItem = FindAncestor< ListBoxItem >( source );
        if ( listBoxItem == null ) return;

        listBoxItem.IsSelected = true;
        if ( listBoxItem.DataContext is AssetBrowserItem { Kind: AssetBrowserItemKind.Folder } folder )
        {
            _contextFolderPath = folder.FullPath;
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnAssetListMouseDoubleClick
    //-----------------------------------------------------------------------------------------------------------------
    private void OnAssetListMouseDoubleClick( object sender, MouseButtonEventArgs e )
    {
        if ( sender is not ListBox { SelectedItem: AssetBrowserItem item } ) return;

        _viewModel.OpenFolder( item );
        QueueSelectedFolderTreeItemSync();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnImportButtonClicked
    //-----------------------------------------------------------------------------------------------------------------
    private void OnImportButtonClicked( object sender, RoutedEventArgs e )
    {
        OpenFileDialog dialog = new OpenFileDialog()
        {
            Filter = EngineAPI.Asset.OpenFileFilter,
        };

        if ( dialog.ShowDialog() != true ) return;

        if ( !AssetImportDialog.TryGetImportType( dialog.FileName, out EngineAPI.Asset.ImportType importType ) )
        {
            Logger.Log( MessageType.Warning, $"Unsupported asset import source: {dialog.FileName}" );
            MessageBox.Show( "Unsupported asset type.", "Asset Import", MessageBoxButton.OK, MessageBoxImage.Warning );
            return;
        }

        new AssetImportDialog( dialog.FileName, importType ).ShowDialog();
        Refresh();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnRefreshButtonClicked
    //-----------------------------------------------------------------------------------------------------------------
    private void OnRefreshButtonClicked( object sender, RoutedEventArgs e )
    {
        Refresh();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnCreateFolderMenuItemClicked
    //-----------------------------------------------------------------------------------------------------------------
    private void OnCreateFolderMenuItemClicked( object sender, RoutedEventArgs e )
    {
        AssetCreateFolderDialog dialog = new AssetCreateFolderDialog();
        if ( dialog.ShowDialog() != true ) return;

        string parentFolderPath = _contextFolderPath ?? _viewModel.SelectedFolderFullPath;
        if ( _viewModel.TryCreateFolder( dialog.FolderName, parentFolderPath, out string errorMessage ) ) return;

        Logger.Log( MessageType.Warning, $"Failed to create asset folder: {errorMessage}" );
        MessageBox.Show( errorMessage, "Create Folder", MessageBoxButton.OK, MessageBoxImage.Warning );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// FindAncestor
    //-----------------------------------------------------------------------------------------------------------------
    private static T? FindAncestor< T >( DependencyObject source ) where T : DependencyObject
    {
        DependencyObject? current = source;
        while ( current != null )
        {
            if ( current is T match ) return match;

            current = VisualTreeHelper.GetParent( current );
        }

        return null;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// QueueSelectedFolderTreeItemSync
    //-----------------------------------------------------------------------------------------------------------------
    private void QueueSelectedFolderTreeItemSync()
    {
        Dispatcher.BeginInvoke( (Action)SelectSelectedFolderTreeItem );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// SelectSelectedFolderTreeItem
    //-----------------------------------------------------------------------------------------------------------------
    private void SelectSelectedFolderTreeItem()
    {
        AssetFolderNodeViewModel? selectedFolder = _viewModel.SelectedFolder;
        if ( selectedFolder == null ) return;

        TreeViewItem? treeViewItem = FindTreeViewItem( FolderTreeView, selectedFolder );
        if ( treeViewItem == null ) return;

        treeViewItem.IsSelected = true;
        treeViewItem.BringIntoView();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// FindTreeViewItem
    //-----------------------------------------------------------------------------------------------------------------
    private static TreeViewItem? FindTreeViewItem( ItemsControl parent, AssetFolderNodeViewModel folder )
    {
        parent.UpdateLayout();

        TreeViewItem? treeViewItem = parent.ItemContainerGenerator.ContainerFromItem( folder ) as TreeViewItem;
        if ( treeViewItem != null ) return treeViewItem;

        foreach ( object item in parent.Items )
        {
            TreeViewItem? childItem = parent.ItemContainerGenerator.ContainerFromItem( item ) as TreeViewItem;
            if ( childItem == null ) continue;

            TreeViewItem? match = FindTreeViewItem( childItem, folder );
            if ( match != null ) return match;
        }

        return null;
    }
}
