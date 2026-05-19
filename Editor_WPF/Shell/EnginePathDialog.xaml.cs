using System.IO;
using System.Windows;
using Editor_WPF.Utilities;


namespace Editor_WPF.Shell;


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

        PathValidation.FullPathRequest request = new PathValidation.FullPathRequest()
        {
            Value                        = path,
            EmptyErrorMessage            = "Invalid path.",
            InvalidCharacterErrorMessage = "Invalid character(s) used in path.",
        };

        if ( !PathValidation.TryGetRequiredFullPath( request, out string fullPath, out string errorMessage ) )
        {
            MessageTextBlock.Text = errorMessage;
        }
        else if ( !Directory.Exists( Path.Combine( fullPath, @"Engine\Source" ) ) )
        {
            MessageTextBlock.Text = "Unable to find the engine at the specified location.";
        }
        else
        {
            if ( !Path.EndsInDirectorySeparator( fullPath ) ) fullPath += @"\";
            EnginePath = fullPath;
            DialogResult = true;
            Close();
        }
    }
}
