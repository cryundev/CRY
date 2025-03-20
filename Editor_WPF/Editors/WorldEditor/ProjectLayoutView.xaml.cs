using System.Windows.Controls;
using Editor_WPF.GameProject;
using Editor_WPF.Objects;
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
        
        world?.AddActorCommand?.Execute( new CrActor( world ) { Name = "Empty Actor" } );     
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnActorsSelectionChanged
    //-----------------------------------------------------------------------------------------------------------------
    private void OnActorsSelectionChanged( object sender, SelectionChangedEventArgs e )
    {
        ListBox? listBox = sender as ListBox;
        
        List< CrActor >? newSelection = listBox?.SelectedItems.Cast< CrActor >().ToList();
        List< CrActor > previousSelection = (newSelection ?? throw new InvalidOperationException())
            .Except( e.AddedItems.Cast< CrActor >() ).Concat( e.RemovedItems.Cast< CrActor >() ).ToList();

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