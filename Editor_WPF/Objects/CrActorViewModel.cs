using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Runtime.Serialization;
using Editor_WPF.Common;
using Editor_WPF.Components;
using Editor_WPF.DllWrappers;
using Editor_WPF.GameProject;
using Editor_WPF.Utilities;


namespace Editor_WPF.Objects;


//---------------------------------------------------------------------------------------------------------------------
/// Actor
//---------------------------------------------------------------------------------------------------------------------
[DataContract]
[KnownType( typeof( CrTransformComponentViewModel ) )]
[KnownType( typeof( CrScriptComponentViewModel    ) )]
public class CrActorViewModel : CrObjectViewModel
{
    private Int64 _actorId = ID.INVALID_ID;
    public Int64 ActorId
    {
        get => _actorId;
        set
        {
            if ( _actorId == value ) return;
            
            _actorId = value;
            OnPropertyChanged( nameof( ActorId ) );
        }
    }

    private bool _isActive;
    public bool IsActive
    {
        get => _isActive;
        set
        {
            if ( _isActive == value ) return;

            _isActive = value;

            if ( _isActive )
            {
                ActorId = EngineAPI.World.SpawnActor( this );
                Debug.Assert( ID.IsValid( ActorId ), "ActorId is invalid" );
            }
            else if( ID.IsValid( ActorId ) )
            {
                EngineAPI.World.DespawnActor( this );
                ActorId = ID.INVALID_ID;
            }

            OnPropertyChanged( nameof( IsActive ) );
        }
    }
     
    private bool _isEnabled = true;

    [DataMember] public bool IsEnabled
    {
        get => _isEnabled;
        set
        {
            if ( _isEnabled == value ) return;
            
            _isEnabled = value;
            OnPropertyChanged( nameof( IsEnabled ) );
        }
    }

    [DataMember] public WorldViewModel ParentWorld { get; private set; }
    
    [DataMember( Name = nameof( Components ) )] private readonly ObservableCollection< CrComponentViewModel > _components = [];
    public ReadOnlyObservableCollection< CrComponentViewModel >? Components { get; private set; }


    //-----------------------------------------------------------------------------------------------------------------
    /// GetCompnent
    //-----------------------------------------------------------------------------------------------------------------
    public CrComponentViewModel? GetCompnent( Type type ) => Components?.FirstOrDefault( x => x.GetType() == type ); 

    //-----------------------------------------------------------------------------------------------------------------
    /// GetComponent
    //-----------------------------------------------------------------------------------------------------------------
    public T? GetComponent< T >() where T : CrComponentViewModel => GetCompnent( typeof( T ) ) as T;

