using System.Windows;
using System.Windows.Media;
using Editor_WPF.AssetBrowser;
using Editor_WPF.AssetImport;
using Editor_WPF.DllWrappers;
using Editor_WPF.GameDev.Scripting;
using Editor_WPF.GameProject;
using Editor_WPF.Utilities;
using Microsoft.Win32;


namespace Editor_WPF.Editors.WorldEditor;


//---------------------------------------------------------------------------------------------------------------------
/// WorldEditorView
//---------------------------------------------------------------------------------------------------------------------
public partial class WorldEditorView
{
    private DateTime _lastRenderTime;
    private ProjectViewModel? _project;

    //-----------------------------------------------------------------------------------------------------------------
    /// WorldEditorView
    //-----------------------------------------------------------------------------------------------------------------
    public WorldEditorView()
    {
        InitializeComponent();

        Loaded             += OnWorldEditorViewLoaded;
        Unloaded           += OnWorldEditorViewUnloaded;
        DataContextChanged += OnDataContextChanged;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnWorldEditorViewLoaded
    //-----------------------------------------------------------------------------------------------------------------
    private void OnWorldEditorViewLoaded( object sender, RoutedEventArgs e )
    {
        Loaded -= OnWorldEditorViewLoaded;

        Focus();

        _lastRenderTime = DateTime.UtcNow;
        CompositionTarget.Rendering += OnRendering;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnWorldEditorViewUnloaded
    //-----------------------------------------------------------------------------------------------------------------
    private void OnWorldEditorViewUnloaded( object sender, RoutedEventArgs e )
    {
        CompositionTarget.Rendering -= OnRendering;
        _UnsubscribeProject();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnDataContextChanged
    //-----------------------------------------------------------------------------------------------------------------
    private void OnDataContextChanged( object sender, DependencyPropertyChangedEventArgs e )
    {
        _SubscribeProject( e.NewValue as ProjectViewModel );
        EngineViewport.ApplyWorldCamera( _project?.ActiveWorld );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnRendering
    //-----------------------------------------------------------------------------------------------------------------
    private void OnRendering( object? sender, EventArgs e )
    {
        DateTime now = DateTime.UtcNow;
        float deltaSeconds = (float)( now - _lastRenderTime ).TotalSeconds;
        
        _lastRenderTime = now;

        EngineViewport.RenderFrame( deltaSeconds );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnNewScriptButtonClicked
    //-----------------------------------------------------------------------------------------------------------------
    private void OnNewScriptButtonClicked( object sender, RoutedEventArgs e )
    {
        new CreateScriptDialog().ShowDialog();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnAssetImportMenuItemClicked
    //-----------------------------------------------------------------------------------------------------------------
    private void OnAssetImportMenuItemClicked( object sender, RoutedEventArgs e )
    {
        OpenFileDialog dialog = new OpenFileDialog()
        {
            Filter = EngineAPI.Asset.OpenFileFilter,
        };

        if ( dialog.ShowDialog() != true ) return;

        if ( !AssetImportDialog.TryGetImportType( dialog.FileName, out var importType ) )
        {
            Logger.Log( MessageType.Warning, $"Unsupported asset import source: {dialog.FileName}" );
            MessageBox.Show( "Unsupported asset type.", "Asset Import", MessageBoxButton.OK, MessageBoxImage.Warning );
            return;
        }

        new AssetImportDialog( dialog.FileName, importType ).ShowDialog();
        AssetFolderView.Instance?.Refresh();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnProjectPropertyChanged
    //-----------------------------------------------------------------------------------------------------------------
    private void OnProjectPropertyChanged( object? sender, System.ComponentModel.PropertyChangedEventArgs e )
    {
        if ( e.PropertyName == nameof( ProjectViewModel.ActiveWorld ) )
        {
            EngineViewport.ApplyWorldCamera( _project?.ActiveWorld );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _SubscribeProject
    //-----------------------------------------------------------------------------------------------------------------
    private void _SubscribeProject( ProjectViewModel? project )
    {
        if ( ReferenceEquals( _project, project ) ) return;

        _UnsubscribeProject();

        _project = project;
        if ( _project != null )
        {
            _project.PropertyChanged += OnProjectPropertyChanged;
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _UnsubscribeProject
    //-----------------------------------------------------------------------------------------------------------------
    private void _UnsubscribeProject()
    {
        if ( _project == null ) return;

        _project.PropertyChanged -= OnProjectPropertyChanged;
        _project = null;
    }
}
