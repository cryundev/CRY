using System.ComponentModel;
using System.IO;
using System.Windows;
using Editor_WPF.GameProject;


namespace Editor_WPF;


//---------------------------------------------------------------------------------------------------------------------
/// MainWindow
//---------------------------------------------------------------------------------------------------------------------
public partial class MainWindow : Window
{
    public static string EnginePath { get; private set; } = @"C:\Project\CRY\";
    
    //-----------------------------------------------------------------------------------------------------------------
    /// MainWindow
    //-----------------------------------------------------------------------------------------------------------------
    public MainWindow()
    {
        InitializeComponent();
        Loaded  += OnMainWindowLoaded;
        Closing += OnMainWindowClosing;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetEnginePath
    //-----------------------------------------------------------------------------------------------------------------
    private void GetEnginePath()
    {
        string? enginePath = Environment.GetEnvironmentVariable( "CRYE_PATH", EnvironmentVariableTarget.User );
        if ( enginePath == null || !Directory.Exists( Path.Combine( enginePath, @"Engine\Source" ) ) )
        {
            EnginePathDialog dialog = new EnginePathDialog();
            if ( dialog.ShowDialog() == true )
            {
                EnginePath = dialog.EnginePath;
                Environment.SetEnvironmentVariable( "CRYE_PATH", EnginePath.ToUpper(), EnvironmentVariableTarget.User );
            }
            else
            {
                Application.Current.Shutdown();
            }
        }
        else
        {
            EnginePath = enginePath;
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnMainWindowLoaded
    //-----------------------------------------------------------------------------------------------------------------
    private void OnMainWindowLoaded( object sender, RoutedEventArgs e )
    {
        Loaded -= OnMainWindowLoaded;
        GetEnginePath();
        OpenProjectBrowserDialog();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnMainWindowClosing
    //-----------------------------------------------------------------------------------------------------------------
    private void OnMainWindowClosing( object? sender, CancelEventArgs e )
    {
        Closing -= OnMainWindowClosing;
        
        Project.Current?.Unload();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OpenProjectBrowserDialog
    //-----------------------------------------------------------------------------------------------------------------
    private void OpenProjectBrowserDialog()
    {
        ProjectBrowserDialog projectBrowserDialog = new ProjectBrowserDialog();
        if ( projectBrowserDialog.ShowDialog() == false || projectBrowserDialog.DataContext == null )
        {
            Application.Current.Shutdown();
        }
        else
        {
            Project.Current?.Unload();
            DataContext = projectBrowserDialog.DataContext;
        }
    }
}