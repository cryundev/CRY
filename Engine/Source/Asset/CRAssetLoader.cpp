#include "CRAssetLoader.h"
#include "CRAssetFile.h"
#include "CRPrimitiveAsset.h"
#include "CRTextureAsset.h"


//---------------------------------------------------------------------------------------------------------------------
/// Read asset type from a casset file header.
//---------------------------------------------------------------------------------------------------------------------
ECRAssetType CRAssetLoader::ReadAssetType( const CRPath& Path )
{
    CRAssetFile::Reader reader;
    if ( !reader.Open( Path ) ) return ECRAssetType::Unknown;
    
    if ( reader.HasValidHeader() )
    {
        return reader.GetAssetType();
    }

    return ECRAssetType::Unknown;
}

//---------------------------------------------------------------------------------------------------------------------
/// Load an asset by file type metadata.
//---------------------------------------------------------------------------------------------------------------------
CRSharedPtr< ICRAsset > CRAssetLoader::Load( const CRPath& Path )
{
    const ECRAssetType assetType = ReadAssetType( Path );

    CRSharedPtr< ICRAsset > asset;
    switch ( assetType )
    {
    case ECRAssetType::Primitive: asset = CRMakeShared< ICRAsset >( new CRPrimitiveAsset() ); break;
    case ECRAssetType::Texture2D: asset = CRMakeShared< ICRAsset >( new CRTextureAsset  () ); break;
    default: return nullptr;
    }

    asset->Load( Path );

    return asset;
}
