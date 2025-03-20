using System.Windows;


namespace Editor_WPF.GameProject;


//---------------------------------------------------------------------------------------------------------------------
/// CreateProjectView
//---------------------------------------------------------------------------------------------------------------------
public partial class CreateProjectView
{
    public CreateProjectView()
    {
        InitializeComponent();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnCreateButtonClick
    //-----------------------------------------------------------------------------------------------------------------
    private void OnCreateButtonClick( object sender, RoutedEventArgs e )
    {
        if ( DataContext is not CreateProject vm ) return;
        
        string projectPath = vm.CreateNewProject( templateListBox.SelectedItem as ProjectTemplate ?? throw new InvalidOperationException() );
        
        bool dialogResult = false;
        Window? win = Window.GetWindow( this );
        
        if( !string.IsNullOrEmpty( projectPath ) )
        {
            dialogResult = true;
            
            Project project = OpenProject.Open( new ProjectData() { ProjectName = vm.ProjectName, ProjectPath = projectPath } );
            if ( win != null ) win.DataContext = project;
        }

        if ( win == null ) return;
        
        win.DialogResult = dialogResult;
        win.Close();
    }
}