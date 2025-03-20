#pragma once


#include "CRObject.h"
#include "Source/Object/Component/CRTransform.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRActor
//---------------------------------------------------------------------------------------------------------------------
class CRActor : public CRObject 
{
public:
    friend class CRWorld;
    
protected:
    CRList< ICRComponent* > Components;
    CRWorld*                World = nullptr;
    
public:
    /// Constructor.
    CRActor() = default;

    /// Initialize components.
    void InitializeComponents();

    /// Get transform.
    inline CRTransform* GetTransform() const { return CRTransform::Get( ObjectId ); }

    /// Set world.
    inline CRWorld* GetWorld() const { return World; }

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

    component->ObjectId = ObjectId;

    Components.push_back( component );

    return component;
}
