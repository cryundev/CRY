#pragma once


#include "../../Core/Strings/CRStringInc.h"
#include "Source/Core/CRTypes.h"
#include <dxgiformat.h>
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

private:
    /// Get WIC.
    static IWICImagingFactory* GetWICFactory();

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
    DXGI_FORMAT            DxgiFormat        = DXGI_FORMAT_UNKNOWN;
    u32                    BPP               = 0;

public:
    /// Destructor.
    ~CRWICTextureLoader();
    
    /// Load texture from file.
    bool LoadFromFile( const CRString& Path );

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

    /// Get DXGI format.
    DXGI_FORMAT GetDxgiFormat() const { return DxgiFormat; }

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
};
