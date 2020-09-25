//=============================================================================
//【 ModelCore_x 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <sstream>
#include "../../../Interface.h"
#include "../ModelManager.h"
#include "../ModelFrame.h"
#include "../ModelCore.h"
#include "AllocateHierarchy.h"
#include "../../../DirectX9/DX9Module.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

    //---------------------------------------------------------------------
	// ● フレームの階層構造をコンソールに出力する
	//---------------------------------------------------------------------
	static void analyzeFrame( DerivedD3DXFrame* frame_, u32 depth_ )
	{
		// depth_ の数だけスペースを打つ
		for ( u32 i = 0; i < depth_; ++i )
		{
			printf( "　" );
		}

		printf( "< Frame Name=\"%s\" ", frame_->Name );

		// メッシュコンテナチェック
		if ( frame_->pMeshContainer )
		{
			printf( "MeshContainerName=\"%s\" ", frame_->pMeshContainer->Name );
		}

		printf( ">\n" );

		// 子フレーム解析
		if ( frame_->pFrameFirstChild )
		{
			analyzeFrame( (DerivedD3DXFrame*)frame_->pFrameFirstChild, depth_ + 1 );
		}

		// 兄弟フレーム解析
		if ( frame_->pFrameSibling )
		{
			analyzeFrame( (DerivedD3DXFrame*)frame_->pFrameSibling, depth_ );
		}
	}

