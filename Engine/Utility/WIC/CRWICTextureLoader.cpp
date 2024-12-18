#include "CRWICTextureLoader.h"
#include "../UtilString.h"
#include "../../RHI/DX11/CRD11.h"
#include "../../RHI/DX11/CRD11Device.h"
#include "../Log/CRLog.h"


struct WICTranslate
{
	GUID        wic;
	DXGI_FORMAT format;
};


static WICTranslate g_WICFormats[] = 
{
	{ .wic= GUID_WICPixelFormat128bppRGBAFloat,    .format= DXGI_FORMAT_R32G32B32A32_FLOAT         },
	{ .wic= GUID_WICPixelFormat64bppRGBAHalf,      .format= DXGI_FORMAT_R16G16B16A16_FLOAT         },
	{ .wic= GUID_WICPixelFormat64bppRGBA,          .format= DXGI_FORMAT_R16G16B16A16_UNORM         },
	{ .wic= GUID_WICPixelFormat32bppRGBA,          .format= DXGI_FORMAT_R8G8B8A8_UNORM             },
	{ .wic= GUID_WICPixelFormat32bppBGRA,          .format= DXGI_FORMAT_B8G8R8A8_UNORM             }, // DXGI 1.1
	{ .wic= GUID_WICPixelFormat32bppBGR,           .format= DXGI_FORMAT_B8G8R8X8_UNORM             }, // DXGI 1.1
	{ .wic= GUID_WICPixelFormat32bppRGBA1010102XR, .format= DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM }, // DXGI 1.1
	{ .wic= GUID_WICPixelFormat32bppRGBA1010102,   .format= DXGI_FORMAT_R10G10B10A2_UNORM          },
	{ .wic= GUID_WICPixelFormat32bppRGBE,          .format= DXGI_FORMAT_R9G9B9E5_SHAREDEXP         },

#ifdef DXGI_1_2_FORMATS

	{ .wic= GUID_WICPixelFormat16bppBGRA5551,         .format= DXGI_FORMAT_B5G5R5A1_UNORM             },
	{ .wic= GUID_WICPixelFormat16bppBGR565,           .format= DXGI_FORMAT_B5G6R5_UNORM               },

#endif // DXGI_1_2_FORMATS

	{ .wic= GUID_WICPixelFormat32bppGrayFloat,     .format= DXGI_FORMAT_R32_FLOAT                  },
	{ .wic= GUID_WICPixelFormat16bppGrayHalf,      .format= DXGI_FORMAT_R16_FLOAT                  },
	{ .wic= GUID_WICPixelFormat16bppGray,          .format= DXGI_FORMAT_R16_UNORM                  },
	{ .wic= GUID_WICPixelFormat8bppGray,           .format= DXGI_FORMAT_R8_UNORM                   },
	{ .wic= GUID_WICPixelFormat8bppAlpha,          .format= DXGI_FORMAT_A8_UNORM                   },

#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
    
	{ .wic= GUID_WICPixelFormat96bppRGBFloat,      .format= DXGI_FORMAT_R32G32B32_FLOAT            },

#endif
};


struct WICConvert
{
    GUID source;
    GUID target;
};


