#pragma once


#include "../Core/Strings/CRStringInc.h"


//---------------------------------------------------------------------------------------------------------------------
/// UtilString
//---------------------------------------------------------------------------------------------------------------------
namespace UtilString
{
	/// Convert string to wstring.
	CRWString ToWString( const CRString& Str );

    /// Convert wstring to string.
    CRString ToString( const CRWString& Str );
};
