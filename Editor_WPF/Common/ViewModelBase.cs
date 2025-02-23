using System.ComponentModel;
using System.Runtime.Serialization;


namespace Editor_WPF.Common;


//---------------------------------------------------------------------------------------------------------------------
/// ViewModelBase
//---------------------------------------------------------------------------------------------------------------------
[DataContract(IsReference = true)]
public class ViewModelBase : INotifyPropertyChanged
{
    public event PropertyChangedEventHandler? PropertyChanged;

    //-----------------------------------------------------------------------------------------------------------------
    /// OnPropertyChanged
    //-----------------------------------------------------------------------------------------------------------------
    protected void OnPropertyChanged( string propertyName )
    {
        PropertyChanged?.Invoke( this, new PropertyChangedEventArgs( propertyName ) );
    }
}
