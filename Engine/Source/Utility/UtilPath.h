#pragma once

#include "Source/Core/CRTypes.h"
#include <cstdlib>
#include <filesystem>


namespace UtilPath
{
    inline CRPath GetEnginePath()
    {
        char* enginePath = nullptr;
        size_t enginePathLength = 0;
        if ( _dupenv_s( &enginePath, &enginePathLength, "CRYE_PATH" ) != 0 || !enginePath || enginePathLength <= 1 )
        {
            free( enginePath );
            return {};
        }

        const CRPath resolvedEnginePath = CRPath( enginePath ).lexically_normal();
        free( enginePath );
        return resolvedEnginePath;
    }

    inline CRPath ResolveEnginePath( const CRPath& RelativePath )
    {
        if ( RelativePath.empty() ) return {};
        if ( RelativePath.is_absolute() ) return {};

        const CRPath enginePath = GetEnginePath();
        if ( enginePath.empty() ) return {};

        return ( enginePath / RelativePath ).lexically_normal();
    }

    inline CRPath ResolveExistingEnginePath( const CRPath& RelativePath )
    {
        const CRPath resolvedPath = ResolveEnginePath( RelativePath );
        if ( resolvedPath.empty() ) return {};
        if ( !std::filesystem::exists( resolvedPath ) ) return {};

        return resolvedPath;
    }
}