static WICConvert g_WICConvert[] = 
{
    // Note target GUID in this conversion table must be one of those directly supported formats (above).

    { .source= GUID_WICPixelFormatBlackWhite,           .target= GUID_WICPixelFormat8bppGray         }, // DXGI_FORMAT_R8_UNORM

    { .source= GUID_WICPixelFormat1bppIndexed,          .target= GUID_WICPixelFormat32bppRGBA        }, // DXGI_FORMAT_R8G8B8A8_UNORM 
    { .source= GUID_WICPixelFormat2bppIndexed,          .target= GUID_WICPixelFormat32bppRGBA        }, // DXGI_FORMAT_R8G8B8A8_UNORM 
    { .source= GUID_WICPixelFormat4bppIndexed,          .target= GUID_WICPixelFormat32bppRGBA        }, // DXGI_FORMAT_R8G8B8A8_UNORM 
    { .source= GUID_WICPixelFormat8bppIndexed,          .target= GUID_WICPixelFormat32bppRGBA        }, // DXGI_FORMAT_R8G8B8A8_UNORM 

    { .source= GUID_WICPixelFormat2bppGray,             .target= GUID_WICPixelFormat8bppGray         }, // DXGI_FORMAT_R8_UNORM 
    { .source= GUID_WICPixelFormat4bppGray,             .target= GUID_WICPixelFormat8bppGray         }, // DXGI_FORMAT_R8_UNORM 

    { .source= GUID_WICPixelFormat16bppGrayFixedPoint,  .target= GUID_WICPixelFormat16bppGrayHalf    }, // DXGI_FORMAT_R16_FLOAT 
    { .source= GUID_WICPixelFormat32bppGrayFixedPoint,  .target= GUID_WICPixelFormat32bppGrayFloat   }, // DXGI_FORMAT_R32_FLOAT 

#ifdef DXGI_1_2_FORMATS

    { .source= GUID_WICPixelFormat16bppBGR555,             .target= GUID_WICPixelFormat16bppBGRA5551    }, // DXGI_FORMAT_B5G5R5A1_UNORM

#else

    { .source= GUID_WICPixelFormat16bppBGR555,          .target= GUID_WICPixelFormat32bppRGBA        }, // DXGI_FORMAT_R8G8B8A8_UNORM
    { .source= GUID_WICPixelFormat16bppBGRA5551,        .target= GUID_WICPixelFormat32bppRGBA        }, // DXGI_FORMAT_R8G8B8A8_UNORM
    { .source= GUID_WICPixelFormat16bppBGR565,          .target= GUID_WICPixelFormat32bppRGBA        }, // DXGI_FORMAT_R8G8B8A8_UNORM

#endif // DXGI_1_2_FORMATS

    { .source= GUID_WICPixelFormat32bppBGR101010,       .target= GUID_WICPixelFormat32bppRGBA1010102 }, // DXGI_FORMAT_R10G10B10A2_UNORM

    { .source= GUID_WICPixelFormat24bppBGR,             .target= GUID_WICPixelFormat32bppRGBA        }, // DXGI_FORMAT_R8G8B8A8_UNORM 
    { .source= GUID_WICPixelFormat24bppRGB,             .target= GUID_WICPixelFormat32bppRGBA        }, // DXGI_FORMAT_R8G8B8A8_UNORM 
    { .source= GUID_WICPixelFormat32bppPBGRA,           .target= GUID_WICPixelFormat32bppRGBA        }, // DXGI_FORMAT_R8G8B8A8_UNORM 
    { .source= GUID_WICPixelFormat32bppPRGBA,           .target= GUID_WICPixelFormat32bppRGBA        }, // DXGI_FORMAT_R8G8B8A8_UNORM 
                                                                                                       
    { .source= GUID_WICPixelFormat48bppRGB,             .target= GUID_WICPixelFormat64bppRGBA        }, // DXGI_FORMAT_R16G16B16A16_UNORM
    { .source= GUID_WICPixelFormat48bppBGR,             .target= GUID_WICPixelFormat64bppRGBA        }, // DXGI_FORMAT_R16G16B16A16_UNORM
    { .source= GUID_WICPixelFormat64bppBGRA,            .target= GUID_WICPixelFormat64bppRGBA        }, // DXGI_FORMAT_R16G16B16A16_UNORM
    { .source= GUID_WICPixelFormat64bppPRGBA,           .target= GUID_WICPixelFormat64bppRGBA        }, // DXGI_FORMAT_R16G16B16A16_UNORM
    { .source= GUID_WICPixelFormat64bppPBGRA,           .target= GUID_WICPixelFormat64bppRGBA        }, // DXGI_FORMAT_R16G16B16A16_UNORM
                                                                                                       
    { .source= GUID_WICPixelFormat48bppRGBFixedPoint,   .target= GUID_WICPixelFormat64bppRGBAHalf    }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
    { .source= GUID_WICPixelFormat48bppBGRFixedPoint,   .target= GUID_WICPixelFormat64bppRGBAHalf    }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
    { .source= GUID_WICPixelFormat64bppRGBAFixedPoint,  .target= GUID_WICPixelFormat64bppRGBAHalf    }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
    { .source= GUID_WICPixelFormat64bppBGRAFixedPoint,  .target= GUID_WICPixelFormat64bppRGBAHalf    }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
    { .source= GUID_WICPixelFormat64bppRGBFixedPoint,   .target= GUID_WICPixelFormat64bppRGBAHalf    }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
    { .source= GUID_WICPixelFormat64bppRGBHalf,         .target= GUID_WICPixelFormat64bppRGBAHalf    }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
    { .source= GUID_WICPixelFormat48bppRGBHalf,         .target= GUID_WICPixelFormat64bppRGBAHalf    }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
                                                                                                       
    { .source= GUID_WICPixelFormat96bppRGBFixedPoint,   .target= GUID_WICPixelFormat128bppRGBAFloat  }, // DXGI_FORMAT_R32G32B32A32_FLOAT 
    { .source= GUID_WICPixelFormat128bppPRGBAFloat,     .target= GUID_WICPixelFormat128bppRGBAFloat  }, // DXGI_FORMAT_R32G32B32A32_FLOAT 
    { .source= GUID_WICPixelFormat128bppRGBFloat,       .target= GUID_WICPixelFormat128bppRGBAFloat  }, // DXGI_FORMAT_R32G32B32A32_FLOAT 
    { .source= GUID_WICPixelFormat128bppRGBAFixedPoint, .target= GUID_WICPixelFormat128bppRGBAFloat  }, // DXGI_FORMAT_R32G32B32A32_FLOAT 
    { .source= GUID_WICPixelFormat128bppRGBFixedPoint,  .target= GUID_WICPixelFormat128bppRGBAFloat  }, // DXGI_FORMAT_R32G32B32A32_FLOAT 
                                                                                                       
    { .source= GUID_WICPixelFormat32bppCMYK,            .target= GUID_WICPixelFormat32bppRGBA        }, // DXGI_FORMAT_R8G8B8A8_UNORM 
    { .source= GUID_WICPixelFormat64bppCMYK,            .target= GUID_WICPixelFormat64bppRGBA        }, // DXGI_FORMAT_R16G16B16A16_UNORM
    { .source= GUID_WICPixelFormat40bppCMYKAlpha,       .target= GUID_WICPixelFormat64bppRGBA        }, // DXGI_FORMAT_R16G16B16A16_UNORM
    { .source= GUID_WICPixelFormat80bppCMYKAlpha,       .target= GUID_WICPixelFormat64bppRGBA        }, // DXGI_FORMAT_R16G16B16A16_UNORM
                                                                                                       
#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)                                                     
                                                                                                       
    { .source= GUID_WICPixelFormat32bppRGB,            .target= GUID_WICPixelFormat32bppRGBA         }, // DXGI_FORMAT_R8G8B8A8_UNORM
    { .source= GUID_WICPixelFormat64bppRGB,            .target= GUID_WICPixelFormat64bppRGBA         }, // DXGI_FORMAT_R16G16B16A16_UNORM
    { .source= GUID_WICPixelFormat64bppPRGBAHalf,      .target= GUID_WICPixelFormat64bppRGBAHalf     }, // DXGI_FORMAT_R16G16B16A16_FLOAT
    
#endif

    // We don't support n-channel formats
};


