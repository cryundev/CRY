using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Runtime.Serialization;
using Editor_WPF.Common;
using Editor_WPF.Utilities;


namespace Editor_WPF.GameProject;


//---------------------------------------------------------------------------------------------------------------------
/// ProjectTemplate
//---------------------------------------------------------------------------------------------------------------------
[DataContract]
public class ProjectTemplate
{
    [DataMember] public required string         ProjectType { get; set; }
    [DataMember] public required string         ProjectFile { get; set; }
    [DataMember] public required List< string > Folders     { get; set; }
    
    public required byte[] Icon            { get; set; }
    public required byte[] Preview         { get; set; }
    public required string IconFilePath    { get; set; }
    public required string PreviewPath     { get; set; }
    public required string ProjectFilePath { get; set; }
    public required string TemplatePath    { get; set; }
}


//---------------------------------------------------------------------------------------------------------------------
/// CreateProject
//---------------------------------------------------------------------------------------------------------------------
public class CreateProject : ViewModelBase
{
    private readonly string _templatePath = @"../../Editor_WPF/ProjectTemplates/";

    private string _projectName = "CreateProject";
    public  string ProjectName
    {
        get => _projectName;
        set
        {
            if ( _projectName == value ) return;
            
            _projectName = value;
            ValidateProjectPath();
            OnPropertyChanged( nameof( ProjectName ) );
        }
    }
    
    private string _projectPath = $@"{ Environment.GetFolderPath( Environment.SpecialFolder.MyDocuments ) }\CRYProject\";
    public  string ProjectPath
    {
        get => _projectPath;
        set
        {
            if ( _projectPath == value ) return;
            
            _projectPath = value;
            ValidateProjectPath();
            OnPropertyChanged( nameof( ProjectPath ) );
        }
    }

    private bool _isValid;
    public bool IsValid
    {
        get => _isValid;
        set
        {
            if ( _isValid == value ) return;
            
            _isValid = value;
            OnPropertyChanged( nameof( IsValid ) );
        }
    }

    private string _errorMsg = "";
    public string ErrorMsg
    {
        get => _errorMsg;
        set
        {
            if ( _errorMsg == value ) return;
            
            _errorMsg = value;
            OnPropertyChanged( nameof( ErrorMsg ) );
        }
    }
    
