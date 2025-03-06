using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using Editor_WPF.Components;
using Editor_WPF.GameProject;
using Editor_WPF.Utilities;


namespace Editor_WPF.Editors;


//---------------------------------------------------------------------------------------------------------------------
/// GameEntityView
//---------------------------------------------------------------------------------------------------------------------
public partial class GameEntityView : UserControl
{
    private Action? _undoAction;
    private string? _propertyName;
    
    public static GameEntityView Instance { get; private set; } = null!;

    //-----------------------------------------------------------------------------------------------------------------
    /// GameEntityView
    //-----------------------------------------------------------------------------------------------------------------
    public GameEntityView()
    {
        InitializeComponent();

        DataContext = null;
        Instance    = this;

        DataContextChanged += ( _, __ ) =>
        {
            if ( DataContext != null )
            {
                MultiSelectionEntity? entity = DataContext as MultiSelectionEntity;
                if ( entity == null ) return;
                
                entity.PropertyChanged += ( s, e ) =>
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
        MultiSelectionEntity? vm = DataContext as MultiSelectionEntity;
        if ( vm == null ) return new Action( () => {} );

        List< (GameEntity entity, string Name) > selection = vm.SelectedEntities.Select( entity => ( entity, entity.Name ) ).ToList();
        
        return new Action( () =>
        {
            selection.ForEach( item => item.entity.Name = item.Name );
            
            MultiSelectionEntity? entity = DataContext as MultiSelectionEntity;
            entity?.Refresh();
        } );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetIsEnabledAction
    //-----------------------------------------------------------------------------------------------------------------
    private Action GetIsEnabledAction()
    {
        MultiSelectionEntity? vm = DataContext as MultiSelectionEntity;
        if ( vm == null ) return new Action( () => { } );

        List< (GameEntity entity, bool IsEnabled) > selection = vm.SelectedEntities.Select( entity => ( entity, entity.IsEnabled ) ).ToList();

        return new Action( () =>
        {
            selection.ForEach( item => item.entity.IsEnabled = item.IsEnabled );

            MultiSelectionEntity? entity = DataContext as MultiSelectionEntity;
            entity?.Refresh();
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
        if ( _propertyName == nameof( MultiSelectionEntity.Name ) && _undoAction != null )
        {
            Project.UndoRedo.Add( new UndoRedoAction( _undoAction, GetRenameAction(), "Rename game entity" ) );
            
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
        MultiSelectionEntity? vm = DataContext as MultiSelectionEntity;
        if ( vm == null ) return;
        
        vm.IsEnabled = ( sender as CheckBox )?.IsChecked == true;

        Action redoAction = GetIsEnabledAction();
        Project.UndoRedo.Add( new UndoRedoAction( undoAction, redoAction, vm.IsEnabled == true ? "Enable game entity" : "Disable game entity" ) );
    }
}