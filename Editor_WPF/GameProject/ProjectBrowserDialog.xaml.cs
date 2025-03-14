using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Animation;


namespace Editor_WPF.GameProject;


//---------------------------------------------------------------------------------------------------------------------
/// ProjectBrowserDialog
//---------------------------------------------------------------------------------------------------------------------
public partial class ProjectBrowserDialog
{
        private SineEase _easing = new SineEase() { EasingMode = EasingMode.EaseInOut };
    
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
            
            OnToggleButtonClicked( CreateProjectButton, new RoutedEventArgs() );

        }

        CreateProjectButton.IsChecked = true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnToggleButton_Click
    //-----------------------------------------------------------------------------------------------------------------
    private void OnToggleButtonClicked( object sender, RoutedEventArgs e )
    {
        if ( Equals( sender, OpenProjectButton ) )
        {
            if ( CreateProjectButton.IsChecked == true )
            {
                CreateProjectButton.IsChecked = false;

                AnimateToOpenProject();
                
                OpenProjectView  .IsEnabled = true;
                CreateProjectView.IsEnabled = false;
            }

            OpenProjectButton.IsChecked = true;
        }
        else
        {
            if ( OpenProjectButton.IsChecked == true )
            {
                OpenProjectButton.IsChecked = false;

                AnimateToCloseProject();

                OpenProjectView  .IsEnabled = false;
                CreateProjectView.IsEnabled = true;
            }

            CreateProjectButton.IsChecked = true;
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// AnimateToOpenProject
    //-----------------------------------------------------------------------------------------------------------------
    private void AnimateToOpenProject()
    {
        DoubleAnimation highlightAnimation = new DoubleAnimation( 400, 100, new Duration( TimeSpan.FromSeconds( 0.15 ) ) );
        highlightAnimation.Completed += ( s, e ) =>
        {
            ThicknessAnimation animation = new ThicknessAnimation( new Thickness( -1600, 0, 0, 0 ), new Thickness( 0 ), new Duration( TimeSpan.FromSeconds( 0.35 ) ) );
            animation.EasingFunction = _easing;
            BrowserContent.BeginAnimation( MarginProperty, animation );
        };
        HighlightRect.BeginAnimation( Canvas.LeftProperty, highlightAnimation );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// AnimateToCloseProject
    //-----------------------------------------------------------------------------------------------------------------
    private void AnimateToCloseProject()
    {
        DoubleAnimation highlightAnimation = new DoubleAnimation( 100, 400, new Duration( TimeSpan.FromSeconds( 0.15 ) ) );
        highlightAnimation.Completed += ( s, e ) =>
        {
            ThicknessAnimation animation = new ThicknessAnimation( new Thickness( 0 ), new Thickness( -1600, 0, 0, 0 ), new Duration( TimeSpan.FromSeconds( 0.35 ) ) );
            animation.EasingFunction = _easing;
            BrowserContent.BeginAnimation( MarginProperty, animation );
        };
        HighlightRect.BeginAnimation( Canvas.LeftProperty, highlightAnimation );
    }
}