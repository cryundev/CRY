using System.ComponentModel;
using System.Windows;
using Editor_WPF.GameProject;


namespace Editor_WPF;


/// <summary>
/// Interaction logic for MainWindow.xaml
/// </summary>
public partial class MainWindow : Window
{
    public MainWindow()
    {
        InitializeComponent();
        Loaded += OnMainWindowLoaded;
        Closing += OnMainWindowClosing;
    }
    
    private void OnMainWindowLoaded( object sender, RoutedEventArgs e )
    {
        Loaded -= OnMainWindowLoaded;
        
        OpenProjectBrowserDialog();
    }
    
    private void OnMainWindowClosing( object sender, CancelEventArgs e )
    {
        Closing -= OnMainWindowClosing;
        
        Project.Current?.Unload();
    }

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