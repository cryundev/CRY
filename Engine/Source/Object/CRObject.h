#pragma once


#include "Source/Core/Identify/CRIdentity.h"


class CRWorld;


//---------------------------------------------------------------------------------------------------------------------
/// CRObject
//---------------------------------------------------------------------------------------------------------------------
DECLARE_TYPE_ID( CRObjectId );
class CRObject
{    
protected:
    CRObjectId ObjectId;
    
public:
    /// Constructor
    CRObject();

    /// Set object id.
    void SetObjectId( const CRObjectId& InObjectId ) { ObjectId = InObjectId; }

    /// Return objects id is valid.
    inline bool IsValid() const { return CRIdentity::CRIDGenerator< CRObjectId >::Get().IsValid( ObjectId ); }
};
