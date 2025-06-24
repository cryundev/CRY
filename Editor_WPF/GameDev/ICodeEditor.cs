using Editor_WPF.GameProject;

namespace Editor_WPF.GameDev;

//---------------------------------------------------------------------------------------------------------------------
/// CodeEditorType
//---------------------------------------------------------------------------------------------------------------------
public enum CodeEditorType
{
    VisualStudio,
    Rider
}

//---------------------------------------------------------------------------------------------------------------------
/// ICodeEditor Interface
//---------------------------------------------------------------------------------------------------------------------
public interface ICodeEditor
{
    bool BuildSucceeded { get; }
    bool BuildDone { get; }

    void OpenEditor( string solutionPath );
    void CloseEditor();
    bool AddFilesToSolution( string solution, string projectName, string[] files );
    void BuildSolution( Project project, string configName, bool showWindow = true );
    bool IsDebugging();
}

//---------------------------------------------------------------------------------------------------------------------
/// CodeEditorFactory
//---------------------------------------------------------------------------------------------------------------------
public static class CodeEditorFactory
{
    public static ICodeEditor CreateEditor( CodeEditorType editorType )
    {
        return editorType switch
        {
            CodeEditorType.VisualStudio => new VisualStudioEditor(),
            CodeEditorType.Rider => new RiderEditor(),
            _ => throw new ArgumentException( $"Unsupported editor type: {editorType}" )
        };
    }
}