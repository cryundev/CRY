using System.Windows;


namespace Editor_WPF.GameProject;


//---------------------------------------------------------------------------------------------------------------------
/// ProjectBrowserDialog
//---------------------------------------------------------------------------------------------------------------------
public partial class ProjectBrowserDialog
{
    //-----------------------------------------------------------------------------------------------------------------
    /// ProjectBrowserDialog
    //-----------------------------------------------------------------------------------------------------------------
    public ProjectBrowserDialog()
    {
        InitializeComponent();

        Loaded += OnProjectBrowserDialogLoaded;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnProjectBrowserDialogLoaded
    //-----------------------------------------------------------------------------------------------------------------
    private void OnProjectBrowserDialogLoaded( object sender, RoutedEventArgs e )
    {
        Loaded -= OnProjectBrowserDialogLoaded;

        if ( !(OpenProject.Projects ?? throw new InvalidOperationException()).Any() )
        {
            OpenProjectButton.IsEnabled = false;
            OpenProjectView.Visibility = Visibility.Hidden;
            
            OnToggleButton_Click( CreateProjectButton, new RoutedEventArgs() );

        }

        CreateProjectButton.IsChecked = true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnToggleButton_Click
    //-----------------------------------------------------------------------------------------------------------------
    private void OnToggleButton_Click( object sender, RoutedEventArgs e )
    {
        if ( Equals( sender, OpenProjectButton ) )
        {
            if ( CreateProjectButton.IsChecked == true )
            {
                CreateProjectButton.IsChecked = false;
                
                BrowserContent.Margin = new Thickness( 0 );
            }

            OpenProjectButton.IsChecked = true;
        }
        else
        {
            if ( OpenProjectButton.IsChecked == true )
            {
                OpenProjectButton.IsChecked = false;
                
                BrowserContent.Margin = new Thickness( -800, 0, 0, 0 );
            }

            CreateProjectButton.IsChecked = true;
        }
    }
}