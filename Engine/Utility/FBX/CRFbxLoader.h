#pragma once


#include "CRFbxInclude.h"
#include "Core/CRPrimitiveData.h"
#include "Core/Containers/CRContainerInc.h"
#include "Core/Strings/CRStringInc.h"
#include "RHI/DX11/CRD11Include.h"


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

    CRArray< CRPrimitiveData > Primitives;

public:
    /// Constructor
    CRFbxLoader() = default;

    /// Clear.
    void Clear();
    
    /// Load fbx file.
    bool Load( const CRString& Path );

    /// Get primitive data array.
    CRArray< CRPrimitiveData >& GetPrimitives() { return Primitives; }

private:
    /// Initialize.
    bool _Initialize( const CRString& Path );

    /// Load node.
    void _LoadNode( FbxNode* Node );

    /// Load mesh node.
    void _LoadMeshNode( FbxNode* Node );
};