//=============================================================================
// ■ ModelCore クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● X ファイル読み込み
    //---------------------------------------------------------------------
    void ModelCore::loadXFile( const void* data_, lnU32 size_, lnU32 flags_, Model** model_ )
    {
        *model_ = NULL;
        bool is_dynamic = ( flags_ & LN_SKINNINGTYPE_SOFTWARE );


        HRESULT hr;
        AllocateHierarchy allocate_hierarchy;
        D3DXFRAME* root_frame = NULL;
        ID3DXAnimationController* dx_anim_controller = NULL;

        LN_COMCALL( DirectX9::DX9Module::getInstance()->D3DXLoadMeshHierarchyFromXInMemory(
            data_,//str.c_str(),
            size_,//str.size(),
			D3DXMESH_MANAGED | D3DXMESH_32BIT,
            mManager->getGraphicsDevice()->getDX9Device(),
			&allocate_hierarchy,
            NULL,
            &root_frame,
            &dx_anim_controller ) );

        //---------------------
        class finally {
        public:
            finally( AllocateHierarchy* ah_, D3DXFRAME* rf_, ID3DXAnimationController* ac_ )
                : mAH( ah_ ), mRF( rf_ ), mAC( ac_ ) { }
            ~finally()
            {
                if ( mRF ) DirectX9::DX9Module::getInstance()->D3DXFrameDestroy( mRF, mAH );
		        SAFE_RELEASE( mAC );
            }
            AllocateHierarchy* mAH;
            D3DXFRAME* mRF;
            ID3DXAnimationController* mAC;
        } finally( &allocate_hierarchy, root_frame, dx_anim_controller );
        //---------------------

        try
        {
            // Model 作成
            *model_ = NULL;
            Model* model = LN_NEW Model( mManager );
            model->mModelCore = this;

            // スキンメッシュではない場合
            if ( !dx_anim_controller )
            {
                mIsSkinMesh = false;

                //-----------------------------------------------------
	            // 頂点・インデックスの数を調べる

                lnU32 all_vertex_num = 0;
                lnU32 all_index_num  = 0;

                DerivedD3DXMeshContainerArray& dx_mesh_containers = allocate_hierarchy.getDerivedD3DXMeshContainerArray();
                if ( dx_mesh_containers.size() != 1 )
                {
                    printf( "XFile error: Mesh container is 2 or more." );
                }

                ln_foreach( DerivedD3DXMeshContainer* c, dx_mesh_containers )
                {
                    // とりあえずこのフォーマットでのみ受け付ける
                    static const DWORD fvf =  D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;
                    if ( c->MeshData.pMesh->GetFVF() == fvf )
                    {
                        all_vertex_num += c->MeshData.pMesh->GetNumVertices();
                        all_index_num  += c->MeshData.pMesh->GetNumFaces() * 3;
                    }
                }

                if ( all_vertex_num == 0 )
                {
                    printf( "XFile error: fvf invalid.\n" );
                }

                //-----------------------------------------------------
                // 頂点バッファ、インデックスバッファ作成

                // 頂点バッファ作成
                mGraphicsDevice->createVertexBuffer(
                    &mVertexBuffer, Vertex::PMDVertex::elements(), all_vertex_num, NULL, is_dynamic );

                // インデックスバッファ作成
                IDirect3DIndexBuffer9* dx_indexbuffer;
                 D3DINDEXBUFFER_DESC dx_indexbuffer_desc;
                dx_mesh_containers[0]->MeshData.pMesh->GetIndexBuffer( &dx_indexbuffer );
                dx_indexbuffer->GetDesc( &dx_indexbuffer_desc );
                dx_indexbuffer->Release();
                mGraphicsDevice->createIndexBuffer(
                    &mIndexBuffer, all_index_num, NULL, is_dynamic, (dx_indexbuffer_desc.Format == D3DFMT_INDEX16) );

                // ソフトウェアスキニング用
                if ( flags_ & LN_SKINNINGTYPE_SOFTWARE )
                {
                    mOrgVertexPositions = LN_NEW LVector3[ all_vertex_num ];
                    mOrgVertexNormals   = LN_NEW LVector3[ all_vertex_num ];
                }

                // 書き込み開始
                Vertex::PMDVertex* vertices = (Vertex::PMDVertex*)mVertexBuffer->lock();
                lnU32* indices = (lnU32*)mIndexBuffer->lock();

                lnU32 vi = 0;
                lnU32 ii = 0;
                //DerivedD3DXMeshContainerArray& dx_mesh_containers = allocate_hierarchy.getDerivedD3DXMeshContainerArray();
                ln_foreach( DerivedD3DXMeshContainer* c, dx_mesh_containers )
                {
                    // とりあえずこのフォーマットでのみ受け付ける
                    static const DWORD fvf =  D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;
                    if ( c->MeshData.pMesh->GetFVF() == fvf )
                    {
                        // 頂点読み取り開始
                        Vertex::BasicXFileVertex* dx_vertices;
                        c->MeshData.pMesh->LockVertexBuffer( 0, (void**)&dx_vertices );

                        lnU32 v_num = c->MeshData.pMesh->GetNumVertices();
                        for ( lnU32 i = 0; i < v_num; ++i, ++vi )
                        {
                            vertices[ vi ].Position = dx_vertices[ i ].Position;
                            vertices[ vi ].Normal   = dx_vertices[ i ].Normal;
                            vertices[ vi ].TexUV    = dx_vertices[ i ].TexUV;
                            vertices[ vi ].Color    = dx_vertices[ i ].Color;
                            memset( vertices[ vi ].BlendIndices, 0, sizeof( vertices[ vi ].BlendIndices ) );
                            memset( vertices[ vi ].BlendWeights, 0, sizeof( vertices[ vi ].BlendWeights ) );

                            if ( flags_ & LN_SKINNINGTYPE_SOFTWARE )
                            {
                                mOrgVertexPositions[ vi ] = vertices[ vi ].Position;
                                mOrgVertexNormals[ vi ]   = vertices[ vi ].Normal;
                            }

                            //vertices[ vi ].Position.dump();
                        }

                        // 頂点読み取り終了
                        c->MeshData.pMesh->UnlockVertexBuffer();


                        // インデックス読み取り開始
                        void* dx_indices;
                        c->MeshData.pMesh->LockIndexBuffer( 0, (void**)&dx_indices );
                        lnU32 i_num = c->MeshData.pMesh->GetNumFaces() * 3;

                        memcpy( &indices[ ii ], dx_indices, mIndexBuffer->getIndexStride() * i_num );
                        ii += i_num;

                        //for ( lnU32 i = 0; i < i_num; ++i )
                        //{
                        //    //_p(indices[i]);
                        //    //indices[ ii ] = dx_indices[ i ];
                        //}
                        

                        // インデックス読み取り終了
                        c->MeshData.pMesh->UnlockIndexBuffer();
                    }
                }

                // 書き込み終了
                mVertexBuffer->unlock();
                mIndexBuffer->unlock();

                //-----------------------------------------------------
                // マテリアル、属性

                // マテリアル数取得
                lnU32 attr_num = allocate_hierarchy.getAllMaterialNum();

                // マテリアル、属性配列
                LMaterial*       materials  = LN_NEW LMaterial[ attr_num ];
                LNMeshAttribute* attributes = LN_NEW LNMeshAttribute[ attr_num ];

                lnU32 mi = 0;
                ln_foreach( DerivedD3DXMeshContainer* c, dx_mesh_containers )
                {
                    static const DWORD fvf =  D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;
                    if ( c->MeshData.pMesh->GetFVF() == fvf )
                    {
                        DWORD   attr_num = 0;
                        c->MeshData.pMesh->GetAttributeTable( NULL, &attr_num );
                        D3DXATTRIBUTERANGE* dx_attrib_table = LN_NEW D3DXATTRIBUTERANGE[ attr_num ];
                        c->MeshData.pMesh->GetAttributeTable( dx_attrib_table, &attr_num );

                        for ( lnU32 i = 0; i < c->NumMaterials; ++i, ++mi )
                        {
                            materials[ mi ].setFromDxMaterial( c->pMaterials[ i ].MatD3D );

                            // テクスチャ名がある場合はテクスチャ作成
                            if ( !c->TextureNames.empty() && !c->TextureNames[ i ].empty() )
                            {
                                ITexture* tex;
                                mManager->createTexture( &tex, c->TextureNames[ i ].c_str() );
                                materials[ mi ].setTexture( tex );
                                LN_SAFE_RELEASE( tex );
                            }

                            // 属性
                            attributes[ mi ].MaterialIndex = i;
                            attributes[ mi ].StartIndex    = dx_attrib_table[ i ].FaceStart * 3;
                            attributes[ mi ].PrimitiveNum  = dx_attrib_table[ i ].FaceCount;
                        }

                        SAFE_DELETE_ARRAY( dx_attrib_table );
                    }
                }
                // Model に格納
                model->mAttrNum    = attr_num;
                model->mAttributes = attributes;
                model->mMaterials  = materials;

            }

            // コリジョンモデルの生成
            if ( flags_ & LN_MODELFLAG_COLLISION )
            {
                _createCollisionBuffers();
            }

            *model_ = model;
        }
        catch ( Base::Exception e )
        {
            throw e;
        }

        return;

        //analyzeFrame( (DerivedD3DXFrame*)root_frame, 0 );

        //-----------------------------------------------------
	    // 必要なボーンの名前の一覧を作る
        //      D3DXFRAME の階層構造には、ボーンではなく
        //      メッシュコンテナを格納しているフレームがある。
        //      混乱を避けるため、ボーンとして扱わないフレームは
        //      Model のフレーム構造からはじきたい。
        //      そのため、本当に必要なボーンの名前一覧を作って
        //      必要なフレームだけを作るようにする。
        
        std::map< LRefString, int > necessary_bone_names;

        DerivedD3DXMeshContainerArray& dx_mesh_containers = allocate_hierarchy.getDerivedD3DXMeshContainerArray();
        ln_foreach( DerivedD3DXMeshContainer* c, dx_mesh_containers )
        {
            lnU32 n = c->pSkinInfo->GetNumBones();
            for ( lnU32 i = 0; i < n; ++i )
            {
                necessary_bone_names.insert(
                    std::pair< LRefTString, int >( LRefString( c->pSkinInfo->GetBoneName( i ) ), i ) );
            }
        }

        //-----------------------------------------------------
	    // 必要なフレームを bone_frames に格納

        DerivedD3DXFrameArray& dx_frames = allocate_hierarchy.getDerivedD3DXFrameArray();
        DerivedD3DXFrameArray  bone_frames;     // 本当に必要なフレーム
        ln_foreach( DerivedD3DXFrame* frame, dx_frames )
        {
            if ( frame->pMeshContainer )
            {
                std::map< LRefString, int >::iterator itr = necessary_bone_names.find( frame->Name );
                if ( itr != necessary_bone_names.end() )
                {
                    bone_frames.push_back( frame );
                }
            }
            else
            {
                bone_frames.push_back( frame );
            }
        }

        //-----------------------------------------------------
	    // フレーム作成

        ModelFrame* model_frames = LN_NEW ModelFrame[ bone_frames.size() ];

        ln_foreach( DerivedD3DXFrame* frame, bone_frames )
        {
            //_p( frame->Name );
            //dx_frames[ i ]->

           // printf( "Frame:%s Sibling:%p Child:%p\n", dx_frames[ i ]->Name, dx_frames[ i ]->pFrameSibling, dx_frames[ i ]->pFrameFirstChild );

        }



        
        //*model_ = model;
    }

    

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================