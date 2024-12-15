#pragma once


#include "CRD11Include.h"


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
};