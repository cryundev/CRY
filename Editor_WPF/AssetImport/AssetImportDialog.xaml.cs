using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using Editor_WPF.AssetBrowser;
using Editor_WPF.DllWrappers;
using Editor_WPF.GameProject;
using Editor_WPF.Utilities;
using Microsoft.Win32;
using Window = System.Windows.Window;


namespace Editor_WPF.AssetImport;


internal enum TextureSizeMode
{
    Original,
    NearestPowerOfTwo,
    FitDownPowerOfTwo,
    FitUpPowerOfTwo,
    CustomPowerOfTwo,
}


internal sealed class TextureSizeModeOption
{
    public TextureSizeMode Mode        { get; }
    public string          DisplayName { get; }

    public TextureSizeModeOption( TextureSizeMode mode, string displayName )
    {
        Mode        = mode;
        DisplayName = displayName;
    }

    public override string ToString()
    {
        return DisplayName;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/// AssetImportDialog
//---------------------------------------------------------------------------------------------------------------------
public partial class AssetImportDialog : Window
{
    private static readonly int MinImportSize = EngineAPI.Asset.TextureImportMinSize;
    private static readonly int MaxImportSize = EngineAPI.Asset.TextureImportMaxSize;

    private static readonly TextureSizeModeOption[] TextureSizeModes =
    {
        new TextureSizeModeOption( TextureSizeMode.Original,          "Original"                 ),
        new TextureSizeModeOption( TextureSizeMode.NearestPowerOfTwo, "Nearest Power Of Two"     ),
        new TextureSizeModeOption( TextureSizeMode.FitDownPowerOfTwo, "Fit Down To Power Of Two" ),
        new TextureSizeModeOption( TextureSizeMode.FitUpPowerOfTwo,   "Fit Up To Power Of Two"   ),
        new TextureSizeModeOption( TextureSizeMode.CustomPowerOfTwo,  "Custom Power Of Two"      ),
    };

    private readonly string _sourcePath;

    private readonly EngineAPI.Asset.ImportType _importType;

    private int _sourceWidth;
    private int _sourceHeight;

    private bool _isUpdatingUI;

    //-----------------------------------------------------------------------------------------------------------------
    /// TryGetImportType
    //-----------------------------------------------------------------------------------------------------------------
    internal static bool TryGetImportType( string sourcePath, out EngineAPI.Asset.ImportType importType )
    {
        importType = EngineAPI.Asset.GetImportType( sourcePath );

        return importType != EngineAPI.Asset.ImportType.Auto;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetProjectRelativePath
    //-----------------------------------------------------------------------------------------------------------------
    private static string GetProjectRelativePath( string path )
    {
        string? projectPath = ProjectViewModel.Current?.Path;
        if ( string.IsNullOrWhiteSpace( projectPath ) ) return path;

        string relativePath = Path.GetRelativePath( projectPath, path );

        return relativePath.StartsWith( ".." ) ? path : relativePath;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetFullDestinationPath
    //-----------------------------------------------------------------------------------------------------------------
    private static string GetFullDestinationPath( string displayedDestinationPath )
    {
        if ( Path.IsPathRooted( displayedDestinationPath ) ) return Path.GetFullPath( displayedDestinationPath );

        string? projectPath = ProjectViewModel.Current?.Path;

        return Path.GetFullPath( Path.Combine( projectPath ?? string.Empty, displayedDestinationPath ) );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// TryGetFullDestinationPath
    //-----------------------------------------------------------------------------------------------------------------
    private static bool TryGetFullDestinationPath( string displayedDestinationPath, out string fullPath, out string errorMessage )
    {
        string path = Path.IsPathRooted( displayedDestinationPath )
            ? displayedDestinationPath
            : Path.Combine( ProjectViewModel.Current?.Path ?? string.Empty, displayedDestinationPath );

        PathValidation.FullPathRequest request = new PathValidation.FullPathRequest()
        {
            Value                        = path,
            EmptyErrorMessage            = "Select a destination path.",
            InvalidCharacterErrorMessage = "Destination path contains invalid characters.",
        };

        return PathValidation.TryGetRequiredFullPath( request, out fullPath, out errorMessage );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetDefaultDestinationPath
    //-----------------------------------------------------------------------------------------------------------------
    private static string GetDefaultDestinationPath( string sourcePath )
    {
        string assetName = $"{Path.GetFileNameWithoutExtension( sourcePath )}{EngineAPI.Asset.CassetExtension}";
        string? projectPath = ProjectViewModel.Current?.Path;

        if ( !string.IsNullOrWhiteSpace( projectPath ) )
        {
            return Path.Combine( "Content", assetName );
        }

        string? sourceDirectory = Path.GetDirectoryName( sourcePath );

        return string.IsNullOrWhiteSpace( sourceDirectory ) ? assetName : Path.GetFullPath( Path.Combine( sourceDirectory, assetName ) );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetPowerOfTwoFloor
    //-----------------------------------------------------------------------------------------------------------------
    private static int GetPowerOfTwoFloor( int value )
    {
        int result = MinImportSize;
        while ( result * 2 <= value && result * 2 <= MaxImportSize )
        {
            result *= 2;
        }

        return result;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetPowerOfTwoCeiling
    //-----------------------------------------------------------------------------------------------------------------
    private static int GetPowerOfTwoCeiling( int value )
    {
        int result = MinImportSize;
        while ( result < value && result < MaxImportSize )
        {
            result *= 2;
        }

        return result;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetNearestPowerOfTwo
    //-----------------------------------------------------------------------------------------------------------------
    private static int GetNearestPowerOfTwo( int value )
    {
        int floor = GetPowerOfTwoFloor( value );
        int ceil  = GetPowerOfTwoCeiling( value );

        return value - floor <= ceil - value ? floor : ceil;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetTextureColorSpace
    //-----------------------------------------------------------------------------------------------------------------
    private int GetTextureColorSpace()
    {
        if ( TextureColorSpaceComboBox.SelectedItem is EngineAPI.Asset.TextureImportOption option )
        {
            return option.Value;
        }

        return 0;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetTextureCompression
    //-----------------------------------------------------------------------------------------------------------------
    private int GetTextureCompression()
    {
        if ( TextureCompressionComboBox.SelectedItem is EngineAPI.Asset.TextureImportOption option )
        {
            return option.Value;
        }

        return 0;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetTextureSizeMode
    //-----------------------------------------------------------------------------------------------------------------
    private TextureSizeMode GetTextureSizeMode()
    {
        if ( TextureSizeModeComboBox.SelectedItem is TextureSizeModeOption option )
        {
            return option.Mode;
        }

        return TextureSizeMode.Original;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetSelectedTextureSize
    //-----------------------------------------------------------------------------------------------------------------
    private (int Width, int Height) GetSelectedTextureSize()
    {
        if ( _importType != EngineAPI.Asset.ImportType.Texture ) return (0, 0);

        TextureSizeMode mode = GetTextureSizeMode();
        return mode switch
        {
            TextureSizeMode.NearestPowerOfTwo => (GetNearestPowerOfTwo( _sourceWidth ), GetNearestPowerOfTwo( _sourceHeight )),
            TextureSizeMode.FitDownPowerOfTwo => (GetPowerOfTwoFloor  ( _sourceWidth ), GetPowerOfTwoFloor  ( _sourceHeight )),
            TextureSizeMode.FitUpPowerOfTwo   => (GetPowerOfTwoCeiling( _sourceWidth ), GetPowerOfTwoCeiling( _sourceHeight )),

            TextureSizeMode.CustomPowerOfTwo  => (TextureWidthComboBox.SelectedItem is int width ? width : _sourceWidth, TextureHeightComboBox.SelectedItem is int height ? height : _sourceHeight),
            _ => (_sourceWidth, _sourceHeight),
        };
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// SetMessage
    //-----------------------------------------------------------------------------------------------------------------
    private void SetMessage( string message, Brush brush )
    {
        MessageTextBlock.Foreground = brush;
        MessageTextBlock.Text       = message;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// InitializeImportKindUI
    //-----------------------------------------------------------------------------------------------------------------
    private void InitializeImportKindUI()
    {
        bool isTextureImport = _importType == EngineAPI.Asset.ImportType.Texture;

        Title = isTextureImport ? "Texture Import" : "Primitive Import";
        DialogTitleTextBlock.Text       = isTextureImport ? "Texture Import" : "Primitive Import";
        DialogDescriptionTextBlock.Text = isTextureImport ? "Review source metadata and configure output settings before import." : "Create a CRY primitive asset from a primitive source.";
        AssetTypeTextBlock.Text         = isTextureImport ? "Texture" : "Primitive";

        TextureOptionsPanel.Visibility   = isTextureImport ? Visibility.Visible   : Visibility.Collapsed;
        PreviewImage.Visibility          = isTextureImport ? Visibility.Visible   : Visibility.Collapsed;
        PrimitiveOptionsPanel.Visibility = isTextureImport ? Visibility.Collapsed : Visibility.Visible;
        PrimitivePreviewPanel.Visibility = isTextureImport ? Visibility.Collapsed : Visibility.Visible;

        PrimitiveExtensionTextBlock.Text = Path.GetExtension( _sourcePath ).ToUpperInvariant();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// InitializeSourceInfo
    //-----------------------------------------------------------------------------------------------------------------
    private void InitializeSourceInfo()
    {
        SourceNameTextBlock.Text = Path.GetFileName( _sourcePath );
        SourcePathTextBlock.Text = _sourcePath;

        FileInfo sourceFile = new FileInfo( _sourcePath );
        string sizeText = sourceFile.Exists ? $"{sourceFile.Length / 1024.0:F1} KB" : "Missing";

        if ( _importType == EngineAPI.Asset.ImportType.Texture && _sourceWidth > 0 && _sourceHeight > 0 )
        {
            SourceDetailTextBlock.Text = $"{_sourceWidth} x {_sourceHeight} | {Path.GetExtension( _sourcePath ).TrimStart( '.' ).ToUpperInvariant()} | {sizeText}";
        }
        else
        {
            SourceDetailTextBlock.Text = $"{Path.GetExtension( _sourcePath ).TrimStart( '.' ).ToUpperInvariant()} | {sizeText}";
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// InitializeTextureControls
    //-----------------------------------------------------------------------------------------------------------------
    private void InitializeTextureControls()
    {
        _isUpdatingUI = true;

        TextureSizeModeComboBox.Items.Clear();
        foreach ( TextureSizeModeOption option in TextureSizeModes )
        {
            TextureSizeModeComboBox.Items.Add( option );
        }

        TextureSizeModeComboBox.SelectedIndex = 0;

        TextureColorSpaceComboBox.Items.Clear();
        foreach ( EngineAPI.Asset.TextureImportOption option in EngineAPI.Asset.GetTextureColorSpaceOptions() )
        {
            TextureColorSpaceComboBox.Items.Add( option );
        }

        TextureColorSpaceComboBox.SelectedIndex = TextureColorSpaceComboBox.Items.Count > 0 ? 0 : -1;

        TextureCompressionComboBox.Items.Clear();
        foreach ( EngineAPI.Asset.TextureImportOption option in EngineAPI.Asset.GetTextureCompressionOptions() )
        {
            TextureCompressionComboBox.Items.Add( option );
        }

        TextureCompressionComboBox.SelectedIndex = TextureCompressionComboBox.Items.Count > 0 ? 0 : -1;

        TextureWidthComboBox.Items.Clear();
        TextureHeightComboBox.Items.Clear();

        for ( int size = MinImportSize; size <= MaxImportSize; size *= 2 )
        {
            TextureWidthComboBox.Items.Add( size );
            TextureHeightComboBox.Items.Add( size );
        }

        TextureWidthComboBox.SelectedItem  = GetNearestPowerOfTwo( _sourceWidth  > 0 ? _sourceWidth  : 1 );
        TextureHeightComboBox.SelectedItem = GetNearestPowerOfTwo( _sourceHeight > 0 ? _sourceHeight : 1 );
        TextureWidthComboBox.IsEnabled     = false;
        TextureHeightComboBox.IsEnabled    = false;

        _isUpdatingUI = false;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// InitializePreview
    //-----------------------------------------------------------------------------------------------------------------
    private void InitializePreview()
    {
        if ( _importType != EngineAPI.Asset.ImportType.Texture ) return;

        try
        {
            BitmapImage image = new BitmapImage();
            image.BeginInit();
            image.CacheOption = BitmapCacheOption.OnLoad;
            image.UriSource   = new Uri( _sourcePath );
            image.EndInit();
            image.Freeze();

            _sourceWidth  = image.PixelWidth;
            _sourceHeight = image.PixelHeight;

            PreviewImage.Source = image;
        }
        catch
        {
            PreviewImage.Visibility = Visibility.Collapsed;
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// UpdateTextureControlState
    //-----------------------------------------------------------------------------------------------------------------
    private void UpdateTextureControlState()
    {
        if ( _importType != EngineAPI.Asset.ImportType.Texture ) return;

        TextureSizeMode mode = GetTextureSizeMode();
        bool isCustomSize = mode == TextureSizeMode.CustomPowerOfTwo;

        TextureWidthComboBox.IsEnabled  = isCustomSize;
        TextureHeightComboBox.IsEnabled = isCustomSize;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// UpdateImportSummary
    //-----------------------------------------------------------------------------------------------------------------
    private void UpdateImportSummary()
    {
        if ( _importType == EngineAPI.Asset.ImportType.Texture )
        {
            (int width, int height) = GetSelectedTextureSize();

            string compression = TextureCompressionComboBox.SelectedItem?.ToString() ?? "No Compression Option";
            string colorSpace  = TextureColorSpaceComboBox.SelectedItem?.ToString() ?? "Linear";
            string mipmaps     = GenerateMipmapsCheckBox.IsChecked == true ? "Generate mipmaps" : "Single mip";

            OutputSummaryTextBlock.Text = $"{width} x {height} | {colorSpace} | {compression} | {mipmaps}";
        }
        else
        {
            OutputSummaryTextBlock.Text = "Primitive asset will be validated and saved into the project Content path.";
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// ValidateImportRequest
    //-----------------------------------------------------------------------------------------------------------------
    private bool ValidateImportRequest()
    {
        string destinationPath = DestinationPathTextBox.Text.Trim();

        string errorMessage = string.Empty;

        if ( !File.Exists( _sourcePath ) )
        {
            errorMessage = "Source asset does not exist.";
        }
        else if ( string.IsNullOrWhiteSpace( destinationPath ) )
        {
            errorMessage = "Select a destination path.";
        }
        else if ( !TryGetFullDestinationPath( destinationPath, out _, out errorMessage ) )
        {
        }
        else if ( _importType == EngineAPI.Asset.ImportType.Texture )
        {
            int compression = GetTextureCompression();
            if ( !EngineAPI.Asset.IsTextureCompressionSupported( compression ) )
            {
                errorMessage = EngineAPI.Asset.GetTextureCompressionUnsupportedReason( compression );
            }
            else if ( GenerateMipmapsCheckBox.IsChecked == true && !EngineAPI.Asset.IsTextureMipmapGenerationSupported )
            {
                errorMessage = EngineAPI.Asset.TextureMipmapGenerationUnsupportedReason;
            }
        }

        if ( string.IsNullOrWhiteSpace( errorMessage ) )
        {
            SetMessage( string.Empty, FindResource( "Editor.Disable.FontBrush" ) as Brush ?? Brushes.Gray );
            ImportButton.IsEnabled = true;
            return true;
        }

        SetMessage( errorMessage, FindResource( "Editor.OrangeBrush" ) as Brush ?? Brushes.Orange );
        ImportButton.IsEnabled = false;
        return false;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnBrowseDestinationButtonClicked
    //-----------------------------------------------------------------------------------------------------------------
    private void OnBrowseDestinationButtonClicked( object sender, RoutedEventArgs e )
    {
        SaveFileDialog dialog = new SaveFileDialog()
        {
            AddExtension     = true,
            DefaultExt       = EngineAPI.Asset.CassetExtension,
            Filter           = EngineAPI.Asset.SaveFileFilter,
            InitialDirectory = Path.Combine( ProjectViewModel.Current?.Path ?? string.Empty, "Content" ),
            FileName         = Path.GetFileName( DestinationPathTextBox.Text ),
        };

        if ( dialog.ShowDialog( this ) != true ) return;

        DestinationPathTextBox.Text = GetProjectRelativePath( dialog.FileName );
        ValidateImportRequest();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnDestinationPathTextBoxTextChanged
    //-----------------------------------------------------------------------------------------------------------------
    private void OnDestinationPathTextBoxTextChanged( object sender, TextChangedEventArgs e )
    {
        if ( _isUpdatingUI ) return;

        ValidateImportRequest();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnTextureOptionChanged
    //-----------------------------------------------------------------------------------------------------------------
    private void OnTextureOptionChanged( object sender, EventArgs e )
    {
        if ( _isUpdatingUI ) return;

        UpdateTextureControlState();
        UpdateImportSummary();
        ValidateImportRequest();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnImportButtonClicked
    //-----------------------------------------------------------------------------------------------------------------
    private async void OnImportButtonClicked( object sender, RoutedEventArgs e )
    {
        if ( !ValidateImportRequest() ) return;

        IsEnabled = false;

        string destinationPath = GetFullDestinationPath( DestinationPathTextBox.Text.Trim() );

        int textureColorSpace = GetTextureColorSpace();

        int textureCompression = GetTextureCompression();

        (int textureWidth, int textureHeight) = GetSelectedTextureSize();

        bool generateMipmaps = GenerateMipmapsCheckBox.IsChecked == true;

        if ( _importType != EngineAPI.Asset.ImportType.Texture || GetTextureSizeMode() == TextureSizeMode.Original )
        {
            textureWidth  = 0;
            textureHeight = 0;
        }

        try
        {
            (bool succeeded, string message) = await Task.Run( () =>
            {
                bool importSucceeded = EngineAPI.Asset.TryImport
                (
                    _sourcePath,
                    destinationPath,
                    out string importMessage,
                    _importType,
                    textureColorSpace,
                    textureCompression,
                    textureWidth,
                    textureHeight,
                    generateMipmaps
                );

                return (importSucceeded, importMessage);
            } );

            if ( succeeded )
            {
                string successMessage = string.IsNullOrWhiteSpace( message ) ? "Asset import succeeded." : message;
                SetMessage( successMessage, FindResource( "Editor.GreenBrush" ) as Brush ?? Brushes.LightGreen );
                Logger.Log( MessageType.Info, $"{successMessage} {destinationPath}" );
                AssetFolderView.Instance?.Refresh();
            }
            else
            {
                string failureMessage = string.IsNullOrWhiteSpace( message ) ? "Asset import failed." : message;
                SetMessage( failureMessage, FindResource( "Editor.RedBrush" ) as Brush ?? Brushes.Red );
                Logger.Log( MessageType.Error, $"{failureMessage} {_sourcePath}" );
            }
        }
        catch ( Exception exception )
        {
            SetMessage( exception.Message, FindResource( "Editor.RedBrush" ) as Brush ?? Brushes.Red );
            Logger.Log( MessageType.Error, $"Asset import failed. {exception.Message}" );
        }
        finally
        {
            IsEnabled = true;
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// AssetImportDialog
    //-----------------------------------------------------------------------------------------------------------------
    internal AssetImportDialog( string sourcePath, EngineAPI.Asset.ImportType importType )
    {
        _sourcePath = sourcePath;
        _importType = importType;

        InitializeComponent();

        Owner = Application.Current.MainWindow;

        _isUpdatingUI = true;
        DestinationPathTextBox.Text = GetDefaultDestinationPath( _sourcePath );
        _isUpdatingUI = false;

        InitializeImportKindUI();
        InitializePreview();
        InitializeSourceInfo();
        InitializeTextureControls();
        UpdateImportSummary();
        ValidateImportRequest();
    }
}
