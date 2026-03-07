using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Runtime.Serialization;
using System.Windows;
using System.Windows.Input;
using Editor_WPF.Common;
using Editor_WPF.DllWrappers;
using Editor_WPF.GameDev;
using Editor_WPF.Utilities;


namespace Editor_WPF.GameProject;


public enum BuildConfiguration
{
    Debug,
    DebugEditor,
    Release,
    ReleaseEditor
}


//---------------------------------------------------------------------------------------------------------------------
/// ProjectViewModel
//---------------------------------------------------------------------------------------------------------------------
[DataContract( Name = "Game" )]
public class ProjectViewModel : ViewModelBase
{
    /// Static Members
    private static readonly string[] _buildConfigurationNames = new string[] { "Debug", "DebugEditor", "Release", "ReleaseEditor" };

    public static string            Extension { get; } = ".cryproject";
    public static UndoRedo          UndoRedo  { get; } = new UndoRedo();
    public static ProjectViewModel? Current   => Application.Current.MainWindow?.DataContext as ProjectViewModel;


    /// Basic Info Properties
    [DataMember] public string Name { get; private set; }
    [DataMember] public string Path { get; private set; }

    public string FullPath => $@"{Path}{Name}{Extension}";
    public string Solution => $@"{Path}{Name}.sln";


    /// World Management Properties
    [DataMember( Name = "Worlds"      )] private ObservableCollection< WorldViewModel > _worlds      = [];
    [DataMember( Name = "ActiveWorld" )] private WorldViewModel?                        _activeWorld;

    public ReadOnlyObservableCollection< WorldViewModel >? Worlds { get; private set; }
    public WorldViewModel? ActiveWorld
    {
        get => _activeWorld;
        set
        {
            if ( _activeWorld == value ) return;
            _activeWorld = value;
            OnPropertyChanged( nameof( ActiveWorld ) );
        }
    }


    /// Build Configuration Properties
    [DataMember] private int _buildConfig;

    public int BuildConfig
    {
        get => _buildConfig;
        set
        {
            if ( _buildConfig == value ) return;
            _buildConfig = value;
            OnPropertyChanged( nameof( BuildConfig ) );
        }
    }
    
    public BuildConfiguration StandAloneBuildConfig => BuildConfig == 0 ? BuildConfiguration.Debug       : BuildConfiguration.Release;
    public BuildConfiguration DllBuildConfig        => BuildConfig == 0 ? BuildConfiguration.DebugEditor : BuildConfiguration.ReleaseEditor;


    /// Script Management Properties
    private string[] _availableScripts = [];

    public string[] AvailableScripts
    {
        get => _availableScripts;
        set
        {
            if ( _availableScripts != value )
            {
                _availableScripts = value;
                OnPropertyChanged( nameof( AvailableScripts ) );
            }
        }
    }


    /// Commands
    public ICommand? UndoCommand        { get; private set; }
    public ICommand? RedoCommand        { get; private set; }
    public ICommand? AddWorldCommand    { get; private set; }
    public ICommand? RemoveWorldCommand { get; private set; }
    public ICommand? SaveCommand        { get; private set; }
    public ICommand? BuildCommand       { get; private set; }


    /// Methods 
    
    private static string GetConfigurationName( BuildConfiguration configuration ) => _buildConfigurationNames[ (int)configuration ];

