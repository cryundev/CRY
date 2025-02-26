using System.Windows.Controls;
using Editor_WPF.Components;
using Editor_WPF.GameProject;


namespace Editor_WPF.Editors;


//---------------------------------------------------------------------------------------------------------------------
/// ProjectLayoutView
//---------------------------------------------------------------------------------------------------------------------
public partial class ProjectLayoutView : UserControl
{

    //-----------------------------------------------------------------------------------------------------------------
    /// ProjectLayoutView
    //-----------------------------------------------------------------------------------------------------------------
    public ProjectLayoutView()
    {
        InitializeComponent();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnAddGameEntitiyButtonClicked
    //-----------------------------------------------------------------------------------------------------------------
    private void OnAddGameEntitiyButtonClicked( object sender, System.Windows.RoutedEventArgs e )
    {
        Button? button = sender as Button;
        Scene? scene = button?.DataContext as Scene;
        
        scene?.AddGameEntityCommand?.Execute( new GameEntity( scene ) { Name = "Empty Game Entity" } );     
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnGameEntitiesSelectionChanged
    //-----------------------------------------------------------------------------------------------------------------
    private void OnGameEntitiesSelectionChanged( object sender, SelectionChangedEventArgs e )
    {
        object? entity = ( sender as ListBox )?.SelectedItems[ 0 ];

        GameEntityView.Instance.DataContext = entity;
    }
}