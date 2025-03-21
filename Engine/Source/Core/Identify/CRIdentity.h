#pragma once


#include "Source/Core/CRTypes.h"
#include "Source/Core/Containers/CRContainerInc.h"
#include "Source/Core/Math/CRMath.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRIdentity
//---------------------------------------------------------------------------------------------------------------------
namespace CRIdentity
{
    using id_t = u64;

    constexpr u32 GenerationBits = 16; // 16 bits
    constexpr u32 IndexBits      = ( sizeof( id_t ) * 8 ) - GenerationBits; // 48 bits

    constexpr id_t GenerationMask = ( id_t{ 1 } << GenerationBits  ) - 1; // 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 1111 1111 1111 1111
    constexpr id_t IndexMask      = ( id_t{ 1 } << IndexBits ) - 1;       // 0000 0000 0000 0000 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111
    constexpr id_t IdMask         = -1;
    constexpr id_t InvalidId      = IdMask;


    using generation_t = std::conditional_t< GenerationBits <= 16, std::conditional_t<GenerationBits <= 8, u8, u16 >, u32 >;
    static_assert( sizeof( generation_t ) * 8 >= GenerationBits );
    static_assert( ( sizeof( id_t ) - sizeof( generation_t ) ) > 0 );
    

    //-----------------------------------------------------------------------------------------------------------------
    /// Return id is valid.
    //-----------------------------------------------------------------------------------------------------------------
    inline bool IsValid( const id_t& id )
    {
        return id != IdMask;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Mask id onto generation.
    //-----------------------------------------------------------------------------------------------------------------
    inline id_t GenerationOf( const id_t& id )
    {
        return ( id >> IndexBits ) & GenerationMask;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Mask id onto index.
    //-----------------------------------------------------------------------------------------------------------------
    inline id_t IndexOf( const id_t& id )
    {
        return id & IndexMask;
    }

    //-----------------------------------------------------------------------------------------------------------------
    /// Create new generation.
    //-----------------------------------------------------------------------------------------------------------------
    inline id_t NewGeneration( const id_t& id )
    {
        id_t generation = GenerationOf( id );
        assert( generation ^ GenerationMask );
        
        return ( generation + 1 ) << IndexBits | IndexOf( id );
    }

#if _DEBUG
    namespace Internal
    {
        struct IdBase
        {
        private:
            id_t Id;

        public:
            constexpr explicit IdBase( id_t InId ) : Id( InId ) {}
            constexpr operator id_t() const { return Id; }
        };
    }

#define DECLARE_TYPE_ID( name ) \
    struct name final : CRIdentity::Internal::IdBase \
    { \
        constexpr explicit name( CRIdentity::id_t InId ) : CRIdentity::Internal::IdBase( InId ) {} \
        constexpr name() : CRIdentity::Internal::IdBase( 0 ) {} \
    }; 
    
#else
#define DEFINE_TYPE_ID( name ) using name = id_t;
#endif

    //-----------------------------------------------------------------------------------------------------------------
    /// CRIDGenerator
    //-----------------------------------------------------------------------------------------------------------------
    template< typename T = Internal::IdBase >
    class CRIDGenerator
    {
    public:
        constexpr inline static u32 THRESHOLD_FREE_IDS_RETURN = 1000;
        
    private:
        CRArray< generation_t > Generations;
        CRDeque< T > FreeIds;

    public:
        //-------------------------------------------------------------------------------------------------------------
        /// Create
        //-------------------------------------------------------------------------------------------------------------
        T Create()
        {
            T id;
        
            if ( FreeIds.size() > THRESHOLD_FREE_IDS_RETURN )
            {
                id = FreeIds.front();
                FreeIds.pop_front();
            
                id = T( CRIdentity::NewGeneration( id ) );

                id_t index = IndexOf( id );
                ++Generations[ index ];
            }
            else
            {
                id = T( Generations.size() );
                Generations.push_back( 0 );
            }

            return id;
        }

        //-------------------------------------------------------------------------------------------------------------
        /// Remove
        //-------------------------------------------------------------------------------------------------------------
        void Remove( const T& Id )
        {
            if ( IsValid( Id ) )
            {
                FreeIds.push_back( Id );
            }
        }

        //-------------------------------------------------------------------------------------------------------------
        /// IsValid
        //-------------------------------------------------------------------------------------------------------------
        bool IsValid( const T& Id )
        {
            id_t generation = CRIdentity::GenerationOf( Id );
            id_t index      = CRIdentity::IndexOf( Id );

            return generation < Generations.size() && Generations[ index ] == generation;
        }

    public:
        //-------------------------------------------------------------------------------------------------------------
        /// Get
        //-------------------------------------------------------------------------------------------------------------
        static CRIDGenerator& Get()
        {
            static CRIDGenerator Instance;
            return Instance;
        }
    };
};


