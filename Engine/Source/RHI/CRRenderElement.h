#pragma once


#include "CRRHITypes.h"
#include "Source/Core/Containers/CRPackedArray.h"
#include "Source/Core/CRTypes.h"


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
};
