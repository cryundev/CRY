#pragma once


#include <vector>


//---------------------------------------------------------------------------------------------------------------------
/// CRArray
//---------------------------------------------------------------------------------------------------------------------
template< typename T >
class CRArray
{
private:
    std::vector< T > container;    

public:
    /// Add element.
    void Add( const T& Element ) { container.push_back( Element ); }

    /// Get element count.
    unsigned int Num() const { return (unsigned int)( container.size() ); }

    /// Get data pointer.
    const T* Data() const { return container.data(); }

    /// operator[]
    T& operator[]( unsigned int Index ) { return container[ Index ]; }
};
