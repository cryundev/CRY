using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Runtime.Serialization;
using System.Windows.Input;
using Editor_WPF.Common;
using Editor_WPF.Components;
using Editor_WPF.Utilities;


namespace Editor_WPF.GameProject;


//---------------------------------------------------------------------------------------------------------------------
/// Scene
//---------------------------------------------------------------------------------------------------------------------
[DataContract]
public class Scene : ViewModelBase
{
    [DataMember( Name = "Name" )]
    private string _name;
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
    
    [DataMember]
    public Project Project { get; private set; }

    [DataMember( Name = "IsActive" )]
    private bool _isActive;
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
    private ObservableCollection< Actor > _actors = [];
    public ReadOnlyObservableCollection< Actor > Actors { get; private set; }

    public ICommand? AddActorCommand    { get; private set; }
    public ICommand? RemoveActorCommand { get; private set; }
    
    //-----------------------------------------------------------------------------------------------------------------
    /// AddActorInternal
    //-----------------------------------------------------------------------------------------------------------------
    private void AddActorInternal( Actor actor )
    {
        Debug.Assert( !_actors.Contains( actor ) );
        
        _actors.Add( actor );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// RemoveActorInternal
    //-----------------------------------------------------------------------------------------------------------------
    private void RemoveActorInternal( Actor actor )
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
        _actors ??= new ObservableCollection< Actor >();
        
        Actors = new ReadOnlyObservableCollection< Actor >( _actors );
        OnPropertyChanged( nameof( Actors ) );

        AddActorCommand = new RelayCommand< Actor >( x =>
        {
            AddActorInternal( x );

            int actorsCount = _actors.Count - 1;
            
            Project.UndoRedo.Add( new UndoRedoAction
            (
                () => RemoveActorInternal( x ),
                () => _actors.Insert( actorsCount, x ),
                $"Add {x.Name} to {Name}"
            ) );
        } );

        RemoveActorCommand = new RelayCommand< Actor >( x =>
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
    /// Scene
    //-----------------------------------------------------------------------------------------------------------------
    public Scene( Project poject, string name )
    {
        Debug.Assert( poject != null );
        
        Project = poject;
        _name   = name;
        
        OnDeserialized( new StreamingContext() );
    }
}