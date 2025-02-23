using System.Diagnostics;
using System.Runtime.Serialization;
using Editor_WPF.Common;


namespace Editor_WPF.GameProject;


//---------------------------------------------------------------------------------------------------------------------
/// Scene
//---------------------------------------------------------------------------------------------------------------------
[DataContract]
public class Scene : ViewModelBase
{
    [DataMember( Name = "Name" )]
    private string _name;
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
    public Project Project { get; private set; }

    [DataMember( Name = "IsActive" )]
    private bool _isActive;
    public bool IsActive
    {
        get => _isActive;
        set
        {
            if ( _isActive == value ) return;
            
            _isActive = value;
            OnPropertyChanged( nameof( IsActive ) );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Scene
    //-----------------------------------------------------------------------------------------------------------------
    public Scene( Project poject, string name )
    {
        Debug.Assert( poject != null );
        
        Project = poject;
        _name   = name;
    }
}