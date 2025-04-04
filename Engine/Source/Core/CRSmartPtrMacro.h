#pragma once


#include <memory>


template< typename T >
using CRSharedPtr = std::shared_ptr< T >;

template< typename T >
using CRWeakPtr = std::weak_ptr< T >;

template< typename T >
CRSharedPtr< T > CRMakeShared( T* New )
{
    return std::shared_ptr< T >( New );
}

template< typename T >
using CRUniquePtr = std::unique_ptr< T >;

template< typename T >
CRUniquePtr< T > CRMakeUnique( T* New )
{
    return std::unique_ptr< T >( New );
}


#define DECLARE_SMART_PTR( ClassName ) \
class ClassName; \
using ClassName##SPtr = CRSharedPtr< ##ClassName >; \
using ClassName##WPtr = CRWeakPtr  < ##ClassName >; \