    private readonly ObservableCollection< ProjectTemplate > _projectTemplates = [];
    public ReadOnlyObservableCollection< ProjectTemplate > ProjectTemplates { get; }

    
    //-----------------------------------------------------------------------------------------------------------------
    /// ValidateProjectPath
    //-----------------------------------------------------------------------------------------------------------------
    private bool ValidateProjectPath()
    {
        string path = ProjectPath;
        if ( Path.EndsInDirectorySeparator( path ) )
        {
            path += @"\";
        }
        
        path += $@"{ ProjectName }\";

        IsValid = false;
        
        if ( string.IsNullOrWhiteSpace( ProjectName.Trim() ) )
        {
            ErrorMsg = "Type in a project name";
        }
        else if ( ProjectName.IndexOfAny( Path.GetInvalidFileNameChars() ) != -1 )
        {
            ErrorMsg = "Invalid character(s) used in project name";
        }
        else if ( string.IsNullOrWhiteSpace( ProjectPath.Trim() ) )
        {
            ErrorMsg = "Select a valid project folder";
        }
        else if ( ProjectPath.IndexOfAny( Path.GetInvalidPathChars() ) != -1 )
        {
            ErrorMsg = "Invalid character(s) used in project path";
        }
        else if ( Directory.Exists( path ) && Directory.EnumerateFileSystemEntries( path ).Any() )
        {
            ErrorMsg = "Select project folder already exists and is not empty";
        }
        else
        {
            ErrorMsg = string.Empty;
            IsValid = true;
        }

        return IsValid;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// CreateNewProject
    //-----------------------------------------------------------------------------------------------------------------
    public string CreateNewProject( ProjectTemplate template )
    {
        ValidateProjectPath();
        
        if ( !IsValid )
        {
            return string.Empty;
        }
        
        if ( !Path.EndsInDirectorySeparator( ProjectPath ) )
        {
            ProjectPath += @"\";
        }
        
        string path = $@"{ ProjectPath }{ ProjectName }\";

        try
        {
            if ( !Directory.Exists( path ) )
            {
                Directory.CreateDirectory( path );
            }
            
            string pathName = Path.GetDirectoryName( path ) ?? throw new InvalidOperationException();

            foreach ( string folder in template.Folders )
            {
                Directory.CreateDirectory( Path.GetFullPath( Path.Combine( Path.GetDirectoryName( path ) ?? throw new InvalidOperationException(), folder ) ) );
            }
            
            DirectoryInfo dirInfo = new DirectoryInfo( path + @".cryproject\" );
            dirInfo.Attributes |= FileAttributes.Hidden;
            
            File.Copy( template.IconFilePath, Path.GetFullPath( Path.Combine( dirInfo.FullName, "Icon.png" ) ) );
            File.Copy( template.PreviewPath, Path.GetFullPath( Path.Combine( dirInfo.FullName, "Preview.png" ) ) );
            
            string projectXml = File.ReadAllText( template.ProjectFilePath );
            projectXml = string.Format( projectXml, ProjectName, path );
            
            string projectPath = Path.GetFullPath( Path.Combine( path, $"{ProjectName}{Project.Extension}" ) );
            File.WriteAllText( projectPath, projectXml );

            CreateMSVCSolution( template, path );

            return path;
        }
        catch ( Exception e )
        {
            Debug.WriteLine( e.Message );
            
            Logger.Log( MessageType.Error, $"Failed to create {ProjectName}" );

            throw;
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// CreateMSVCSolution
    //-----------------------------------------------------------------------------------------------------------------
    private void CreateMSVCSolution( ProjectTemplate template, string path )
    {
        string msvcSolutionPath = Path.Combine( template.TemplatePath, "MSVCSolution" );
        string msvcProjectPath  = Path.Combine( template.TemplatePath, "MSVCProject"  );
        
        Debug.Assert( File.Exists( msvcSolutionPath ) );
        Debug.Assert( File.Exists( msvcProjectPath  ) );
        
        string engineSourcePath = Path.Combine( MainWindow.EnginePath, @"Engine\" );
        Debug.Assert( Directory.Exists( engineSourcePath ) );

        string _0 = ProjectName;
        string _1 = "{" + Guid.NewGuid().ToString().ToUpper() + "}";
        string _2 = engineSourcePath;
        string _3 = MainWindow.EnginePath;

        string solution = File.ReadAllText( msvcSolutionPath );
        solution = string.Format( solution, _0, _1, "{" + Guid.NewGuid().ToString().ToUpper() + "}" );
        
        File.WriteAllText( Path.GetFullPath( Path.Combine( path, $"{_0}.sln" ) ), solution );

        string project = File.ReadAllText( msvcProjectPath );
        project = string.Format( project, _0, _1, _2, _3 );
        
        File.WriteAllText( Path.GetFullPath( Path.Combine( path, $@"Source\{_0}.vcxproj" ) ), project );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// CreateProject
    //-----------------------------------------------------------------------------------------------------------------
    public CreateProject()
    {
        ProjectTemplates = new ReadOnlyObservableCollection< ProjectTemplate >( _projectTemplates );
        
        try
        {
            string[] templatesFiles = Directory.GetFiles( _templatePath, "template.xml", SearchOption.AllDirectories );

            Debug.Assert( templatesFiles.Any() );

            foreach ( string file in templatesFiles )
            {
                ProjectTemplate template = Serializer.FromFile< ProjectTemplate >( file );

                string filePathName = Path.GetDirectoryName( file ) ?? throw new InvalidOperationException();

                template.TemplatePath    = filePathName;
                template.IconFilePath    = Path.GetFullPath( Path.Combine( filePathName, "Icon.png"    ) );
                template.PreviewPath     = Path.GetFullPath( Path.Combine( filePathName, "Preview.png" ) );;
                template.ProjectFilePath = Path.GetFullPath( Path.Combine( filePathName, template.ProjectFile ) );
                
                
                template.Icon    = File.ReadAllBytes( template.IconFilePath );
                template.Preview = File.ReadAllBytes( template.PreviewPath  );
                
                _projectTemplates.Add( template );

                ValidateProjectPath();
            }
        }
        catch ( Exception e )
        {
            Debug.WriteLine( e.Message );
            
            Logger.Log( MessageType.Error, "Failed to load project templates" );

            throw;
        }
    }
}
