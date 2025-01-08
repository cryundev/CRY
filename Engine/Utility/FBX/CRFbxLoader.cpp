#include "CRFbxLoader.h"
#include "Utility/Log/CRLog.h"


//---------------------------------------------------------------------------------------------------------------------
/// Clear.
//---------------------------------------------------------------------------------------------------------------------
void CRFbxLoader::Clear()
{
    if ( FbxManagerPtr )
    {
        FbxManagerPtr->Destroy();
        FbxManagerPtr = nullptr;
    }

    FbxIOSettingsPtr = nullptr;
    FbxImporterPtr   = nullptr;
    FbxScenePtr      = nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/// Load fbx file.
//---------------------------------------------------------------------------------------------------------------------
bool CRFbxLoader::Load( const CRString& Path )
{
    Clear();

    if ( !_Initialize( Path ) ) return false;

    _LoadMeshNode( FbxScenePtr->GetRootNode() );

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Initialize.
//---------------------------------------------------------------------------------------------------------------------
bool CRFbxLoader::_Initialize( const CRString& Path )
{
    FbxManagerPtr    = FbxManager::Create();
    FbxIOSettingsPtr = FbxIOSettings::Create( FbxManagerPtr, IOSROOT );
    FbxImporterPtr   = FbxImporter::Create( FbxManagerPtr, "" );
    FbxScenePtr      = FbxScene::Create( FbxManagerPtr, "" );

    if ( !FbxManagerPtr || !FbxIOSettingsPtr || !FbxImporterPtr || !FbxScenePtr )
    {
        GLog.AddLog( "Failed to create fbx objects" );
        return false;
    }

    FbxManagerPtr->SetIOSettings( FbxIOSettingsPtr );
    
    if ( !FbxImporterPtr->Initialize( Path.c_str(), -1, FbxIOSettingsPtr ) )
    {
        GLog.AddLog( FbxImporterPtr->GetStatus().GetErrorString() );
        return false;
    }
    
    FbxImporterPtr->Import( FbxScenePtr );

    // FbxAxisSystem directXAxisSystem( FbxAxisSystem::eDirectX );
    // directXAxisSystem.ConvertScene( FbxScenePtr );
    //
    // FbxGeometryConverter fbxGeometryConverter( FbxManagerPtr );
    // fbxGeometryConverter.Triangulate( FbxScenePtr, true );
    
    FbxImporterPtr->Destroy();

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Load node.
//---------------------------------------------------------------------------------------------------------------------
void CRFbxLoader::_LoadNode( FbxNode* Node )
{
    if ( !Node ) return;

    for ( int i = 0; i < Node->GetChildCount(); ++i )
    {
        _LoadMeshNode( Node->GetChild( i ) );
    }
    
    FbxNodeAttribute* nodeAttribute = Node->GetNodeAttribute();
    if ( !nodeAttribute ) return;

    switch ( nodeAttribute->GetAttributeType() )
    {
        case FbxNodeAttribute::eMesh: _LoadMeshNode( Node ); break;
        default: break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Load mesh node.
//---------------------------------------------------------------------------------------------------------------------
void CRFbxLoader::_LoadMeshNode( FbxNode* Node )
{
    if ( !Node ) return;

    FbxMesh* mesh = Node->GetMesh();
    if ( !mesh ) return;

    Meshes.push_back( MeshData() );
    MeshData& meshData = Meshes.back();
    meshData.Reserve( mesh->GetPolygonCount() * 3 );

    for ( int polygonIndex = 0; polygonIndex < mesh->GetPolygonCount(); ++polygonIndex )
    {
        FbxVector4* fbxVertices = mesh->GetControlPoints();

        for ( int vertexIndex = 0; vertexIndex < 3; ++vertexIndex )
        {
            int index = mesh->GetPolygonVertex( polygonIndex, vertexIndex );

            meshData.Positions.push_back( CRVector( fbxVertices[ index ].mData[ 0 ], fbxVertices[ index ].mData[ 1 ], fbxVertices[ index ].mData[ 2 ] ) );
        }
    }
}
