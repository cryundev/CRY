using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using Editor_WPF.GameProject;
using GameProject = Editor_WPF.GameProject;
using Editor_WPF.Utilities;
using Path = System.IO.Path;


namespace Editor_WPF.GameDev;


public class RiderEditor : ICodeEditor
{
    private Process? _riderInstance;

    public bool BuildSucceeded { get; private set; } = true;
    public bool BuildDone { get; private set; } = true;

    [DllImport( "user32.dll" )]
    private static extern bool ShowWindow( IntPtr hWnd, int nCmdShow );

    [DllImport( "user32.dll" )]
    private static extern bool SetForegroundWindow( IntPtr hWnd );

    private const int SW_RESTORE = 9;

    public void OpenEditor( string solutionPath )
    {
        try
        {
            if ( _riderInstance == null || _riderInstance.HasExited )
            {
                var riderPath = GetRiderExecutablePath();
                if ( string.IsNullOrEmpty( riderPath ) )
                {
                    Logger.Log( MessageType.Error, "Cannot open Rider: Rider installation not found." );
                    return;
                }

                var startInfo = new ProcessStartInfo
                {
                    FileName = riderPath,
                    Arguments = $"\"{solutionPath}\"",
                    UseShellExecute = true,
                    WindowStyle = ProcessWindowStyle.Normal
                };

                _riderInstance = Process.Start( startInfo );

                if ( _riderInstance != null )
                {
                    Logger.Log( MessageType.Info, "Rider opened successfully." );
                }
                else
                {
                    Logger.Log( MessageType.Error, "Failed to start Rider process." );
                }
            }
            else
            {
                BringRiderToFront();
            }
        }
        catch ( Exception ex )
        {
            Debug.WriteLine( ex.Message );
            Logger.Log( MessageType.Error, $"Failed to open Rider: {ex.Message}" );
        }
    }

    public void CloseEditor()
    {
        try
        {
            if ( _riderInstance != null && !_riderInstance.HasExited )
            {
                _riderInstance.CloseMainWindow();

                if ( !_riderInstance.WaitForExit( 5000 ) )
                {
                    _riderInstance.Kill();
                }

                _riderInstance = null;
                Logger.Log( MessageType.Info, "Rider closed successfully." );
            }
        }
        catch ( Exception ex )
        {
            Debug.WriteLine( ex.Message );
            Logger.Log( MessageType.Error, "Failed to close Rider." );
        }
    }

    public bool AddFilesToSolution( string solution, string projectName, string[] files )
    {
        try
        {
            // Rider는 MSBuild 프로젝트 파일을 직접 수정하지 않고
            // 프로젝트를 열면 자동으로 파일을 인식합니다.
            OpenEditor( solution );

            // CPP 파일이 있으면 열어줍니다
            string? cpp = files.FirstOrDefault( x => Path.GetExtension( x ) == ".cpp" );
            if ( !string.IsNullOrEmpty( cpp ) )
            {
                // 잠시 대기 후 파일 열기
                System.Threading.Thread.Sleep( 2000 );
                OpenFileInRider( cpp );
            }

            return true;
        }
        catch ( Exception ex )
        {
            Debug.WriteLine( ex.Message );
            Logger.Log( MessageType.Error, $"Failed to add files to Rider project: {ex.Message}" );
            return false;
        }
    }

    public void BuildSolution( GameProject.Project project, string configName, bool showWindow = true )
    {
        if ( IsDebugging() )
        {
            Logger.Log( MessageType.Error, "Build process is currently running." );
            return;
        }

        try
        {
            BuildDone = BuildSucceeded = false;

            // Rider를 열고 솔루션을 로드
            OpenEditor( project.Solution );

            if ( _riderInstance != null && !_riderInstance.HasExited )
            {
                Logger.Log( MessageType.Info, $"Building {project.Name} with {configName} configuration in Rider" );

                // Rider에서 빌드 명령 실행 (Ctrl+F9)
                var riderPath = GetRiderExecutablePath();
                if ( !string.IsNullOrEmpty( riderPath ) )
                {
                    var buildStartInfo = new ProcessStartInfo
                    {
                        FileName = riderPath,
                        Arguments = $"--build \"{project.Solution}\" --config {configName}",
                        UseShellExecute = true,
                        WindowStyle = showWindow ? ProcessWindowStyle.Normal : ProcessWindowStyle.Hidden
                    };

                    using var buildProcess = Process.Start( buildStartInfo );
                    if ( buildProcess != null )
                    {
                        buildProcess.WaitForExit();
                        
                        BuildDone = true;
                        BuildSucceeded = buildProcess.ExitCode == 0;

                        if ( BuildSucceeded )
                        {
                            Logger.Log( MessageType.Info, $"Building {configName} configuration succeeded" );
                        }
                        else
                        {
                            Logger.Log( MessageType.Error, $"Building {configName} configuration failed" );
                        }
                    }
                }
                else
                {
                    Logger.Log( MessageType.Error, "Rider executable not found for building." );
                    BuildDone = true;
                    BuildSucceeded = false;
                }
            }
            else
            {
                Logger.Log( MessageType.Error, "Rider is not running for build." );
                BuildDone = true;
                BuildSucceeded = false;
            }
        }
        catch ( Exception ex )
        {
            Debug.WriteLine( ex.Message );
            Logger.Log( MessageType.Error, $"Failed to build project in Rider: {ex.Message}" );
            BuildDone = true;
            BuildSucceeded = false;
        }
    }

