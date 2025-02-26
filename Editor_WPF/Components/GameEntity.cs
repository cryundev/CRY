using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Runtime.Serialization;
using Editor_WPF.Common;
using Editor_WPF.GameProject;


namespace Editor_WPF.Components;


//---------------------------------------------------------------------------------------------------------------------
/// GameEntity
//---------------------------------------------------------------------------------------------------------------------
[DataContract]
[KnownType( typeof( Transform ) )]
public class GameEntity : ViewModelBase
{
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
    }

}