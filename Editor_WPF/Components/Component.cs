using System.Diagnostics;
using System.Runtime.Serialization;
using Editor_WPF.Common;


namespace Editor_WPF.Components;


//---------------------------------------------------------------------------------------------------------------------
/// IMultiSelectionComponent
//---------------------------------------------------------------------------------------------------------------------
interface IMultiSelectionComponent
{
}


//---------------------------------------------------------------------------------------------------------------------
/// Component
//---------------------------------------------------------------------------------------------------------------------
[DataContract]
public class Component : ViewModelBase
{
    [DataMember]
    public GameEntity Owner { get; private set; }
    
    public Component( GameEntity owner )
    {
        Debug.Assert( owner != null );
        
        Owner = owner;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/// MultiSelectionComponent
//---------------------------------------------------------------------------------------------------------------------
abstract class MultiSelectionComponent< T > : ViewModelBase, IMultiSelectionComponent where T : Component
{
}