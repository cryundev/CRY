using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;


namespace Editor_WPF.GameProject;


//---------------------------------------------------------------------------------------------------------------------
/// OpenProjectView
//---------------------------------------------------------------------------------------------------------------------
public partial class OpenProjectView
{
    //-----------------------------------------------------------------------------------------------------------------
    /// OpenProjectView
    //-----------------------------------------------------------------------------------------------------------------
    public OpenProjectView()
    {
        InitializeComponent();

        Loaded += ( s, e ) =>
        {
            ListBoxItem? item = projectsListBox.ItemContainerGenerator.ContainerFromIndex( projectsListBox.SelectedIndex ) as ListBoxItem;
            item?.Focus();
        };
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnOpenButtonClicked
    //-----------------------------------------------------------------------------------------------------------------
    private void OnOpenButtonClicked( object sender, RoutedEventArgs e )
    {
        OpenSelectedProject();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnListBoxItemDoubleClicked
    //-----------------------------------------------------------------------------------------------------------------
    private void OnListBoxItemDoubleClicked( object sender, MouseButtonEventArgs e )
    {
        OpenSelectedProject();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OpenSelectedProject
    //-----------------------------------------------------------------------------------------------------------------
    private void OpenSelectedProject() 
    {
        Project project = GameProject.OpenProject.Open( projectsListBox.SelectedItem as ProjectData ?? throw new InvalidOperationException() );
        
        bool dialogResult = false;

        Window? win = Window.GetWindow( this );

        dialogResult = true;
        
        if ( win == null ) return;
        
        win.DataContext  = project;
        win.DialogResult = dialogResult;
        win.Close();
    }
}