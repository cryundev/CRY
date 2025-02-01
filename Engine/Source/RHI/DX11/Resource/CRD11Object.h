#pragma once


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
};