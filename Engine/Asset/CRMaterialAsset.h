#pragma once


#include "ICRAsset.h"
#include "Core/Containers/CRContainerInc.h"


class CRMaterialAsset : public ICRAsset
{
public:
    CRString            VertexShaderName;
    CRString            PixelShaderName;
    CRString            InputLayoutName;
    CRArray< CRString > TexturePaths;
};
