#pragma once


#include "CRAssetImporter.h"
#include "Source/Utility/UtilPath.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRAssetImportFormats
//---------------------------------------------------------------------------------------------------------------------
namespace CRAssetImportFormats
{
    inline constexpr const char* CassetExtension = ".cra";

    inline constexpr const char* CassetExtensions[] =
    {
        CassetExtension,
    };

    inline constexpr const char* PrimitiveSourceExtensions[] =
    {
        ".fbx",
    };

    inline constexpr const char* TextureSourceExtensions[] =
    {
        ".png",
        ".jpg",
        ".jpeg",
        ".bmp",
        ".gif",
        ".tif",
        ".tiff",
    };

    template < size_t ExtensionCount >
    inline bool IsExtensionInList( const CRString& Extension, const char* const ( &Extensions )[ ExtensionCount ] )
    {
        for ( const char* supportedExtension : Extensions )
        {
            if ( Extension == supportedExtension ) return true;
        }

        return false;
    }

    template < size_t ExtensionCount >
    inline CRString BuildWildcardList( const char* const ( &Extensions )[ ExtensionCount ] )
    {
        CRString wildcardList;

        for ( const char* extension : Extensions )
        {
            if ( !wildcardList.empty() ) wildcardList += ";";

            wildcardList += "*";
            wildcardList += extension;
        }

        return wildcardList;
    }

    template < size_t ExtensionCount >
    inline CRString BuildFilterGroup( const char* DisplayName, const char* const ( &Extensions )[ ExtensionCount ] )
    {
        const CRString wildcardList = BuildWildcardList( Extensions );

        CRString filterGroup = DisplayName;
        filterGroup += " (";
        filterGroup += wildcardList;
        filterGroup += ")|";
        filterGroup += wildcardList;

        return filterGroup;
    }

    inline const CRString& GetOpenFileFilter()
    {
        static const CRString filter = []()
        {
            const CRString primitiveWildcards = BuildWildcardList( PrimitiveSourceExtensions );
            const CRString textureWildcards   = BuildWildcardList( TextureSourceExtensions   );

            CRString result = "Supported Assets|";
            result += primitiveWildcards;
            result += ";";
            result += textureWildcards;
            result += "|";
            result += BuildFilterGroup( "Primitive Source Files", PrimitiveSourceExtensions );
            result += "|";
            result += BuildFilterGroup( "Texture Files", TextureSourceExtensions );
            result += "|All Files|*.*";

            return result;
        }();

        return filter;
    }

    inline const CRString& GetSaveFileFilter()
    {
        static const CRString filter = []()
        {
            CRString result = BuildFilterGroup( "CRY Asset", CassetExtensions );
            result += "|All Files|*.*";

            return result;
        }();

        return filter;
    }

    inline bool IsPrimitiveSourceExtension( const CRPath& Path )
    {
        return IsExtensionInList( UtilPath::GetLowerExtension( Path ), PrimitiveSourceExtensions );
    }

    inline bool IsTextureSourceExtension( const CRPath& Path )
    {
        return IsExtensionInList( UtilPath::GetLowerExtension( Path ), TextureSourceExtensions );
    }

    inline ECRAssetImportType GetImportType( const CRPath& Path )
    {
        if ( IsPrimitiveSourceExtension( Path ) ) return ECRAssetImportType::Primitive;
        if ( IsTextureSourceExtension  ( Path ) ) return ECRAssetImportType::Texture;

        return ECRAssetImportType::Auto;
    }
}
