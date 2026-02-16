#pragma once


#include "Source/Utility/Generic/CRGeneric.h"
#include <d3d11.h>


//---------------------------------------------------------------------------------------------------------------------
/// CRD11Object
//---------------------------------------------------------------------------------------------------------------------
template< typename T >
class CRD11Object
{
protected:
    T* ObjectPtr = nullptr;

public:
    /// Constructor
    CRD11Object() = default;

    /// Destructor
    virtual ~CRD11Object() { if ( ObjectPtr ) { ObjectPtr->Release(); ObjectPtr = nullptr; } }

    /// Get object.
    T* GetObjectPtr() const { return ObjectPtr; }

    /// Set object.
    void SetObjectPtr( T* InObjectPtr ) { if( ObjectPtr ) { ObjectPtr->Release(); } ObjectPtr = InObjectPtr; }

protected:
    /// Commit newly created D3D object and release old object safely.
    bool CommitCreatedObject( T* InCreatedObject, HRESULT Hr )
    {
        bool bSucceeded = !CRGeneric::CheckError( Hr );
        
        if ( bSucceeded )
        {
            SetObjectPtr( InCreatedObject );
            
        }
        else
        {
            if ( InCreatedObject )
            {
                InCreatedObject->Release();
            }
        }
        
        return bSucceeded;
    }
};
