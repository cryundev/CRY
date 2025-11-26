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
/// WorldViewModel
//---------------------------------------------------------------------------------------------------------------------
[DataContract( Name = "World" )]
public class WorldViewModel : ViewModelBase
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
    
    [DataMember] public ProjectViewModel Project { get; internal set; }

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
    private ObservableCollection< CrActorViewModel > _actors = [];
    public ReadOnlyObservableCollection< CrActorViewModel > Actors { get; private set; }

    public ICommand? AddActorCommand    { get; private set; }
    public ICommand? RemoveActorCommand { get; private set; }
    
    //-----------------------------------------------------------------------------------------------------------------
    /// AddActorInternal
    //-----------------------------------------------------------------------------------------------------------------
    private void AddActorInternal( CrActorViewModel actor, int index = -1 )
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
    private void RemoveActorInternal( CrActorViewModel actor )
    {
        Debug.Assert( _actors.Contains( actor ) );
        
        _actors.Remove( actor );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Initialize
    //-----------------------------------------------------------------------------------------------------------------
    private void Initialize()
    {
        _actors ??= new ObservableCollection< CrActorViewModel >();

        Actors = new ReadOnlyObservableCollection< CrActorViewModel >( _actors );
        OnPropertyChanged( nameof( Actors ) );

        foreach ( CrActorViewModel actor in _actors )
        {
            actor.IsActive = IsActive;
        }

        InitializeCommands();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// InitializeCommands
    //-----------------------------------------------------------------------------------------------------------------
    private void InitializeCommands()
    {
        AddActorCommand = new RelayCommand< CrActorViewModel >( x =>
        {
            AddActorInternal( x );

            int actorIndex = _actors.Count - 1;

            ProjectViewModel.UndoRedo.Add( new UndoRedoAction
            (
                () => RemoveActorInternal( x ),
                () => AddActorInternal( x, actorIndex ),
                $"Add {x.Name} to {Name}"
            ) );
        } );

        RemoveActorCommand = new RelayCommand< CrActorViewModel >( x =>
        {
            RemoveActorInternal( x );

            int actorsCount = _actors.Count;

            ProjectViewModel.UndoRedo.Add( new UndoRedoAction
            (
                () => _actors.Insert( actorsCount, x ),
                () => RemoveActorInternal( x ),
                $"Remove {x.Name} from {Name}"
            ) );
        } );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnDeserialized
    //-----------------------------------------------------------------------------------------------------------------
    [OnDeserialized]
    private void OnDeserialized( StreamingContext context )
    {
        Initialize();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// WorldViewModel
    //-----------------------------------------------------------------------------------------------------------------
    public WorldViewModel( ProjectViewModel poject, string name )
    {
        Debug.Assert( poject != null );

        Project = poject;
        _name   = name;

        Initialize();
    }
}