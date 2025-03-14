using System.Windows;


namespace Editor_WPF.Dictionaries;


//---------------------------------------------------------------------------------------------------------------------
/// MainWindowStyle
//---------------------------------------------------------------------------------------------------------------------
public partial class MainWindowStyle : ResourceDictionary
{
    //-----------------------------------------------------------------------------------------------------------------
    /// OnCloseButtonClicked
    //-----------------------------------------------------------------------------------------------------------------
    private void OnCloseButtonClicked( object sender, RoutedEventArgs e )
    {
        Window? window = (Window)( (FrameworkElement)sender ).TemplatedParent;
        window?.Close();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnMaximizeButtonClicked
    //-----------------------------------------------------------------------------------------------------------------
    private void OnMaximizeButtonClicked( object sender, RoutedEventArgs e )
    {
        Window? window = (Window)( (FrameworkElement)sender ).TemplatedParent;

        window.WindowState = window.WindowState == WindowState.Normal ? WindowState.Maximized : WindowState.Normal;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnMinimizeButtonClicked
    //-----------------------------------------------------------------------------------------------------------------
    private void OnMinimizeButtonClicked( object sender, RoutedEventArgs e )
    {
        Window? window = (Window)( (FrameworkElement)sender ).TemplatedParent;
        window.WindowState = WindowState.Minimized;
    }
}