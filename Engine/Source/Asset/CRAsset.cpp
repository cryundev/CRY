#include "CRAsset.h"


//---------------------------------------------------------------------------------------------------------------------
/// Save.
//---------------------------------------------------------------------------------------------------------------------
void CRAsset::Save( const CRPath& Path )
{
    if ( !CanSave() ) return;

    CRAssetFile::Writer writer;
    if ( !writer.Open( Path, GetAssetType(), GetMetadataSize(), GetPayloadSize() ) ) return;

    if ( !SaveMetadata( writer ) ) return;
    if ( !SavePayload ( writer ) ) return;
}

//---------------------------------------------------------------------------------------------------------------------
/// Load.
//---------------------------------------------------------------------------------------------------------------------
void CRAsset::Load( const CRPath& Path )
{
    ClearAsset();

    CRAssetFile::Reader reader;
    if ( !reader.Open( Path ) || !TryLoad( reader ) )
    {
        ClearAsset();
        return;
    }

    PostLoad();
}

//---------------------------------------------------------------------------------------------------------------------
/// Try loading asset data from reader.
//---------------------------------------------------------------------------------------------------------------------
bool CRAsset::TryLoad( CRAssetFile::Reader& Reader )
{
    if ( !Reader.HasValidHeader() ) return false;
    if ( Reader.GetAssetType() != GetAssetType() ) return false;

    return LoadMetadata( Reader ) && LoadPayload( Reader );
}
