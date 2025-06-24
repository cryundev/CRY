using System.Diagnostics;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Animation;
using Editor_WPF.Utilities;
using EnvDTE;
using Window = System.Windows.Window;
using Project = Editor_WPF.GameProject.Project;


namespace Editor_WPF.GameDev;


//---------------------------------------------------------------------------------------------------------------------
/// CreateScriptDialog
//---------------------------------------------------------------------------------------------------------------------
public partial class CreateScriptDialog : Window
{
    private static readonly string _cppCode = @"#include ""{0}.h""


REGISTER_SCRIPT( {0} );


//---------------------------------------------------------------------------------------------------------------------
/// Begin play
//---------------------------------------------------------------------------------------------------------------------
void {0}::BeginPlay()
{{

}}

//---------------------------------------------------------------------------------------------------------------------
/// Update
//---------------------------------------------------------------------------------------------------------------------
void {0}::Update( float DeltaSeconds )
{{
 
}}";

    private static readonly string _hCode = @"#pragma once


#include ""Source/Object/CRScript.h""


//---------------------------------------------------------------------------------------------------------------------
/// {0}
//---------------------------------------------------------------------------------------------------------------------
class {0} : public CRScript
{{
public:
    /// Constructor
    explicit {0}( CRObject Object )
    : CRScript( Object )
    {{
    }}

    /// Begin play
    virtual void BeginPlay() override;

    /// Update
    virtual void Update( float DeltaSeconds ) override;
}};";

    private static readonly string _namespace = GetNamespaceFromProjectName();

    
    //-----------------------------------------------------------------------------------------------------------------
    /// GetNamespaceFromProjectName
    //-----------------------------------------------------------------------------------------------------------------
    private static string GetNamespaceFromProjectName()
    {
        string projectName = Project.Current.Name;
        projectName = projectName.Replace( ' ', '_' );

        return projectName;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Validate
    //-----------------------------------------------------------------------------------------------------------------
    bool Validate()
    {
        bool isValid = false;

        string name = ScriptNameTextBox.Text.Trim();
        string path = ScriptPathTextBox.Text.Trim();

        string errorMessage = string.Empty;
        if ( string.IsNullOrEmpty( name ) )
        {
            errorMessage = "Type in a script name.";
        }
        else if ( name.IndexOfAny( Path.GetInvalidFileNameChars() ) != -1 || name.Any( x => char.IsWhiteSpace( x ) ) )
        {
            errorMessage = "Invalid character(s) used in script name.";
        }

        if ( string.IsNullOrEmpty( path ) )
        {
            errorMessage = "Select a valid script folder";
        }
        else if ( path.IndexOfAny( Path.GetInvalidPathChars() ) != -1 )
        {
            errorMessage = "Invalid character(s) used in script path.";
        }
        else if ( !Path.GetFullPath( Path.Combine( Project.Current.Path, path ) ).Contains( Path.Combine( Project.Current.Path, @"Source\" ) ) )
        {
            errorMessage = "Script must be added to ( a sub-folder of ) Source.";
        }
        else if ( File.Exists( Path.GetFullPath( Path.Combine( Path.Combine( Project.Current.Path, path ), $"{name}.cpp" ) ) ) ||
                  File.Exists( Path.GetFullPath( Path.Combine( Path.Combine( Project.Current.Path, path ), $"{name}.h" ) ) ) )
        {
            errorMessage = $"script {name} already exists in this folder.";
        }
        else
        {
            isValid = true;
        }

        if ( !isValid )
        {
            MessageTextBlock.Foreground = FindResource( "Editor.RedBrush" ) as Brush;
        }
        else
        {
            MessageTextBlock.Foreground = FindResource( "Editor.FontBrush" ) as Brush;
        }
        
        MessageTextBlock.Text = errorMessage;

        return isValid;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnCreateButtonClicked
    //-----------------------------------------------------------------------------------------------------------------
    private async void OnCreateButtonClicked( object sender, RoutedEventArgs e )
    {
        if ( !Validate() ) return;

        IsEnabled = false;
        
        DoubleAnimation fadeIn = new DoubleAnimation( 0, 1, new Duration( TimeSpan.FromMilliseconds( 500 ) ) );

        BusyAnimation.Opacity = 0;
        BusyAnimation.Visibility = Visibility.Visible;
        BusyAnimation.BeginAnimation( OpacityProperty, fadeIn );

        try
        {
            string name = ScriptNameTextBox.Text.Trim();
            string path = Path.GetFullPath( Path.Combine( Project.Current.Path, ScriptPathTextBox.Text.Trim() ) );
            string solution = Project.Current.Solution;
            string projectName = Project.Current.Name;

            await Task.Run( () => CreateScript( name, path, solution, projectName ) );
        }
        catch ( Exception exception )
        {
            Debug.WriteLine( exception.Message );
            Logger.Log( MessageType.Error, $"Failed to create script {ScriptNameTextBox.Text}" );
        }
        finally
        {
            DoubleAnimation fadeOut = new DoubleAnimation( 1, 0, new Duration( TimeSpan.FromMilliseconds( 200 ) ) );
            fadeOut.Completed += ( s, e ) =>
            {
                BusyAnimation.Opacity = 0;
                BusyAnimation.Visibility = Visibility.Hidden;

                Close();
            };
            BusyAnimation.BeginAnimation( OpacityProperty, fadeOut );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnScriptNameTextBoxTextChanged
    //-----------------------------------------------------------------------------------------------------------------
    private void OnScriptNameTextBoxTextChanged( object sender, TextChangedEventArgs e )
    {
        if ( !Validate() ) return;

        string name = ScriptNameTextBox.Text.Trim();
        MessageTextBlock.Text = $"{name}.h and {name}.cpp will be added to {Project.Current.Name}";
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnScriptPathTextBoxTextChanged
    //-----------------------------------------------------------------------------------------------------------------
    private void OnScriptPathTextBoxTextChanged( object sender, TextChangedEventArgs e )
    {
        Validate();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// CreateScript
    //-----------------------------------------------------------------------------------------------------------------
    private void CreateScript( string name, string path, string solution, string projectName )
    {
        if ( !Directory.Exists( path ) ) Directory.CreateDirectory( path );
        
        string cpp = Path.GetFullPath( Path.Combine( path, $"{name}.cpp" ) );
        string h   = Path.GetFullPath( Path.Combine( path, $"{name}.h"   ) );

        using ( StreamWriter sw = File.CreateText( cpp ) )
        {
            sw.Write( string.Format( _cppCode, name, _namespace ) );
        }

        using ( StreamWriter sw = File.CreateText( h ) )
        {
            sw.WriteLine( string.Format( _hCode, name ) );
        }

        string[] files = new string[] { cpp, h };

        for ( int i = 0; i < 3; ++i )
        {
            if ( !CodeEditorManager.AddFilesToSolution( solution, projectName, files ) )
            {
                System.Threading.Thread.Sleep( 1000 );
            }
            else
            {
                break;
            }
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// CreateScriptDialog
    //-----------------------------------------------------------------------------------------------------------------
    public CreateScriptDialog()
    {
        InitializeComponent();

        Owner = Application.Current.MainWindow;
    }
}