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

    Primitives.push_back( CRPrimitiveData() );

    _LoadNode( FbxScenePtr->GetRootNode() );

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

    const FbxAxisSystem& sceneAxisSystem  = FbxScenePtr->GetGlobalSettings().GetAxisSystem();
    const FbxAxisSystem& targetAxisSystem = FbxAxisSystem::OpenGL;
    if ( sceneAxisSystem != targetAxisSystem )
    {
        targetAxisSystem.ConvertScene( FbxScenePtr );
    }
    
    // FbxSystemUnit sceneSystemUnit = FbxScenePtr->GetGlobalSettings().GetSystemUnit();
    // if ( sceneSystemUnit.GetScaleFactor() != 1.0 )
    // {
    //     FbxSystemUnit::cm.ConvertScene( FbxScenePtr );
    // }
    
    FbxGeometryConverter fbxGeometryConverter( FbxManagerPtr );
    fbxGeometryConverter.Triangulate( FbxScenePtr, true );
    
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
        _LoadNode( Node->GetChild( i ) );
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

    CRPrimitiveData& primitiveData = Primitives.back();
    
    int vertexCount = mesh->GetPolygonCount() * 3;
    int vertexIndex = primitiveData.VertexCount;
    
    primitiveData.Initialize( primitiveData.VertexCount + vertexCount );

    const FbxVector4& t = Node->GetGeometricTranslation( FbxNode::eSourcePivot );
    const FbxVector4& r = Node->GetGeometricRotation   ( FbxNode::eSourcePivot );
    const FbxVector4& s = Node->GetGeometricScaling    ( FbxNode::eSourcePivot );

    FbxAMatrix trsMatrix( t, r, s );

    bool allByControlPoint = true;

    bool hasNormal = mesh->GetElementNormalCount() > 0;  
    bool hasUV     = mesh->GetElementUVCount()     > 0;  

    FbxGeometryElement::EMappingMode normalMappingMode = FbxGeometryElement::eNone;  
    FbxGeometryElement::EMappingMode uvMappingMode     = FbxGeometryElement::eNone;

    if ( hasNormal )  
    {  
        normalMappingMode = mesh->GetElementNormal( 0 )->GetMappingMode();  
        if ( normalMappingMode == FbxGeometryElement::eNone )  
        {  
            hasNormal = false;  
        }  
        if ( hasNormal && normalMappingMode != FbxGeometryElement::eByControlPoint )  
        {  
            allByControlPoint = false;  
        }  
    }

    const FbxAMatrix& transformMatrix = Node->EvaluateGlobalTransform() * trsMatrix;
    const FbxVector4* fbxVertices     = mesh->GetControlPoints();

    for ( int polygonIndex = 0; polygonIndex < mesh->GetPolygonCount(); ++polygonIndex )
    {
        for ( int t = 0; t < 3; ++t )
        {
            int index = mesh->GetPolygonVertex( polygonIndex, t );

            const FbxVector4& fbxPosition = transformMatrix.Transpose().MultT( fbxVertices[ index ] );
            
            primitiveData.Positions[ vertexIndex ].x = fbxPosition.mData[ 0 ];
            primitiveData.Positions[ vertexIndex ].y = fbxPosition.mData[ 1 ];
            primitiveData.Positions[ vertexIndex ].z = fbxPosition.mData[ 2 ];

            FbxVector4 fbxNormal;
            mesh->GetPolygonVertexNormal( polygonIndex, t, fbxNormal );

            const FbxVector4& transformedNormal = fbxNormal;
            primitiveData.Normals[ vertexIndex ].x = transformedNormal.mData[ 0 ];
            primitiveData.Normals[ vertexIndex ].y = transformedNormal.mData[ 1 ];
            primitiveData.Normals[ vertexIndex ].z = transformedNormal.mData[ 2 ];

            ++vertexIndex;
        }
    }
}
