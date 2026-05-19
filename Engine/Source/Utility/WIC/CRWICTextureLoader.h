#pragma once


#include "../../Core/Strings/CRStringInc.h"
#include "Source/Core/CRTextureFormat.h"
#include "Source/Core/CRTypes.h"
#include <guiddef.h>
#include <wincodec.h>


struct IWICImagingFactory;
struct IWICBitmapDecoder;
struct IWICBitmapFrameDecode;


//---------------------------------------------------------------------------------------------------------------------
/// CRWICTextureLoader
//---------------------------------------------------------------------------------------------------------------------
class CRWICTextureLoader
{
public:
    static IWICImagingFactory* sWICFactory;
    static bool                sCoInitialized;

private:
    /// Get WIC.
    static IWICImagingFactory* GetWICFactory();

public:
    /// Shutdown static WIC factory and COM initialization done by this loader.
    static void ShutdownWICFactory();

private:
    CRWString              ImagePath;
    u8*                    Pixels            = nullptr;
    IWICBitmapDecoder*     BitmapDecoder     = nullptr;
    IWICBitmapFrameDecode* BitmapFrameDecode = nullptr;
    u32                    ImageWidth        = 0;
    u32                    ImageHeight       = 0;
    u32                    TextureWidth      = 0;
    u32                    TextureHeight     = 0;
    u32                    RowPitch          = 0;
    u32                    ImageSize         = 0;
    WICPixelFormatGUID     WicFormat         = GUID_WICPixelFormatDontCare;
    WICPixelFormatGUID     ConvertToFormat   = GUID_WICPixelFormatDontCare;
    ECRTextureFormat       TextureFormat     = ECRTextureFormat::Unknown;
    u32                    BPP               = 0;

public:
    /// Destructor.
    ~CRWICTextureLoader();
    
    /// Load texture from file.
    bool LoadFromFile( const CRPath& Path );

    /// Load texture from file using an explicit output size.
    bool LoadFromFile( const CRPath& Path, u32 RequestedWidth, u32 RequestedHeight );

    /// Get pixels.
    const unsigned char* GetPixels() const { return Pixels; }

    /// Get width.
    u32 GetWidth() const { return ImageWidth; }

    /// Get height.
    u32 GetHeight() const { return ImageHeight; }

    /// Get row pitch.
    u32 GetRowPitch() const { return RowPitch; }

    /// Get image size.
    u32 GetImageSize() const { return ImageSize; }

    /// Get texture format.
    ECRTextureFormat GetTextureFormat() const { return TextureFormat; }

private:
    /// Create decoder.
    bool _CreateDecoder();

    /// Get texture size.
    bool _GetTextureSize();

    /// Get format and bit per pixel.
    bool _GetFormatAndBPP();

    /// Get bit per pixel.
    u32 _GetBPP( REFGUID targetGUID ) const;

    /// Copy pixel from scaler.
    bool _CopyPixelFromScaler() const;

    /// Copy pixel from converter.
    bool _CopyPixelFromConverter( IWICBitmapSource* BitmapSource ) const;

    /// Load texture after path and optional requested size are configured.
    bool _Load();
};
