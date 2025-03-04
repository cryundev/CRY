using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Runtime.Serialization;
using Editor_WPF.Utilities;


namespace Editor_WPF.GameProject;


//---------------------------------------------------------------------------------------------------------------------
/// ProjectData
//---------------------------------------------------------------------------------------------------------------------
[DataContract]
public class ProjectData
{
    [DataMember]
    public required string ProjectName { get; set; }
    
    [DataMember]
    public required string ProjectPath { get; set; }
    
    [DataMember]
    public DateTime Date { get; set; }
    
    public string? FullPath => $"{ProjectPath}{ProjectName}{Project.Extension}";
    public byte[]? Icon { get; set; }
    public byte[]? Preview { get; set; }
}


//---------------------------------------------------------------------------------------------------------------------
/// ProjectDataList
//---------------------------------------------------------------------------------------------------------------------
[DataContract]
public class ProjectDataList
{
    [DataMember]
    public required List< ProjectData > Projects { get; set; }
}


//---------------------------------------------------------------------------------------------------------------------
/// OpenProject
//---------------------------------------------------------------------------------------------------------------------
public class OpenProject
{
    private static readonly string ApplicationDataPath = $@"{ Environment.GetFolderPath( Environment.SpecialFolder.ApplicationData ) }\Editor_WPF\";
    
    private static readonly string ProjectDataPath;
    
    private static readonly ObservableCollection< ProjectData > _projects = new ObservableCollection< ProjectData >();
    public static ReadOnlyCollection< ProjectData >? Projects { get; }


    //-----------------------------------------------------------------------------------------------------------------
    /// ReadProjectData
    //-----------------------------------------------------------------------------------------------------------------
    private static void ReadProjectData()
    {
        if ( !File.Exists( ProjectDataPath ) ) return;
        
        IOrderedEnumerable< ProjectData > projects = Serializer.FromFile< ProjectDataList >( ProjectDataPath )
            .Projects.OrderByDescending( x => x.Date );
            
        _projects.Clear();

        foreach ( ProjectData project in projects )
        {
            if ( !File.Exists( project.FullPath ) ) continue;
            
            project.Icon    = File.ReadAllBytes( $@"{project.ProjectPath}\.cryproject\Icon.png"    );
            project.Preview = File.ReadAllBytes( $@"{project.ProjectPath}\.cryproject\Preview.png" );

            _projects.Add( project );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// WriteProjectData
    //-----------------------------------------------------------------------------------------------------------------
    private static void WriteProjectData()
    {
        List< ProjectData > projects = _projects.OrderBy( x => x.Date ).ToList();
        
        Serializer.ToFile( new ProjectDataList() { Projects = projects }, ProjectDataPath );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Open
    //-----------------------------------------------------------------------------------------------------------------
    public static Project Open( ProjectData data )
    {
        ReadProjectData();
        
        ProjectData? project = _projects.FirstOrDefault( x => x.FullPath == data.FullPath );

        if ( project != null )
        {
            project.Date = DateTime.Now;
        }
        else
        {
            project = data;
            project.Date = DateTime.Now;
            
            _projects.Add( project );
        }

        WriteProjectData();

        return Project.Load( project.FullPath );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OpenProject
    //-----------------------------------------------------------------------------------------------------------------
    static OpenProject()
    {
        ProjectDataPath = $@"{ApplicationDataPath}ProjectData.xml";
        
        try
        {
            if ( !Directory.Exists( ApplicationDataPath ) )
            {
                Directory.CreateDirectory( ApplicationDataPath );
            }

            Projects = new ReadOnlyObservableCollection< ProjectData >( _projects );
            
            ReadProjectData();
        }
        catch ( Exception e )
        {
            Debug.WriteLine( e.Message );

            Logger.Log( MessageType.Error, $"Failed to read project data" );

            throw;
        }
    }
}