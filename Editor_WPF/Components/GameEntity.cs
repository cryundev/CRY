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
        
        OnDeserialized( new StreamingContext() );
    }
}


//---------------------------------------------------------------------------------------------------------------------
/// MultiSelectionEntity
//---------------------------------------------------------------------------------------------------------------------
abstract class MultiSelectionEntity : ViewModelBase
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

    public List< GameEntity > SelectedEntities { get; }


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
    public static float? GetMixedValue( List< GameEntity > entities, Func< GameEntity, float > getProperty )
    {
        float value = getProperty( entities.First() );

        foreach ( GameEntity entity in entities.Skip( 1 ) )
        {
            if ( !value.IsTheSameAs( getProperty( entity ) ) )
            {
                return null;
            }
        }

        return value;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetMixedValue
    //-----------------------------------------------------------------------------------------------------------------
    public static bool? GetMixedValue( List< GameEntity > entities, Func< GameEntity, bool > getProperty )
    {
        bool value = getProperty( entities.First() );

        foreach ( GameEntity entity in entities.Skip( 1 ) )
        {
            if ( value != getProperty( entity ) )
            {
                return null;
            }
        }

        return value;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetMixedValue
    //-----------------------------------------------------------------------------------------------------------------
    public static string? GetMixedValue( List< GameEntity > entities, Func< GameEntity, string > getProperty )
    {
        string value = getProperty( entities.First() );

        foreach ( GameEntity entity in entities.Skip( 1 ) )
        {
            if ( value != getProperty( entity ) )
            {
                return null;
            }
        }

        return value;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// UpdateMultiSelectionEntity
    //-----------------------------------------------------------------------------------------------------------------
    protected virtual bool UpdateMultiSelectionEntity()
    {
        IsEnabled = GetMixedValue( SelectedEntities, new Func< GameEntity, bool   >( x => x.IsEnabled ) );
        Name      = GetMixedValue( SelectedEntities, new Func< GameEntity, string >( x => x.Name      ) );

        return true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Refresh
    //-----------------------------------------------------------------------------------------------------------------
    public void Refresh()
    {
        _enableUpdates = false;
        
        UpdateMultiSelectionEntity();
        
        _enableUpdates = true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// MultiSelectionEntity
    //-----------------------------------------------------------------------------------------------------------------
    public MultiSelectionEntity( List< GameEntity > entities )
    {
        Debug.Assert( entities?.Any() == true );

        Components = new ReadOnlyObservableCollection< IMultiSelectionComponent >( _components );

        SelectedEntities = entities;

        PropertyChanged += ( s, e ) => { if ( _enableUpdates ) UpdateGameEntities( e.PropertyName ); };
    }
}


//---------------------------------------------------------------------------------------------------------------------
/// MultiSelectionGameEntity
//---------------------------------------------------------------------------------------------------------------------
class MultiSelectionGameEntity : MultiSelectionEntity
{
    public MultiSelectionGameEntity( List< GameEntity > entities ) : base( entities )
    {
        Refresh();
    }
}