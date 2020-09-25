//=============================================================================
//�y ModelCore �z
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
namespace LNote
{
namespace Core
{
namespace Graphics
{

//=============================================================================
// �� ModelCore �N���X
//=============================================================================

    /// IK�f�[�^�\�[�g�p��r�֐�
    static int cmpIKDataFunc( const void* l_, const void* r_ )
    {
	    return (int)(((ModelIK *)l_)->getSortVal() - ((ModelIK *)r_)->getSortVal());
    }

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    ModelCore::ModelCore( ModelManager* manager_ )
        : mManager              ( manager_ )
        , mGraphicsDevice       ( manager_->getGraphicsDevice() )
        , mVertexBuffer         ( NULL )
        , mIndexBuffer          ( NULL )
        , mOrgVertexPositions   ( NULL )
        , mOrgVertexNormals     ( NULL )
        , mCollisionMesh        ( NULL )
        , mIKNum                ( 0 )
        , mIKArray              ( NULL )
        , mIKLimitter           ( NULL )
        , mFaceNum              ( 0 )
        , mModelFaces           ( NULL )
        , mIsSkinMesh           ( false )
    {
    }

    //---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
    ModelCore::~ModelCore()
    {
        _release();
    }

    //---------------------------------------------------------------------
	// �� PMD ���f���ǂݍ���
    //---------------------------------------------------------------------
    LNRESULT ModelCore::loadPMD( const void* data_top_, lnU32 size_, lnU32 flags_, Model** model_ )
    {
        LNRESULT lr;
        _release();

        // �X�L�����b�V��
        mIsSkinMesh = true;

        *model_ = NULL;

        bool is_dynamic = ( flags_ &LN_SKINNINGTYPE_SOFTWARE );

        const lnByte* data = static_cast< const lnByte* >( data_top_ );

        
        
        //-----------------------------------------------------
	    // �w�b�_�̃`�F�b�N

	    PMD_Header	*pPMDHeader = (PMD_Header *)data;
	    if( pPMDHeader->szMagic[0] != 'P' || pPMDHeader->szMagic[1] != 'm' || pPMDHeader->szMagic[2] != 'd' || pPMDHeader->fVersion != 1.0f )
		    return LN_ERR_FORMAT;	// �t�@�C���`�����Ⴄ

        // ���f�����̃R�s�[
        char model_name[21];	// ���f����
	    strncpy( model_name, pPMDHeader->szName, 20 );
	    model_name[20] = '\0';
        mModelName = model_name;

	    data += sizeof( PMD_Header );


        // �t�@�C���`���`�F�b�N��AModel �����
        Model* model = LN_NEW Model( mManager );
        model->mModelCore = this;

        //-----------------------------------------------------
	    // ���_���擾
        
        lnU32 v_num = *((lnU32 *)data);
        data += sizeof( lnU32 );

        // ���_�o�b�t�@�쐬
        LN_CALL_R( mGraphicsDevice->createVertexBuffer( &mVertexBuffer, Vertex::PMDVertex::elements(), v_num, NULL, is_dynamic ) );

        // �\�t�g�E�F�A�X�L�j���O�p
        if ( flags_ &LN_SKINNINGTYPE_SOFTWARE )
        {
            mOrgVertexPositions = LN_NEW LVector3[ v_num ];
            mOrgVertexNormals   = LN_NEW LVector3[ v_num ];
        }

	    // �f�[�^�𗬂�����
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

            if ( flags_ & LN_SKINNINGTYPE_SOFTWARE )
            {
                mOrgVertexPositions[ i ] = v->Position;
                mOrgVertexNormals[ i ]   = v->Normal;
            }
	    }

        mVertexBuffer->unlock();

        lnU32 size = sizeof( PMD_Vertex ) * v_num;
	    data += size;

        
        
        //-----------------------------------------------------
	    // ���_�C���f�b�N�X

	    lnU32 i_num = *((lnU32 *)data);
	    data += sizeof( lnU32 );
        lnU16* indices = NULL;

        // �C���f�b�N�X�o�b�t�@�쐬 (PMD �� 16bit)
        LN_CALL_R( mGraphicsDevice->createIndexBuffer( &mIndexBuffer, i_num, NULL, is_dynamic, true ) );

	    // ���_�C���f�b�N�X�z����R�s�[
        indices = (lnU16*)mIndexBuffer->lock();
	    memcpy( indices, data, sizeof( u16 ) * i_num );
        mIndexBuffer->unlock();

	    data += ( sizeof( u16 ) * i_num );

        //-----------------------------------------------------
	    // �}�e���A��

        // �}�e���A�����擾
        lnU32 attr_num = *((lnU32 *)data);
	    data += sizeof( lnU32 );

