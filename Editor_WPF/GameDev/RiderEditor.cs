using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using Editor_WPF.GameProject;
using Editor_WPF.Utilities;
using Path = System.IO.Path;

namespace Editor_WPF.GameDev;
public class RiderEditor : ICodeEditor
{
    private Process? _riderInstance;

    public bool BuildSucceeded { get; private set; } = true;
    public bool BuildDone { get; private set; } = true;
    
    public event EventHandler< bool >? BuildCompleted;

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
                string? riderPath = GetRiderExecutablePath();
                if ( string.IsNullOrEmpty( riderPath ) )
                {
                    Logger.Log( MessageType.Error, "Cannot open Rider: Rider installation not found." );
                    return;
                }

                ProcessStartInfo startInfo = new ProcessStartInfo
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
            AddFilesToProject( solution, projectName, files );
            
            OpenEditor( solution );

            string? cpp = files.FirstOrDefault( x => Path.GetExtension( x ) == ".cpp" );
            if ( !string.IsNullOrEmpty( cpp ) )
            {
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
    public void BuildSolution( Project project, string configName, bool showWindow = true )
    {
        if ( IsDebugging() )
        {
            Logger.Log( MessageType.Error, "Build process is currently running." );
            return;
        }

        try
        {
            BuildDone = BuildSucceeded = false;

            OpenEditor( project.Solution );

            Logger.Log( MessageType.Info, $"Building {project.Name} with {configName} configuration using MSBuild" );

            string? msbuildPath = GetMSBuildPath();
            if ( !string.IsNullOrEmpty( msbuildPath ) )
            {
                ProcessStartInfo buildStartInfo = new ProcessStartInfo
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

                using Process? buildProcess = Process.Start( buildStartInfo );
                if ( buildProcess != null )
                {
                    string output = buildProcess.StandardOutput.ReadToEnd();
                    string error = buildProcess.StandardError.ReadToEnd();

                    buildProcess.WaitForExit();

                    BuildDone = true;
                    BuildSucceeded = buildProcess.ExitCode == 0;

                    Logger.Log( BuildSucceeded ? MessageType.Info : MessageType.Error, 
                              $"Building {configName} configuration {(BuildSucceeded ? "succeeded" : "failed")}" );
                    
                    if ( !string.IsNullOrEmpty( error?.Trim() ) )
                        Logger.Log( MessageType.Error, $"Build error: {error.Trim()}" );
                    
                    if ( !string.IsNullOrEmpty( output?.Trim() ) )
                        Logger.Log( MessageType.Info, $"Build output: {output.Trim()}" );
                    
                    BuildCompleted?.Invoke( this, BuildSucceeded );
                }
                else
                {
                    HandleBuildFailure( "Failed to start MSBuild process." );
                }
            }
            else
            {
                HandleBuildFailure( "MSBuild executable not found." );
            }
        }
        catch ( Exception ex )
        {
            Debug.WriteLine( ex.Message );
            HandleBuildFailure( $"Failed to build project in Rider: {ex.Message}" );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// IsDebugging
    //-----------------------------------------------------------------------------------------------------------------
    public bool IsDebugging() => !BuildDone;

    //-----------------------------------------------------------------------------------------------------------------
    /// HandleBuildFailure
    //-----------------------------------------------------------------------------------------------------------------
    private void HandleBuildFailure( string message )
    {
        Logger.Log( MessageType.Error, message );
        BuildDone = true;
        BuildSucceeded = false;
        BuildCompleted?.Invoke( this, false );
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetRiderExecutablePath
    //-----------------------------------------------------------------------------------------------------------------
    private static string? GetRiderExecutablePath()
    {
        string[] possiblePaths = new[]
        {
            @"C:\Users\%USERNAME%\AppData\Local\JetBrains\Toolbox\apps\Rider\ch-0\*\bin\rider64.exe",
            @"C:\Program Files\JetBrains\JetBrains Rider *\bin\rider64.exe",
            @"C:\Program Files (x86)\JetBrains\JetBrains Rider *\bin\rider64.exe"
        };

        foreach ( string pathPattern in possiblePaths )
        {
            string expandedPath = Environment.ExpandEnvironmentVariables( pathPattern );

            if ( pathPattern.Contains( "*" ) )
            {
                if ( pathPattern.Contains( @"JetBrains\JetBrains Rider *" ) )
                {
                    string[] jetbrainsDirs = new[]
                    {
                        @"C:\Program Files\JetBrains",
                        @"C:\Program Files (x86)\JetBrains"
                    };

                    foreach ( string jetbrainsDir in jetbrainsDirs )
                    {
                        if ( Directory.Exists( jetbrainsDir ) )
                        {
                            string[] riderDirs = Directory.GetDirectories( jetbrainsDir, "JetBrains Rider *" );

                            foreach ( string dir in riderDirs )
                            {
                                string binPath = Path.Combine( dir, "bin", "rider64.exe" );
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
                    string? directory = Path.GetDirectoryName( expandedPath );
                    string? fileName = Path.GetFileName( expandedPath );

                    if ( !string.IsNullOrEmpty( directory ) && Directory.Exists( directory ) )
                    {
                        string[] matchingFiles = Directory.GetFiles( directory, fileName, SearchOption.AllDirectories );

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
            string? msbuildFromEnv = Environment.GetEnvironmentVariable( "MSBUILD_EXE_PATH" );
            if ( !string.IsNullOrEmpty( msbuildFromEnv ) && File.Exists( msbuildFromEnv ) )
            {
                return msbuildFromEnv;
            }

            string? vsToolsVersion = Environment.GetEnvironmentVariable( "VisualStudioVersion" );
            if ( !string.IsNullOrEmpty( vsToolsVersion ) )
            {
                string? vsInstallDir = Environment.GetEnvironmentVariable( "VSINSTALLDIR" );
                if ( !string.IsNullOrEmpty( vsInstallDir ) )
                {
                    string msbuildPath = Path.Combine( vsInstallDir, "MSBuild", "Current", "Bin", "MSBuild.exe" );
                    if ( File.Exists( msbuildPath ) )
                    {
                        return msbuildPath;
                    }
                }
            }

            string vswherePath = Path.Combine( Environment.GetFolderPath( Environment.SpecialFolder.ProgramFilesX86 ),
                "Microsoft Visual Studio", "Installer", "vswhere.exe" );

            if ( File.Exists( vswherePath ) )
            {
                string? vsPath = GetVSInstallPathFromVSWhere( vswherePath );
                if ( !string.IsNullOrEmpty( vsPath ) )
                {
                    string msbuildPath = Path.Combine( vsPath, "MSBuild", "Current", "Bin", "MSBuild.exe" );
                    if ( File.Exists( msbuildPath ) )
                    {
                        return msbuildPath;
                    }
                }
            }

            string? msbuildFromRegistry = GetMSBuildPathFromRegistry();
            if ( !string.IsNullOrEmpty( msbuildFromRegistry ) )
            {
                return msbuildFromRegistry;
            }

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
            ProcessStartInfo startInfo = new ProcessStartInfo
            {
                FileName = vswherePath,
                Arguments = "-latest -products * -requires Microsoft.Component.MSBuild -property installationPath",
                UseShellExecute = false,
                RedirectStandardOutput = true,
                CreateNoWindow = true
            };

            using Process? process = Process.Start( startInfo );
            if ( process != null )
            {
                string output = process.StandardOutput.ReadToEnd().Trim();
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
            using Microsoft.Win32.RegistryKey? key = Microsoft.Win32.Registry.LocalMachine.OpenSubKey( @"SOFTWARE\Microsoft\MSBuild\ToolsVersions\Current" );
            string? msbuildToolsPath = key?.GetValue( "MSBuildToolsPath" ) as string;

            if ( !string.IsNullOrEmpty( msbuildToolsPath ) )
            {
                string msbuildPath = Path.Combine( msbuildToolsPath, "MSBuild.exe" );
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
            string[] programFiles = new[]
            {
                Environment.GetFolderPath( Environment.SpecialFolder.ProgramFiles ),
                Environment.GetFolderPath( Environment.SpecialFolder.ProgramFilesX86 )
            };

            string[] searchPatterns = new[]
            {
                @"Microsoft Visual Studio\*\*\MSBuild\Current\Bin\MSBuild.exe",
                @"JetBrains\JetBrains Rider*\tools\MSBuild\Current\Bin\MSBuild.exe"
            };

            foreach ( string programFile in programFiles )
            {
                if ( !Directory.Exists( programFile ) ) continue;

                foreach ( string pattern in searchPatterns )
                {
                    string searchPath = Path.Combine( programFile, pattern );
                    string? directoryPart = Path.GetDirectoryName( searchPath );
                    string? filePart = Path.GetFileName( searchPath );

                    if ( !string.IsNullOrEmpty( directoryPart ) )
                    {
                        try
                        {
                            string[] matchingFiles = Directory.GetFiles( directoryPart, filePart, SearchOption.AllDirectories );
                            if ( matchingFiles.Length > 0 )
                            {
                                                return matchingFiles.OrderByDescending( f => f ).First();
                            }
                        }
                        catch ( DirectoryNotFoundException )
                        {
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
                IntPtr handle = _riderInstance.MainWindowHandle;
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

            string? riderPath = GetRiderExecutablePath();
            if ( string.IsNullOrEmpty( riderPath ) )
            {
                Logger.Log( MessageType.Error, "Cannot open file in Rider: Rider installation not found." );
                return;
            }

            ProcessStartInfo startInfo = new ProcessStartInfo
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

    //-----------------------------------------------------------------------------------------------------------------
    /// AddFilesToProject
    //-----------------------------------------------------------------------------------------------------------------
    private void AddFilesToProject( string solutionPath, string projectName, string[] files )
    {
        try
        {
            string? solutionDir = Path.GetDirectoryName( solutionPath );
            if ( string.IsNullOrEmpty( solutionDir ) )
            {
                Logger.Log( MessageType.Error, "Invalid solution path." );
                return;
            }

            string projectFilePath = Path.Combine( solutionDir, "Source", $"{projectName}.vcxproj" );
            if ( !File.Exists( projectFilePath ) )
            {
                Logger.Log( MessageType.Error, $"Project file not found: {projectFilePath}" );
                return;
            }

            string projectContent = File.ReadAllText( projectFilePath );
            string updatedContent = projectContent;

            string[] cppFiles    = files.Where( f => Path.GetExtension( f ).Equals( ".cpp", StringComparison.OrdinalIgnoreCase ) ).ToArray();
            string[] headerFiles = files.Where( f => Path.GetExtension( f ).Equals( ".h",   StringComparison.OrdinalIgnoreCase ) ).ToArray();

            if ( cppFiles.Length > 0 )
            {
                string cppItemGroup = "  <ItemGroup>\r\n";
                foreach ( string cppFile in cppFiles )
                {
                    string relativePath = GetRelativePath( Path.GetDirectoryName( projectFilePath ), cppFile );
                    cppItemGroup += $"    <ClCompile Include=\"{relativePath}\" />\r\n";
                }
                cppItemGroup += "  </ItemGroup>\r\n";

                int projectEndIndex = updatedContent.LastIndexOf( "</Project>" );
                if ( projectEndIndex > 0 )
                {
                    updatedContent = updatedContent.Insert( projectEndIndex, cppItemGroup );
                }
            }

            if ( headerFiles.Length > 0 )
            {
                string headerItemGroup = "  <ItemGroup>\r\n";
                foreach ( string headerFile in headerFiles )
                {
                    string relativePath = GetRelativePath( Path.GetDirectoryName( projectFilePath ), headerFile );
                    headerItemGroup += $"    <ClInclude Include=\"{relativePath}\" />\r\n";
                }
                headerItemGroup += "  </ItemGroup>\r\n";

                int projectEndIndex = updatedContent.LastIndexOf( "</Project>" );
                if ( projectEndIndex > 0 )
                {
                    updatedContent = updatedContent.Insert( projectEndIndex, headerItemGroup );
                }
            }

            if ( updatedContent != projectContent )
            {
                File.WriteAllText( projectFilePath, updatedContent );
                Logger.Log( MessageType.Info, $"Added {files.Length} files to project: {projectName}" );
            }
        }
        catch ( Exception ex )
        {
            Debug.WriteLine( ex.Message );
            Logger.Log( MessageType.Error, $"Failed to add files to project: {ex.Message}" );
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// GetRelativePath
    //-----------------------------------------------------------------------------------------------------------------
    private static string GetRelativePath( string? fromPath, string toPath )
    {
        if ( string.IsNullOrEmpty( fromPath ) )
        {
            return toPath;
        }

        try
        {
            Uri fromUri = new Uri( fromPath + Path.DirectorySeparatorChar );
            Uri toUri = new Uri( toPath );
            Uri relativeUri = fromUri.MakeRelativeUri( toUri );
            return Uri.UnescapeDataString( relativeUri.ToString() ).Replace( '/', Path.DirectorySeparatorChar );
        }
        catch
        {
            return toPath;
        }
    }
}