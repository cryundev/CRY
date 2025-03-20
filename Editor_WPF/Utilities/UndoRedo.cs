using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Runtime.CompilerServices;


namespace Editor_WPF.Utilities;


//---------------------------------------------------------------------------------------------------------------------
/// IUndoRedo
//---------------------------------------------------------------------------------------------------------------------
public interface IUndoRedo
{
    string Name { get; }
    
    void Undo();
    void Redo();
}


//---------------------------------------------------------------------------------------------------------------------
/// UndoRedoAction
//---------------------------------------------------------------------------------------------------------------------
public class UndoRedoAction : IUndoRedo
{
    private readonly Action _undoAction;
    private readonly Action _redoAction;
    
    public String Name { get; }
    
    public void Undo() => _undoAction();
    public void Redo() => _redoAction();


    //-----------------------------------------------------------------------------------------------------------------
    /// UndoRedoAction
    //-----------------------------------------------------------------------------------------------------------------
    public UndoRedoAction( string name )
    {
        Name = name;
        
        _undoAction = () => { };
        _redoAction = () => { };
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// UndoRedoAction
    //-----------------------------------------------------------------------------------------------------------------
    public UndoRedoAction( Action undo, Action redo, string name )
    : this( name )
    {
        Debug.Assert( undo != null && redo != null );
        
        _undoAction = undo;
        _redoAction = redo;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// UndoRedoAction
    //-----------------------------------------------------------------------------------------------------------------
    public UndoRedoAction( string property, object instance, object undoValue, object redoValue, string name )
    : this
    (
        () => instance.GetType().GetProperty( property )?.SetValue( instance, undoValue ), 
        () => instance.GetType().GetProperty( property )?.SetValue( instance, redoValue ),
        name
    )
    {
    }
}


//---------------------------------------------------------------------------------------------------------------------
/// UndoRedo
//---------------------------------------------------------------------------------------------------------------------
public class UndoRedo
{
    private bool _enableAdd = true;
    
    private readonly ObservableCollection< IUndoRedo > _undoList = [];
    private readonly ObservableCollection< IUndoRedo > _redoList = [];

    public ReadOnlyObservableCollection< IUndoRedo > UndoList { get; }
    public ReadOnlyObservableCollection< IUndoRedo > RedoList { get; }
    

    //-----------------------------------------------------------------------------------------------------------------
    /// Reset
    //-----------------------------------------------------------------------------------------------------------------
    public void Reset()
    {
        _redoList.Clear();
        _undoList.Clear();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Add
    //-----------------------------------------------------------------------------------------------------------------
    public void Add( IUndoRedo cmd )
    {
        if ( _enableAdd == false ) return;
        
        _undoList.Add( cmd );
        _redoList.Clear();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Undo
    //-----------------------------------------------------------------------------------------------------------------
    public void Undo()
    {
        if ( !_undoList.Any() ) return;
        
        IUndoRedo cmd = _undoList.Last();
        _undoList.RemoveAt( _undoList.Count - 1 );
        _enableAdd = false;
        cmd.Undo();
        _enableAdd = true;
        _redoList.Insert( 0, cmd );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Redo
    //-----------------------------------------------------------------------------------------------------------------
    public void Redo()
    {
        if ( !_redoList.Any() ) return;
        
        IUndoRedo cmd = _redoList.First();
        _redoList.RemoveAt( 0 );
        _enableAdd = false;
        cmd.Redo();
        _enableAdd = true;
        _undoList.Add( cmd );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// UndoRedo
    //-----------------------------------------------------------------------------------------------------------------
    public UndoRedo()
    {
        RedoList = new ReadOnlyObservableCollection< IUndoRedo >( _redoList );
        UndoList = new ReadOnlyObservableCollection< IUndoRedo >( _undoList );
    }
}