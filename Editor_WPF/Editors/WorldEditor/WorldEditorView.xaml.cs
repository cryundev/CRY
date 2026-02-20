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

    //-----------------------------------------------------------------------------------------------------------------
    /// WorldEditorView
    //-----------------------------------------------------------------------------------------------------------------
    public WorldEditorView()
    {
        InitializeComponent();

        Loaded   += OnWorldEditorViewLoaded;
        Unloaded += OnWorldEditorViewUnloaded;
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
}
