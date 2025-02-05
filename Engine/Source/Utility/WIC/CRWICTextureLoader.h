#pragma once


#include "../../Core/Strings/CRStringInc.h"
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
    unsigned char*         Pixels            = nullptr;
    IWICBitmapDecoder*     BitmapDecoder     = nullptr;
    IWICBitmapFrameDecode* BitmapFrameDecode = nullptr;
    unsigned int           ImageWidth        = 0;
    unsigned int           ImageHeight       = 0;
    unsigned int           TextureWidth      = 0;
    unsigned int           TextureHeight     = 0;
    unsigned int           RowPitch          = 0;
    unsigned int           ImageSize         = 0;
    WICPixelFormatGUID     WicFormat         = GUID_WICPixelFormatDontCare;
    WICPixelFormatGUID     ConvertToFormat   = GUID_WICPixelFormatDontCare;
    DXGI_FORMAT            DxgiFormat        = DXGI_FORMAT_UNKNOWN;
    unsigned int           BPP               = 0;

public:
    /// Destructor.
    ~CRWICTextureLoader();
    
    /// Load texture from file.
    bool LoadFromFile( const CRString& Path );

    /// Get pixels.
    const unsigned char* GetPixels() const { return Pixels; }

    /// Get width.
    unsigned int GetWidth() const { return ImageWidth; }

    /// Get height.
    unsigned int GetHeight() const { return ImageHeight; }

    /// Get row pitch.
    unsigned int GetRowPitch() const { return RowPitch; }

    /// Get image size.
    unsigned int GetImageSize() const { return ImageSize; }

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
    unsigned int _GetBPP( REFGUID targetGUID ) const;

    /// Copy pixel from scaler.
    bool _CopyPixelFromScaler() const;

    /// Copy pixel from converter.
    bool _CopyPixelFromConverter( IWICBitmapSource* BitmapSource ) const;
};
