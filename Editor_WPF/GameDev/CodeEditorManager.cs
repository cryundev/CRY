using System.Windows.Input;
using Editor_WPF.GameProject;

namespace Editor_WPF.GameDev;

//---------------------------------------------------------------------------------------------------------------------
/// CodeEditorManager
//---------------------------------------------------------------------------------------------------------------------
public static class CodeEditorManager
{
    private static ICodeEditor? _currentEditor;
    private static CodeEditorType _currentEditorType = CodeEditorType.VisualStudio;
    
    public static event EventHandler< bool >? BuildCompleted;
    
    static CodeEditorManager()
    {
        _ = CurrentEditor;
    }

    public static CodeEditorType CurrentEditorType
    {
        get => _currentEditorType;
        set
        {
            if ( _currentEditorType != value )
            {
                if ( _currentEditor != null )
                {
                    _currentEditor.BuildCompleted -= OnEditorBuildCompleted;
                }
                _currentEditorType = value;
                _currentEditor = null;
            }
        }
    }

    public static ICodeEditor CurrentEditor
    {
        get
        {
            if ( _currentEditor == null )
            {
                _currentEditor = CodeEditorFactory.CreateEditor( _currentEditorType );
                _currentEditor.BuildCompleted += OnEditorBuildCompleted;
            }
            return _currentEditor;
        }
    }
    
    private static void OnEditorBuildCompleted( object? sender, bool success )
    {
        BuildCompleted?.Invoke( sender, success );
    }

    public static bool BuildSucceeded => CurrentEditor.BuildSucceeded;
    public static bool BuildDone => CurrentEditor.BuildDone;

    public static void OpenEditor( string solutionPath ) => CurrentEditor.OpenEditor( solutionPath );
    public static void CloseEditor() => CurrentEditor.CloseEditor();
    public static bool AddFilesToSolution( string solution, string projectName, string[] files ) => 
        CurrentEditor.AddFilesToSolution( solution, projectName, files );
    public static void BuildSolution( Project project, string configName, bool showWindow = true )
    {
        CurrentEditor.BuildSolution( project, configName, showWindow );
        CommandManager.InvalidateRequerySuggested();
    }
    public static bool IsDebugging() => CurrentEditor.IsDebugging();
}