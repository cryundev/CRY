using System.Windows;
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
    // Stores the selected actors for a specific actor list.
    private sealed record ActorSelection( ListBox ListBox, List< CrActorViewModel > SelectedActors );

    private readonly Dictionary< WorldViewModel, ListBox > _actorListsByWorld = [];
    private bool _suppressSelectionUndo;

    public static ProjectLayoutView? Instance { get; private set; }

    //-----------------------------------------------------------------------------------------------------------------
    /// ProjectLayoutView
    //-----------------------------------------------------------------------------------------------------------------
    public ProjectLayoutView()
    {
        InitializeComponent();
        Instance = this;
        Unloaded += ( _, _ ) => { if ( ReferenceEquals( Instance, this ) ) Instance = null; };
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// SelectActorFromViewport
    //-----------------------------------------------------------------------------------------------------------------
    public void SelectActorFromViewport( long actorId )
    {
        WorldViewModel? activeWorld = ProjectViewModel.Current?.ActiveWorld;
        if ( activeWorld == null ) return;
        if ( !_actorListsByWorld.TryGetValue( activeWorld, out ListBox? activeWorldListBox ) ) return;

        CrActorViewModel? actor = ID.IsValid( actorId ) ? activeWorld.Actors.FirstOrDefault( x => x.ActorId == actorId ) : null;
        if ( _IsSelectionAlreadySynced( activeWorldListBox, actor ) ) return;

        List< ActorSelection > previousSelection = _CaptureSelectionState();
        List< ActorSelection > nextSelection     = _CreateViewportSelectionState( activeWorldListBox, actor );

        _ApplySelectionState( nextSelection );
        _AddSelectionUndo( previousSelection, nextSelection );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _CaptureSelectionState
    //-----------------------------------------------------------------------------------------------------------------
    private List< ActorSelection > _CaptureSelectionState()
    {
        return _GetRegisteredActorLists()
            .Select( listBox => new ActorSelection( listBox, listBox.SelectedItems.Cast< CrActorViewModel >().ToList() ) )
            .ToList();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _CreateViewportSelectionState
    //-----------------------------------------------------------------------------------------------------------------
    private List< ActorSelection > _CreateViewportSelectionState
    (
        ListBox activeWorldListBox,
        CrActorViewModel? actor
    )
    {
        return _GetRegisteredActorLists()
            .Select( listBox => new ActorSelection
            (
                listBox,
                ReferenceEquals( listBox, activeWorldListBox ) && actor != null ? [ actor ] : []
            ) )
            .ToList();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _ApplySelectionState
    //-----------------------------------------------------------------------------------------------------------------
    private void _ApplySelectionState( List< ActorSelection > selectionState )
    {
        _suppressSelectionUndo = true;

        try
        {
            foreach ( ActorSelection selection in selectionState )
            {
                selection.ListBox.UnselectAll();
            }

            foreach ( ActorSelection selection in selectionState )
            {
                if ( selection.SelectedActors.Count == 1 )
                {
                    selection.ListBox.SelectedItem = selection.SelectedActors[0];
                    selection.ListBox.ScrollIntoView( selection.SelectedActors[0] );
                    continue;
                }

                foreach ( CrActorViewModel selectedActor in selection.SelectedActors )
                {
                    selection.ListBox.ScrollIntoView( selectedActor );
                    selection.ListBox.UpdateLayout();

                    if ( selection.ListBox.ItemContainerGenerator.ContainerFromItem( selectedActor ) is ListBoxItem item )
                    {
                        item.IsSelected = true;
                    }
                }
            }
        }
        finally
        {
            _suppressSelectionUndo = false;
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _IsSelectionAlreadySynced
    //-----------------------------------------------------------------------------------------------------------------
    private static bool _IsSelectionAlreadySynced( ListBox listBox, CrActorViewModel? actor )
    {
        if ( actor == null )
        {
            return listBox.SelectedItems.Count == 0;
        }

        return listBox.SelectedItems.Count == 1 && ReferenceEquals( listBox.SelectedItem, actor );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _GetRegisteredActorLists
    //-----------------------------------------------------------------------------------------------------------------
    private IReadOnlyList< ListBox > _GetRegisteredActorLists()
    {
        return _actorListsByWorld.Values.Distinct().ToList();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnActorListLoaded
    //-----------------------------------------------------------------------------------------------------------------
    private void OnActorListLoaded( object sender, RoutedEventArgs e )
    {
        if ( sender is ListBox { DataContext: WorldViewModel world } listBox )
        {
            _actorListsByWorld[world] = listBox;
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnActorListUnloaded
    //-----------------------------------------------------------------------------------------------------------------
    private void OnActorListUnloaded( object sender, RoutedEventArgs e )
    {
        if ( sender is not ListBox { DataContext: WorldViewModel world } listBox ) return;

        bool hasRegisteredList = _actorListsByWorld.TryGetValue( world, out ListBox? registeredListBox );
        if ( !hasRegisteredList ) return;

        bool isSameListInstance = ReferenceEquals( registeredListBox, listBox );
        if ( !isSameListInstance ) return;

        _actorListsByWorld.Remove( world );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _TryGetSelectionDelta
    //-----------------------------------------------------------------------------------------------------------------
    private static bool _TryGetSelectionDelta( ListBox? listBox, SelectionChangedEventArgs e, out List< CrActorViewModel > previousSelection, out List< CrActorViewModel > newSelection )
    {
        if ( listBox == null )
        {
            previousSelection = [];
            newSelection = [];
            return false;
        }

        newSelection = listBox.SelectedItems.Cast< CrActorViewModel >().ToList();
        previousSelection = newSelection
            .Except( e.AddedItems.Cast< CrActorViewModel >() )
            .Concat( e.RemovedItems.Cast< CrActorViewModel >() )
            .ToList();

        return true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _UpdateSelectedActorView
    //-----------------------------------------------------------------------------------------------------------------
    private static void _UpdateSelectedActorView( List< CrActorViewModel > newSelection )
    {
        ActorView.Instance.DataContext = newSelection.Count > 0 ? new MultiSelectionGameActor( newSelection ) : null;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// _AddSelectionUndo
    //-----------------------------------------------------------------------------------------------------------------
    private void _AddSelectionUndo( List< ActorSelection > previousSelection, List< ActorSelection > nextSelection )
    {
        ProjectViewModel.UndoRedo.Add( new UndoRedoAction
        (
            () => _ApplySelectionState( previousSelection ),
            () => _ApplySelectionState( nextSelection ),
            "Selection Changed"
        ) );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnAddActorButtonClicked
    //-----------------------------------------------------------------------------------------------------------------
    private void OnAddActorButtonClicked( object sender, System.Windows.RoutedEventArgs e )
    {
        Button? button = sender as Button;
        WorldViewModel? world = button?.DataContext as WorldViewModel;

        world?.AddActorCommand?.Execute( new CrActorViewModel( world ) { Name = "Empty Actor" } );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnActorsSelectionChanged
    //-----------------------------------------------------------------------------------------------------------------
    private void OnActorsSelectionChanged( object sender, SelectionChangedEventArgs e )
    {
        ListBox? listBox = sender as ListBox;
        if ( !_TryGetSelectionDelta( listBox, e, out List< CrActorViewModel > previousSelection, out List< CrActorViewModel > newSelection ) ) return;

        if ( !_suppressSelectionUndo && listBox != null )
        {
            List< ActorSelection > prevState = [ new ActorSelection( listBox, previousSelection ) ];
            List< ActorSelection > nextState = [ new ActorSelection( listBox, newSelection ) ];
            _AddSelectionUndo( prevState, nextState );
        }

        _UpdateSelectedActorView( newSelection );
    }
}
