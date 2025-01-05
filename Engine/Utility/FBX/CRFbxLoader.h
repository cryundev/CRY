#pragma once


#include "CRFbxInclude.h"
#include "Core/Containers/CRContainerInc.h"
#include "Core/Strings/CRStringInc.h"
#include "RHI/DX11/CRD11Include.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRFbxLoader
//---------------------------------------------------------------------------------------------------------------------
class CRFbxLoader
{
public:
    struct MeshData
    {
        CRArray< CRVector     > Positions;
        CRArray< CRVector     > Normals;
        CRArray< CRVector     > Tangents;
        CRArray< CRVector     > Binormals;
        CRArray< CRVector     > Colors;
        CRArray< CRVector2D   > UVs;    
        CRArray< unsigned int > Indices;

        void Reserve( int VertexCount )
        {
            Positions.reserve( VertexCount );
            Normals  .reserve( VertexCount );
            Tangents .reserve( VertexCount );
            Binormals.reserve( VertexCount );
            Colors   .reserve( VertexCount );
            UVs      .reserve( VertexCount );
        }
    };
    
private:
    FbxManager*    FbxManagerPtr    = nullptr;
    FbxIOSettings* FbxIOSettingsPtr = nullptr;
    FbxImporter*   FbxImporterPtr   = nullptr;
    FbxScene*      FbxScenePtr      = nullptr;

    CRArray< MeshData > Meshes;

public:
    /// Constructor
    CRFbxLoader() = default;

    /// Clear.
    void Clear();
    
    /// Load fbx file.
    bool Load( const CRString& Path );

private:
    /// Initialize.
    bool _Initialize( const CRString& Path );

    /// Load mesh node.
    void _LoadMeshNode( FbxNode* Node );
};
