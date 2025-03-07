#pragma once


#include "CRObject.h"
#include "Source/Core/CRTransform.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRActor
//---------------------------------------------------------------------------------------------------------------------
class CRActor : public CRObject 
{
public:
    /// Constructor
    CRActor() = default;

    /// Get transform
    inline CRTransform& GetTransform() const
    {
        return CRTransform::Get( ObjectId );
    };
};