    //-----------------------------------------------------------------------------------------------------------------
    /// AddComponent
    //-----------------------------------------------------------------------------------------------------------------
    public bool AddComponent( CrComponentViewModel component )
    {
        Debug.Assert( component != null );
        
        if ( Components.Any( x => x.GetType() == component.GetType() ) )
        {
            Logger.Log( MessageType.Warning, $"Entity {Name} already has a {component.GetType().Name} component" );
            return false;
        }

        IsActive = false;
        _components.Add( component );
        IsActive = true;
        
        return true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// RemoveComponent
    //-----------------------------------------------------------------------------------------------------------------
    public void RemoveComponent( CrComponentViewModel component )
    {
        Debug.Assert( component != null );
        
        if ( component is CrTransformComponentViewModel ) return;

        if ( _components.Contains( component ) )
        {
            IsActive = false;
            _components.Remove( component );
            IsActive = true;
        }

    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnDeserialized
    //-----------------------------------------------------------------------------------------------------------------
    [OnDeserialized]
    void OnDeserialized( StreamingContext context )
    {
        Components = new ReadOnlyObservableCollection< CrComponentViewModel >( _components );
        OnPropertyChanged( nameof( Components ) );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Actor
    //-----------------------------------------------------------------------------------------------------------------
    public CrActorViewModel( WorldViewModel world )
    {
        Debug.Assert( world != null );

        Name = "";
        _components.Add( new CrTransformComponentViewModel( this ) );
        
        ParentWorld = world;
        
        OnDeserialized( new StreamingContext() );
    }
}


//---------------------------------------------------------------------------------------------------------------------
/// MultiSelectionActor
//---------------------------------------------------------------------------------------------------------------------
public abstract class MultiSelectionActor : ViewModelBase
{
    private bool _enableUpdates = true;
    
    private bool? _isEnabled;
    public bool? IsEnabled
    {
        get => _isEnabled;
        set
        {
            if ( _isEnabled == value ) return;
            
            _isEnabled = value;
            OnPropertyChanged( nameof( IsEnabled ) );
        }
    }

    private string? _name;
    public string? Name
    {
        get => _name;
        set
        {
            if ( _name == value ) return;
            
            _name = value;
            OnPropertyChanged( nameof( Name ) );
        }
    }
    
    private readonly ObservableCollection< IMultiSelectionComponent > _components = new ObservableCollection< IMultiSelectionComponent >();
    public ReadOnlyObservableCollection< IMultiSelectionComponent > Components { get; }

    public List< CrActorViewModel > SelectedActors { get; }


    //-----------------------------------------------------------------------------------------------------------------
    /// GetComponent
    //-----------------------------------------------------------------------------------------------------------------
    public T? GetComponent< T >() where T : IMultiSelectionComponent
    {
        return (T)Components.FirstOrDefault( x => x.GetType() == typeof( T ) );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// MakeComponentList
    //-----------------------------------------------------------------------------------------------------------------
    private void MakeComponentList()
    {
        _components.Clear();
        
        CrActorViewModel? firstEntity = SelectedActors.FirstOrDefault();
        if ( firstEntity == null ) return;

        if ( firstEntity.Components != null )
        {
            foreach ( CrComponentViewModel component in firstEntity.Components )
            {
                Type type = component.GetType();

                if ( SelectedActors.Skip( 1 ).All( actor => actor.GetCompnent( type ) != null ) )
                {
                    Debug.Assert( Components.FirstOrDefault( x => x.GetType() == type ) == null );

                    _components.Add( component.GetMultiSelectionComponent( this ) );
                }
            }
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// UpdateActors
    //-----------------------------------------------------------------------------------------------------------------
    protected virtual bool UpdateActors( string propertyName )
    {
        switch ( propertyName )
        {
            case nameof( IsEnabled ) : SelectedActors.ForEach( x => x.IsEnabled = IsEnabled ?? false ); return true;
            case nameof( Name )      : SelectedActors.ForEach( x => x.Name = Name ?? "" ); return true;
        }

        return false;
    }

    

    //-----------------------------------------------------------------------------------------------------------------
    /// UpdateMultiSelectionActor
    //-----------------------------------------------------------------------------------------------------------------
    protected virtual bool UpdateMultiSelectionActor()
    {
        IsEnabled = UtilObject.GetMixedValue( SelectedActors, new Func< CrActorViewModel, bool   >( x => x.IsEnabled ) );
        Name      = UtilObject.GetMixedValue( SelectedActors, new Func< CrActorViewModel, string >( x => x.Name      ) );

        return true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Refresh
    //-----------------------------------------------------------------------------------------------------------------
    public void Refresh()
    {
        _enableUpdates = false;
        
        UpdateMultiSelectionActor();
        MakeComponentList();
        
        _enableUpdates = true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// MultiSelectionActor
    //-----------------------------------------------------------------------------------------------------------------
    public MultiSelectionActor( List< CrActorViewModel > actors )
    {
        Debug.Assert( actors?.Any() == true );

        Components = new ReadOnlyObservableCollection< IMultiSelectionComponent >( _components );

        SelectedActors = actors;

        PropertyChanged += ( s, e ) => { if ( _enableUpdates ) UpdateActors( e.PropertyName ); };
    }
}


//---------------------------------------------------------------------------------------------------------------------
/// MultiSelectionGameActor
//---------------------------------------------------------------------------------------------------------------------
class MultiSelectionGameActor : MultiSelectionActor
{
    public MultiSelectionGameActor( List< CrActorViewModel > actors ) : base( actors )
    {
        Refresh();
    }
}