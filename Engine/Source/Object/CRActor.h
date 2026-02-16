#pragma once


#include "CRObject.h"
#include "Source/Object/Component/CRTransformComponent.h"
#include <algorithm>


//---------------------------------------------------------------------------------------------------------------------
/// CRActor
//---------------------------------------------------------------------------------------------------------------------
class CRActor : public CRObject 
{
public:
    friend class CRWorld;
    
protected:
    using CRComponentRemover = void(*)( const CRIdentity::id_t& Id );

protected:
    CRArray< CRComponentRemover > ComponentRemovers;
    CRWorld*                      World = nullptr;
    
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

private:
    template< ComponentType T >
    void _RegisterComponentRemover();

    template< ComponentType T >
    void _UnregisterComponentRemover();

    /// Remove component internal.
    template< ComponentType T >
    static void _RemoveComponent( const CRIdentity::id_t& Id )
    {
        T::Remove( Id );
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

    _RegisterComponentRemover< T >();

    return component;
}

//---------------------------------------------------------------------------------------------------------------------
/// Remove component.
//---------------------------------------------------------------------------------------------------------------------
template < ComponentType T >
void CRActor::RemoveComponent()
{
    const bool bRemoved = T::Remove( ObjectId );
    if ( !bRemoved ) return;

    _UnregisterComponentRemover< T >();
}

//---------------------------------------------------------------------------------------------------------------------
/// Register component remover.
//---------------------------------------------------------------------------------------------------------------------
template < ComponentType T >
void CRActor::_RegisterComponentRemover()
{
    const CRComponentRemover remover = &CRActor::_RemoveComponent< T >;
    if ( std::ranges::find( ComponentRemovers, remover ) != ComponentRemovers.end() ) return;

    ComponentRemovers.push_back( remover );
}

//---------------------------------------------------------------------------------------------------------------------
/// Unregister component remover.
//---------------------------------------------------------------------------------------------------------------------
template < ComponentType T >
void CRActor::_UnregisterComponentRemover()
{
    const CRComponentRemover remover = &CRActor::_RemoveComponent< T >;
    const auto iter = std::ranges::find( ComponentRemovers, remover );
    if ( iter == ComponentRemovers.end() ) return;

    ComponentRemovers.erase( iter );
}
