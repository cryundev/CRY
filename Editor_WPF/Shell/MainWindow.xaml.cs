using System.ComponentModel;
using System.IO;
using System.Windows;
using Editor_WPF.GameProject;
using Editor_WPF.GameProject.ProjectBrowser;


namespace Editor_WPF.Shell;


//---------------------------------------------------------------------------------------------------------------------
/// MainWindow
//---------------------------------------------------------------------------------------------------------------------
public partial class MainWindow : Window
{
    public static string EnginePath { get; private set; }
    
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

        if ( !TryAutoOpenProject() )
        {
            OpenProjectBrowserDialog();
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnMainWindowClosing
    //-----------------------------------------------------------------------------------------------------------------
    private void OnMainWindowClosing( object? sender, CancelEventArgs e )
    {
        Closing -= OnMainWindowClosing;

        ProjectViewModel.Current?.Unload();
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
            ProjectViewModel.Current?.Unload();
            DataContext = projectBrowserDialog.DataContext;
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// TryAutoOpenProject
    //-----------------------------------------------------------------------------------------------------------------
    private bool TryAutoOpenProject()
    {
        const string autoOpenProjectEnv = "CRYE_AUTO_OPEN_PROJECT";

        string? requestedProject = Environment.GetEnvironmentVariable( autoOpenProjectEnv );
        if ( string.IsNullOrWhiteSpace( requestedProject ) )
        {
            return false;
        }

        ProjectData? projectData = null;
        if ( string.Equals( requestedProject, "recent", StringComparison.OrdinalIgnoreCase ) )
        {
            projectData = OpenProject.Projects?.FirstOrDefault();
        }
        else
        {
            projectData = OpenProject.Projects?.FirstOrDefault
            (
                x => string.Equals( x.FullPath, requestedProject, StringComparison.OrdinalIgnoreCase )
            );

            if ( projectData == null && File.Exists( requestedProject ) )
            {
                string? projectDirectory = Path.GetDirectoryName( requestedProject );
                if ( !string.IsNullOrWhiteSpace( projectDirectory ) )
                {
                    if ( !Path.EndsInDirectorySeparator( projectDirectory ) )
                    {
                        projectDirectory += Path.DirectorySeparatorChar;
                    }

                    projectData = new ProjectData()
                    {
                        ProjectName = Path.GetFileNameWithoutExtension( requestedProject ),
                        ProjectPath = projectDirectory,
                    };
                }
            }
        }

        if ( projectData == null ) return false;

        ProjectViewModel.Current?.Unload();
        DataContext = OpenProject.Open( projectData );
        return true;
    }
}
