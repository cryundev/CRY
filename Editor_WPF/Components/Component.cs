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
    public Actor Owner { get; private set; }
    
    public Component( Actor owner )
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