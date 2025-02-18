using System.Windows;
using System.Windows.Controls;


namespace Editor_WPF.GameProject;


public partial class CreateProjectView : UserControl
{
    public CreateProjectView()
    {
        InitializeComponent();
    }
    
    private void OnCreateButtonClick( object sender, RoutedEventArgs e )
    {
        var vm = DataContext as CreateProject;
        string projectPath = vm.CreateNewProject( templateListBox.SelectedItem as ProjectTemplate );
        
        bool dialogResult = false;
        var win = Window.GetWindow( this );
        
        if( !string.IsNullOrEmpty( projectPath ) )
        {
            dialogResult = true;
            
            Project project = OpenProject.Open( new ProjectData() { ProjectName = vm.ProjectName, ProjectPath = projectPath } );
        }
        
        win.DialogResult = dialogResult;
        win.Close();
    }
}