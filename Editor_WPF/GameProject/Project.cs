using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Runtime.Serialization;
using System.Windows;
using System.Windows.Input;
using Editor_WPF.Common;
using Editor_WPF.GameDev;
using Editor_WPF.Utilities;


namespace Editor_WPF.GameProject;


//---------------------------------------------------------------------------------------------------------------------
/// Project
//---------------------------------------------------------------------------------------------------------------------
[DataContract( Name = "Game" )]
public class Project : ViewModelBase
{
    public static string Extension { get; } = ".cryproject";

    [DataMember]
    public string Name { get; private set; }

    [DataMember]
    public string Path { get; private set; }

    public string FullPath => $@"{Path}{Name}{Extension}";
    public string Solution => $@"{Path}{Name}.sln";

    [DataMember( Name = "Worlds" )]
    private ObservableCollection< World > _worlds = [];
    public ReadOnlyObservableCollection< World >? Worlds { get; private set; }

    [DataMember( Name = "ActiveWorld" )]
    private World? _activeWorld;
    public World? ActiveWorld
    {
        get => _activeWorld;
        set
        {
            if ( _activeWorld == value ) return;
            
            _activeWorld = value;
            OnPropertyChanged( nameof( ActiveWorld ) );
        }
    }
    
    public static Project? Current => Application.Current.MainWindow?.DataContext as Project;

    public static UndoRedo UndoRedo { get; } = new UndoRedo();

    public ICommand? UndoCommand { get; private set; }
    public ICommand? RedoCommand { get; private set; }

    public ICommand? AddWorldCommand    { get; private set; }
    public ICommand? RemoveWorldCommand { get; private set; }
    public ICommand? SaveCommand        { get; private set; }

    //-----------------------------------------------------------------------------------------------------------------
    /// AddWorldInternal
    //-----------------------------------------------------------------------------------------------------------------
    private void AddWorldInternal( string worldName )
    {
        Debug.Assert( !string.IsNullOrEmpty( worldName.Trim() ) );
        _worlds.Add( new World( this, worldName ) );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// RemoveWorldInternal
    //-----------------------------------------------------------------------------------------------------------------
    private void RemoveWorldInternal( World world )
    {
        Debug.Assert( _worlds.Contains( world ) );
        _worlds.Remove( world );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Load
    //-----------------------------------------------------------------------------------------------------------------
    public static Project Load( string? file )
    {
        Debug.Assert( File.Exists( file ) );
        return Serializer.FromFile< Project >( file );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Unload
    //-----------------------------------------------------------------------------------------------------------------
    public void Unload()
    {
        VisualStudio.CloseVisualStudio();
        UndoRedo.Reset();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Save
    //-----------------------------------------------------------------------------------------------------------------
    public static void Save( Project project )
    {
        Serializer.ToFile( project, project.FullPath );
        
        Logger.Log( MessageType.Info, $"Project saved to {project.FullPath}" );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnDeserialized
    //-----------------------------------------------------------------------------------------------------------------
    [OnDeserialized]
    private void OnDeserialized( StreamingContext context )
    {
        Worlds = new ReadOnlyObservableCollection< World >( _worlds );
        OnPropertyChanged( nameof( Worlds ) );

        ActiveWorld = Worlds.FirstOrDefault( x => x is { IsActive: true } );

        AddWorldCommand = new RelayCommand< object >( _ =>
        {
            AddWorldInternal( $"New World {_worlds.Count}" );
            
            World newWorld = _worlds.Last();
            int worldIndex = _worlds.Count - 1;
            
            UndoRedoAction action = new UndoRedoAction
            ( 
                () => RemoveWorldInternal( newWorld ),
                () => _worlds.Insert( worldIndex, newWorld ),
                $"Add {newWorld.Name}"
            ); 
            
            UndoRedo.Add( action );
        } );
        
        RemoveWorldCommand = new RelayCommand< World >( x =>
        {
            if ( x == null ) throw new ArgumentNullException( nameof( x ) );
            int worldIndex = _worlds.IndexOf( x );
            
            RemoveWorldInternal( x );
            
            UndoRedoAction action = new UndoRedoAction
            ( 
                () => _worlds.Insert( worldIndex, x ),
                () => RemoveWorldInternal( x ),
                $"Remove {x.Name}"
            );
            
            UndoRedo.Add( action );
        }, x => !x.IsActive );

        UndoCommand = new RelayCommand< object >( _ => UndoRedo.Undo() );
        RedoCommand = new RelayCommand< object >( _ => UndoRedo.Redo() );
        SaveCommand = new RelayCommand< object >( _ => Save( this ) );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Project
    //-----------------------------------------------------------------------------------------------------------------
    public Project( string name, string path )
    {
        Name = name;
        Path = path;
        
        OnDeserialized( new StreamingContext() );
        
        _worlds.Add( new World( this, "Default World" ) );
    }
}