    //-----------------------------------------------------------------------------------------------------------------
    /// AddWorldInternal
    //-----------------------------------------------------------------------------------------------------------------
    private void AddWorldInternal( string worldName )
    {
        Debug.Assert( !string.IsNullOrEmpty( worldName.Trim() ) );
        _worlds.Add( new WorldViewModel( this, worldName ) );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// RemoveWorldInternal
    //-----------------------------------------------------------------------------------------------------------------
    private void RemoveWorldInternal( WorldViewModel world )
    {
        Debug.Assert( _worlds.Contains( world ) );
        _worlds.Remove( world );
    }


    //-----------------------------------------------------------------------------------------------------------------
    /// Load
    //-----------------------------------------------------------------------------------------------------------------
    public static ProjectViewModel Load( string? file )
    {
        Debug.Assert( File.Exists( file ) );
        return Serializer.FromFile< ProjectViewModel >( file );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Unload
    //-----------------------------------------------------------------------------------------------------------------
    public void Unload()
    {
        CodeEditorManager.CloseEditor();
        UndoRedo.Reset();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Save
    //-----------------------------------------------------------------------------------------------------------------
    public static void Save( ProjectViewModel project )
    {
        Serializer.ToFile( project, project.FullPath );

        Logger.Log( MessageType.Info, $"Project saved to {project.FullPath}" );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// BuildGameCodeDll
    //-----------------------------------------------------------------------------------------------------------------
    private void BuildGameCodeDll( bool showWindow = true )
    {
        _ = BuildGameCodeDllAsync( showWindow );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// BuildGameCodeDllAsync
    //-----------------------------------------------------------------------------------------------------------------
    private async Task BuildGameCodeDllAsync( bool showWindow = true )
    {
        try
        {
            await Task.Run( () =>
            {
                UnloadGameCodeDll();

                CodeEditorManager.BuildCompleted -= OnBuildCompleted;
                CodeEditorManager.BuildCompleted += OnBuildCompleted;

                try
                {
                    CodeEditorManager.BuildSolution( this, GetConfigurationName( DllBuildConfig ), showWindow );
                }
                catch
                {
                    CodeEditorManager.BuildCompleted -= OnBuildCompleted;
                    throw;
                }
            } );
        }
        catch ( Exception exception )
        {
            Debug.WriteLine( exception.Message );
        }
    }
    
    //-----------------------------------------------------------------------------------------------------------------
    /// OnBuildCompleted
    //-----------------------------------------------------------------------------------------------------------------
    private void OnBuildCompleted( object? sender, bool success )
    {
        try
        {
            CodeEditorManager.BuildCompleted -= OnBuildCompleted;
            
            if ( success )
            {
                LoadGameCodeDll();
            }
        }
        catch ( Exception exception )
        {
            Debug.WriteLine( exception.Message );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// LoadGameCodeDll
    //-----------------------------------------------------------------------------------------------------------------
    private void LoadGameCodeDll()
    {
        string configName = GetConfigurationName( DllBuildConfig );
        string dll = $@"{Path}x64\{configName}\{Name}.dll";
        
        if ( File.Exists( dll ) && EngineAPI.LoadGameCodeDLL( dll ) != 0 )
        {
            AvailableScripts = EngineAPI.GetScriptNames();
            
            Logger.Log( MessageType.Info, "Game code DLL loaded successfully" );        
        }
        else
        {
            Logger.Log( MessageType.Warning, "Failed to load game code DLL file. Try to build the project first." );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// UnloadGameCodeDll
    //-----------------------------------------------------------------------------------------------------------------
    private void UnloadGameCodeDll()
    {
        if ( EngineAPI.UnloadGameCodeDLL() != 0 )
        {
            Logger.Log( MessageType.Info, "Game code DLL unloaded" );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Initialize
    //-----------------------------------------------------------------------------------------------------------------
    private void Initialize()
    {
        _worlds ??= [];

        foreach ( WorldViewModel world in _worlds )
        {
            world.Project = this;
        }

        Worlds = new ReadOnlyObservableCollection< WorldViewModel >( _worlds );
        OnPropertyChanged( nameof( Worlds ) );

        ActiveWorld = Worlds.FirstOrDefault( x => x is { IsActive: true } );

        InitializeCommands();

        BuildGameCodeDll( false );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// InitializeCommands
    //-----------------------------------------------------------------------------------------------------------------
    private void InitializeCommands()
    {
        AddWorldCommand = new RelayCommand< object >( _ =>
        {
            AddWorldInternal( $"New World {_worlds.Count}" );

            WorldViewModel newWorld = _worlds.Last();
            int worldIndex = _worlds.Count - 1;

            UndoRedoAction action = new UndoRedoAction
            (
                () => RemoveWorldInternal( newWorld ),
                () => _worlds.Insert( worldIndex, newWorld ),
                $"Add {newWorld.Name}"
            );

            UndoRedo.Add( action );
        } );

        RemoveWorldCommand = new RelayCommand< WorldViewModel >( x =>
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

        UndoCommand  = new RelayCommand< object >( x => UndoRedo.Undo(), x => UndoRedo.UndoList.Any() );
        RedoCommand  = new RelayCommand< object >( _ => UndoRedo.Redo(), x => UndoRedo.RedoList.Any() );
        SaveCommand  = new RelayCommand< object >( _ => Save( this ) );
        BuildCommand = new RelayCommand< bool   >( x => BuildGameCodeDll( x ), x => !CodeEditorManager.IsDebugging() && CodeEditorManager.BuildDone );

        OnPropertyChanged( nameof( AddWorldCommand ) );
        OnPropertyChanged( nameof( RemoveWorldCommand ) );
        OnPropertyChanged( nameof( UndoCommand ) );
        OnPropertyChanged( nameof( RedoCommand ) );
        OnPropertyChanged( nameof( SaveCommand ) );
        OnPropertyChanged( nameof( BuildCommand ) );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnDeserialized
    //-----------------------------------------------------------------------------------------------------------------
    [OnDeserialized]
    private void OnDeserialized( StreamingContext context )
    {
        Initialize();
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// ProjectViewModel
    //-----------------------------------------------------------------------------------------------------------------
    public ProjectViewModel( string name, string path )
    {
        Name = name;
        Path = path;

        Initialize();

        _worlds.Add( new WorldViewModel( this, "Default World" ) );
    }
}
