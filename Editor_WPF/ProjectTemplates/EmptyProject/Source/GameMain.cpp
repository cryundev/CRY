#include "Source/Object/CRScript.h"

#ifdef WITH_EDITOR
extern "C" __declspec( dllexport ) CRScriptCreator GetScriptCreator( const CRName& Name )
{
    return CRScript::GetScriptCreator( Name );
}

extern "C" __declspec( dllexport ) LPSAFEARRAY GetScriptNames()
{
    return nullptr;
}
#endif
