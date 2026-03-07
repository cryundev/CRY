using System.Windows;
using System.Windows.Media;
using Editor_WPF.GameDev;
using Editor_WPF.GameProject;


namespace Editor_WPF.Editors;


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
