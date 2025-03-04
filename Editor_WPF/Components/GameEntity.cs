using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Runtime.Serialization;
using System.Windows.Input;
using Editor_WPF.Common;
using Editor_WPF.GameProject;
using Editor_WPF.Utilities;


namespace Editor_WPF.Components;


//---------------------------------------------------------------------------------------------------------------------
/// GameEntity
//---------------------------------------------------------------------------------------------------------------------
[DataContract]
[KnownType( typeof( Transform ) )]
public class GameEntity : ViewModelBase
{
    private bool _isEnabled = true;

    [DataMember]
    public bool IsEnabled
    {
        get => _isEnabled;
        set
        {
            if ( _isEnabled == value ) return;
            
            _isEnabled = value;
            OnPropertyChanged( nameof( IsEnabled ) );
        }
    } 
        
    private string _name;
    
    [DataMember]
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
    public Scene ParentScene { get; private set; }
    
    [DataMember( Name = nameof( Components ) )]
    private readonly ObservableCollection< Component > _components = [];
    public ReadOnlyObservableCollection< Component > Components { get; private set; }
    
    public ICommand RenameCommand    { get; private set; }
    public ICommand IsEnabledCommand { get; private set; }
    

    //-----------------------------------------------------------------------------------------------------------------
    /// OnDeserialized
    //-----------------------------------------------------------------------------------------------------------------
    [OnDeserialized]
    void OnDeserialized( StreamingContext context )
    {
        if ( _components != null )
        {
            Components = new ReadOnlyObservableCollection< Component >( _components );
            OnPropertyChanged( nameof( Components ) );
        }
        
        RenameCommand = new RelayCommand< string >( x =>
        {
            string oldName = _name;
            Name = x;

            Project.UndoRedo.Add
            (
                new UndoRedoAction
                (
                    nameof( Name ), this, oldName, x, $"Rename entity `{oldName}` to `{x}`" 
                )
            );
        }, x => x != _name );
        
        IsEnabledCommand = new RelayCommand< bool >( x =>
        {
            bool oldValue = _isEnabled;
            IsEnabled = x;

            Project.UndoRedo.Add
            (
                new UndoRedoAction
                (
                    nameof( IsEnabled ), this, oldValue, x, x ? $"Enable {Name}" : $"Disable {Name}"
                )
            );
        } );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GameEntity
    //-----------------------------------------------------------------------------------------------------------------
    public GameEntity( Scene scene )
    {
        Debug.Assert( scene != null );

        _name = "";
        _components.Add( new Transform( this ) );
        
        ParentScene = scene;
        
        OnDeserialized( new StreamingContext() );
    }

}