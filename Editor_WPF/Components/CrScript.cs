using System.Runtime.Serialization;
using Editor_WPF.Objects;
using Editor_WPF.Utilities;


namespace Editor_WPF.Components;


//---------------------------------------------------------------------------------------------------------------------
/// Script
//---------------------------------------------------------------------------------------------------------------------
[DataContract]
class CrScript : CrComponent
{
    private string _name;
    [DataMember] public string Name
    {
        get => _name;
        set
        {
            if ( _name == value )
            {
                return;
            }

            _name = value;
            OnPropertyChanged( nameof( Name ) );
        }
    }

    public override IMultiSelectionComponent GetMultiSelectionComponent( MultiSelectionActor multiSelectionActor )
        => new MultiSelectionScript( multiSelectionActor );

    //-----------------------------------------------------------------------------------------------------------------
    /// CrScript
    //-----------------------------------------------------------------------------------------------------------------
    public CrScript( CrObject? owner ) : base( owner )
    {
    }
}


//---------------------------------------------------------------------------------------------------------------------
/// MultiSelectionScript
//---------------------------------------------------------------------------------------------------------------------
sealed class MultiSelectionScript : MultiSelectionComponent< CrScript >
{
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

    //-----------------------------------------------------------------------------------------------------------------
    /// UpdateComponents
    //-----------------------------------------------------------------------------------------------------------------
    protected override bool UpdateComponents( string propertyName )
    {
        switch ( propertyName )
        {
            case nameof( Name ):
            {
                if ( _name != null )
                {
                    SelectedComponents.ForEach( c => c.Name = _name );
                }
                return true;
            }
        }

        return false;
    }
    
    //-----------------------------------------------------------------------------------------------------------------
    /// UpdateMultiSelectionComponent
    //-----------------------------------------------------------------------------------------------------------------
    protected override bool UpdateMultiSelectionComponent()
    {
        Name = UtilObject.GetMixedValue( SelectedComponents, new Func< CrScript, string >( x => x.Name ) );

        return true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// MultiSelectionScript
    //-----------------------------------------------------------------------------------------------------------------
    public MultiSelectionScript( MultiSelectionActor multiSelectionActor ) : base( multiSelectionActor )
    {
        Refresh();
    }
}
