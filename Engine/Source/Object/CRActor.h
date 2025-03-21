#pragma once


#include "CRObject.h"
#include "Source/Object/Component/CRTransformComponent.h"


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

    /// Destory
    virtual void Destroy() override;

    /// Initialize components.
    void InitializeComponents();

    /// Get transform.
    inline CRTransformComponent* GetTransform() const { return GetComponent< CRTransformComponent >(); }

    /// Set world.
    inline CRWorld* GetWorld() const { return World; }

    /// Add component.
    template< ComponentType T >
    T* AddComponent();

    /// Remove component.
    template< ComponentType T >
    void RemoveComponent();

    template< ComponentType T >
    T* GetComponent() const 
    {
        return T::Get( ObjectId );
    }
};


template< typename T >
concept ActorType = std::is_base_of_v< CRActor, T >;


//---------------------------------------------------------------------------------------------------------------------
/// Add component.
//---------------------------------------------------------------------------------------------------------------------
template < ComponentType T >
T* CRActor::AddComponent()
{
    T* component = T::Add( ObjectId );

    component->ObjectId = ObjectId;

    Components.push_back( component );

    return component;
}

//---------------------------------------------------------------------------------------------------------------------
/// Remove component.
//---------------------------------------------------------------------------------------------------------------------
template < ComponentType T >
void CRActor::RemoveComponent()
{
}
