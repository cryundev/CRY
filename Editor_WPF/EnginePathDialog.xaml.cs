using System.IO;
using System.Windows;


namespace Editor_WPF;


//---------------------------------------------------------------------------------------------------------------------
/// EnginePathDialog
//---------------------------------------------------------------------------------------------------------------------
public partial class EnginePathDialog : Window
{
    public string EnginePath { get; private set; } = @"C:\Projects\CRY\";

    //-----------------------------------------------------------------------------------------------------------------
    /// EnginePathDialog
    //-----------------------------------------------------------------------------------------------------------------
    public EnginePathDialog()
    {
        InitializeComponent();
        Owner = Application.Current.MainWindow;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnOkButtonClicked
    //-----------------------------------------------------------------------------------------------------------------
    private void OnOkButtonClicked( object sender, RoutedEventArgs e )
    {
        string path = PathTextBox.Text;
        MessageTextBlock.Text = string.Empty;

        if ( string.IsNullOrEmpty( path ) )
        {
            MessageTextBlock.Text = "Invalid path.";
        }
        else if ( path.IndexOfAny( Path.GetInvalidPathChars() ) != -1 )
        {
            MessageTextBlock.Text = "Invalid character(s) used in path.";
        }
        else if ( !Directory.Exists( Path.Combine( path, @"Engine\Source" ) ) )
        {
            MessageTextBlock.Text = "Unable to find the engine at the specified location.";
        }
        else
        {
            if ( !Path.EndsInDirectorySeparator( path ) ) path += @"\";
            EnginePath = path;
            DialogResult = true;
            Close();
        }
    }
}