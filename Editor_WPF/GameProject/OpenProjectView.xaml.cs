using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;


namespace Editor_WPF.GameProject;


public partial class OpenProjectView : UserControl
{
    public OpenProjectView()
    {
        InitializeComponent();

        Loaded += ( s, e ) =>
        {
            ListBoxItem? item = projectsListBox.ItemContainerGenerator.ContainerFromIndex( projectsListBox.SelectedIndex ) as ListBoxItem;
            item?.Focus();
        };
    }
    
    private void OnOpenButtonClicked( object sender, RoutedEventArgs e )
    {
        OpenSelectedProject();
    }
    
    private void OnListBoxItemDoubleClicked( object sender, MouseButtonEventArgs e )
    {
        OpenSelectedProject();
    }
    
    private void OpenSelectedProject() 
    {
        Project project = GameProject.OpenProject.Open( projectsListBox.SelectedItem as ProjectData );
        
        bool dialogResult = false;

        Window? win = Window.GetWindow( this );
        
        if ( project != null )
        {
            dialogResult = true;
            win.DataContext = project;
        }

        win.DialogResult = dialogResult;
        win.Close();
    }
}