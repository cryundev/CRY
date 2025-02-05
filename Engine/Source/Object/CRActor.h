#pragma once


#include "CRObject.h"
#include "Source/Core/CRTransform.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRActor
//---------------------------------------------------------------------------------------------------------------------
class CRActor : public CRObject 
{
public:
    CRTransform Transform;
    
public:
    /// Constructor
    CRActor() = default;
};
