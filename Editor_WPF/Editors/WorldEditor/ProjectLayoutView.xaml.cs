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
    /// OnAddActorButtonClicked
    //-----------------------------------------------------------------------------------------------------------------
    private void OnAddActorButtonClicked( object sender, System.Windows.RoutedEventArgs e )
    {
        Button? button = sender as Button;
        World? world = button?.DataContext as World;
        
        world?.AddActorCommand?.Execute( new Actor( world ) { Name = "Empty Actor" } );     
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnActorsSelectionChanged
    //-----------------------------------------------------------------------------------------------------------------
    private void OnActorsSelectionChanged( object sender, SelectionChangedEventArgs e )
    {
        ListBox? listBox = sender as ListBox;
        
        List< Actor >? newSelection = listBox?.SelectedItems.Cast< Actor >().ToList();
        List< Actor > previousSelection = (newSelection ?? throw new InvalidOperationException())
            .Except( e.AddedItems.Cast< Actor >() ).Concat( e.RemovedItems.Cast< Actor >() ).ToList();

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

        MultiSelectionActor multiSelectionActor = null;
        if ( newSelection.Any() )
        {
            multiSelectionActor = new MultiSelectionGameActor( newSelection );
        }
        
        ActorView.Instance.DataContext = multiSelectionActor;
    }
}