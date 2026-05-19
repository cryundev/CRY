using System.Windows;


namespace Editor_WPF.Resources.Styles;


//---------------------------------------------------------------------------------------------------------------------
/// ProjectBrowserDialogStyle
//---------------------------------------------------------------------------------------------------------------------
public partial class ProjectBrowserDialogStyle : ResourceDictionary
{
    //-----------------------------------------------------------------------------------------------------------------
    /// OnCloseButtonClicked
    //-----------------------------------------------------------------------------------------------------------------
    private void OnCloseButtonClicked( object sender, RoutedEventArgs e )
    {
        Window? window = (Window)( (FrameworkElement)sender ).TemplatedParent;
        window?.Close();
    }
}