        // �}�e���A���A�����z��
        LMaterial*       materials  = LN_NEW LMaterial[ attr_num ];
        LNMeshAttribute* attributes = LN_NEW LNMeshAttribute[ attr_num ];

	    PMD_Material* pmd_material = (PMD_Material*)data;

        u32 index_offset = 0;
        lnU32 face_sum = 0;
        LMaterial* m;
        
	    for ( lnU32 i = 0 ; i < attr_num ; ++i )
	    {
            m = &materials[ i ];

            // �F���擾���Ă���
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
		   
            // �e�N�X�`����������ꍇ�̓e�N�X�`���쐬
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
                LN_SAFE_RELEASE( tex );

                // �X�t�B�A�e�N�X�`��
                if ( sphere_name )
                {
                    LN_CALL_R( mManager->createTexture( &tex, sphere_name ) );
                    m->setSphereTexture( tex );
                    LN_SAFE_RELEASE( tex );
                }                
		    }
		    else
		    {
			    m->setTexture( NULL );
                m->setSphereTexture( NULL );
		    }

            // �����e�[�u���𖄂߂�
            attributes[ i ].MaterialIndex = i;
            attributes[ i ].StartIndex = index_offset;
            attributes[ i ].PrimitiveNum = pmd_material->ulNumIndices / 3;
            //attributes[ i ].FaceStart = face_sum;
            //attributes[ i ].VertexCount = pmd_material->ulNumIndices / 3;

            index_offset += pmd_material->ulNumIndices;
            face_sum += attributes[ i ].PrimitiveNum;
		    pmd_material++;
	    }

        data += sizeof( PMD_Material ) * attr_num;

        // Model �Ɋi�[
        model->mAttrNum    = attr_num;
        model->mAttributes = attributes;
        model->mMaterials  = materials;

        //-----------------------------------------------------
	    // �{�[�����擾
	    lnU32 frame_num = *((lnU16 *)data);
	    data += sizeof( lnU16 );

	    // �{�[���z����쐬
	    ModelFrame* model_frames = LN_NEW ModelFrame[ frame_num ];

	    for ( lnU16 i = 0 ; i < frame_num ; ++i )
	    {
		    model_frames[ i ].initializePMD( this, (const PMD_Bone *)data, i, model_frames );

            mFrameIndexMap.insert( FrameIndexPair( model_frames[ i ].getName(), i ) );

		    data += sizeof( PMD_Bone );
	    }

        // �S�{�[���̏��������I��������ɁA�e�̂���{�[���̃I�t�Z�b�g���Čv�Z
        for ( lnU16 i = 0 ; i < frame_num ; ++i ) model_frames[i].postInitializePMD();

        // Model �Ɋi�[
        lnU32 root_frame_idx = getFrameIndexByName( _T( "�Z���^�[" ) );
        if ( root_frame_idx == LN_INVALID_INDEX ) // PMD �Ȃ�"�Z���^�["������͂������ǁA�O�̂��ߖ��������ꍇ�͐e�̂Ȃ��ŏ��̃t���[�������[�g�Ƃ���
        {
            printf( "PMD : �Z���^�[ �{�[��������܂���\n" );
        }
        model->mFrameNum    = frame_num;
        model->mModelFrames = model_frames;
        model->mRootFrame   = &model_frames[ root_frame_idx ];
        for ( lnU32 i = 0; i < frame_num; ++i )
        {
            model_frames[ i ].updateWorldMatrixFromLocalAndParent();

            // ���[�g�t���[���ȊO�̐e�̂Ȃ��t���[���̃C���f�b�N�X���X�g
            if ( i != root_frame_idx && model_frames[ i ].getParent() == NULL )
            {
                model->mNoParentFrameIndexArray.push_back( i );
            }
        }

        //-----------------------------------------------------
	    // IK���擾
	    mIKNum = *((unsigned short *)data);
	    data += sizeof( unsigned short );

	    // IK�z����쐬
	    if ( mIKNum )
	    {
		    mIKArray = LN_NEW ModelIK[ mIKNum ];

		    for( unsigned short i = 0 ; i < mIKNum ; ++i )
		    {
			    mIKArray[ i ].initializePMD( this, (const PMD_IK *)data, model_frames );
			    data += sizeof( PMD_IK ) + sizeof(unsigned short) * (((PMD_IK *)data)->cbNumLink - 1);
		    }
		    qsort( mIKArray, mIKNum, sizeof( ModelIK ), cmpIKDataFunc );
	    }

        mIKLimitter = LN_NEW ModelIKLimitter();

        
	    //-----------------------------------------------------
	    // �\��擾
	    mFaceNum = *((unsigned short *)data);
	    data += sizeof( unsigned short );

