using System.Configuration;
using System.Data;
using System.Windows;
using System.Windows.Threading;


namespace Editor_WPF;


//---------------------------------------------------------------------------------------------------------------------
/// App
//---------------------------------------------------------------------------------------------------------------------
public partial class App : Application
{
    //-----------------------------------------------------------------------------------------------------------------
    /// App_DispatcherUnhandledException
    //-----------------------------------------------------------------------------------------------------------------
    void App_DispatcherUnhandledException( object sender, DispatcherUnhandledExceptionEventArgs args )
    {
        MessageBox.Show( args.Exception.Message );

        args.Handled = true;

        Environment.Exit( 0 );
    }
}