//---------------------------------------------------------------------------------------------------------------------
/// Convert WIC to DXGI.
//---------------------------------------------------------------------------------------------------------------------
static DXGI_FORMAT ConvertWICToDXGI( const GUID& WicFormat )
{
	for ( size_t i = 0; i < _countof( g_WICFormats ); ++i )
	{
		if ( memcmp( &g_WICFormats[ i ].wic, &WicFormat, sizeof( GUID ) ) == 0 )
		{
		    return g_WICFormats[ i ].format;
		}
	}

	return DXGI_FORMAT_UNKNOWN;
}


IWICImagingFactory* CRWICTextureLoader::sWICFactory = nullptr;


//---------------------------------------------------------------------------------------------------------------------
/// Get WIC.
//---------------------------------------------------------------------------------------------------------------------
IWICImagingFactory* CRWICTextureLoader::GetWICFactory()
{
	if ( !sWICFactory )
	{
	    CoInitialize( nullptr );

	    HRESULT hr = CoCreateInstance( CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, __uuidof( IWICImagingFactory ), ( LPVOID* )&sWICFactory );

		if ( FAILED( hr ) )
		{
			GLog.AddErrorLog( hr );

			return nullptr;
		}
	}

	return sWICFactory;
}

//---------------------------------------------------------------------------------------------------------------------
/// Destructor.
//---------------------------------------------------------------------------------------------------------------------
CRWICTextureLoader::~CRWICTextureLoader()
{
    if ( BitmapDecoder )
    {
        BitmapDecoder->Release();
        BitmapDecoder = nullptr;
    }

    if ( BitmapFrameDecode )
    {
        BitmapFrameDecode->Release();
        BitmapFrameDecode = nullptr;
    }

    if ( Pixels != nullptr )
    {
        delete[] Pixels;
        Pixels = nullptr;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Load texture from file.
//---------------------------------------------------------------------------------------------------------------------
bool CRWICTextureLoader::LoadFromFile( const CRString& Path )
{
    IWICImagingFactory* wic = GetWICFactory();

	if ( !wic )
	{
		GLog.AddLog( "Failed to create WIC factory" );
		return false;
	}

    ImagePath = UtilString::ToWString( Path );

    if ( !_CreateDecoder()   ) return false;
    if ( !_GetTextureSize()  ) return false;
    if ( !_GetFormatAndBPP() ) return false;

    RowPitch  = ( TextureWidth * BPP + sizeof( unsigned char ) - 1 ) / sizeof( unsigned char );
    ImageSize = RowPitch * TextureHeight;

    Pixels = new unsigned char[ ImageSize ];
    ZeroMemory( Pixels, ImageSize );

	bool sameFormat = memcmp( &ConvertToFormat, &WicFormat, sizeof( GUID ) ) == 0;
	bool sameSize   = ImageWidth == TextureWidth && ImageHeight == TextureHeight;

	if ( sameFormat && sameSize )
	{
		HRESULT hr = BitmapFrameDecode->CopyPixels( nullptr, RowPitch, ImageSize, Pixels );

		if ( FAILED( hr ) )
		{
            GLog.AddErrorLog( hr );
            return false;
		}
	}
	else if ( !sameSize )
	{
	    return _CopyPixelFromScaler();
	}
	else
	{
	    return _CopyPixelFromConverter( BitmapFrameDecode );
	}

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Create decoder.
//---------------------------------------------------------------------------------------------------------------------
bool CRWICTextureLoader::_CreateDecoder()
{
    IWICImagingFactory* wic = GetWICFactory();

    if ( !wic )
    {
        GLog.AddLog( "Failed to create WIC factory" );
        return false;
    }
    
	HRESULT hr = wic->CreateDecoderFromFilename( ImagePath.c_str(), 0, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &BitmapDecoder ); 

	if ( FAILED( hr ) )
	{
		GLog.AddErrorLog( hr );
		return false;
	}

	hr = BitmapDecoder->GetFrame( 0, &BitmapFrameDecode );

	if ( FAILED( hr ) )
	{
		GLog.AddErrorLog( hr );
	    return false;
	}

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get texture size.
//---------------------------------------------------------------------------------------------------------------------
bool CRWICTextureLoader::_GetTextureSize()
{
	HRESULT hr = BitmapFrameDecode->GetSize( &ImageWidth, &ImageHeight );

	if ( FAILED( hr ) )
	{
		GLog.AddErrorLog( hr );
		return false;
	}

	unsigned int maxSize = GD11.GetMaxTextureSize();

	if ( maxSize == 0 )
	{
		GLog.AddLog( "Invalid maxsize" );
		return false;
	}

	if ( ImageWidth > maxSize || ImageHeight > maxSize )
	{
		float ar = (float)( ImageHeight ) / (float)( ImageWidth );
    
		if ( ImageWidth > ImageHeight )
		{
			TextureWidth  = (unsigned int)( maxSize );
			TextureHeight = (unsigned int)( (float)( maxSize ) * ar ); 
		}
		else
		{
			TextureHeight = (unsigned int)( maxSize ); 
			TextureWidth  = (unsigned int)( (float)( maxSize ) / ar ); 
		}

		if ( TextureWidth > maxSize || TextureHeight > maxSize )
		{
			GLog.AddLog( "Invalid texture size" );
			return false;
		}
	}
	else
	{
		TextureWidth  = ImageWidth;
		TextureHeight = ImageHeight;
	}

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get format and bit per pixel.
//---------------------------------------------------------------------------------------------------------------------
bool CRWICTextureLoader::_GetFormatAndBPP()
{
	HRESULT hr = BitmapFrameDecode->GetPixelFormat( &WicFormat );

	if ( FAILED( hr ) )
	{
		GLog.AddErrorLog( hr );
		return false;
	}

	memcpy( &ConvertToFormat, &WicFormat, sizeof( WICPixelFormatGUID ) );

	DxgiFormat = ConvertWICToDXGI( WicFormat );

	if ( DxgiFormat == DXGI_FORMAT_UNKNOWN )
	{
		for ( size_t i = 0; i < _countof( g_WICConvert ); ++i )
		{
			if ( memcmp( &g_WICConvert[ i ].source, &WicFormat, sizeof( WICPixelFormatGUID ) ) == 0 )
			{
				memcpy( &ConvertToFormat, &g_WICConvert[ i ].target, sizeof( WICPixelFormatGUID ) );

				DxgiFormat = ConvertWICToDXGI( g_WICConvert[ i ].target );
				break;
			}
		}

	    if ( DxgiFormat == DXGI_FORMAT_UNKNOWN )
	    {
	        GLog.AddLog( "Invalid format" );
	        return false;
	    }

	    BPP = _GetBPP( ConvertToFormat );
	}
    else
    {
        BPP = _GetBPP( WicFormat );
    }

	if ( BPP == 0 )
	{
		GLog.AddLog( "Invalid bpp" );
		return false;
	}

	unsigned int support = 0;

	hr = GD11.GetDevice()->CheckFormatSupport( DxgiFormat, &support );

	if ( FAILED( hr ) || !( support & D3D11_FORMAT_SUPPORT_TEXTURE2D ) )
	{
		memcpy( &ConvertToFormat, &GUID_WICPixelFormat32bppRGBA, sizeof(WICPixelFormatGUID) );
		DxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		BPP = 32;
	}

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get bit per pixel.
//---------------------------------------------------------------------------------------------------------------------
unsigned int CRWICTextureLoader::_GetBPP( REFGUID targetGUID ) const
{
    IWICImagingFactory* wic = GetWICFactory();

    if ( !wic )
    {
        GLog.AddLog( "Failed to create WIC factory" );
        return 0;
    }

	IWICComponentInfo* info = nullptr;
	HRESULT hr = wic->CreateComponentInfo( targetGUID, &info );

	if ( FAILED( hr ) )
	{
		GLog.AddErrorLog( hr );

		return 0;
	}

	WICComponentType type;
	hr = info->GetComponentType( &type );

	if ( FAILED( hr ) || type != WICPixelFormat )
	{
		GLog.AddErrorLog( hr );

		info->Release();

		return 0;
	}

	IWICPixelFormatInfo* pixelFormat = nullptr;
	hr = info->QueryInterface( __uuidof( IWICPixelFormatInfo ), (void**)( &pixelFormat ) );

	if ( FAILED( hr ) )
	{
		GLog.AddErrorLog( hr );

		info->Release();

		return 0;
	}

	unsigned int bpp = 0;
	hr = pixelFormat->GetBitsPerPixel( &bpp );

	if ( FAILED( hr ) )
	{
		GLog.AddErrorLog( hr );
	}

	info->Release();
	pixelFormat->Release();

	return bpp;
}

//---------------------------------------------------------------------------------------------------------------------
/// Copy pixel from scaler.
//---------------------------------------------------------------------------------------------------------------------
bool CRWICTextureLoader::_CopyPixelFromScaler() const
{
    IWICImagingFactory* wic = GetWICFactory();

    if ( !wic )
    {
        GLog.AddLog( "Failed to create WIC factory" );
        return false;
    }
    
	IWICBitmapScaler* scaler = nullptr;
	HRESULT hr = wic->CreateBitmapScaler( &scaler );

	if ( FAILED( hr ) )
	{
		GLog.AddErrorLog( hr );
		return false;
	}

	hr = scaler->Initialize( BitmapFrameDecode, TextureWidth, TextureHeight, WICBitmapInterpolationModeFant );

	if ( FAILED( hr ) )
	{
		GLog.AddErrorLog( hr );
		return false;
	}

	WICPixelFormatGUID scaledWicFormat;
	hr = scaler->GetPixelFormat( &scaledWicFormat );

	if ( FAILED( hr ) )
	{
		GLog.AddErrorLog( hr );
		return false;
	}

	if ( memcmp( &ConvertToFormat, &scaledWicFormat, sizeof(GUID) ) == 0 )
	{
		hr = scaler->CopyPixels( 0, RowPitch, ImageSize, Pixels );

		if ( FAILED( hr ) )
		{
			GLog.AddErrorLog( hr );
			return false;
		}
	}
	else
	{
	    return _CopyPixelFromConverter( scaler );
	}

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Copy pixel from converter.
//---------------------------------------------------------------------------------------------------------------------
bool CRWICTextureLoader::_CopyPixelFromConverter( IWICBitmapSource* BitmapSource ) const
{
    IWICImagingFactory* wic = GetWICFactory();

    if ( !wic )
    {
        GLog.AddLog( "Failed to create WIC factory" );
        return false;
    }
    
    IWICFormatConverter* converter = nullptr;
    HRESULT hr = wic->CreateFormatConverter( &converter );
			
    if ( FAILED( hr ) )
    {
        GLog.AddErrorLog( hr );
        return false;
    }

    hr = converter->Initialize( BitmapSource, ConvertToFormat, WICBitmapDitherTypeErrorDiffusion, 0, 0, WICBitmapPaletteTypeCustom );

    if ( FAILED( hr ) )
    {
        GLog.AddErrorLog( hr );
        return false;
    }

    hr = converter->CopyPixels( nullptr, RowPitch, ImageSize, Pixels );  

    if ( FAILED( hr ) )
    {
        GLog.AddErrorLog( hr );
        return false;
    }

    return true;
}