    public bool IsDebugging()
    {
        // Rider에서는 디버깅 상태를 쉽게 감지할 수 없으므로
        // 빌드가 진행 중인지만 확인
        return !BuildDone;
    }

    private static string? GetRiderExecutablePath()
    {
        var possiblePaths = new[]
        {
            @"C:\Users\%USERNAME%\AppData\Local\JetBrains\Toolbox\apps\Rider\ch-0\*\bin\rider64.exe",
            @"C:\Program Files\JetBrains\JetBrains Rider *\bin\rider64.exe",
            @"C:\Program Files (x86)\JetBrains\JetBrains Rider *\bin\rider64.exe"
        };

        foreach ( var pathPattern in possiblePaths )
        {
            var expandedPath = Environment.ExpandEnvironmentVariables( pathPattern );

            if ( pathPattern.Contains( "*" ) )
            {
                if ( pathPattern.Contains( @"JetBrains\JetBrains Rider *" ) )
                {
                    var jetbrainsDirs = new[]
                    {
                        @"C:\Program Files\JetBrains",
                        @"C:\Program Files (x86)\JetBrains"
                    };

                    foreach ( var jetbrainsDir in jetbrainsDirs )
                    {
                        if ( Directory.Exists( jetbrainsDir ) )
                        {
                            var riderDirs = Directory.GetDirectories( jetbrainsDir, "JetBrains Rider *" );

                            foreach ( var dir in riderDirs )
                            {
                                var binPath = Path.Combine( dir, "bin", "rider64.exe" );
                                if ( File.Exists( binPath ) )
                                {
                                    return binPath;
                                }
                            }
                        }
                    }
                }
                else
                {
                    var directory = Path.GetDirectoryName( expandedPath );
                    var fileName = Path.GetFileName( expandedPath );

                    if ( !string.IsNullOrEmpty( directory ) && Directory.Exists( directory ) )
                    {
                        var matchingFiles = Directory.GetFiles( directory, fileName, SearchOption.AllDirectories );

                        if ( matchingFiles.Length > 0 )
                        {
                            return matchingFiles.OrderByDescending( f => new FileInfo( f ).LastWriteTime ).First();
                        }
                    }
                }
            }
            else if ( File.Exists( expandedPath ) )
            {
                return expandedPath;
            }
        }

        return null;
    }


    private void BringRiderToFront()
    {
        try
        {
            if ( _riderInstance != null && !_riderInstance.HasExited )
            {
                var handle = _riderInstance.MainWindowHandle;
                if ( handle != IntPtr.Zero )
                {
                    ShowWindow( handle, SW_RESTORE );
                    SetForegroundWindow( handle );
                }
            }
        }
        catch ( Exception ex )
        {
            Debug.WriteLine( ex.Message );
        }
    }

    private static void OpenFileInRider( string filePath )
    {
        try
        {
            if ( !File.Exists( filePath ) )
            {
                return;
            }

            var riderPath = GetRiderExecutablePath();
            if ( string.IsNullOrEmpty( riderPath ) )
            {
                Logger.Log( MessageType.Error, "Cannot open file in Rider: Rider installation not found." );
                return;
            }

            var startInfo = new ProcessStartInfo
            {
                FileName = riderPath,
                Arguments = $"--line 1 \"{filePath}\"",
                UseShellExecute = true
            };

            Process.Start( startInfo );
        }
        catch ( Exception ex )
        {
            Debug.WriteLine( $"OpenFileInRider error: {ex.Message}" );
        }
    }
}