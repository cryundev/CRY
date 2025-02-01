#include "CRGeneric.h"
#include "../Log/CRLog.h"


//---------------------------------------------------------------------------------------------------------------------
/// Check error.
//---------------------------------------------------------------------------------------------------------------------
bool CRGeneric::CheckError( HRESULT hr )
{
    if ( FAILED( hr ) )
    {
        GLog.AddErrorLog( hr );
        return true;
    }
    return false;
}
