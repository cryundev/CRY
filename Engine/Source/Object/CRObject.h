#pragma once


#include "Source/Core/Identify/CRIdentity.h"
#include "Source/Core/Strings/CRStringInc.h"


class CRWorld;


//---------------------------------------------------------------------------------------------------------------------
/// CRObject
//---------------------------------------------------------------------------------------------------------------------
DECLARE_TYPE_ID( CRObjectId );
class CRObject
{    
protected:
    CRName     ObjectName;  
    CRObjectId ObjectId;
    
public:
    /// Constructor
    CRObject();

    /// Constructor
    constexpr CRObject( const CRIdentity::id_t Id ) : ObjectId( Id ) {}

    /// Destructor
    virtual ~CRObject() = default;

    /// Destroy.
    virtual void Destroy();

    /// Set name.
    void SetName( const CRName& InName ) { ObjectName = InName; }

    /// Get name.
    const CRName& GetName() const { return ObjectName; }

    /// Set object id.
    CRObjectId GetObjectId() const { return ObjectId; }

    /// Return objects id is valid.
    inline bool IsValid() const { return CRIdentity::CRIDGenerator< CRObjectId >::Get().IsValid( ObjectId ); }
};