	    // �\��z����쐬
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
	    // �������獄�̏��܂œǂݔ�΂�

	    // �\��g�p�\�����X�g
	    unsigned char	cbFaceDispNum = *((unsigned char *)data);
	    data += sizeof( unsigned char );

	    data += sizeof( unsigned short ) * cbFaceDispNum;

	    // �{�[���g�p�g�����X�g
	    unsigned char	cbBoneDispNum = *((unsigned char *)data);
	    data += sizeof( unsigned char );

	    data += sizeof( char ) * 50 * cbBoneDispNum;

	    // �{�[���g�p�\�����X�g
	    unsigned long	ulBoneFrameDispNum = *((unsigned long *)data);
	    data += sizeof( unsigned long );

	    data += 3 * ulBoneFrameDispNum;

	    // �㑱�f�[�^�̗L�����`�F�b�N
	    if( (unsigned long)data - (unsigned long)data_top_ >= size_ )	return true;

	    // �p�ꖼ�Ή�
	    unsigned char	cbEnglishNameExist = *((unsigned char *)data);
	    data += sizeof( unsigned char );

	    if( cbEnglishNameExist )
	    {
		    // ���f�����ƃR�����g(�p��)
            char* name = (char*)data;
		    data += sizeof( char ) * 20;

            char* comment = (char*)data;
            data += sizeof( char ) * 256;

		    // �{�[����(�p��)
            for ( lnU32 i = 0; i < frame_num; ++i )
            {
                //char* name = (char*)data;
                //printf( "%d : %s\n", i, name );
		        data += 20;
            }

		    // �\�(�p��)
            for ( lnU32 i = 0; i < mFaceNum - 1; ++i )	// "base"�͉p�ꖼ���X�g�ɂ͊܂܂�Ȃ�
            {
                //LRefTString name( (char*)data, 20 );
                //printf( "%d : %s\n", i, name.c_str() );
		        data += 20;
            }

		    // �{�[���g�p�g�����X�g(�p��)
		    data += sizeof( char ) * 50 * (cbBoneDispNum);
	    }

	    // �㑱�f�[�^�̗L�����`�F�b�N(�����̃`�F�b�N�͕s�v����)
	    if( (unsigned long)data - (unsigned long)data_top_ >= size_ )	return LN_OK;

	    // �g�D�[���e�N�X�`�����X�g
        for ( int i = 0; i < 10; ++i )
        {
            //char* name = (char*)data;// �t�@�C���� 100Byte * 10��
            data += 100;
        }	

	    // �����܂œǂݔ�΂�
	    //-----------------------------------------------------

	    // �㑱�f�[�^�̗L�����`�F�b�N
	    if( (unsigned long)data - (unsigned long)data_top_ >= size_ )	return LN_OK;

        //-----------------------------------------------------
	    // ����
        ModelRigidBody* rigidbodys = NULL;
	    lnU32 rigidbody_num = *((unsigned long *)data);
	    data += sizeof( unsigned long );

	    // ���̔z����쐬
	    if ( rigidbody_num )
	    {
            rigidbodys = LN_NEW ModelRigidBody[ rigidbody_num ];

		    for ( lnU32 i = 0 ; i < rigidbody_num ; ++i )
		    {
                // ������Ƌ���̍�c
                new ( &rigidbodys[ i ] ) ModelRigidBody( mManager );
                

			    const PMD_RigidBody* pmd_rigidbody = (const PMD_RigidBody *)data;

			    lnU32 frame_idx;
			    if ( pmd_rigidbody->unBoneIndex == 0xFFFF )	frame_idx = getFrameIndexByName( _T( "�Z���^�[" ) );
			    else										frame_idx = pmd_rigidbody->unBoneIndex;


                if ( frame_idx != 0xffffffff )
                {
			        rigidbodys[ i ].initializePMD( pmd_rigidbody, &model_frames[ frame_idx ] );
                }
			    data += sizeof( PMD_RigidBody );
		    }
	    }

        // Model �Ɋi�[
        model->mRigidBodyNum    = rigidbody_num;
        model->mModelRigidBodys = rigidbodys;

        //-----------------------------------------------------
	    // �R���X�g���C���g
        ModelConstraint** constraints = NULL;
        lnU32 constraint_num = *((unsigned long *)data);
	    data += sizeof( unsigned long );

