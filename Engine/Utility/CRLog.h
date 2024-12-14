#pragma once


#include "../CRTypes.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRLog
//---------------------------------------------------------------------------------------------------------------------
class CRLog
{
private:
	CRArray< CRString  > Logs;
	CRArray< CWRString > ErrorLogs;

public:
	/// Add log.
	void AddLog( const CRString& Log );

	/// Add error log.
	void AddErrorLog( HRESULT HR );
};


extern CRLog GLog;
