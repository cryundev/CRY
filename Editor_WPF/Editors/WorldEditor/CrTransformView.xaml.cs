using System.Diagnostics;
using System.Numerics;
using System.Security.Cryptography.Xml;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using Editor_WPF.Components;
using Editor_WPF.GameProject;
using Editor_WPF.Objects;
using Editor_WPF.Utilities;


namespace Editor_WPF.Editors;


//---------------------------------------------------------------------------------------------------------------------
/// CrTransformView
//---------------------------------------------------------------------------------------------------------------------
public partial class CrTransformView : UserControl
{
    private Action? _undoAction = null;
    private bool _propertyChanged = false;
    
    //-----------------------------------------------------------------------------------------------------------------
    /// CrTransformView
    //-----------------------------------------------------------------------------------------------------------------
    public CrTransformView()
    {
        InitializeComponent();

        Loaded += OnTransformViewLoaded;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnTransformViewLoaded
    //-----------------------------------------------------------------------------------------------------------------
    private void OnTransformViewLoaded( object sender, RoutedEventArgs e )
    {
        Loaded -= OnTransformViewLoaded;

        (DataContext as MultiSelectionTransform).PropertyChanged += ( s, e ) => _propertyChanged = true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetAction
    //-----------------------------------------------------------------------------------------------------------------
    private Action? GetAction( Func< CrTransform, ( CrTransform transform, Vector3 ) > selector, Action< ( CrTransform transform, Vector3 ) > forEachAction )
    {
        if ( !( DataContext is MultiSelectionTransform multiSelectionTransform ) )
        {
            _undoAction = null;
            _propertyChanged = false;
            return null;
        }

        var selection = multiSelectionTransform.SelectedComponents.Select( selector ).ToList();
        return new Action( () =>
        {
            selection.ForEach( forEachAction );
            ( ActorView.Instance.DataContext as MultiSelectionActor )?.GetComponent< MultiSelectionTransform >()?.Refresh();
        } );
    }

    private Action? GetPositionAction() => GetAction( ( x ) => ( x, x.Position ), ( x ) => x.transform.Position = x.Item2 );
    private Action? GetRotationAction() => GetAction( ( x ) => ( x, x.Rotation ), ( x ) => x.transform.Rotation = x.Item2 );
    private Action? GetScaleAction()    => GetAction( ( x ) => ( x, x.Scale    ), ( x ) => x.transform.Scale    = x.Item2 );

    //-----------------------------------------------------------------------------------------------------------------
    /// RecordAction
    //-----------------------------------------------------------------------------------------------------------------
    private void RecordAction( Action? redoAction, string name )
    {
        if ( _propertyChanged )
        {
            Debug.Assert( _undoAction != null );
            Debug.Assert(  redoAction != null );
            
            _propertyChanged = false;
            
            Project.UndoRedo.Add( new UndoRedoAction( _undoAction, redoAction, name ) );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnPositionVectorBoxPreviewMouseLeftButtonDown
    //-----------------------------------------------------------------------------------------------------------------
    private void OnPositionVectorBoxPreviewMouseLeftButtonDown( object sender, MouseButtonEventArgs? e )
    {
        _propertyChanged = false;
        _undoAction = GetPositionAction();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnPositionVectorBoxPreviewMouseLeftButtonUp
    //-----------------------------------------------------------------------------------------------------------------
    private void OnPositionVectorBoxPreviewMouseLeftButtonUp( object sender, MouseButtonEventArgs? e )
    {
        RecordAction( GetPositionAction(), "Position changed" );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnPositionVectorBoxLostKeyboardFocus
    //-----------------------------------------------------------------------------------------------------------------
    private void OnPositionVectorBoxLostKeyboardFocus( object sender, KeyboardFocusChangedEventArgs e )
    {
        if ( _propertyChanged && _undoAction != null )
        {
            OnPositionVectorBoxPreviewMouseLeftButtonUp( sender, null );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnRotationVectorBoxMouseLeftButtonDown
    //-----------------------------------------------------------------------------------------------------------------
    private void OnRotationVectorBoxMouseLeftButtonDown( object sender, MouseButtonEventArgs? e )
    {
        _propertyChanged = false;
        _undoAction = GetRotationAction();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnRotationVectorBoxMouseLeftButtonUp
    //-----------------------------------------------------------------------------------------------------------------
    private void OnRotationVectorBoxMouseLeftButtonUp( object sender, MouseButtonEventArgs? e )
    {
        RecordAction( GetRotationAction(), "Rotation changed" );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnRotationVectorBoxLostKeyboardFocus
    //-----------------------------------------------------------------------------------------------------------------
    private void OnRotationVectorBoxLostKeyboardFocus( object sender, KeyboardFocusChangedEventArgs e )
    {
        if ( _propertyChanged && _undoAction != null )
        {
            OnRotationVectorBoxMouseLeftButtonUp( sender, null );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnScaleVectorBoxMouseLeftButtonDown
    //-----------------------------------------------------------------------------------------------------------------
    private void OnScaleVectorBoxMouseLeftButtonDown( object sender, MouseButtonEventArgs? e )
    {
        _propertyChanged = false;
        _undoAction = GetScaleAction();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnScaleVectorBoxMouseLeftButtonUp
    //-----------------------------------------------------------------------------------------------------------------
    private void OnScaleVectorBoxMouseLeftButtonUp( object sender, MouseButtonEventArgs? e )
    {
        RecordAction( GetScaleAction(), "Scale changed" );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnScaleVectorBoxLostKeyboardFocus
    //-----------------------------------------------------------------------------------------------------------------
    private void OnScaleVectorBoxLostKeyboardFocus( object sender, KeyboardFocusChangedEventArgs e )
    {
        if ( _propertyChanged && _undoAction != null )
        {
            OnScaleVectorBoxMouseLeftButtonUp( sender, null );
        }
    }
}