	    // �R���X�g���C���g�z����쐬
	    if ( constraint_num )
	    {
		    constraints = LN_NEW ModelConstraint*[ constraint_num ];

		    for( lnU32 i = 0 ; i < constraint_num ; ++i )
		    {
			    const PMD_Constraint* pmd_constraint = (const PMD_Constraint *)data;

			    ModelRigidBody* body_a = &rigidbodys[ pmd_constraint->ulRigidA ];
				ModelRigidBody* body_b = &rigidbodys[ pmd_constraint->ulRigidB ];

                constraints[ i ] = LN_NEW ModelConstraint();
			    constraints[ i ]->initializePMD( mManager, pmd_constraint, body_a, body_b );
			    data += sizeof( PMD_Constraint );
		    }
	    }

        model->mConstraintNum       = constraint_num;
        model->mConstraints         = constraints;


        

        

        //-----------------------------------------------------
	    // 
        
        model->mMotionController    = LN_NEW MotionController( model->mModelFrames, model->mFrameNum, mModelFaces, mFaceNum );

        model->resetRigidBody();

        //model->updateFrameMatrix();

        



        //SAFE_DELETE_ARRAY( model->mConstraints );



        // �ϊ��e�X�g
        //if ( false )
        /*
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
                mFrameNum,
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
        */

        if ( flags_ & LN_MODELFLAG_COLLISION )
        {
            _createCollisionBuffers();
        }

        *model_ = model;
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// �� ���O����t���[���ԍ����擾���� (������Ȃ��ꍇ�� 0xffffffff)
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
	// �� �\�t�g�E�F�A�X�L�j���O
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


            // �Ō�̈�ȊO�̉e���{�[���K�p
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

            // �Ō�̉e���{�[���K�p
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
	// �� ���f���f�[�^���
    //---------------------------------------------------------------------
    void ModelCore::_release()
    {
        LN_SAFE_RELEASE( mVertexBuffer );
        LN_SAFE_RELEASE( mIndexBuffer );
        SAFE_DELETE_ARRAY( mOrgVertexPositions );
        SAFE_DELETE_ARRAY( mOrgVertexNormals );
        if ( mCollisionMesh )
        {
            SAFE_DELETE_ARRAY( mCollisionMesh->CollisionVertexBuffer );
            SAFE_DELETE_ARRAY( mCollisionMesh->CollisionIndexBuffer );
            SAFE_DELETE( mCollisionMesh );
        }
        
        SAFE_DELETE_ARRAY( mIKArray );
        SAFE_DELETE( mIKLimitter );
        SAFE_DELETE_ARRAY( mModelFaces );
        
    }

    //---------------------------------------------------------------------
    // �� �X�t�B�A�}�b�v�p�̃e�N�X�`�������𔻒肷��
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

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void ModelCore::_createCollisionBuffers()
    {
        mCollisionMesh = LN_NEW CollisionMesh;

        //-----------------------------------------------------
        // ���_�o�b�t�@

        lnU32 v_num = mVertexBuffer->getVertexNum();
        LVector3* vertices = LN_NEW LVector3[ v_num ];

        Vertex::PMDVertex* org_vertices = (Vertex::PMDVertex*)mVertexBuffer->lock();
        for ( lnU32 i = 0 ; i < v_num; ++i )
	    {
            vertices[i] = org_vertices[i].Position;
	    }
        mVertexBuffer->unlock();

        mCollisionMesh->VertexCount = v_num;
        mCollisionMesh->CollisionVertexBuffer = vertices;

        //-----------------------------------------------------
	    // �C���f�b�N�X�o�b�t�@

        lnU32 i_num = mIndexBuffer->getIndexNum();
        lnU32 stride = mIndexBuffer->getIndexStride();
#if 1
        lnByte* indices = LN_NEW lnByte[i_num * 4];
        lnByte* org_indices = (lnByte*)mIndexBuffer->lock();

        if ( stride == 2 )
        {
            for ( lnU32 i = 0; i < i_num; ++i )
            {
                *((lnU32*)&indices[i * 4]) = *((lnU16*)&org_indices[i * stride]);
            }
        }
        else
        {
            for ( lnU32 i = 0; i < i_num; ++i )
            {
                *((lnU32*)&indices[i * 4]) = *((lnU32*)&org_indices[i * stride]);
            }
        }
        mIndexBuffer->unlock();

        mCollisionMesh->IndexCount = i_num;
        mCollisionMesh->IndexStride = 4;
        mCollisionMesh->CollisionIndexBuffer = indices;
#else
        lnByte* indices = LN_NEW lnByte[i_num * stride];
        
        memcpy( indices, mIndexBuffer->lock(), i_num * stride );
        mIndexBuffer->unlock();

        mCollisionMesh->IndexCount = i_num;
        mCollisionMesh->IndexStride = stride;
        mCollisionMesh->CollisionIndexBuffer = indices;
#endif
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