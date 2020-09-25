//=============================================================================
//【 ModelCore 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../Interface.h"
#include "PMDTypes.h"
#include "ModelManager.h"
#include "ModelFrame.h"
#include "ModelFace.h"
#include "ModelIK.h"
#include "ModelRigidBody.h"
#include "ModelConstraint.h"
#include "ModelCore.h"
#include "ConvertBlendedMesh.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace Lumine
{
namespace Graphics
{

//=============================================================================
// ■ ModelCore クラス
//=============================================================================

    /// IKデータソート用比較関数
    static int cmpIKDataFunc( const void* l_, const void* r_ )
    {
	    return (int)(((ModelIK *)l_)->getSortVal() - ((ModelIK *)r_)->getSortVal());
    }

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    ModelCore::ModelCore( ModelManager* manager_ )
        : mManager          ( manager_ )
        , mGraphicsDevice   ( manager_->getGraphicsDevice() )
        , mVertexBuffer     ( NULL )
        , mIndexBuffer      ( NULL )
        , mOrgVertexPositions   ( NULL )
        , mOrgVertexNormals     ( NULL )
        , mAttrNum          ( 0 )
        , mMaterials        ( NULL )
        , mAttributeRanges  ( NULL )
        , mBoneNum          ( 0 )
        , mModelFrames      ( NULL )
        , mIKNum            ( 0 )
        , mIKArray          ( NULL )
        , mFaceNum          ( 0 )
        , mModelFaces       ( NULL )
        , mRigidBodyNum     ( 0 )
        , mModelRigidBodys  ( NULL )
    {
    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    ModelCore::~ModelCore()
    {
        _release();
    }

    //---------------------------------------------------------------------
	// ● PMD モデル読み込み
    //---------------------------------------------------------------------
    LNRESULT ModelCore::loadPMD( const void* data_top_, lnU32 size_, bool sw_skinning_, Model** model_ )
    {
        LNRESULT lr;
        _release();

        *model_ = NULL;
        Model* model = LN_NEW Model();


        const lnByte* data = static_cast< const lnByte* >( data_top_ );

        // ヘッダのチェック
	    PMD_Header	*pPMDHeader = (PMD_Header *)data;
	    if( pPMDHeader->szMagic[0] != 'P' || pPMDHeader->szMagic[1] != 'm' || pPMDHeader->szMagic[2] != 'd' || pPMDHeader->fVersion != 1.0f )
		    return LN_ERR_FORMAT;	// ファイル形式が違う

        // モデル名のコピー
        char model_name[21];	// モデル名
	    strncpy( model_name, pPMDHeader->szName, 20 );
	    model_name[20] = '\0';
        mModelName = model_name;

	    data += sizeof( PMD_Header );

	    lnU32	ulSize;

        //-----------------------------------------------------
	    // 頂点数取得
        
        lnU32 v_num = *((lnU32 *)data);
        data += sizeof( lnU32 );

        // 頂点バッファ作成
        LN_CALL_R( mGraphicsDevice->createVertexBuffer( &mVertexBuffer, Vertex::PMDVertex::elements(), v_num, NULL, sw_skinning_ ) );

        // ソフトウェアスキニング用
        if ( sw_skinning_ )
        {
            mOrgVertexPositions = LN_NEW LVector3[ v_num ];
            mOrgVertexNormals   = LN_NEW LVector3[ v_num ];
        }

	    // データを流し込む
        PMD_Vertex* pmd_vertex = (PMD_Vertex*)data;
        Vertex::PMDVertex* vertices = (Vertex::PMDVertex*)mVertexBuffer->lock();
        Vertex::PMDVertex* v;
        for ( lnU32 i = 0 ; i < v_num; ++i, ++pmd_vertex )
	    {
            v = &vertices[ i ];

            v->Position = pmd_vertex->vec3Pos;
            v->Normal   = pmd_vertex->vec3Normal;
            v->TexUV    = pmd_vertex->uvTex;
            v->Color    = 0xffffffff;

            v->BlendIndices[ 0 ] = static_cast< u8 >( pmd_vertex->unBoneNo[ 0 ] );
            v->BlendIndices[ 1 ] = static_cast< u8 >( pmd_vertex->unBoneNo[ 1 ] );
            v->BlendIndices[ 2 ] = 0;
            v->BlendIndices[ 3 ] = 0;
            v->BlendWeights[ 0 ] = pmd_vertex->cbWeight * 0.01f;
            v->BlendWeights[ 1 ] = 1.0f - v->BlendWeights[ 0 ];
            v->BlendWeights[ 2 ] = 0.0f;
            v->BlendWeights[ 3 ] = 0.0f;


            

            if ( sw_skinning_ )
            {
                mOrgVertexPositions[ i ] = v->Position;
                mOrgVertexNormals[ i ]   = v->Normal;
            }
	    }

        mVertexBuffer->unlock();

        ulSize = sizeof( PMD_Vertex ) * v_num;
	    data += ulSize;
        
        //-----------------------------------------------------
	    // 頂点インデックス

	    u32 i_num = *((lnU32 *)data);
	    data += sizeof( lnU32 );
        u16* indices = NULL;

        // インデックスバッファ作成
        LN_CALL_R( mGraphicsDevice->createIndexBuffer( &mIndexBuffer, i_num, NULL, sw_skinning_ ) );


	    // 頂点インデックス配列をコピー
        indices = mIndexBuffer->lock();
	    memcpy( indices, data, sizeof( u16 ) * i_num );
        mIndexBuffer->unlock();

	    data += ( sizeof( u16 ) * i_num );

        //-----------------------------------------------------
	    // マテリアル

        // マテリアル数取得
        mAttrNum = *((lnU32 *)data);
	    data += sizeof( lnU32 );

        // マテリアル配列
        mMaterials = LN_NEW LMaterial[ mAttrNum ];

        // 属性配列
        mAttributeRanges = LN_NEW LNMeshAttributeRange[ mAttrNum ];

	    PMD_Material* pmd_material = (PMD_Material*)data;

        u32 index_offset = 0;
        lnU32 face_sum = 0;
        LMaterial* m;
        
	    for ( lnU32 i = 0 ; i < mAttrNum ; ++i )
	    {
            
            m = &mMaterials[ i ];

            // 色を取得していく
            m->Diffuse = pmd_material->col4Diffuse;

            m->Power = pmd_material->fShininess;

            m->Specular.red   = pmd_material->col3Specular.x;
            m->Specular.green = pmd_material->col3Specular.y;
            m->Specular.blue  = pmd_material->col3Specular.z;
            m->Specular.alpha = 1.0f;

            m->Ambient.red   = pmd_material->col3Ambient.x;
            m->Ambient.green = pmd_material->col3Ambient.y;
            m->Ambient.blue  = pmd_material->col3Ambient.z;
            m->Ambient.alpha = 1.0f;
		   
            // テクスチャ名がある場合はテクスチャ作成
		    if ( pmd_material->szTextureFileName[ 0 ] )
		    {
                char* tex_name = pmd_material->szTextureFileName;
                char* sphere_name = NULL;
                char* c = pmd_material->szTextureFileName;
                while ( *c )
                {
                    if ( *c == '*' )
                    {
                        *c = '\0';
                        sphere_name = ( c + 1 );
                        break;
                    }
                    ++c;
                }
                
                ITexture* tex;
                LN_CALL_R( mManager->createTexture( &tex, tex_name ) );
                m->setTexture( tex );

                if ( sphere_name )
                {
                    LN_CALL_R( mManager->createTexture( &tex, sphere_name ) );
                    m->setSphereTexture( tex );
                }

			    //m->IsSphereMap = _isSphereMapTexName( pmd_material->szTextureFileName );
                
		    }
		    else
		    {
			    m->setTexture( NULL );
                m->setSphereTexture( NULL );
			    //m->IsSphereMap = false;
		    }

            // 属性テーブルを埋める
            mAttributeRanges[ i ].MaterialIndex = i;
            mAttributeRanges[ i ].StartIndex = index_offset;
            mAttributeRanges[ i ].PrimitiveNum = pmd_material->ulNumIndices / 3;
            //mAttributeRanges[ i ].FaceStart = face_sum;
            //mAttributeRanges[ i ].VertexCount = pmd_material->ulNumIndices / 3;

            index_offset += pmd_material->ulNumIndices;
            face_sum += mAttributeRanges[ i ].PrimitiveNum;
		    pmd_material++;
	    }

        data += sizeof( PMD_Material ) * mAttrNum;


        //-----------------------------------------------------
	    // ボーン数取得
	    mBoneNum = *((unsigned short *)data);
	    data += sizeof( unsigned short );

	    // ボーン配列を作成
	    mModelFrames = LN_NEW ModelFrame[ mBoneNum ];

	    for( unsigned short i = 0 ; i < mBoneNum ; ++i )
	    {
		    mModelFrames[ i ].initializePMD( this, (const PMD_Bone *)data, i );

		    // 首のボーンを保存
		    //if( strncmp( m_pBoneArray[i].getName(), "頭", 20 ) == 0 )	m_pNeckBone = &m_pBoneArray[i];

            mFrameIndexMap.insert( FrameIndexPair( mModelFrames[ i ].getName(), i ) );

		    data += sizeof( PMD_Bone );
	    }

        // 全ボーンの初期化が終了した後に、親のあるボーンのオフセットを再計算
        for( unsigned short i = 0 ; i < mBoneNum ; ++i ) mModelFrames[i].postInitializePMD();

        //-----------------------------------------------------
	    // IK数取得
	    mIKNum = *((unsigned short *)data);
	    data += sizeof( unsigned short );

	    // IK配列を作成
	    if ( mIKNum )
	    {
		    mIKArray = LN_NEW ModelIK[ mIKNum ];

		    for( unsigned short i = 0 ; i < mIKNum ; ++i )
		    {
			    mIKArray[ i ].initializePMD( this, (const PMD_IK *)data );
			    data += sizeof( PMD_IK ) + sizeof(unsigned short) * (((PMD_IK *)data)->cbNumLink - 1);
		    }
		    qsort( mIKArray, mIKNum, sizeof( ModelIK ), cmpIKDataFunc );
	    }

	    //-----------------------------------------------------
	    // 表情数取得
	    mFaceNum = *((unsigned short *)data);
	    data += sizeof( unsigned short );

	    // 表情配列を作成
	    if ( mFaceNum )
	    {
		    mModelFaces = LN_NEW ModelFace[ mFaceNum ];

		    for( unsigned short i = 0 ; i < mFaceNum ; ++i )
		    {
			    mModelFaces[ i ].initializePMD( (const PMD_Face *)data, &mModelFaces[ 0 ] );
			    data += sizeof( PMD_Face ) + sizeof(PMD_FaceVtx) * (((PMD_Face *)data)->ulNumVertices - 1 );
		    }
	    }

        //-----------------------------------------------------
	    // ここから剛体情報まで読み飛ばし

	    // 表情枠用表示リスト
	    unsigned char	cbFaceDispNum = *((unsigned char *)data);
	    data += sizeof( unsigned char );

	    data += sizeof( unsigned short ) * cbFaceDispNum;

	    // ボーン枠用枠名リスト
	    unsigned char	cbBoneDispNum = *((unsigned char *)data);
	    data += sizeof( unsigned char );

	    data += sizeof( char ) * 50 * cbBoneDispNum;

	    // ボーン枠用表示リスト
	    unsigned long	ulBoneFrameDispNum = *((unsigned long *)data);
	    data += sizeof( unsigned long );

	    data += 3 * ulBoneFrameDispNum;

	    // 後続データの有無をチェック
	    if( (unsigned long)data - (unsigned long)data_top_ >= size_ )	return true;

	    // 英語名対応
	    unsigned char	cbEnglishNameExist = *((unsigned char *)data);
	    data += sizeof( unsigned char );

	    if( cbEnglishNameExist )
	    {
		    // モデル名とコメント(英語)
		    data += sizeof( char ) * 276;

		    // ボーン名(英語)
		    data += sizeof( char ) * 20 * mBoneNum;

		    // 表情名(英語)
		    data += sizeof( char ) * 20 * (mFaceNum - 1);	// "base"は英語名リストには含まれない

		    // ボーン枠用枠名リスト(英語)
		    data += sizeof( char ) * 50 * (cbBoneDispNum);
	    }

	    // 後続データの有無をチェック(ここのチェックは不要かも)
	    if( (unsigned long)data - (unsigned long)data_top_ >= size_ )	return true;

	    // トゥーンテクスチャリスト
	    data += 100 * 10;		// ファイル名 100Byte * 10個

	    // ここまで読み飛ばし
	    //-----------------------------------------------------

	    // 後続データの有無をチェック
	    if( (unsigned long)data - (unsigned long)data_top_ >= size_ )	return true;

        //-----------------------------------------------------
	    // 剛体数取得
	    mRigidBodyNum = *((unsigned long *)data);
	    data += sizeof( unsigned long );

	    // 剛体配列を作成
	    if ( mRigidBodyNum )
	    {
		    mModelRigidBodys = LN_NEW ModelRigidBody[ mRigidBodyNum ];

		    for ( lnU32 i = 0 ; i < mRigidBodyNum ; ++i )
		    {
                 new ( &mModelRigidBodys[ i ] ) ModelRigidBody( mManager );
                

			    const PMD_RigidBody* pmd_rigidbody = (const PMD_RigidBody *)data;

			    lnU32 frame_idx;
			    if ( pmd_rigidbody->unBoneIndex == 0xFFFF )	frame_idx = getFrameIndexByName( _T( "センター" ) );
			    else										frame_idx = pmd_rigidbody->unBoneIndex;


                if ( frame_idx != 0xffffffff )
                {
			        mModelRigidBodys[i].initializePMD( pmd_rigidbody, &mModelFrames[ frame_idx ] );
                }
			    data += sizeof( PMD_RigidBody );
		    }
	    }

        //-----------------------------------------------------
	    // コンストレイント
        ModelConstraint* constraints = NULL;
        lnU32 constraint_num = *((unsigned long *)data);
	    data += sizeof( unsigned long );

	    // コンストレイント配列を作成
	    if ( constraint_num )
	    {
		    constraints = LN_NEW ModelConstraint[ constraint_num ];

		    for( lnU32 i = 0 ; i < constraint_num ; ++i )
		    {
			    const PMD_Constraint* pmd_constraint = (const PMD_Constraint *)data;

			    ModelRigidBody* body_a = &mModelRigidBodys[ pmd_constraint->ulRigidA ];
				ModelRigidBody* body_b = &mModelRigidBodys[ pmd_constraint->ulRigidB ];

			    constraints[ i ].initializePMD( mManager, pmd_constraint, body_a, body_b );
			    data += sizeof( PMD_Constraint );
		    }
	    }


        //-----------------------------------------------------
	    // Model に格納

        model->mModelCore = this;
        lnU32 root_frame_idx = getFrameIndexByName( _T( "センター" ) );

        // PMD なら"センター"があるはずだけど、念のため無かった場合は親のない最初のフレームをルートとする
        // (右足 IK とかは親がないけど…)
        if ( root_frame_idx == 0xffffffff )
        {
            printf( "PMD : no センター\n" );
        }
       //if ( mModelFrames[ i ].getParent() == NULL )
        //{
        //    _p( mModelFrames[ i ].getName().c_str() );
        //}


        model->mFrameNum = mBoneNum;
        model->mModelFrames = mModelFrames;
        model->mRootFrame = &mModelFrames[ root_frame_idx ];
        for ( lnU32 i = 0; i < mBoneNum; ++i )
        {
            mModelFrames[ i ].updateWorldMatrixFromLocalAndParent();

            // ルートフレーム以外の親のないフレームのインデックスリスト
            if ( i != root_frame_idx && mModelFrames[ i ].getParent() == NULL )
            {
                mNoParentFrameIndexArray.push_back( i );
            }
        }


        model->mRigidBodyNum        = mRigidBodyNum;
        model->mModelRigidBodys     = mModelRigidBodys;
        model->mConstraintNum       = constraint_num;
        model->mConstraints         = constraints;
        model->mMotionController    = LN_NEW MotionController( mModelFrames, mBoneNum, mModelFaces, mFaceNum );

        //model->updateFrameMatrix();

        model->resetRigidBody();



        //SAFE_DELETE_ARRAY( model->mConstraints );



        // 変換テスト
        //if ( false )
        if ( !sw_skinning_ )
        {
            lnU32 new_attr_num;
            LNMeshAttributeRange* new_attrs;
            IVertexBuffer* new_vb;

            ConvertBlendedMesh< Vertex::PMDVertex, lnU16 > cbm;
            LN_CALL_R( cbm.convert(
                mGraphicsDevice,
                mVertexBuffer,
                mIndexBuffer,
                mAttrNum,
                mAttributeRanges,
                mBoneNum,
                2,
                26,
                &new_attr_num,
                &new_attrs,
                &new_vb ));

            LN_SAFE_RELEASE( mVertexBuffer );
            SAFE_DELETE_ARRAY( mAttributeRanges );
            mVertexBuffer = new_vb;
            mAttrNum = new_attr_num;
            mAttributeRanges = new_attrs;


            //_p(mIndexBuffer->getIndexNum());

            //SAFE_DELETE_ARRAY( mAttributeRanges );
            //mAttrNum = new_attr_num;
            //mAttributeRanges = new_attrs;
        }


        *model_ = model;
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● サブセットの描画
    //---------------------------------------------------------------------
    LNRESULT ModelCore::drawSubset( lnU32 idx_ )
    {
        IRenderer* r = mGraphicsDevice->getRenderer();
        r->setVertexBuffer( mVertexBuffer );
        r->setIndexBuffer( mIndexBuffer );
        r->drawPrimitiveIndexed(
            LN_PRIM_TRIANGLELIST, mAttributeRanges[ idx_ ].StartIndex, mAttributeRanges[ idx_ ].PrimitiveNum );
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● 剛体の位置修正、IK 更新
    //---------------------------------------------------------------------
    void ModelCore::correction( lnfloat elapsed_time_ )
    {
        // ボーン位置あわせ (剛体位置の移動)
	    for ( lnU32 i = 0 ; i < mRigidBodyNum ; ++i )
	    {
		    mModelRigidBodys[ i ].fixPosition( elapsed_time_ ); // もともとは elapsed_frame_
	    }

        // IKの更新
	    for ( lnU32 i = 0 ; i < mIKNum ; ++i )
	    {
		    mIKArray[i].update( mModelFrames ); // mModelFramesは仮
	    }
    }

    //---------------------------------------------------------------------
	// ● 名前からフレーム番号を取得する (見つからない場合は 0xffffffff)
    //---------------------------------------------------------------------
    lnU32 ModelCore::getFrameIndexByName( const lnChar* name_ )
    {
        FrameIndexMap::iterator it = mFrameIndexMap.find( name_ );
        if ( it != mFrameIndexMap.end() )
        {
            return it->second;
        }
        return 0xffffffff;
    }


    void skinningVertex(
        Vertex::PMDVertex* v_,
        const LVector3& ipos,
        const LVector3& inor,
        const LMatrix& skinning_mat,
        lnfloat weight_ )
    {
        LVector3 tmp_vec;

        LVector3::transform( &tmp_vec, ipos, skinning_mat );
        v_->Position += tmp_vec * weight_;

        LVector3::rotate( &tmp_vec, inor, skinning_mat );
        v_->Normal   += tmp_vec * weight_;
    }

    //---------------------------------------------------------------------
	// ● ソフトウェアスキニング
    //---------------------------------------------------------------------
    void ModelCore::skinning( ModelFrame* frames_ )
    {
        static const int EFF_BONE_NUM = 2;

        //LMatrix tmp_mat;
        LVector3 tem_vec;
        lnU32 v_num = mVertexBuffer->getVertexNum();
        Vertex::PMDVertex* vertices = (Vertex::PMDVertex*)mVertexBuffer->lock();
        Vertex::PMDVertex* v;
        LVector3* ipos;
        LVector3* inor;
        for ( lnU32 i = 0; i < v_num; ++i )
        {
            v    = &vertices[ i ];
            ipos = &mOrgVertexPositions[ i ];
            inor = &mOrgVertexNormals[ i ];
            v->Position = LVector3::ZERO;
            v->Normal   = LVector3::ZERO;

            //if ( i == 17957 )//i == 5985 )
            //{
            //    v->Position.cdump();
            //    //_p(last_weight);
            //    frames_[ v->BlendIndices[ 0 ] ].getSkinningMatrix().cdump();
            //    //ipos->cdump();
            //    //v->Position.cdump();
            //    //_p( v->BlendWeights[ 0 ] );
            //    //_p( v->BlendWeights[ 1 ] );
            //}

            // 最後の一つ以外の影響ボーン適用

            lnU32 bone_idx = 0;
            lnfloat last_weight = 0.0f;
            for ( ; bone_idx < EFF_BONE_NUM - 1; ++bone_idx )
            {
                skinningVertex(
                    v, *ipos, *inor,
                    frames_[ v->BlendIndices[ bone_idx ] ].getSkinningMatrix(),
                    v->BlendWeights[ bone_idx ] );

                last_weight += v->BlendWeights[ bone_idx ];
            }

            // 最後の影響ボーン適用
            skinningVertex(
                v, *ipos, *inor,
                frames_[ v->BlendIndices[ bone_idx ] ].getSkinningMatrix(),
                1.0f - last_weight );

            //if ( i == 17957 )//i == 5985 )
            //{
            //    v->Position.cdump();
            //    //_p(last_weight);
            //    frames_[ v->BlendIndices[ 0 ] ].getSkinningMatrix().cdump();
            //    //ipos->cdump();
            //    //v->Position.cdump();
            //    //_p( v->BlendWeights[ 0 ] );
            //    //_p( v->BlendWeights[ 1 ] );
            //}
            

            /*
            const LMatrix&  skinning_mat = frames_[ v->BlendIndices[ bone_idx ] ].getSkinningMatrix();
            last_weight = 1.0f - last_weight;

            LVector3::transform( &tem_vec, (*ipos), skinning_mat );
            v->Position += tem_vec * last_weight;

            LVector3::transform( &tem_vec, (*inor), skinning_mat );
            v->Normal   += tem_vec * last_weight;
            */

            /*
            tem_vec = (*ipos) + frames_[ v->BlendIndices[ bone_idx ] ].getSkinningMatrix().getPosition();
            v->Position += tem_vec * last_weight;
            
            LVector3::rotate( &tem_vec, *inor, frames_[ v->BlendIndices[ bone_idx ] ].getSkinningMatrix() );
            v->Normal   += tem_vec * last_weight;
            */


            v->Normal.normalize();
        }

        mVertexBuffer->unlock();

    }

    //---------------------------------------------------------------------
	// ● モデルデータ解放
    //---------------------------------------------------------------------
    void ModelCore::_release()
    {
        LN_SAFE_RELEASE( mVertexBuffer );
        LN_SAFE_RELEASE( mIndexBuffer );
        SAFE_DELETE_ARRAY( mOrgVertexPositions );
        SAFE_DELETE_ARRAY( mOrgVertexNormals );
        SAFE_DELETE_ARRAY( mMaterials );
        SAFE_DELETE_ARRAY( mAttributeRanges );
        SAFE_DELETE_ARRAY( mModelFrames );
        SAFE_DELETE_ARRAY( mIKArray );
        SAFE_DELETE_ARRAY( mModelFaces );
        SAFE_DELETE_ARRAY( mModelRigidBodys );
    }

    //---------------------------------------------------------------------
    // ● スフィアマップ用のテクスチャ名かを判定する
    //---------------------------------------------------------------------
    bool ModelCore::_isSphereMapTexName( const char* tex_name_ ) const
    {
	    int		len = strlen( tex_name_ );
	    bool	r = false;

	    if ( ( tex_name_[ len - 3 ] == 's' || tex_name_[ len - 3 ] == 'S') &&
		     ( tex_name_[ len - 2 ] == 'p' || tex_name_[ len - 2 ] == 'P') &&
		     ( tex_name_[ len - 1 ] == 'h' || tex_name_[ len - 1 ] == 'H') )
	    {
		    r = true;
	    }

	    return r;
    }

    

        //vertices = (Vertex::PMDVertex*)mVertexBuffer->lock();
        //mModelFaces[ 16 ].blendFace(vertices, 1.0f);
        //mVertexBuffer->unlock();



        /*
        D3DVERTEXELEMENT9 decl[MAXD3DDECLLENGTH];
        UINT decl_num;
        ((DirectX9::VertexBuffer*)mVertexBuffer)->getDxVertexDeclaration()->GetDeclaration( decl, &decl_num );

        ID3DXMesh* dx_mesh = NULL;
        D3DXCreateMesh(
            mIndexBuffer->getIndexNum() / 3,
            mVertexBuffer->getVertexNum(),
            D3DXMESH_MANAGED,
            decl,
            mGraphicsDevice->getDX9Device(),
            &dx_mesh );


        vertices = (Vertex::PMDVertex*)mVertexBuffer->lock();
        indices = mIndexBuffer->lock();


        void* buf;
        dx_mesh->LockVertexBuffer( 0, &buf );
        memcpy( buf, vertices, mVertexBuffer->getVertexNum() * mVertexBuffer->getVertexStride() );
        dx_mesh->UnlockVertexBuffer();

        dx_mesh->LockIndexBuffer( 0, &buf );
        memcpy( buf, indices, mIndexBuffer->getIndexNum() * sizeof( lnU16 ) );
        dx_mesh->UnlockIndexBuffer();



        D3DXATTRIBUTERANGE dx_attrs[ 256 ];
        for ( int i = 0; i < mAttrNum; ++i )
        {
            dx_attrs[ i ].AttribId = mAttributeRanges[ i ].AttribID;
            dx_attrs[ i ].FaceCount = mAttributeRanges[ i ].PrimitiveNum;
            dx_attrs[ i ].FaceStart = mAttributeRanges[ i ].FaceStart;
            dx_attrs[ i ].VertexCount = mAttributeRanges[ i ].VertexCount;
            dx_attrs[ i ].VertexStart = mAttributeRanges[ i ].StartIndex;       // ちょっと怪しい
        }

        dx_mesh->SetAttributeTable( dx_attrs, mAttrNum );


        mIndexBuffer->unlock();
        mVertexBuffer->unlock();



        SAFE_RELEASE( dx_mesh );

        _p(decl_num);
*/

 
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Lumine

//=============================================================================
//								end of file
//=============================================================================