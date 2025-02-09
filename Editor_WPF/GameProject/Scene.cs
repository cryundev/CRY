using System.Diagnostics;
using System.Runtime.Serialization;
using Editor_WPF.Common;


namespace Editor_WPF.GameProject;


[DataContract]
public class Scene : ViewModelBase
{
    private string _name;
    
    [DataMember]
    public string Name
    {
        get => _name;
        set
        {
            if ( _name != value )
            {
                _name = value;
                OnPropertyChanged( nameof( Name ) );
            }
        }
    }
    
    [DataMember]
    public Project Project { get; private set; }
    
    public Scene( Project InPoject, string Iname )
    {
        Debug.Assert( InPoject != null );
        
        Project = InPoject;
        Name    = Iname;
    }
}