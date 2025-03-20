using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Runtime.Serialization;
using System.Windows.Input;
using Editor_WPF.Common;
using Editor_WPF.Components;
using Editor_WPF.Objects;
using Editor_WPF.Utilities;


namespace Editor_WPF.GameProject;


//---------------------------------------------------------------------------------------------------------------------
/// World
//---------------------------------------------------------------------------------------------------------------------
[DataContract]
public class World : ViewModelBase
{
    [DataMember( Name = "Name" )] private string _name;
    public string Name
    {
        get => _name;
        set
        {
            if ( _name == value ) return;
            
            _name = value;
            OnPropertyChanged( nameof( Name ) );
        }
    }
    
    [DataMember] public Project Project { get; private set; }

    [DataMember( Name = "IsActive" )] private bool _isActive;
    public bool IsActive
    {
        get => _isActive;
        set
        {
            if ( _isActive == value ) return;
            
            _isActive = value;
            OnPropertyChanged( nameof( IsActive ) );
        }
    }
    
    [DataMember( Name = "Actors" )]
    private ObservableCollection< CrActor > _actors = [];
    public ReadOnlyObservableCollection< CrActor > Actors { get; private set; }

    public ICommand? AddActorCommand    { get; private set; }
    public ICommand? RemoveActorCommand { get; private set; }
    
    //-----------------------------------------------------------------------------------------------------------------
    /// AddActorInternal
    //-----------------------------------------------------------------------------------------------------------------
    private void AddActorInternal( CrActor actor, int index = -1 )
    {
        Debug.Assert( !_actors.Contains( actor ) );
        
        actor.IsActive = IsActive;

        if ( index == -1 )
        {
            _actors.Add( actor );
        }
        else
        {
            _actors.Insert( index, actor );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// RemoveActorInternal
    //-----------------------------------------------------------------------------------------------------------------
    private void RemoveActorInternal( CrActor actor )
    {
        Debug.Assert( _actors.Contains( actor ) );
        
        _actors.Remove( actor );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnDeserialized
    //-----------------------------------------------------------------------------------------------------------------
    [OnDeserialized]
    private void OnDeserialized( StreamingContext context )
    {
        _actors ??= new ObservableCollection< CrActor >();
        
        Actors = new ReadOnlyObservableCollection< CrActor >( _actors );
        OnPropertyChanged( nameof( Actors ) );

        foreach ( CrActor actor in _actors )
        {
            actor.IsActive = IsActive;
        }

        AddActorCommand = new RelayCommand< CrActor >( x =>
        {
            AddActorInternal( x );

            int actorIndex = _actors.Count - 1;
            
            Project.UndoRedo.Add( new UndoRedoAction
            (
                () => RemoveActorInternal( x ),
                () => AddActorInternal( x, actorIndex ),
                $"Add {x.Name} to {Name}"
            ) );
        } );

        RemoveActorCommand = new RelayCommand< CrActor >( x =>
        {
            RemoveActorInternal( x );

            int actorsCount = _actors.Count;
            
            Project.UndoRedo.Add( new UndoRedoAction
            (
                () => _actors.Insert( actorsCount, x ),
                () => RemoveActorInternal( x ),
                $"Remove {x.Name} from {Name}"
            ) );
        } );
        
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// World
    //-----------------------------------------------------------------------------------------------------------------
    public World( Project poject, string name )
    {
        Debug.Assert( poject != null );
        
        Project = poject;
        _name   = name;
        
        OnDeserialized( new StreamingContext() );
    }
}