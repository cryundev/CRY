#include "CRAssetFile.h"
#include <cstring>
#include <ios>


namespace CRAssetFile
{
//---------------------------------------------------------------------------------------------------------------------
/// Create asset file header.
//---------------------------------------------------------------------------------------------------------------------
Header Header::Create( ECRAssetType InAssetType, u64 InMetadataSize, u64 InPayloadSize )
{
    Header header;
    
    std::memcpy( header.Signature, AssetFileSignature, sizeof( header.Signature ) );
    
    header.Version      = CurrentVersion;
    header.AssetType    = InAssetType;
    header.HeaderSize   = sizeof( Header );
    header.MetadataSize = InMetadataSize;
    header.PayloadSize  = InPayloadSize;

    return header;
}

//---------------------------------------------------------------------------------------------------------------------
/// Returns true when this header matches the current asset file format.
//---------------------------------------------------------------------------------------------------------------------
bool Header::IsValid() const
{
    return std::memcmp( Signature, AssetFileSignature, sizeof( Signature ) ) == 0 && Version == CurrentVersion && HeaderSize == sizeof( Header );
}

//---------------------------------------------------------------------------------------------------------------------
/// Open file and attempt to read casset header.
//---------------------------------------------------------------------------------------------------------------------
bool Reader::Open( const CRPath& Path )
{
    Stream.open( Path, std::ios::binary );
    
    bOpen = Stream.is_open();
    if ( !bOpen ) return false;

    Stream.read( reinterpret_cast< char* >( &FileHeader ), sizeof( Header ) );
    bHasValidHeader = Stream.good() && FileHeader.IsValid();

    if ( !bHasValidHeader )
    {
        FileHeader = Header();
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Open file and write casset header.
//---------------------------------------------------------------------------------------------------------------------
bool Writer::Open( const CRPath& Path, ECRAssetType AssetType, u64 MetadataSize, u64 PayloadSize )
{
    Stream.open( Path, std::ios::binary );
    bOpen = Stream.is_open();
    if ( !bOpen ) return false;

    const Header header = Header::Create( AssetType, MetadataSize, PayloadSize );
    Stream.write( reinterpret_cast< const char* >( &header ), sizeof( Header ) );

    return Stream.good();
}
}
