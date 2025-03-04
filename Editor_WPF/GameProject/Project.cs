using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Runtime.Serialization;
using System.Windows;
using System.Windows.Input;
using Editor_WPF.Common;
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

    private string FullPath => $@"{Path}{Name}\{Name}{Extension}";

    [DataMember( Name = "Scenes" )]
    private ObservableCollection< Scene > _scenes = [];
    public ReadOnlyObservableCollection< Scene >? Scenes { get; private set; }

    [DataMember( Name = "ActiveScene" )]
    private Scene? _activeScene;
    public Scene? ActiveScene
    {
        get => _activeScene;
        set
        {
            if ( _activeScene == value ) return;
            
            _activeScene = value;
            OnPropertyChanged( nameof( ActiveScene ) );
        }
    }
    
    public static Project? Current => Application.Current.MainWindow?.DataContext as Project;

    public static UndoRedo UndoRedo { get; } = new UndoRedo();

    public ICommand? UndoCommand { get; private set; }
    public ICommand? RedoCommand { get; private set; }

    public ICommand? AddSceneCommand    { get; private set; }
    public ICommand? RemoveSceneCommand { get; private set; }
    public ICommand? SaveCommand        { get; private set; }

    //-----------------------------------------------------------------------------------------------------------------
    /// AddSceneInternal
    //-----------------------------------------------------------------------------------------------------------------
    private void AddSceneInternal( string sceneName )
    {
        Debug.Assert( !string.IsNullOrEmpty( sceneName.Trim() ) );
        _scenes.Add( new Scene( this, sceneName ) );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// RemoveSceneInternal
    //-----------------------------------------------------------------------------------------------------------------
    private void RemoveSceneInternal( Scene scene )
    {
        Debug.Assert( _scenes.Contains( scene ) );
        _scenes.Remove( scene );
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
        Scenes = new ReadOnlyObservableCollection< Scene >( _scenes );
        OnPropertyChanged( nameof( Scenes ) );

        ActiveScene = Scenes.FirstOrDefault( x => x is { IsActive: true } );

        AddSceneCommand = new RelayCommand< object >( _ =>
        {
            AddSceneInternal( $"New Scene {_scenes.Count}" );
            
            Scene newScene = _scenes.Last();
            int sceneIndex = _scenes.Count - 1;
            
            UndoRedoAction action = new UndoRedoAction
            ( 
                () => RemoveSceneInternal( newScene ),
                () => _scenes.Insert( sceneIndex, newScene ),
                $"Add {newScene.Name}"
            ); 
            
            UndoRedo.Add( action );
        } );
        
        RemoveSceneCommand = new RelayCommand< Scene >( x =>
        {
            if ( x == null ) throw new ArgumentNullException( nameof( x ) );
            int sceneIndex = _scenes.IndexOf( x );
            
            RemoveSceneInternal( x );
            
            UndoRedoAction action = new UndoRedoAction
            ( 
                () => _scenes.Insert( sceneIndex, x ),
                () => RemoveSceneInternal( x ),
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
        
        _scenes.Add( new Scene( this, "Default Scene" ) );
    }
}