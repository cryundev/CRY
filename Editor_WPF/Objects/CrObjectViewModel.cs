using System.Diagnostics;
using System.Runtime.Serialization;
using Editor_WPF.Common;


namespace Editor_WPF.Objects;


//---------------------------------------------------------------------------------------------------------------------
/// Object ViewModel
//---------------------------------------------------------------------------------------------------------------------
[DataContract]
public abstract class CrObjectViewModel : ViewModelBase
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

    [DataMember] public CrObjectViewModel? Owner { get; protected set; } = null;
}