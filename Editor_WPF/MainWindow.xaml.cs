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
    }
    
    private void OnMainWindowLoaded( object sender, RoutedEventArgs e )
    {
        Loaded -= OnMainWindowLoaded;
        
        OpenProjectBrowserDialog();
    }

    private void OpenProjectBrowserDialog()
    {
        var projectBrowserDialog = new ProjectBrowserDialog();
        if ( projectBrowserDialog.ShowDialog() == false )
        {
            Application.Current.Shutdown();
        }
        else
        {
            
        }
    }
}