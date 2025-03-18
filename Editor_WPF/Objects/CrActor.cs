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
[KnownType( typeof( CrTransform ) )]
public class CrActor : CrObject
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
                ActorId = EngineAPI.SpawnActor( this );
                Debug.Assert( ID.IsValid( ActorId ), "ActorId is invalid" );
            }
            else if( ID.IsValid( ActorId ) )
            {
                EngineAPI.DespawnActor( this );
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

    [DataMember] public World ParentWorld { get; private set; }
    
    [DataMember( Name = nameof( Components ) )] private readonly ObservableCollection< CrComponent > _components = [];
    public ReadOnlyObservableCollection< CrComponent >? Components { get; private set; }


    //-----------------------------------------------------------------------------------------------------------------
    /// GetCompnent
    //-----------------------------------------------------------------------------------------------------------------
    public CrComponent? GetCompnent( Type type ) => Components?.FirstOrDefault( x => x.GetType() == type ); 

    //-----------------------------------------------------------------------------------------------------------------
    /// GetComponent
    //-----------------------------------------------------------------------------------------------------------------
    public T? GetComponent< T >() where T : CrComponent => GetCompnent( typeof( T ) ) as T;

    //-----------------------------------------------------------------------------------------------------------------
    /// OnDeserialized
    //-----------------------------------------------------------------------------------------------------------------
    [OnDeserialized]
    void OnDeserialized( StreamingContext context )
    {
        Components = new ReadOnlyObservableCollection< CrComponent >( _components );
        OnPropertyChanged( nameof( Components ) );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Actor
    //-----------------------------------------------------------------------------------------------------------------
    public CrActor( World world )
    {
        Debug.Assert( world != null );

        Name = "";
        _components.Add( new CrTransform( this ) );
        
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

    public List< CrActor > SelectedActors { get; }


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
        
        CrActor? firstEntity = SelectedActors.FirstOrDefault();
        if ( firstEntity == null ) return;

        if ( firstEntity.Components != null )
        {
            foreach ( CrComponent component in firstEntity.Components )
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
    /// GetMixedValue
    //-----------------------------------------------------------------------------------------------------------------
    public static float? GetMixedValue< T >( List< T > objects, Func< T, float > getProperty )
    {
        float value = getProperty( objects.First() );

        return objects.Skip( 1 ).Any( x => !getProperty( x ).IsTheSameAs( value ) ) ? (float?)null : value;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetMixedValue
    //-----------------------------------------------------------------------------------------------------------------
    public static bool? GetMixedValue< T >( List< T > objects, Func< T, bool > getProperty )
    {
        bool value = getProperty( objects.First() );

        return objects.Skip( 1 ).Any( x => value != getProperty( x ) ) ? (bool?)null : value;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetMixedValue
    //-----------------------------------------------------------------------------------------------------------------
    public static string? GetMixedValue< T >( List< T > objects, Func< T, string > getProperty )
    {
        string value = getProperty( objects.First() );

        return objects.Skip( 1 ).Any( x => value != getProperty( x ) ) ? (string?)null : value;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// UpdateMultiSelectionActor
    //-----------------------------------------------------------------------------------------------------------------
    protected virtual bool UpdateMultiSelectionActor()
    {
        IsEnabled = GetMixedValue( SelectedActors, new Func< CrActor, bool   >( x => x.IsEnabled ) );
        Name      = GetMixedValue( SelectedActors, new Func< CrActor, string >( x => x.Name      ) );

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
    public MultiSelectionActor( List< CrActor > actors )
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
    public MultiSelectionGameActor( List< CrActor > actors ) : base( actors )
    {
        Refresh();
    }
}