#include "CRLog.h"
#include "Source/Utility/UtilString.h"
#include <comdef.h>


CRLog GLog;


//---------------------------------------------------------------------------------------------------------------------
/// Add log.
//---------------------------------------------------------------------------------------------------------------------
void CRLog::AddLog( const CRString& Log )
{
	Logs.push_back( Log );
}

//---------------------------------------------------------------------------------------------------------------------
/// Add error log.
//---------------------------------------------------------------------------------------------------------------------
void CRLog::AddErrorLog( HRESULT HR )
{
	_com_error comError( HR );

	Logs.emplace_back( UtilString::ToString( comError.ErrorMessage() ) );
}
