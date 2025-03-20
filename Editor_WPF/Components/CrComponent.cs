using System.Diagnostics;
using System.Runtime.Serialization;
using Editor_WPF.Common;
using Editor_WPF.Objects;


namespace Editor_WPF.Components;


//---------------------------------------------------------------------------------------------------------------------
/// IMultiSelectionComponent
//---------------------------------------------------------------------------------------------------------------------
public interface IMultiSelectionComponent
{
}


//---------------------------------------------------------------------------------------------------------------------
/// Component
//---------------------------------------------------------------------------------------------------------------------
[DataContract]
public abstract class CrComponent : CrObject
{
    public abstract IMultiSelectionComponent GetMultiSelectionComponent( MultiSelectionActor multiSelectionActor );
    
    //-----------------------------------------------------------------------------------------------------------------
    /// Component
    //-----------------------------------------------------------------------------------------------------------------
    public CrComponent( CrObject? owner )
    {
        Owner = owner;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/// MultiSelectionComponent
//---------------------------------------------------------------------------------------------------------------------
public abstract class MultiSelectionComponent< T > : ViewModelBase, IMultiSelectionComponent where T : CrComponent
{
    private bool _enableUpdates;
    
    public List< T > SelectedComponents { get; }

    
    protected abstract bool UpdateComponents( string propertyName );
    protected abstract bool UpdateMultiSelectionComponent();

    
    //-----------------------------------------------------------------------------------------------------------------
    /// Refresh
    //-----------------------------------------------------------------------------------------------------------------
    public void Refresh()
    {
        _enableUpdates = false;
        UpdateMultiSelectionComponent();
        _enableUpdates = true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// MultiSelectionComponent
    //-----------------------------------------------------------------------------------------------------------------
    public MultiSelectionComponent( MultiSelectionActor multiSelectionActor )
    {
        Debug.Assert( multiSelectionActor?.SelectedActors?.Any() == true );

        SelectedComponents = multiSelectionActor.SelectedActors.Select( actor => actor.GetComponent< T >() ).ToList();

        PropertyChanged += ( s, e ) =>
        {
            if ( _enableUpdates ) UpdateComponents( e.PropertyName );
        };
    }
}