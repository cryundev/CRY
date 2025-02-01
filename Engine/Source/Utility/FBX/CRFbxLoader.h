#pragma once


#include "CRFbxInclude.h"
#include "Source/Asset//CRPrimitiveAsset.h"
#include "Source/Core/Containers/CRContainerInc.h"
#include "Source/Core/Strings/CRStringInc.h"
#include "Source/RHI/DX11/CRD11Include.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRFbxLoader
//---------------------------------------------------------------------------------------------------------------------
class CRFbxLoader
{
private:
    FbxManager*    FbxManagerPtr    = nullptr;
    FbxIOSettings* FbxIOSettingsPtr = nullptr;
    FbxImporter*   FbxImporterPtr   = nullptr;
    FbxScene*      FbxScenePtr      = nullptr;

    CRArray< CRPrimitiveAsset > Primitives;

public:
    /// Constructor
    CRFbxLoader() = default;

    /// Clear.
    void Clear();
    
    /// Load fbx file.
    bool Load( const CRString& Path );

    /// Get primitive data array.
    CRArray< CRPrimitiveAsset >& GetPrimitives() { return Primitives; }

private:
    /// Initialize.
    bool _Initialize( const CRString& Path );

    /// Load node.
    void _LoadNode( FbxNode* Node );

    /// Load mesh node.
    void _LoadMeshNode( FbxNode* Node );
};
