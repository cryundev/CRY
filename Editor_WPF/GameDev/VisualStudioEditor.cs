using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.ComTypes;
using Editor_WPF.GameProject;
using Editor_WPF.Utilities;
using EnvDTE;
using EnvDTE80;
using Path = System.IO.Path;


namespace Editor_WPF.GameDev;


//---------------------------------------------------------------------------------------------------------------------
/// EnvDTEConstants
//---------------------------------------------------------------------------------------------------------------------
internal abstract class EnvDTEConstants
{
    public const string vsViewKindTextView = "{7651A703-06E5-11D1-8EBD-00A0C90F26EA}";
}


//---------------------------------------------------------------------------------------------------------------------
/// VisualStudioEditor
//---------------------------------------------------------------------------------------------------------------------
public class VisualStudioEditor : ICodeEditor
{
    private DTE2? _vsInstance;
    private const string ProgramId = "VisualStudio.DTE.17.0";

    public bool BuildSucceeded { get; private set; } = true;
    public bool BuildDone { get; private set; } = true;
    
    public event EventHandler< bool >? BuildCompleted;

    [DllImport( "ole32.dll" )]
    private static extern int CreateBindCtx( uint reserved, out IBindCtx? ppBindCtx );

    [DllImport( "ole32.dll" )]
    private static extern int GetRunningObjectTable( uint reserved, out IRunningObjectTable? ppRunningObjectTable );

