using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;


namespace Editor_WPF.DllWrappers;


//----------------------------------------------------------------------------------------------------------------------
/// EngineAPI Asset
//----------------------------------------------------------------------------------------------------------------------
public static partial class EngineAPI
{
    //------------------------------------------------------------------------------------------------------------------
    /// Asset
    //------------------------------------------------------------------------------------------------------------------
    internal static class Asset
    {
        private const int ImportMessageCapacity = 1024;

        //--------------------------------------------------------------------------------------------------------------
        /// Public Types
        //--------------------------------------------------------------------------------------------------------------
        public enum ImportType
        {
            Auto      = 0,
            Texture   = 1,
            Primitive = 2,
        }

        public sealed class TextureImportOption
        {
            public int    Value       { get; }
            public string DisplayName { get; }

            public TextureImportOption( int value, string displayName )
            {
                Value       = value;
                DisplayName = displayName;
            }

            public override string ToString()
            {
                return DisplayName;
            }
        }

        //--------------------------------------------------------------------------------------------------------------
        /// Import File Metadata
        //--------------------------------------------------------------------------------------------------------------
        public static string OpenFileFilter  => ToManagedString( Native.GetOpenFileFilter () );
        public static string SaveFileFilter  => ToManagedString( Native.GetSaveFileFilter () );
        public static string CassetExtension => ToManagedString( Native.GetCassetExtension() );

        //--------------------------------------------------------------------------------------------------------------
        /// Texture Import Capabilities
        //--------------------------------------------------------------------------------------------------------------
        public static int TextureImportMinSize => Native.GetTextureImportMinSize();
        public static int TextureImportMaxSize => Native.GetTextureImportMaxSize();

        public static bool IsTextureMipmapGenerationSupported => Native.IsTextureMipmapGenerationSupported();

        public static string TextureMipmapGenerationUnsupportedReason => ToManagedString( Native.GetTextureMipmapGenerationUnsupportedReason() );

        //--------------------------------------------------------------------------------------------------------------
        /// Asset Import Queries
        //--------------------------------------------------------------------------------------------------------------
        public static ImportType GetImportType( string sourcePath )
        {
            return (ImportType)Native.GetImportType( sourcePath ?? string.Empty );
        }

        //--------------------------------------------------------------------------------------------------------------
        /// Texture Import Options
        //--------------------------------------------------------------------------------------------------------------
        public static IReadOnlyList< TextureImportOption > GetTextureColorSpaceOptions()
        {
            return GetTextureImportOptions( Native.GetTextureColorSpaceOptionCount(), Native.GetTextureColorSpaceOptionValue, Native.GetTextureColorSpaceOptionName );
        }

        public static IReadOnlyList< TextureImportOption > GetTextureCompressionOptions()
        {
            return GetTextureImportOptions( Native.GetTextureCompressionOptionCount(), Native.GetTextureCompressionOptionValue, Native.GetTextureCompressionOptionName );
        }

        public static bool IsTextureCompressionSupported( int textureCompression )
        {
            return Native.IsTextureCompressionSupported( textureCompression );
        }

        public static string GetTextureCompressionUnsupportedReason( int textureCompression )
        {
            return ToManagedString( Native.GetTextureCompressionUnsupportedReason( textureCompression ) );
        }

        //--------------------------------------------------------------------------------------------------------------
        /// Asset Import Execution
        //--------------------------------------------------------------------------------------------------------------
        public static bool TryImport
        (
            string sourcePath,
            string destinationPath,
            out string message,
            ImportType importType = ImportType.Auto,
            int textureColorSpace = 0,
            int textureCompression = 0,
            int textureWidth = 0,
            int textureHeight = 0,
            bool generateMipmaps = false
        )
        {
            StringBuilder importMessage = new StringBuilder( ImportMessageCapacity );

            bool succeeded = Native.ImportAsset
            (
                sourcePath ?? string.Empty,
                destinationPath ?? string.Empty,
                (int)importType,
                textureColorSpace,
                textureCompression,
                textureWidth,
                textureHeight,
                generateMipmaps,
                importMessage,
                importMessage.Capacity
            );

            message = importMessage.ToString();

            return succeeded;
        }

        //--------------------------------------------------------------------------------------------------------------
        /// Managed Conversion Helpers
        //--------------------------------------------------------------------------------------------------------------
        private static IReadOnlyList< TextureImportOption > GetTextureImportOptions( int optionCount, Func< int, int > getValue, Func< int, IntPtr > getName )
        {
            List< TextureImportOption > options = new List< TextureImportOption >( optionCount );

            for ( int index = 0; index < optionCount; ++index )
            {
                int    value       = getValue( index );
                string displayName = ToManagedString( getName( index ) );

                options.Add( new TextureImportOption( value, displayName ) );
            }

            return options;
        }

