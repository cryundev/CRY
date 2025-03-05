using System.Windows;
using System.Windows.Controls;


namespace Editor_WPF.Utilities;


//---------------------------------------------------------------------------------------------------------------------
/// LoggerView
//---------------------------------------------------------------------------------------------------------------------
public partial class LoggerView : UserControl
{
    //-----------------------------------------------------------------------------------------------------------------
    /// LoggerView
    //-----------------------------------------------------------------------------------------------------------------
    public LoggerView()
    {
        InitializeComponent();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnClearButtonClicked
    //-----------------------------------------------------------------------------------------------------------------
    private void OnClearButtonClicked( object sender, RoutedEventArgs e )
    {
        Logger.Clear();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnMessageFilterButtonClicked
    //-----------------------------------------------------------------------------------------------------------------
    private void OnMessageFilterButtonClicked( object sender, RoutedEventArgs e )
    {
        int filter = 0x0;
        
        if ( toggleInfo.IsChecked    == true ) filter |= (int)MessageType.Info;
        if ( toggleWarning.IsChecked == true ) filter |= (int)MessageType.Warning;
        if ( toggleError.IsChecked   == true ) filter |= (int)MessageType.Error;

        Logger.SetMessageFilter( filter );
    }
}