    //-----------------------------------------------------------------------------------------------------------------
    /// OpenEditor
    //-----------------------------------------------------------------------------------------------------------------
    public void OpenEditor( string solutionPath )
    {
        try
        {
            FindOrCreateVSInstance();

            if ( _vsInstance != null )
            {
                if ( !_vsInstance.Solution.IsOpen || _vsInstance.Solution.FullName != solutionPath )
                {
                    _vsInstance.Solution.Open( solutionPath );
                }

                _vsInstance.MainWindow.Activate();
                _vsInstance.MainWindow.Visible = true;
            }
        }
        catch ( Exception ex )
        {
            Debug.WriteLine( ex.Message );
            Logger.Log( MessageType.Error, $"Failed to open Visual Studio: {ex.Message}" );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// CloseEditor
    //-----------------------------------------------------------------------------------------------------------------
    public void CloseEditor()
    {
        try
        {
            if ( _vsInstance?.Solution.IsOpen == true )
            {
                _vsInstance.ExecuteCommand( "File.SaveAll" );
                _vsInstance.Solution.Close();
            }

            _vsInstance?.Quit();
            _vsInstance = null;
        }
        catch ( Exception ex )
        {
            Debug.WriteLine( ex.Message );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// AddFilesToSolution
    //-----------------------------------------------------------------------------------------------------------------
    public bool AddFilesToSolution( string solution, string projectName, string[] files )
    {
        OpenEditor( solution );

        try
        {
            if ( _vsInstance != null )
            {
                if ( !_vsInstance.Solution.IsOpen ) _vsInstance.Solution.Open( solution );
                else _vsInstance.ExecuteCommand( "File.SaveAll" );

                foreach ( EnvDTE.Project project in _vsInstance.Solution.Projects )
                {
                    if ( project.UniqueName.Contains( projectName ) )
                    {
                        foreach ( var file in files )
                        {
                            project.ProjectItems.AddFromFile( file );
                        }
                    }
                }

                string? cpp = files.FirstOrDefault( x => Path.GetExtension( x ) == ".cpp" );

                if ( !string.IsNullOrEmpty( cpp ) )
                {
                    _vsInstance.ItemOperations.OpenFile( cpp, EnvDTEConstants.vsViewKindTextView ).Visible = true;
                }

                _vsInstance.MainWindow.Activate();
                _vsInstance.MainWindow.Visible = true;
            }
        }
        catch ( Exception exception )
        {
            Debug.WriteLine( exception.Message );
            Debug.WriteLine( "Failed to add files to Visual Studio project" );
            return false;
        }

        return true;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// BuildSolution
    //-----------------------------------------------------------------------------------------------------------------
    public void BuildSolution( Editor_WPF.GameProject.ProjectViewModel project, string configName, bool showWindow = true )
    {
        if ( IsDebugging() )
        {
            Logger.Log( MessageType.Error, "Visual Studio is currently running a process." );
            return;
        }

        OpenEditor( project.Solution );
        BuildDone = BuildSucceeded = false;

        TimeSpan timeout    = TimeSpan.FromMinutes( 2 );
        DateTime startTime  = DateTime.Now;
        TimeSpan retryDelay = TimeSpan.FromSeconds( 1 );

        while ( DateTime.Now - startTime < timeout )
        {
            try
            {
                if ( _vsInstance != null && !_vsInstance.Solution.IsOpen )
                {
                    _vsInstance.Solution.Open( project.Solution );
                }

                if ( _vsInstance != null )
                {
                    _vsInstance.MainWindow.Visible = showWindow;

                    _vsInstance.Events.BuildEvents.OnBuildProjConfigBegin += OnBuildSolutionBegin;
                    _vsInstance.Events.BuildEvents.OnBuildProjConfigDone  += OnBuildSolutionDone;
                    
                    try
                    {
                        foreach ( var pdbFile in Directory.GetFiles( Path.Combine( $"{project.Path}", $@"x64\{configName}" ), "*.pbd" ) )
                        {
                            File.Delete( pdbFile );    
                        }
                    }
                    catch ( Exception ex )
                    {
                        Debug.WriteLine( ex.Message );
                    }

                    _vsInstance.Solution.SolutionBuild.SolutionConfigurations.Item( configName ).Activate();
                    _vsInstance.ExecuteCommand( "Build.BuildSolution" );
                    return;
                }
            }
            catch ( Exception exception )
            {
                TimeSpan elapsed = DateTime.Now - startTime;
                Debug.WriteLine( $"Build attempt failed after {elapsed.TotalSeconds:F1}s: {exception.Message}" );
                
                if ( DateTime.Now - startTime >= timeout )
                {
                    Logger.Log( MessageType.Error, $"Failed to build {project.Name} - timeout after {timeout.TotalMinutes} minutes" );
                    return;
                }
                
                System.Threading.Thread.Sleep( retryDelay );
            }
        }

        Logger.Log( MessageType.Error, $"Failed to build {project.Name} - timeout reached" );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// IsDebugging
    //-----------------------------------------------------------------------------------------------------------------
    public bool IsDebugging()
    {
        bool result = false;

        try
        {
            result = _vsInstance != null && ( _vsInstance.Debugger.CurrentProgram != null || _vsInstance.Debugger.CurrentMode == EnvDTE.dbgDebugMode.dbgRunMode );
        }
        catch ( Exception exception )
        {
            Debug.Write( exception.Message );

            if ( !result )
            {
                System.Threading.Thread.Sleep( 1000 );
            }
        }

        return result;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnBuildSolutionDone
    //-----------------------------------------------------------------------------------------------------------------
    private void OnBuildSolutionDone( string project, string projectconfig, string platform, string solutionconfig, bool success )
    {
        if ( BuildDone ) return;

        if ( success )
        {
            Logger.Log( MessageType.Info, $"Building {projectconfig} configuration succeeded" );
        }
        else
        {
            Logger.Log( MessageType.Error, $"Building {projectconfig} configuration failed" );
        }

        BuildDone = true;
        BuildSucceeded = success;
        
        BuildCompleted?.Invoke( this, success );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// OnBuildSolutionBegin
    //-----------------------------------------------------------------------------------------------------------------
    private void OnBuildSolutionBegin( string project, string projectconfig, string platform, string solutionconfig )
    {
        Logger.Log( MessageType.Info, $"Building {project}, {projectconfig}, {platform}, {solutionconfig}" );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// FindOrCreateVSInstance
    //-----------------------------------------------------------------------------------------------------------------
    private void FindOrCreateVSInstance()
    {
        if ( _vsInstance != null ) return;

        IRunningObjectTable? runningObjectTable = null;
        IEnumMoniker? monikerTable = null;
        IBindCtx? bindCtx = null;

        try
        {
            int result = GetRunningObjectTable( 0, out runningObjectTable );
            if ( result < 0 || runningObjectTable == null )
            {
                throw new COMException( $"GetRunningObjectTable() returned HRESULT : {result:X8}" );
            }

            runningObjectTable.EnumRunning( out monikerTable );
            monikerTable.Reset();

            result = CreateBindCtx( 0, out bindCtx );
            if ( result < 0 || bindCtx == null )
            {
                throw new COMException( $"CreateBindCtx() returned HRESULT : {result:X8}" );
            }

            IMoniker[] currentMoniker = new IMoniker[ 1 ];
            while ( monikerTable.Next( 1, currentMoniker, IntPtr.Zero ) == 0 )
            {
                currentMoniker[ 0 ].GetDisplayName( bindCtx, null, out string name );

                if ( name.Contains( ProgramId ) )
                {
                    result = runningObjectTable.GetObject( currentMoniker[ 0 ], out object obj );
                    if ( result >= 0 && obj is DTE2 dte )
                    {
                        _vsInstance = dte;
                        return;
                    }
                }
            }

            // 실행 중인 인스턴스가 없으면 새로 생성
            Type? type = Type.GetTypeFromProgID( ProgramId );
            if ( type != null )
            {
                _vsInstance = Activator.CreateInstance( type ) as DTE2;
            }
        }
        catch ( Exception ex )
        {
            Debug.WriteLine( ex.Message );
            throw;
        }
        finally
        {
            if ( monikerTable != null ) Marshal.ReleaseComObject( monikerTable );
            if ( runningObjectTable != null ) Marshal.ReleaseComObject( runningObjectTable );
            if ( bindCtx != null ) Marshal.ReleaseComObject( bindCtx );
        }
    }
}