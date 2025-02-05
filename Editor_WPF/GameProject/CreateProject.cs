using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Runtime.Serialization;
using Editor_WPF;
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
                    OnPropertyChanged( nameof( ProjectPath ) );
                }
            }
        }
        
        private ObservableCollection< ProjectTemplate > _projectTemplates = new ObservableCollection< ProjectTemplate >();
        public ReadOnlyObservableCollection< ProjectTemplate > ProjectTemplates { get; }

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
                    _projectTemplates.Add( template );
                }
            }
            catch ( Exception e )
            {
                Debug.WriteLine( e.Message );
            }
        }
    }
}