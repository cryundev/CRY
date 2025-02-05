#include "UtilString.h"
#include <Windows.h>


//---------------------------------------------------------------------------------------------------------------------
/// ToWString
//---------------------------------------------------------------------------------------------------------------------
CRWString UtilString::ToWString( const CRString& Str )
{
	int bufferSize = MultiByteToWideChar( CP_UTF8, 0, Str.c_str(), -1, nullptr, 0 );

	CRWString wstr( bufferSize, 0 );

	MultiByteToWideChar( CP_UTF8, 0, Str.c_str(), -1, wstr.data(), bufferSize );

	return wstr;
}

//---------------------------------------------------------------------------------------------------------------------
/// Convert wstring to string.
//---------------------------------------------------------------------------------------------------------------------
CRString UtilString::ToString( const CRWString& Str )
{
    int bufferSize = WideCharToMultiByte( CP_UTF8, 0, Str.c_str(), -1, nullptr, 0, nullptr, nullptr );

    CRString str( bufferSize, 0 );

    WideCharToMultiByte( CP_UTF8, 0, Str.c_str(), -1, str.data(), bufferSize, nullptr, nullptr );

    return str;
}
