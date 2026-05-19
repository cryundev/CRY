#pragma once


#include "CRAssetTypes.h"
#include "Source/Core/CRTypes.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRAssetFile
//---------------------------------------------------------------------------------------------------------------------
namespace CRAssetFile
{
    constexpr char AssetFileSignature[ 8 ] = "CRASSET";
    constexpr u32  CurrentVersion          = 1;

    //-----------------------------------------------------------------------------------------------------------------
    /// Header
    //-----------------------------------------------------------------------------------------------------------------
    struct Header
    {
        char         Signature[ 8 ] = {};
        u32          Version      = 0;
        ECRAssetType AssetType    = ECRAssetType::Unknown;
        u32          HeaderSize   = 0;
        u64          MetadataSize = 0;
        u64          PayloadSize  = 0;

        /// Create asset file header.
        static Header Create( ECRAssetType InAssetType, u64 InMetadataSize, u64 InPayloadSize );

        /// Returns true when this header matches the current asset file format.
        bool IsValid() const;
    };

    //-----------------------------------------------------------------------------------------------------------------
    /// Reader
    //-----------------------------------------------------------------------------------------------------------------
    class Reader
    {
    private:
        CRInputFileStream Stream;
        Header            FileHeader;
        bool              bOpen           = false;
        bool              bHasValidHeader = false;

    public:
        /// Open file and attempt to read casset header.
        bool Open( const CRPath& Path );

        /// Returns true when the file stream is open.
        bool IsOpen() const { return bOpen; }

        /// Returns true when the file has a valid casset header.
        bool HasValidHeader() const { return bHasValidHeader; }

        /// Get casset header.
        const Header& GetHeader() const { return FileHeader; }

        /// Get asset type.
        ECRAssetType GetAssetType() const { return FileHeader.AssetType; }

        /// Read a typed value from the file.
        template< typename T >
        bool ReadValue( T& OutValue )
        {
            Stream.read( reinterpret_cast< char* >( &OutValue ), sizeof( T ) );
            return Stream.good();
        }

        /// Read a typed array from the file.
        template< typename T >
        bool ReadArray( T* OutValues, u64 Count )
        {
            if ( Count == 0 ) return true;
            if ( !OutValues ) return false;

            Stream.read( reinterpret_cast< char* >( OutValues ), sizeof( T ) * Count );
            return Stream.good();
        }
    };

    //-----------------------------------------------------------------------------------------------------------------
    /// Writer
    //-----------------------------------------------------------------------------------------------------------------
    class Writer
    {
    private:
        CROutputFileStream Stream;
        bool               bOpen = false;

    public:
        /// Open file and write casset header.
        bool Open( const CRPath& Path, ECRAssetType AssetType, u64 MetadataSize, u64 PayloadSize );

        /// Returns true when the file stream is open.
        bool IsOpen() const { return bOpen; }

        /// Write a typed value to the file.
        template< typename T >
        bool WriteValue( const T& Value )
        {
            Stream.write( reinterpret_cast< const char* >( &Value ), sizeof( T ) );
            return Stream.good();
        }

        /// Write a typed array to the file.
        template< typename T >
        bool WriteArray( const T* Values, u64 Count )
        {
            if ( Count == 0 ) return true;
            if ( !Values ) return false;

            Stream.write( reinterpret_cast< const char* >( Values ), sizeof( T ) * Count );
            return Stream.good();
        }
    };
}
