#pragma once


#include "CRAssetImporter.h"
#include "Source/Asset/CRTextureAsset.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRTextureImportOptions
//---------------------------------------------------------------------------------------------------------------------
struct CRTextureImportOptions
{
    ECRTextureColorSpace ColorSpace       = ECRTextureColorSpace::Linear;
    ECRTextureFormat     TargetFormat     = ECRTextureFormat::Unknown;
    u32                  Width            = 0;
    u32                  Height           = 0;
    bool                 bGenerateMipmaps = false;
};

//---------------------------------------------------------------------------------------------------------------------
/// CRTextureImportRequest
//---------------------------------------------------------------------------------------------------------------------
struct CRTextureImportRequest : public CRAssetImportRequest
{
    CRTextureImportOptions TextureOptions;

    CRTextureImportRequest() { ImportType = ECRAssetImportType::Texture; }
};


//---------------------------------------------------------------------------------------------------------------------
/// CRTextureImporter
//---------------------------------------------------------------------------------------------------------------------
class CRTextureImporter : public CRAssetImporter
{
public:
    /// Returns true when this importer can handle the request.
    virtual bool CanImport( const CRAssetImportRequest& Request ) const override;

    /// Import source texture into the destination path.
    virtual CRAssetImportResult Import( const CRAssetImportRequest& Request ) const override;

    /// Import a source image into a raw texture asset.
    static bool ImportFromFile( const CRAssetImportRequest& Request, CRTextureAsset& OutAsset );

    /// Import a source image and save it as a texture casset.
    static bool ImportToCasset( const CRAssetImportRequest& Request );
};
