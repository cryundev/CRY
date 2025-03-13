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
            else
            {
                EngineAPI.DespawnActor( this );
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
abstract class MultiSelectionActor : ViewModelBase
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
    public static float? GetMixedValue( List< CrActor > actors, Func< CrActor, float > getProperty )
    {
        float value = getProperty( actors.First() );

        foreach ( CrActor actor in actors.Skip( 1 ) )
        {
            if ( !value.IsTheSameAs( getProperty( actor ) ) )
            {
                return null;
            }
        }

        return value;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetMixedValue
    //-----------------------------------------------------------------------------------------------------------------
    public static bool? GetMixedValue( List< CrActor > actors, Func< CrActor, bool > getProperty )
    {
        bool value = getProperty( actors.First() );

        foreach ( CrActor actor in actors.Skip( 1 ) )
        {
            if ( value != getProperty( actor ) )
            {
                return null;
            }
        }

        return value;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetMixedValue
    //-----------------------------------------------------------------------------------------------------------------
    public static string? GetMixedValue( List< CrActor > actors, Func< CrActor, string > getProperty )
    {
        string value = getProperty( actors.First() );

        foreach ( CrActor actor in actors.Skip( 1 ) )
        {
            if ( value != getProperty( actor ) )
            {
                return null;
            }
        }

        return value;
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