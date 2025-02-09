using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Runtime.Serialization;
using Editor_WPF;
using Editor_WPF.Common;
using Editor_WPF.Utilities;


namespace Editor_WPF.GameProject
{
    [DataContract]
    public class ProjectTemplate
    {
        [DataMember] public string         ProjectType { get; set; }
        [DataMember] public string         ProjectFile { get; set; }
        [DataMember] public List< string > Folders     { get; set; }
        
        public byte[] Icon            { get; set; }
        public byte[] Preview         { get; set; }
        public string IconFilePath    { get; set; }
        public string PreviewPath     { get; set; }
        public string ProjectFilePath { get; set; }
        
    }

    public class CreateProject : ViewModelBase
    {
        private readonly string _templatePath = @"../../Editor_WPF/ProjectTemplates/";
        
        private string _projectName = "CreateProject";
        public  string ProjectName
        {
            get => _projectName;
            set
            {
                if ( _projectName != value )
                {
                    _projectName = value;
                    ValidateProjectPath();
                    OnPropertyChanged( nameof( ProjectName ) );
                }
            }
        }
        
        private string _projectPath = $@"{ Environment.GetFolderPath( Environment.SpecialFolder.MyDocuments ) }\CRYProject\";
        public  string ProjectPath
        {
            get => _projectPath;
            set
            {
                if ( _projectPath != value )
                {
                    _projectPath = value;
                    ValidateProjectPath();
                    OnPropertyChanged( nameof( ProjectPath ) );
                }
            }
        }

        private bool _isValid;
        public bool IsValid
        {
            get => _isValid;
            set
            {
                if ( _isValid != value )
                {
                    _isValid = value;
                    OnPropertyChanged( nameof( IsValid ) );
                }
            }
        }

        private string _errorMsg;
        public string ErrorMsg
        {
            get => _errorMsg;
            set
            {
                if ( _errorMsg != value )
                {
                    _errorMsg = value;
                    OnPropertyChanged( nameof( ErrorMsg ) );
                }
            }
        }
        
        private ObservableCollection< ProjectTemplate > _projectTemplates = new ObservableCollection< ProjectTemplate >();
        public ReadOnlyObservableCollection< ProjectTemplate > ProjectTemplates { get; }

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

        public string CreateNewProject( ProjectTemplate InTemplate )
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

                foreach ( string folder in InTemplate.Folders )
                {
                    Directory.CreateDirectory( Path.GetFullPath( Path.Combine( Path.GetDirectoryName( path ), folder ) ) );
                }
                
                var dirInfo = new DirectoryInfo( path + @".cryproject\" );
                dirInfo.Attributes |= FileAttributes.Hidden;
                
                File.Copy( InTemplate.IconFilePath, Path.GetFullPath( Path.Combine( dirInfo.FullName, "Icon.png" ) ) );
                File.Copy( InTemplate.PreviewPath, Path.GetFullPath( Path.Combine( dirInfo.FullName, "Preview.png" ) ) );
                
                string projectXml = File.ReadAllText( InTemplate.ProjectFilePath );
                projectXml = string.Format( projectXml, ProjectName, ProjectPath );
                
                string projectPath = Path.GetFullPath( Path.Combine( path, $"{ProjectName}{Project.Extension}" ) );
                File.WriteAllText( projectPath, projectXml );

                return path;
            }
            catch ( Exception e )
            {
                Debug.WriteLine( e.Message );

                return String.Empty;
            }
        }

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
                    
                    template.IconFilePath    = Path.GetFullPath( Path.Combine( Path.GetDirectoryName( file ), "Icon.png"    ) );
                    template.PreviewPath     = Path.GetFullPath( Path.Combine( Path.GetDirectoryName( file ), "Preview.png" ) );;
                    template.ProjectFilePath = Path.GetFullPath( Path.Combine( Path.GetDirectoryName( file ), template.ProjectFile ) );
                    
                    template.Icon    = File.ReadAllBytes( template.IconFilePath );
                    template.Preview = File.ReadAllBytes( template.PreviewPath  );
                    
                    _projectTemplates.Add( template );

                    ValidateProjectPath();
                }
            }
            catch ( Exception e )
            {
                Debug.WriteLine( e.Message );
            }
        }
        
        
    }
}