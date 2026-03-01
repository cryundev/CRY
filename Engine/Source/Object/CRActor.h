#pragma once


#include "CRObject.h"
#include "Source/Object/Component/CRCollisionComponent.h"
#include "Source/Object/Component/CRTransformComponent.h"
#include <algorithm>
#include <functional>
#include <typeinfo>


//---------------------------------------------------------------------------------------------------------------------
/// CRActor
//---------------------------------------------------------------------------------------------------------------------
class CRActor : public CRObject 
{
public:
    friend class CRWorld;
    
protected:
    using CRComponentRemover = std::function< void() >;
    using CRComponentTypeHash = size_t;

protected:
    CRArray< CRComponentRemover > ComponentRemovers;
    CRArray< CRComponentTypeHash > ComponentRemoverTypes;
    CRWorld*                      World = nullptr;
    
public:
    /// Constructor.
    CRActor() = default;

    /// Destroy.
    virtual void Destroy() override;

    /// Initialize components.
    void InitializeComponents();

    /// Get transform.
    inline CRTransformComponent* GetTransform() const { return GetComponent< CRTransformComponent >(); }

    /// Get collision.
    inline CRCollisionComponent* GetCollision() const { return GetComponent< CRCollisionComponent >(); }

    /// Set world.
    inline CRWorld* GetWorld() const { return World; }

    /// Add component.
    template < ComponentType T >
    T* Add();

    /// Remove component.
    template < ComponentType T >
    void RemoveComponent();

    /// Get component.
    template < ComponentType T >
    T* GetComponent() const
    {
        return T::Get( ObjectId );
    }

    /// Get all components.
    template < ComponentType T >
    CRArray< T* > GetComponents() const
    {
        return T::GetAll( ObjectId );
    }

private:
    template < ComponentType T >
    void _RegisterComponentRemover();

    template < ComponentType T >
    void _UnregisterComponentRemover();
};


template < typename T >
concept ActorType = std::is_base_of_v< CRActor, T >;


//---------------------------------------------------------------------------------------------------------------------
/// Add component.
//---------------------------------------------------------------------------------------------------------------------
template < ComponentType T >
T* CRActor::Add()
{
    T* component = T::Add( ObjectId );
    if ( !component ) return nullptr;

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
    const CRComponentTypeHash typeHash = typeid( T ).hash_code();
    if ( std::ranges::find( ComponentRemoverTypes, typeHash ) != ComponentRemoverTypes.end() ) return;

    ComponentRemoverTypes.push_back( typeHash );

    ComponentRemovers.emplace_back( [ this ]()
    {
        RemoveComponent< T >();
    } );
}

//---------------------------------------------------------------------------------------------------------------------
/// Unregister component remover.
//---------------------------------------------------------------------------------------------------------------------
template < ComponentType T >
void CRActor::_UnregisterComponentRemover()
{
    const CRComponentTypeHash typeHash = typeid( T ).hash_code();

    size_t removeIndex = ComponentRemoverTypes.size();
    for ( size_t i = 0; i < ComponentRemoverTypes.size(); ++i )
    {
        if ( ComponentRemoverTypes[ i ] != typeHash )
        {
            continue;
        }

        removeIndex = i;
        break;
    }

    if ( removeIndex >= ComponentRemoverTypes.size() ) return;

    ComponentRemoverTypes.erase( ComponentRemoverTypes.begin() + removeIndex );

    if ( removeIndex >= ComponentRemovers.size() ) return;

    ComponentRemovers.erase( ComponentRemovers.begin() + removeIndex );
}
