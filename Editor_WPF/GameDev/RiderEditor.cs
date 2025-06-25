using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using Editor_WPF.GameProject;
using GameProject = Editor_WPF.GameProject;
using Editor_WPF.Utilities;
using Path = System.IO.Path;


namespace Editor_WPF.GameDev;


//---------------------------------------------------------------------------------------------------------------------
/// RiderEditor
//---------------------------------------------------------------------------------------------------------------------
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

    //-----------------------------------------------------------------------------------------------------------------
    /// OpenEditor
    //-----------------------------------------------------------------------------------------------------------------
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

    //-----------------------------------------------------------------------------------------------------------------
    /// CloseEditor
    //-----------------------------------------------------------------------------------------------------------------
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

    //-----------------------------------------------------------------------------------------------------------------
    /// AddFilesToSolution
    //-----------------------------------------------------------------------------------------------------------------
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

    //-----------------------------------------------------------------------------------------------------------------
    /// BuildSolution
    //-----------------------------------------------------------------------------------------------------------------
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

            // Rider를 열고 솔루션을 로드한 후 MSBuild로 빌드 실행
            Logger.Log( MessageType.Info, $"Building {project.Name} with {configName} configuration using MSBuild" );

            // MSBuild를 직접 호출하여 빌드 실행
            var msbuildPath = GetMSBuildPath();
            if ( !string.IsNullOrEmpty( msbuildPath ) )
            {
                var buildStartInfo = new ProcessStartInfo
                {
                    FileName = msbuildPath,
                    Arguments = $"\"{project.Solution}\" /p:Configuration={configName} /p:Platform=x64",
                    UseShellExecute = false,
                    CreateNoWindow = !showWindow,
                    RedirectStandardOutput = true,
                    RedirectStandardError = true,
                    WindowStyle = showWindow ? ProcessWindowStyle.Normal : ProcessWindowStyle.Hidden,
                    StandardOutputEncoding = System.Text.Encoding.Default,
                    StandardErrorEncoding = System.Text.Encoding.Default
                };

                using var buildProcess = Process.Start( buildStartInfo );
                if ( buildProcess != null )
                {
                    // 출력 로그 읽기
                    var output = buildProcess.StandardOutput.ReadToEnd();
                    var error = buildProcess.StandardError.ReadToEnd();

                    buildProcess.WaitForExit();

                    BuildDone = true;
                    BuildSucceeded = buildProcess.ExitCode == 0;

                    if ( BuildSucceeded )
                    {
                        Logger.Log( MessageType.Info, $"Building {configName} configuration succeeded" );
                        if ( !string.IsNullOrEmpty( output ) && output.Trim().Length > 0 )
                        {
                            Logger.Log( MessageType.Info, $"Build output: {output.Trim()}" );
                        }
                    }
                    else
                    {
                        Logger.Log( MessageType.Error, $"Building {configName} configuration failed" );
                        if ( !string.IsNullOrEmpty( error ) && error.Trim().Length > 0 )
                        {
                            Logger.Log( MessageType.Error, $"Build error: {error.Trim()}" );
                        }

                        if ( !string.IsNullOrEmpty( output ) && output.Trim().Length > 0 )
                        {
                            Logger.Log( MessageType.Info, $"Build output: {output.Trim()}" );
                        }
                    }
                }
                else
                {
                    Logger.Log( MessageType.Error, "Failed to start MSBuild process." );
                    BuildDone = true;
                    BuildSucceeded = false;
                }
            }
            else
            {
                Logger.Log( MessageType.Error, "MSBuild executable not found." );
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

    //-----------------------------------------------------------------------------------------------------------------
    /// IsDebugging
    //-----------------------------------------------------------------------------------------------------------------
    public bool IsDebugging()
    {
        // Rider에서는 디버깅 상태를 쉽게 감지할 수 없으므로
        // 빌드가 진행 중인지만 확인
        return !BuildDone;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetRiderExecutablePath
    //-----------------------------------------------------------------------------------------------------------------
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

    //-----------------------------------------------------------------------------------------------------------------
    /// GetMSBuildPath
    //-----------------------------------------------------------------------------------------------------------------
    private static string? GetMSBuildPath()
    {
        try
        {
            // 1. 환경변수에서 MSBuild 경로 확인
            var msbuildFromEnv = Environment.GetEnvironmentVariable( "MSBUILD_EXE_PATH" );
            if ( !string.IsNullOrEmpty( msbuildFromEnv ) && File.Exists( msbuildFromEnv ) )
            {
                return msbuildFromEnv;
            }

            // 2. VS Developer Command Prompt에서 설정되는 경로
            var vsToolsVersion = Environment.GetEnvironmentVariable( "VisualStudioVersion" );
            if ( !string.IsNullOrEmpty( vsToolsVersion ) )
            {
                var vsInstallDir = Environment.GetEnvironmentVariable( "VSINSTALLDIR" );
                if ( !string.IsNullOrEmpty( vsInstallDir ) )
                {
                    var msbuildPath = Path.Combine( vsInstallDir, "MSBuild", "Current", "Bin", "MSBuild.exe" );
                    if ( File.Exists( msbuildPath ) )
                    {
                        return msbuildPath;
                    }
                }
            }

            // 3. vswhere.exe를 사용하여 Visual Studio 설치 경로 찾기
            var vswherePath = Path.Combine( Environment.GetFolderPath( Environment.SpecialFolder.ProgramFilesX86 ),
                "Microsoft Visual Studio", "Installer", "vswhere.exe" );

            if ( File.Exists( vswherePath ) )
            {
                var vsPath = GetVSInstallPathFromVSWhere( vswherePath );
                if ( !string.IsNullOrEmpty( vsPath ) )
                {
                    var msbuildPath = Path.Combine( vsPath, "MSBuild", "Current", "Bin", "MSBuild.exe" );
                    if ( File.Exists( msbuildPath ) )
                    {
                        return msbuildPath;
                    }
                }
            }

            // 4. Registry에서 찾기 (Windows Registry)
            var msbuildFromRegistry = GetMSBuildPathFromRegistry();
            if ( !string.IsNullOrEmpty( msbuildFromRegistry ) )
            {
                return msbuildFromRegistry;
            }

            // 5. 동적으로 일반적인 설치 경로 검색
            return SearchMSBuildInCommonPaths();
        }
        catch ( Exception ex )
        {
            Debug.WriteLine( $"GetMSBuildPath error: {ex.Message}" );
            return null;
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetVSInstallPathFromVSWhere
    //-----------------------------------------------------------------------------------------------------------------
    private static string? GetVSInstallPathFromVSWhere( string vswherePath )
    {
        try
        {
            var startInfo = new ProcessStartInfo
            {
                FileName = vswherePath,
                Arguments = "-latest -products * -requires Microsoft.Component.MSBuild -property installationPath",
                UseShellExecute = false,
                RedirectStandardOutput = true,
                CreateNoWindow = true
            };

            using var process = Process.Start( startInfo );
            if ( process != null )
            {
                var output = process.StandardOutput.ReadToEnd().Trim();
                process.WaitForExit();

                if ( process.ExitCode == 0 && !string.IsNullOrEmpty( output ) )
                {
                    return output;
                }
            }
        }
        catch ( Exception ex )
        {
            Debug.WriteLine( $"GetVSInstallPathFromVSWhere error: {ex.Message}" );
        }

        return null;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetMSBuildPathFromRegistry
    //-----------------------------------------------------------------------------------------------------------------
    private static string? GetMSBuildPathFromRegistry()
    {
        try
        {
            // .NET Framework MSBuild 경로 (하위 호환)
            using var key = Microsoft.Win32.Registry.LocalMachine.OpenSubKey( @"SOFTWARE\Microsoft\MSBuild\ToolsVersions\Current" );
            var msbuildToolsPath = key?.GetValue( "MSBuildToolsPath" ) as string;

            if ( !string.IsNullOrEmpty( msbuildToolsPath ) )
            {
                var msbuildPath = Path.Combine( msbuildToolsPath, "MSBuild.exe" );
                if ( File.Exists( msbuildPath ) )
                {
                    return msbuildPath;
                }
            }
        }
        catch ( Exception ex )
        {
            Debug.WriteLine( $"GetMSBuildPathFromRegistry error: {ex.Message}" );
        }

        return null;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// SearchMSBuildInCommonPaths
    //-----------------------------------------------------------------------------------------------------------------
    private static string? SearchMSBuildInCommonPaths()
    {
        try
        {
            var programFiles = new[]
            {
                Environment.GetFolderPath( Environment.SpecialFolder.ProgramFiles ),
                Environment.GetFolderPath( Environment.SpecialFolder.ProgramFilesX86 )
            };

            var searchPatterns = new[]
            {
                @"Microsoft Visual Studio\*\*\MSBuild\Current\Bin\MSBuild.exe",
                @"JetBrains\JetBrains Rider*\tools\MSBuild\Current\Bin\MSBuild.exe"
            };

            foreach ( var programFile in programFiles )
            {
                if ( !Directory.Exists( programFile ) ) continue;

                foreach ( var pattern in searchPatterns )
                {
                    var searchPath = Path.Combine( programFile, pattern );
                    var directoryPart = Path.GetDirectoryName( searchPath );
                    var filePart = Path.GetFileName( searchPath );

                    if ( !string.IsNullOrEmpty( directoryPart ) )
                    {
                        try
                        {
                            var matchingFiles = Directory.GetFiles( directoryPart, filePart, SearchOption.AllDirectories );
                            if ( matchingFiles.Length > 0 )
                            {
                                // 가장 최신 버전 반환 (파일 경로 기준 정렬)
                                return matchingFiles.OrderByDescending( f => f ).First();
                            }
                        }
                        catch ( DirectoryNotFoundException )
                        {
                            // 디렉토리가 없으면 다음 패턴으로 계속
                            continue;
                        }
                    }
                }
            }
        }
        catch ( Exception ex )
        {
            Debug.WriteLine( $"SearchMSBuildInCommonPaths error: {ex.Message}" );
        }

        return null;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// BringRiderToFront
    //-----------------------------------------------------------------------------------------------------------------
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

    //-----------------------------------------------------------------------------------------------------------------
    /// OpenFileInRider
    //-----------------------------------------------------------------------------------------------------------------
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