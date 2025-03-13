using System.Diagnostics;
using System.Runtime.InteropServices.JavaScript;
using System.Runtime.Serialization;
using Editor_WPF.Common;


namespace Editor_WPF.Objects;


//---------------------------------------------------------------------------------------------------------------------
/// Object
//---------------------------------------------------------------------------------------------------------------------
[DataContract]
public class CrObject : ViewModelBase
{
    private string _name = "";

    [DataMember] public string Name
    {
        get => _name;
        set
        {
            if ( _name == value ) return;

            _name = value;
            OnPropertyChanged( nameof( Name ) );
        }
    }

    [DataMember] public CrObject? Owner { get; protected set; } = null;
}