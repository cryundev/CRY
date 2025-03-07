using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Runtime.Serialization;
using Editor_WPF.Common;
using Editor_WPF.GameProject;
using Editor_WPF.Utilities;


namespace Editor_WPF.Components;


//---------------------------------------------------------------------------------------------------------------------
/// Actor
//---------------------------------------------------------------------------------------------------------------------
[DataContract]
[KnownType( typeof( Transform ) )]
public class Actor : ViewModelBase
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
    /// Actor
    //-----------------------------------------------------------------------------------------------------------------
    public Actor( Scene scene )
    {
        Debug.Assert( scene != null );

        _name = "";
        _components.Add( new Transform( this ) );
        
        ParentScene = scene;
        
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

    public List< Actor > SelectedEntities { get; }


    //-----------------------------------------------------------------------------------------------------------------
    /// UpdateGameEntities
    //-----------------------------------------------------------------------------------------------------------------
    protected virtual bool UpdateGameEntities( string propertyName )
    {
        switch ( propertyName )
        {
            case nameof( IsEnabled ) : SelectedEntities.ForEach( x => x.IsEnabled = IsEnabled ?? false ); return true;
            case nameof( Name )      : SelectedEntities.ForEach( x => x.Name = Name ?? "" ); return true;
        }

        return false;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetMixedValue
    //-----------------------------------------------------------------------------------------------------------------
    public static float? GetMixedValue( List< Actor > actors, Func< Actor, float > getProperty )
    {
        float value = getProperty( actors.First() );

        foreach ( Actor actor in actors.Skip( 1 ) )
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
    public static bool? GetMixedValue( List< Actor > actors, Func< Actor, bool > getProperty )
    {
        bool value = getProperty( actors.First() );

        foreach ( Actor actor in actors.Skip( 1 ) )
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
    public static string? GetMixedValue( List< Actor > actors, Func< Actor, string > getProperty )
    {
        string value = getProperty( actors.First() );

        foreach ( Actor actor in actors.Skip( 1 ) )
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
        IsEnabled = GetMixedValue( SelectedEntities, new Func< Actor, bool   >( x => x.IsEnabled ) );
        Name      = GetMixedValue( SelectedEntities, new Func< Actor, string >( x => x.Name      ) );

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
    public MultiSelectionActor( List< Actor > entities )
    {
        Debug.Assert( entities?.Any() == true );

        Components = new ReadOnlyObservableCollection< IMultiSelectionComponent >( _components );

        SelectedEntities = entities;

        PropertyChanged += ( s, e ) => { if ( _enableUpdates ) UpdateGameEntities( e.PropertyName ); };
    }
}


//---------------------------------------------------------------------------------------------------------------------
/// MultiSelectionGameActor
//---------------------------------------------------------------------------------------------------------------------
class MultiSelectionGameActor : MultiSelectionActor
{
    public MultiSelectionGameActor( List< Actor > entities ) : base( entities )
    {
        Refresh();
    }
}