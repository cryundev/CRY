using System.Diagnostics;
using System.Runtime.Serialization;
using Editor_WPF.Common;


namespace Editor_WPF.Components;


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