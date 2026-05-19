using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using Editor_WPF.Utilities;
using Window = System.Windows.Window;


namespace Editor_WPF.AssetBrowser;


//---------------------------------------------------------------------------------------------------------------------
/// AssetCreateFolderDialog
//---------------------------------------------------------------------------------------------------------------------
public partial class AssetCreateFolderDialog : Window
{
    public string FolderName => FolderNameTextBox.Text.Trim();

    //-----------------------------------------------------------------------------------------------------------------
    /// AssetCreateFolderDialog
    //-----------------------------------------------------------------------------------------------------------------
    public AssetCreateFolderDialog()
    {
        InitializeComponent();

        Owner = Application.Current.MainWindow;

        Loaded += ( _, _ ) =>
        {
            FolderNameTextBox.Focus();
            FolderNameTextBox.SelectAll();
        };
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Validate
    //-----------------------------------------------------------------------------------------------------------------
    private bool Validate()
    {
        PathValidation.FileNameRequest request = new PathValidation.FileNameRequest()
        {
            Value                        = FolderName,
            EmptyErrorMessage            = "Type in a folder name.",
            InvalidCharacterErrorMessage = "Invalid character(s) used in folder name.",
        };

        PathValidation.TryGetRequiredFileName( request, out _, out string errorMessage );

        MessageTextBlock.Foreground = string.IsNullOrWhiteSpace( errorMessage )
            ? FindResource( "Editor.FontBrush" ) as Brush
            : FindResource( "Editor.RedBrush"  ) as Brush;

        MessageTextBlock.Text = errorMessage;

        return string.IsNullOrWhiteSpace( errorMessage );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnFolderNameTextBoxTextChanged
    //-----------------------------------------------------------------------------------------------------------------
    private void OnFolderNameTextBoxTextChanged( object sender, TextChangedEventArgs e )
    {
        Validate();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnCreateButtonClicked
    //-----------------------------------------------------------------------------------------------------------------
    private void OnCreateButtonClicked( object sender, RoutedEventArgs e )
    {
        if ( !Validate() ) return;

        DialogResult = true;
        Close();
    }
}
