using System.Windows.Controls;
using Editor_WPF.Components;
using Editor_WPF.GameProject;
using Editor_WPF.Utilities;


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
        GameEntityView.Instance.DataContext = null;
        ListBox? listBox = sender as ListBox;
        
        if ( e.AddedItems.Count > 0 )
        {
            object? entity = listBox?.SelectedItems[ 0 ];
            GameEntityView.Instance.DataContext = entity;
        }
        
        List< GameEntity >? newSelection = listBox?.SelectedItems.Cast< GameEntity >().ToList();
        List< GameEntity > previousSelection = (newSelection ?? throw new InvalidOperationException())
            .Except( e.AddedItems.Cast< GameEntity >() ).Concat( e.RemovedItems.Cast< GameEntity >() ).ToList();

        Project.UndoRedo.Add( new UndoRedoAction
        (
            () =>
            {
                listBox?.UnselectAll();
                previousSelection?.ForEach( x => ( (listBox?.ItemContainerGenerator.ContainerFromItem( x ) as ListBoxItem)! ).IsSelected = true );
            },
            () => 
            {
                listBox?.UnselectAll();
                newSelection?.ForEach( x => ( (listBox?.ItemContainerGenerator.ContainerFromItem( x ) as ListBoxItem)! ).IsSelected = true ); 
            },
            "Selection Changed"
        ) );
    }
}