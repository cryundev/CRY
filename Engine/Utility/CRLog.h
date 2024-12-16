#pragma once


#include "../Core/Strings/CRStringInc.h"
#include "../Core/Containers/CRContainerInc.h"
#include <intsafe.h>


//---------------------------------------------------------------------------------------------------------------------
/// CRLog
//---------------------------------------------------------------------------------------------------------------------
class CRLog
{
private:
	CRArray< CRString  > Logs;
	CRArray< CRWString > ErrorLogs;

public:
	/// Add log.
	void AddLog( const CRString& Log );

	/// Add error log.
	void AddErrorLog( HRESULT HR );
};


extern CRLog GLog;
