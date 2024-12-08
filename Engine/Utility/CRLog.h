#pragma once


#include "../CRTypes.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRLog
//---------------------------------------------------------------------------------------------------------------------
class CRLog
{
private:
	CRArray< CRString > Logs;

public:
	/// Add log.
	void AddLog( const CRString& Log );
};


extern CRLog GLog;
