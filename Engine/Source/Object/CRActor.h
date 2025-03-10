#pragma once


#include "CRObject.h"
#include "Source/Object/Component/CRTransform.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRActor
//---------------------------------------------------------------------------------------------------------------------
class CRActor : public CRObject 
{
protected:
    CRArray< ICRComponent* > Components;
    CRTransform* Transform = nullptr;
    
public:
    /// Constructor.
    CRActor() = default;

    /// Initialize components.
    void InitializeComponents();

    /// Get transform.
    inline CRTransform* GetTransform() const { return Transform; }

    /// Add component.
    template< typename T = CRComponent >
    T* AddComponent();
};


//---------------------------------------------------------------------------------------------------------------------
/// Add component.
//---------------------------------------------------------------------------------------------------------------------
template < typename T >
T* CRActor::AddComponent()
{
    T* component = T::Add( ObjectId );

    component->SetObjectId( ObjectId );

    Components.push_back( component );

    return component;
}
