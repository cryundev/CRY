#pragma once


#include "CRRHITypes.h"
#include "Source/Core/Containers/CRPackedArray.h"
#include "Source/Core/CRTypes.h"
#include "Source/Core/Identify/CRIdentity.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRRenderElementHandle
//---------------------------------------------------------------------------------------------------------------------
using CRRenderElementHandle = CRPackedHandle;


//---------------------------------------------------------------------------------------------------------------------
/// CRRenderElement
//---------------------------------------------------------------------------------------------------------------------
struct CRRenderElement
{
    ICRRHIMeshWPtr     Mesh;
    ICRRHIMaterialWPtr Material;
    CRIdentity::id_t   OwnerId = CRIdentity::InvalidId;
};
