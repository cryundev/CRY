using System.ComponentModel;
using System.Windows;
using Editor_WPF.GameProject;


namespace Editor_WPF;


//---------------------------------------------------------------------------------------------------------------------
/// MainWindow
//---------------------------------------------------------------------------------------------------------------------
public partial class MainWindow : Window
{
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
    /// OnMainWindowLoaded
    //-----------------------------------------------------------------------------------------------------------------
    private void OnMainWindowLoaded( object sender, RoutedEventArgs e )
    {
        Loaded -= OnMainWindowLoaded;
        
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