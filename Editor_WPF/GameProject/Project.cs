using System.Collections.ObjectModel;
using System.Runtime.Serialization;
using Editor_WPF.Common;


namespace Editor_WPF.GameProject;


[DataContract( Name = "Game" )]
public class Project : ViewModelBase
{
    public static string Extension { get; } = ".cryproject";

    [DataMember]
    public string Name { get; private set; }

    [DataMember]
    public string Path { get; private set; }
    
    public string FullPath => $"{Path}{Name}{Extension}";

    [DataMember( Name = "Scenes" )]
    private ObservableCollection< Scene > _scenes = new ObservableCollection< Scene >();
    public ReadOnlyObservableCollection< Scene > Scenes { get; }

    public Project( string InName, string InPath )
    {
        Name = InName;
        Path = InPath;
        
        _scenes.Add( new Scene( this, "Default Scene" ) );
    }
}