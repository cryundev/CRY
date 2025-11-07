using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Input;
using Editor_WPF.Components;
using Editor_WPF.GameProject;
using Editor_WPF.Objects;
using Editor_WPF.Utilities;


namespace Editor_WPF.Editors;


//---------------------------------------------------------------------------------------------------------------------
/// ActorView
//---------------------------------------------------------------------------------------------------------------------
public partial class ActorView : UserControl
{
    private Action? _undoAction;
    private string? _propertyName;
    
    public static ActorView Instance { get; private set; } = null!;

    //-----------------------------------------------------------------------------------------------------------------
    /// ActorView
    //-----------------------------------------------------------------------------------------------------------------
    public ActorView()
    {
        InitializeComponent();

        DataContext = null;
        Instance    = this;

        DataContextChanged += ( _, __ ) =>
        {
            if ( DataContext != null )
            {
                MultiSelectionActor? actor = DataContext as MultiSelectionActor;
                if ( actor == null ) return;
                
                actor.PropertyChanged += ( s, e ) =>
                {
                    if ( e.PropertyName == null ) return;
                    
                    _propertyName = e.PropertyName;
                };
            }
        };
    }


    //-----------------------------------------------------------------------------------------------------------------
    /// GetRenameAction
    //-----------------------------------------------------------------------------------------------------------------
    private Action GetRenameAction()
    {
        MultiSelectionActor? vm = DataContext as MultiSelectionActor;
        if ( vm == null ) return new Action( () => {} );

        List< (CrActor actor, string Name) > selection = vm.SelectedActors.Select( actor => ( actor, actor.Name ) ).ToList();
        
        return new Action( () =>
        {
            selection.ForEach( item => item.actor.Name = item.Name );
            
            MultiSelectionActor? actor = DataContext as MultiSelectionActor;
            actor?.Refresh();
        } );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetIsEnabledAction
    //-----------------------------------------------------------------------------------------------------------------
    private Action GetIsEnabledAction()
    {
        MultiSelectionActor? vm = DataContext as MultiSelectionActor;
        if ( vm == null ) return new Action( () => { } );

        List< (CrActor actor, bool IsEnabled) > selection = vm.SelectedActors.Select( actor => ( actor, actor.IsEnabled ) ).ToList();

        return new Action( () =>
        {
            selection.ForEach( item => item.actor.IsEnabled = item.IsEnabled );

            MultiSelectionActor? actor = DataContext as MultiSelectionActor;
            actor?.Refresh();
        } );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnNameTextBoxKeyboardFocusGot
    //-----------------------------------------------------------------------------------------------------------------
    private void OnNameTextBoxKeyboardFocusGot( object sender, KeyboardFocusChangedEventArgs e )
    {
        _propertyName = string.Empty;
        _undoAction = GetRenameAction();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnNameTextBoxKeyboardFocusLost
    //-----------------------------------------------------------------------------------------------------------------
    private void OnNameTextBoxKeyboardFocusLost( object sender, KeyboardFocusChangedEventArgs e )
    {
        if ( _propertyName == nameof( MultiSelectionActor.Name ) && _undoAction != null )
        {
            ProjectViewModel.UndoRedo.Add( new UndoRedoAction( _undoAction, GetRenameAction(), "Rename actor" ) );

            _propertyName = null;
        }

        _undoAction = null;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnIsEnabledCheckBoxClicked
    //-----------------------------------------------------------------------------------------------------------------
    private void OnIsEnabledCheckBoxClicked( object sender, RoutedEventArgs e )
    {
        Action undoAction = GetIsEnabledAction();
        MultiSelectionActor? vm = DataContext as MultiSelectionActor;
        if ( vm == null ) return;
        
        vm.IsEnabled = ( sender as CheckBox )?.IsChecked == true;

        Action redoAction = GetIsEnabledAction();
        ProjectViewModel.UndoRedo.Add( new UndoRedoAction( undoAction, redoAction, vm.IsEnabled == true ? "Enable actor" : "Disable actor" ) );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// AddComponent
    //-----------------------------------------------------------------------------------------------------------------
    private void AddComponent( ComponentType componentType, object data )
    {
        Func< CrActor, object, CrComponent > creationFuction = CrComponentFactory.GetCreationFunction( componentType );
        
        List< (CrActor actor, CrComponent compnent) > changedActors = new List< ( CrActor actor, CrComponent compnent ) >();
        
        MultiSelectionActor? vm = DataContext as MultiSelectionActor;

        foreach ( CrActor actor in vm.SelectedActors )
        {
            var component = creationFuction( actor, data );
            if ( actor.AddComponent( component ) )
            {
                changedActors.Add( ( actor, component ) );
            }
        }

        if ( changedActors.Any() )
        {
            vm.Refresh();

            ProjectViewModel.UndoRedo.Add( new UndoRedoAction
            (
                () =>
                {
                    changedActors.ForEach( x => x.actor.RemoveComponent( x.compnent ) );
                    (DataContext as MultiSelectionActor).Refresh();
                },
                () =>
                {
                    changedActors.ForEach( x => x.actor.AddComponent( x.compnent ) );
                    (DataContext as MultiSelectionActor).Refresh();
                },
                $"Add {componentType} component"
            ) );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnAddScriptComponent
    //-----------------------------------------------------------------------------------------------------------------
    private void OnAddScriptComponent( object sender, RoutedEventArgs e )
    {
        AddComponent( ComponentType.Script, (sender as MenuItem).Header.ToString() );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnAddComponentPreviewMouseLeftButtonDowned
    //-----------------------------------------------------------------------------------------------------------------
    private void OnAddComponentPreviewMouseLeftButtonDowned( object sender, MouseButtonEventArgs e )
    {
        ContextMenu? menu = FindResource( "addComponentMenu" ) as ContextMenu;
        ToggleButton? button = sender as ToggleButton;
        
        button.IsChecked = true;
        
        menu.Placement = PlacementMode.Bottom;
        menu.PlacementTarget = button;
        menu.MinWidth = button.ActualWidth;
        menu.IsOpen = true;
    }
}