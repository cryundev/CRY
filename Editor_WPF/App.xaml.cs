using System.Windows;
using System.Windows.Threading;
using System.IO;


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
        try
        {
            string logDirectory = Path.Combine( Environment.GetFolderPath( Environment.SpecialFolder.ApplicationData ), "Editor_WPF" );
            Directory.CreateDirectory( logDirectory );

            string logPath = Path.Combine( logDirectory, "Crash.log" );
            File.WriteAllText( logPath, args.Exception.ToString() );
        }
        catch
        {
        }

        MessageBox.Show( args.Exception.Message );

        args.Handled = true;

        Environment.Exit( 0 );
    }
}
