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

    private string FullPath => $"{Path}{Name}{Extension}";

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

    public ICommand? Undo { get; private set; }
    public ICommand? Redo { get; private set; }

    public ICommand? AddScene    { get; private set; }
    public ICommand? RemoveScene { get; private set; }

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
        
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Save
    //-----------------------------------------------------------------------------------------------------------------
    public static void Save( Project project )
    {
        Serializer.ToFile( project, project.FullPath );
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

        AddScene = new RelayCommand< object >( x =>
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
        
        RemoveScene = new RelayCommand< Scene >( x =>
        {
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

        Undo = new RelayCommand< object >( x => UndoRedo.Undo() );
        Redo = new RelayCommand< object >( x => UndoRedo.Redo() );
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