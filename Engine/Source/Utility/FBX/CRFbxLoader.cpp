#include "CRFbxLoader.h"
#include "Source/Utility/Log/CRLog.h"


//---------------------------------------------------------------------------------------------------------------------
/// Destructor.
//---------------------------------------------------------------------------------------------------------------------
CRFbxLoader::~CRFbxLoader()
{
    Clear();
}

//---------------------------------------------------------------------------------------------------------------------
/// Clear.
//---------------------------------------------------------------------------------------------------------------------
void CRFbxLoader::Clear()
{
    Primitives.clear();

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
bool CRFbxLoader::Load( const CRPath& Path )
{
    Clear();

    if ( !_Initialize( Path ) ) return false;

    Primitives.push_back( CRPrimitiveAsset() );

    _LoadNode( FbxScenePtr->GetRootNode() );

    for ( CRPrimitiveAsset& primitive : Primitives )
    {
        if ( !primitive.HasValidTangentBasis() )
        {
            primitive.EnsureTangentBasis();
        }
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Initialize.
//---------------------------------------------------------------------------------------------------------------------
bool CRFbxLoader::_Initialize( const CRPath& Path )
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
    
    const CRString importPath = Path.lexically_normal().string();
    if ( !FbxImporterPtr->Initialize( importPath.c_str(), -1, FbxIOSettingsPtr ) )
    {
        GLog.AddLog( FbxImporterPtr->GetStatus().GetErrorString() );
        return false;
    }
    
    FbxImporterPtr->Import( FbxScenePtr );

    const FbxAxisSystem& sceneAxisSystem  = FbxScenePtr->GetGlobalSettings().GetAxisSystem();
    const FbxAxisSystem& targetAxisSystem = FbxAxisSystem::DirectX;
    // if ( sceneAxisSystem != targetAxisSystem )
    // {
    //     targetAxisSystem.ConvertScene( FbxScenePtr );
    // }
    
    // FbxSystemUnit sceneSystemUnit = FbxScenePtr->GetGlobalSettings().GetSystemUnit();
    // if ( sceneSystemUnit.GetScaleFactor() != 1.0 )
    // {
    //     FbxSystemUnit::cm.ConvertScene( FbxScenePtr );
    // }
    
    FbxGeometryConverter fbxGeometryConverter( FbxManagerPtr );
    fbxGeometryConverter.Triangulate( FbxScenePtr, true );
    
    FbxImporterPtr->Destroy();
    FbxImporterPtr = nullptr;

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Load node.
//---------------------------------------------------------------------------------------------------------------------
void CRFbxLoader::_LoadNode( FbxNode* Node )
{
    if ( !Node ) return;

    for ( i32 i = 0; i < Node->GetChildCount(); ++i )
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

    CRPrimitiveAsset& primitiveData = Primitives.back();
    
    i32 vertexCount = mesh->GetPolygonCount() * 3;
    i32 vertexIndex = primitiveData.VertexCount;
    
    primitiveData.Initialize( primitiveData.VertexCount + vertexCount );

    const FbxVector4& t = Node->GetGeometricTranslation( FbxNode::eSourcePivot );
    const FbxVector4& r = Node->GetGeometricRotation   ( FbxNode::eSourcePivot );
    const FbxVector4& s = Node->GetGeometricScaling    ( FbxNode::eSourcePivot );

    FbxAMatrix trsMatrix( t, r, s );

    bool hasNormal = mesh->GetElementNormalCount() > 0;  
    bool hasUV     = mesh->GetElementUVCount()     > 0;  
    
    FbxStringList uvSetNames;
    mesh->GetUVSetNames( uvSetNames );    
    const char* uvSetName = ( hasUV && uvSetNames.GetCount() > 0 ) ? uvSetNames[ 0 ] : nullptr;

    FbxAMatrix mat;
    mat.SetIdentity();
    mat *= -1;

    const FbxAMatrix& transformMatrix = (Node->EvaluateGlobalTransform() * trsMatrix);// * mat;
    const FbxVector4* fbxVertices     = mesh->GetControlPoints();

    for ( i32 polygonIndex = 0; polygonIndex < mesh->GetPolygonCount(); ++polygonIndex )
    {
        for ( i32 t = 0; t < 3; ++t )
        {
            int index = mesh->GetPolygonVertex( polygonIndex, t );

            const FbxVector4& fbxPosition = transformMatrix.MultT( fbxVertices[ index ] );
            
            primitiveData.Positions[ vertexIndex ].x = (f32)fbxPosition.mData[ 0 ];
            primitiveData.Positions[ vertexIndex ].y = (f32)fbxPosition.mData[ 1 ];
            primitiveData.Positions[ vertexIndex ].z = (f32)fbxPosition.mData[ 2 ];

            FbxVector4 fbxNormal( 0.0, 0.0, 1.0, 0.0 );
            if ( hasNormal )
            {
                mesh->GetPolygonVertexNormal( polygonIndex, t, fbxNormal );
                fbxNormal.Normalize();
            }

            const FbxVector4& transformedNormal = fbxNormal;
            
            primitiveData.Normals[ vertexIndex ].x = (f32)transformedNormal.mData[ 0 ];
            primitiveData.Normals[ vertexIndex ].y = (f32)transformedNormal.mData[ 1 ];
            primitiveData.Normals[ vertexIndex ].z = (f32)transformedNormal.mData[ 2 ];

            FbxVector2 fbxUV( 0.0, 0.0 );
            bool       bUnmappedUV = false;
            if ( uvSetName )
            {
                mesh->GetPolygonVertexUV( polygonIndex, t, uvSetName, fbxUV, bUnmappedUV );
            }

            primitiveData.UVs[ vertexIndex ].x = (f32)fbxUV.mData[ 0 ];
            primitiveData.UVs[ vertexIndex ].y = (f32)fbxUV.mData[ 1 ];

            ++vertexIndex;
        }
    }
}
