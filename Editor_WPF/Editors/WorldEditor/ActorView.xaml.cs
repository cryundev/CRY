using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using Editor_WPF.Components;
using Editor_WPF.GameProject;
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

        List< (Actor actor, string Name) > selection = vm.SelectedEntities.Select( actor => ( actor, actor.Name ) ).ToList();
        
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

        List< (Actor actor, bool IsEnabled) > selection = vm.SelectedEntities.Select( actor => ( actor, actor.IsEnabled ) ).ToList();

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
        _undoAction = GetRenameAction();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnNameTextBoxKeyboardFocusLost
    //-----------------------------------------------------------------------------------------------------------------
    private void OnNameTextBoxKeyboardFocusLost( object sender, KeyboardFocusChangedEventArgs e )
    {
        if ( _propertyName == nameof( MultiSelectionActor.Name ) && _undoAction != null )
        {
            Project.UndoRedo.Add( new UndoRedoAction( _undoAction, GetRenameAction(), "Rename actor" ) );
            
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
        Project.UndoRedo.Add( new UndoRedoAction( undoAction, redoAction, vm.IsEnabled == true ? "Enable actor" : "Disable actor" ) );
    }
}