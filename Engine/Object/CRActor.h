#pragma once


#include "CRObject.h"
#include "Core/CRTransform.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRActor
//---------------------------------------------------------------------------------------------------------------------
class CRActor : public CRObject 
{
public:
    CRTransform Transform = CRTransform::Identity;
    
public:
    /// Constructor
    CRActor() = default;
};
