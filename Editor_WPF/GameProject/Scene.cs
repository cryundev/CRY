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
    
    [DataMember( Name = "GameEntities" )]
    private ObservableCollection< GameEntity > _gameEntities = [];
    public ReadOnlyObservableCollection< GameEntity > GameEntities { get; private set; }

    public ICommand? AddGameEntityCommand    { get; private set; }
    public ICommand? RemoveGameEntityCommand { get; private set; }
    
    //-----------------------------------------------------------------------------------------------------------------
    /// AddGameEntityInternal
    //-----------------------------------------------------------------------------------------------------------------
    private void AddGameEntityInternal( GameEntity gameEntity )
    {
        Debug.Assert( !_gameEntities.Contains( gameEntity ) );
        
        _gameEntities.Add( gameEntity );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// RemoveGameEntityInternal
    //-----------------------------------------------------------------------------------------------------------------
    private void RemoveGameEntityInternal( GameEntity gameEntity )
    {
        Debug.Assert( _gameEntities.Contains( gameEntity ) );
        
        _gameEntities.Remove( gameEntity );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnDeserialized
    //-----------------------------------------------------------------------------------------------------------------
    [OnDeserialized]
    private void OnDeserialized( StreamingContext context )
    {
        _gameEntities ??= new ObservableCollection< GameEntity >();
        
        GameEntities = new ReadOnlyObservableCollection< GameEntity >( _gameEntities );
        OnPropertyChanged( nameof( GameEntities ) );

        AddGameEntityCommand = new RelayCommand< GameEntity >( x =>
        {
            AddGameEntityInternal( x );

            int entityIndex = _gameEntities.Count - 1;
            
            Project.UndoRedo.Add( new UndoRedoAction
            (
                () => RemoveGameEntityInternal( x ),
                () => _gameEntities.Insert( entityIndex, x ),
                $"Add {x.Name} to {Name}"
            ) );
        } );

        RemoveGameEntityCommand = new RelayCommand< GameEntity >( x =>
        {
            RemoveGameEntityInternal( x );

            int entityIndex = _gameEntities.Count;
            
            Project.UndoRedo.Add( new UndoRedoAction
            (
                () => _gameEntities.Insert( entityIndex, x ),
                () => RemoveGameEntityInternal( x ),
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