        private static string ToManagedString( IntPtr nativeString )
        {
            return Marshal.PtrToStringAnsi( nativeString ) ?? string.Empty;
        }

        //--------------------------------------------------------------------------------------------------------------
        /// Native
        //--------------------------------------------------------------------------------------------------------------
        private static class Native
        {
            //----------------------------------------------------------------------------------------------------------
            /// Asset Import
            //----------------------------------------------------------------------------------------------------------
            [DllImport( EngineDllName, EntryPoint = "ImportAsset", CharSet = CharSet.Ansi )]
            [return: MarshalAs( UnmanagedType.I1 )]
            public static extern bool ImportAsset
            (
                string sourcePath,
                string destinationPath,
                int importType,
                int textureColorSpace,
                int textureCompression,
                int textureWidth,
                int textureHeight,
                [MarshalAs( UnmanagedType.I1 )] bool generateMipmaps,
                StringBuilder outMessage,
                int messageCapacity
            );

            [DllImport( EngineDllName, EntryPoint = "GetAssetImportType", CharSet = CharSet.Ansi )]
            public static extern int GetImportType( string sourcePath );

            //----------------------------------------------------------------------------------------------------------
            /// Import File Metadata
            //----------------------------------------------------------------------------------------------------------
            [DllImport( EngineDllName, EntryPoint = "GetAssetImportOpenFileFilter" )]
            public static extern IntPtr GetOpenFileFilter();

            [DllImport( EngineDllName, EntryPoint = "GetAssetImportSaveFileFilter" )]
            public static extern IntPtr GetSaveFileFilter();

            [DllImport( EngineDllName, EntryPoint = "GetAssetImportCassetExtension" )]
            public static extern IntPtr GetCassetExtension();

            //----------------------------------------------------------------------------------------------------------
            /// Texture Import Options
            //----------------------------------------------------------------------------------------------------------
            [DllImport( EngineDllName, EntryPoint = "GetTextureImportColorSpaceOptionCount" )]
            public static extern int GetTextureColorSpaceOptionCount();

            [DllImport( EngineDllName, EntryPoint = "GetTextureImportColorSpaceOptionValue" )]
            public static extern int GetTextureColorSpaceOptionValue( int index );

            [DllImport( EngineDllName, EntryPoint = "GetTextureImportColorSpaceOptionName" )]
            public static extern IntPtr GetTextureColorSpaceOptionName( int index );

            [DllImport( EngineDllName, EntryPoint = "GetTextureImportCompressionOptionCount" )]
            public static extern int GetTextureCompressionOptionCount();

            [DllImport( EngineDllName, EntryPoint = "GetTextureImportCompressionOptionValue" )]
            public static extern int GetTextureCompressionOptionValue( int index );

            [DllImport( EngineDllName, EntryPoint = "GetTextureImportCompressionOptionName" )]
            public static extern IntPtr GetTextureCompressionOptionName( int index );

            //----------------------------------------------------------------------------------------------------------
            /// Texture Import Capabilities
            //----------------------------------------------------------------------------------------------------------
            [DllImport( EngineDllName, EntryPoint = "IsTextureImportCompressionOptionSupported" )]
            [return: MarshalAs( UnmanagedType.I1 )]
            public static extern bool IsTextureCompressionSupported( int textureCompression );

            [DllImport( EngineDllName, EntryPoint = "GetTextureImportCompressionOptionUnsupportedReason" )]
            public static extern IntPtr GetTextureCompressionUnsupportedReason( int textureCompression );

            [DllImport( EngineDllName, EntryPoint = "IsTextureImportMipmapGenerationSupported" )]
            [return: MarshalAs( UnmanagedType.I1 )]
            public static extern bool IsTextureMipmapGenerationSupported();

            [DllImport( EngineDllName, EntryPoint = "GetTextureImportMipmapGenerationUnsupportedReason" )]
            public static extern IntPtr GetTextureMipmapGenerationUnsupportedReason();

            [DllImport( EngineDllName, EntryPoint = "GetTextureImportMinSize" )]
            public static extern int GetTextureImportMinSize();

            [DllImport( EngineDllName, EntryPoint = "GetTextureImportMaxSize" )]
            public static extern int GetTextureImportMaxSize();
        }
    }
}
