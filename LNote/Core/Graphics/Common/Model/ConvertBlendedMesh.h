//=============================================================================
//�y ConvertBlendedMesh �z
//-----------------------------------------------------------------------------
///**
//  @file       ConvertBlendedMesh.h
//  @brief      ConvertBlendedMesh
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <list>
#include "../../../Base/BoolArray.h"
#include "../../Interface.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

#if 0

//=============================================================================
// �� ConvertBlendedMesh �N���X
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
template <class VERTEX_T_, typename INDEX_T_>
class ConvertBlendedMesh
{
public:

    LNRESULT convert(
        IGraphicsDevice* device_,
        //lnU32 v_num_,
        //VERTEX_T_* vertices_,
        //lnU32 index_num_,
        //INDEX_T_*  indices_,
        IVertexBuffer* vb_,         // �ꎞ�o�b�t�@�Ƃ��Ă��낢��g����̂ŁA�g�p��͊J������
        IIndexBuffer* ib_,          // �ꕔ�����������܂�
        lnU32 attr_num_,
        LNMeshAttribute* attrs_,
        lnU32 max_bone_num_,                // �S�{�[����
        lnU32 max_vertex_bone_num_,              // ���_1������ɉe������ő�{�[���� (�ő� 255)
        lnU32 eff_bone_num_,
        lnU32* out_attr_num_,
        LNMeshAttributeRange** out_attrs_,
        IVertexBuffer** out_vb_ )
    {
        *out_attr_num_    = 0;
        *out_attrs_       = NULL;

        mGraphicsDevice = device_;
        mOrgVertexNum   = vb_->getVertexNum();
        mOrgVertices    = (VERTEX_T_*)vb_->lock();
        mOrgIndexNum    = ib_->getIndexNum();
        mOrgIndices     = ib_->lock();
        mMaxBoneNum     = max_bone_num_;
        mMaxVertexBoneNum = max_vertex_bone_num_;
        mOrgAttrNum     = attr_num_;
        mOrgAttrArray   = attrs_;
        mEffBoneNum     = eff_bone_num_;

        mSelectedVertexFlags.resize( mOrgVertexNum );
        mSelectedSubsetVertexFlags.resize( mOrgVertexNum );
        mIndexUpdatedVertexFlags.resize( mOrgVertexNum );
        mIndexUpdatedIndexFlags.resize( mOrgIndexNum );

        mNewVertexBuffer = NULL;

        // �ꎞ�������X�g (mNewAttrList) �̍쐬
        step1_CreateTempAttr();

        // �{�[���ԍ��̍Đݒ�
        step2_SetBoneIndices();

        // �V�������_�o�b�t�@�̍쐬�E�C���f�b�N�X�o�b�t�@�̏�������
        step3_CreateBuffer( vb_->isDynamic() );

        // �V���������̍쐬
        step4_CreateAttribute();

        vb_->unlock();
        ib_->unlock();


        int i = 0; 
        ln_foreach( TempAttr* it, mNewAttrList )
        {
            ++i;
            _p(i);
            SAFE_DELETE( it );
        }

        *out_attr_num_ = mNewAttrArraySize;
        *out_attrs_ = mNewAttrArray;
        *out_vb_ = mNewVertexBuffer;
        
        return LN_OK;
    

    }

private:

    //---------------------------------------------------------------------
    // �ꎞ�������X�g (mNewAttrList) �̍쐬
    void step1_CreateTempAttr()
    {
        mSelectedVertexFlags.clear();
        mNewIncreaseVertexCount = 0;

        mNewAttrList.clear();

        for ( lnU32 attr_i = 0; attr_i < mOrgAttrNum; ++attr_i )
        {
            LNMeshAttributeRange& ln_attr = mOrgAttrArray[ attr_i ];

            lnU32 v_num             = ln_attr.PrimitiveNum * 3;
            lnU32 start_index       = ln_attr.StartIndex;
            lnU32 end_index         = ln_attr.StartIndex + v_num - 1;
            lnU32 bone_count        = 0;
            lnU32 total_bone_count  = 0;
            lnU32 last_start_index  = start_index;
            lnU32 i;

            TempAttr* tmp_attr = LN_NEW TempAttr();
            tmp_attr->MaterialIndex = ln_attr.MaterialIndex;
            tmp_attr->StartIndex    = last_start_index;
            tmp_attr->EndIndex      = end_index;
            tmp_attr->resetSelectedBoneFlags( mMaxBoneNum, mMaxVertexBoneNum, &mIndexUpdatedVertexFlags );

            mSelectedSubsetVertexFlags.clear();

            for ( i = start_index; i <= end_index; )
            {
                // 3 ���̒��_(�O�p�`)�𒲂ׂ邽�߁A3���[�v
                for ( lnU32 j = 0; j < 3; ++j )
                {
                    lnU32 idx = i + j;                      // �C���f�b�N�X�o�b�t�@�̔ԍ�
                    INDEX_T_ v_idx = mOrgIndices[ idx ];    // ���_�ԍ�

                    // ���_���ǂ̃T�u�Z�b�g������Q�Ƃ���Ă��Ȃ��ꍇ
                    if ( !mSelectedVertexFlags.get( v_idx ) )
                    {
                        mSelectedVertexFlags.set( v_idx, true );
                        mSelectedSubsetVertexFlags.set( v_idx, true );
                    }
                    // ���_�����łɑ��̃T�u�Z�b�g�ɎQ�Ƃ���Ă���ꍇ
                    else
                    {
                        // ���݂̃T�u�Z�b�g(attr_i)����͂܂��Q�Ƃ���Ă��Ȃ��ꍇ
                        if ( !mSelectedSubsetVertexFlags.get( v_idx ) )
                        {
                            NewIndexPair pr;
                            pr.ToWriteIndex = idx;
                            pr.NewIndex = mOrgVertexNum + mNewIncreaseVertexCount;
                            pr.OrgBoneIndices.push_back( mOrgVertices[ v_idx ].BlendIndices[ 0 ] );  // �ꎞ�p�����
                            pr.OrgBoneIndices.push_back( mOrgVertices[ v_idx ].BlendIndices[ 1 ] );
                            tmp_attr->IndexPairList.push_back( pr );

                            ++mNewIncreaseVertexCount;
                            mSelectedSubsetVertexFlags.set( v_idx, true );
                        }
                    }
                }

                if ( mOrgIndices[ i + 0 ] == 12646 ||
                     mOrgIndices[ i + 1 ] == 12646 ||
                     mOrgIndices[ i + 2 ] == 12646 )
                {
                    _p(mOrgIndices[ i + 0 ] );
                    _p(mOrgIndices[ i + 1 ] );
                    _p(mOrgIndices[ i + 2 ] );
                }
                
                bone_count = tmp_attr->getNewEffBoneNum(
                    &mOrgVertices[ mOrgIndices[ i + 0 ] ],
                    &mOrgVertices[ mOrgIndices[ i + 1 ] ],
                    &mOrgVertices[ mOrgIndices[ i + 2 ] ] );

                // ����̐���ǉ�����ƁA�T�u�Z�b�g�̍ő�e���{�[�����𒴂��Ă��܂��ꍇ
                if ( total_bone_count + bone_count > mEffBoneNum )
                {
                    // �V���������Ƃ��Ă܂Ƃ߂�
                    tmp_attr->EndIndex = i - 1;
                    tmp_attr->EffBoneCount = total_bone_count;
                    mNewAttrList.push_back( tmp_attr );

                    // ���̃T�u�Z�b�g�p�̏���
                    total_bone_count = 0;
                    last_start_index = i;
                    

                    tmp_attr                = LN_NEW TempAttr();
                    tmp_attr->MaterialIndex = ln_attr.MaterialIndex;
                    tmp_attr->StartIndex    = last_start_index;
                    tmp_attr->EndIndex      = end_index;
                    tmp_attr->resetSelectedBoneFlags( mMaxBoneNum, mMaxVertexBoneNum, &mIndexUpdatedVertexFlags );

                    mSelectedSubsetVertexFlags.clear();
                    
                    // ����̃��[�v�Œ��ׂ��ʂ͎��̃T�u�Z�b�g�ɓ����̂ŁAi �͑��₳���ɑ�����
                    continue;
                }

                total_bone_count += bone_count;
                i += 3;
            }

            // �V���������Ƃ��Ă܂Ƃ߂�
            tmp_attr->EffBoneCount = total_bone_count;
            mNewAttrList.push_back( tmp_attr );
        }

        // ���ʃ`�F�b�N
        printf( "---- step1_CreateNewAttr() Result ----\n" );
        ln_foreach( TempAttr* i, mNewAttrList )
        {
            printf( "material:%d start_i:%u end_i:%u bone_count:%u\n",
                i->MaterialIndex, i->StartIndex, i->EndIndex, i->EffBoneCount );
        }   
        printf( "�ǉ�����ׂ����_��:%d\n", mNewIncreaseVertexCount );
        
    }

    //---------------------------------------------------------------------
    // �{�[���ԍ��̍Đݒ�
    void step2_SetBoneIndices()
    {
        // �����O�̍X�V�ςݒ��_���m�F
        {
            lnU32 count = 0;
            for ( lnU32 i = 0; i < mOrgVertexNum; ++i )
            {
                if ( mIndexUpdatedVertexFlags.get( i ) )
                {
                    ++count;
                }
            }
            printf( "update_count:%u mOrgVertexNum:%u\n", count, mOrgVertexNum );
        }



        int iiii = 0;
        ln_foreach( TempAttr* attr, mNewAttrList )
        {
            //_p(iiii);
            ++iiii;
            for ( lnU32 i = attr->StartIndex; i <= attr->EndIndex; i += 3 )
            {
                if ( mOrgIndices[ i + 0 ] == 11580 ||
                     mOrgIndices[ i + 1 ] == 11580 ||
                     mOrgIndices[ i + 2 ] == 11580 )
                {
                    _p(mOrgIndices[ i + 0 ] );
                    _p(mOrgIndices[ i + 1 ] );
                    _p(mOrgIndices[ i + 2 ] );

                    if ( mOrgIndices[ i + 0 ] == 11580 ) _p( (int)mOrgVertices[ mOrgIndices[ i + 0 ] ].BlendIndices[ 0 ] );
                    if ( mOrgIndices[ i + 1 ] == 11580 ) _p( (int)mOrgVertices[ mOrgIndices[ i + 1 ] ].BlendIndices[ 0 ] );
                    if ( mOrgIndices[ i + 2 ] == 11580 ) _p( (int)mOrgVertices[ mOrgIndices[ i + 2 ] ].BlendIndices[ 0 ] );
                }


                // �V�����e���{�[���ԍ����Z�b�g����
                attr->setNewBoneIndex( &mOrgVertices[ mOrgIndices[ i + 0 ] ], mOrgIndices[ i + 0 ] );
                attr->setNewBoneIndex( &mOrgVertices[ mOrgIndices[ i + 1 ] ], mOrgIndices[ i + 1 ] );
                attr->setNewBoneIndex( &mOrgVertices[ mOrgIndices[ i + 2 ] ], mOrgIndices[ i + 2 ] );
            }
        }


        // ������̍X�V�ςݒ��_���m�F
        {
            lnU32 count = 0;
            for ( lnU32 i = 0; i < mOrgVertexNum; ++i )
            {
                if ( mIndexUpdatedVertexFlags.get( i ) )
                {
                    ++count;
                }
            }
            printf( "update_count:%u mOrgVertexNum:%u\n", count, mOrgVertexNum );
        }
    }

    //---------------------------------------------------------------------
    // �V�������_�o�b�t�@�̍쐬�E�C���f�b�N�X�o�b�t�@�̏�������
    LNRESULT step3_CreateBuffer( bool is_dynamic_ )
    {
        LNRESULT lr;

        LN_CALL_R( mGraphicsDevice->createVertexBuffer(
            &mNewVertexBuffer, VERTEX_T_::elements(), mOrgVertexNum + mNewIncreaseVertexCount, NULL, is_dynamic_ ) );

        VERTEX_T_* new_vertices = (VERTEX_T_*)mNewVertexBuffer->lock();

        // �܂��͑S���R�s�[
        memcpy( new_vertices, mOrgVertices, sizeof( VERTEX_T_ ) * mOrgVertexNum );

        // �V�����쐬�������_�ւ̃C���f�b�N�X��ݒ�
        ln_foreach( TempAttr* attr, mNewAttrList )
        {
            ln_foreach( NewIndexPair& pr, attr->IndexPairList )
            {
                //new_vertices[ pr.NewIndex ] = mOrgVertices[ mOrgIndices[ pr.ToWriteIndex ] ];
                lnU32 idxx = mOrgIndices[ pr.ToWriteIndex ];
                memcpy( &new_vertices[ pr.NewIndex ], &mOrgVertices[ idxx ], sizeof( VERTEX_T_ ) );


                // �V�������_�̃{�[���C���f�b�N�X���Ȃ�������
                lnU32 cnt = 0;//mMaxVertexBoneNum�܂�
                ln_foreach( lnU32 bone_i, pr.OrgBoneIndices )
                {
                    VERTEX_T_& v = new_vertices[ pr.NewIndex ];
                    v.BlendIndices[ cnt ] = bone_i;


                    if ( attr->mSelectedBoneFlags[ v.BlendIndices[ cnt ] ] < 26 )//!= 0xffffffff )
                    {
                        v.BlendIndices[ cnt ] = static_cast< lnU8 >( attr->mSelectedBoneFlags[ v.BlendIndices[ cnt ] ] );
                    }
                    else if ( v.BlendWeights[ bone_i ] == 0.0f )
                    {
                        v.BlendIndices[ cnt ] = 0;
                    }

                    ++cnt;
                }
            }
        }

        ln_foreach( TempAttr* attr, mNewAttrList )
        {
            ln_foreach( NewIndexPair& pr, attr->IndexPairList )
            {


                if ( !mIndexUpdatedIndexFlags.get( pr.ToWriteIndex ) )
                {
                    mOrgIndices[ pr.ToWriteIndex ] = pr.NewIndex;
                    mIndexUpdatedIndexFlags.set( pr.ToWriteIndex, true );
                }
            }
        }


        // ���ʃ`�F�b�N
        printf( "---- step3_CreateBuffer() Result ----\n" );
        //for ( lnU32 i = 0; i < mOrgIndexNum; ++i )
        for ( lnU32 i = 0; i < mOrgVertexNum + mNewIncreaseVertexCount; ++i )
        {
            VERTEX_T_& v = new_vertices[ i ];//new_vertices[ mOrgIndices[ i ] ];

            // �u�����h���� 0 �ł͂Ȃ���
            if ( v.BlendWeights[ 0 ] == 0.0f &&
                 v.BlendWeights[ 1 ] == 0.0f )
            {
                printf( "�� invalid vertex %d\n", mOrgIndices[ i ] );
            }

            // �{�[���ԍ����e���ő吔�𒴂��Ă��Ȃ���
            if ( ( v.BlendIndices[ 0 ] >= mEffBoneNum ) ||
                 ( v.BlendIndices[ 1 ] >= mEffBoneNum ) )
            {
                //printf( "�� invalid vertex %d  BlendIndices[0]:%u [1]:%u\n",
                //    mOrgIndices[ i ],
                //    v.BlendIndices[ 0 ],
                //    v.BlendIndices[ 1 ] );
            }
        }

        mNewVertexBuffer->unlock();
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �V���������̍쐬
    void step4_CreateAttribute()
    {
        mNewAttrArraySize = mNewAttrList.size();
        mNewAttrArray = LN_NEW LNMeshAttributeRange[ mNewAttrArraySize ];

        lnU32 attr_i = 0;
        ln_foreach( TempAttr* attr, mNewAttrList )
        {
            mNewAttrArray[ attr_i ].MaterialIndex = attr->MaterialIndex;
            mNewAttrArray[ attr_i ].StartIndex = attr->StartIndex;
            mNewAttrArray[ attr_i ].PrimitiveNum = ( attr->EndIndex - attr->StartIndex ) / 3;
            mNewAttrArray[ attr_i ].BoneIndices.clear();

            // �{�[���ԍ����l�߂Ă���
            //      step1 �łЂƂ̃T�u�Z�b�g�̍ő�e���{�[�����ɍ��킹�ăT�u�Z�b�g�𕪊�����Ƃ��A
            //      getNewEffBoneNum() �� �ő吔�𒴂��Ă��Ȃ��� �Ƃ����菇�Ŕ��肵�Ă���B
            //      ���̂��߁A�������̎��̃T�u�Z�b�g�ɑ�����ׂ��{�[���ԍ����A
            //      �������̃T�u�Z�b�g�̂��̂Ƃ��Ċi�[����Ă���B
            //      (�ő�{�[������ 26 �ł���ꍇ�Aattr->mSelectedBoneNum �� 29 �Ƃ�)
            //      ���ۂɃX�L�j���O����Ƃ��ɉe���͂Ȃ����ǁA���ʂȗ̈悪�ł��Ă��܂��̂ŁA
            //      �����Ă���{�[�������e���ő吔(attr->EffBoneCount)�𒴂��Ă��܂��ꍇ��
            //      �����I�Ƀ��[�v���~�߂�����őΉ����Ă���B
            lnU32 total_bone_num = 0;
            for ( lnU32 bone_i = 0; bone_i < mMaxBoneNum; ++bone_i )
            {
                for ( lnU32 bone_j = 0; bone_j < mMaxBoneNum; ++bone_j )
                {
                    if ( attr->mSelectedBoneFlags[ bone_j ] == bone_i )
                    {
                        mNewAttrArray[ attr_i ].BoneIndices.push_back( bone_j );


                        ++total_bone_num;
                       
                        break;
                        
                    }

                    if ( total_bone_num >= attr->EffBoneCount )
                    {
                        break;
                    }
                }
            }

            // �`�F�b�N
            VERTEX_T_* new_vertices = (VERTEX_T_*)mNewVertexBuffer->lock();
            for ( lnU32 i = attr->StartIndex; i < attr->EndIndex; ++i )
            {
                lnU32 idx = mOrgIndices[ i ];
                VERTEX_T_& v = new_vertices[ idx ];

                // �{�[���ԍ����T�u�Z�b�g���Ƃ̉e���ő吔�𒴂��Ă��Ȃ���
                if ( ( v.BlendIndices[ 0 ] >= attr->EffBoneCount ) ||
                     ( v.BlendIndices[ 1 ] >= attr->EffBoneCount ) )
                {
                    printf( "�� invalid vertex %d  BlendIndices[0]:%u [1]:%u  / %u  attr:%d\n",
                        mOrgIndices[ i ],
                        v.BlendIndices[ 0 ],
                        v.BlendIndices[ 1 ],
                        attr->EffBoneCount,
                        attr_i );
                }
            }
            mNewVertexBuffer->unlock();

            ++attr_i;
        }
    }

private:

    struct NewIndexPair
    {
        INDEX_T_    ToWriteIndex;    ///< �C���f�b�N�X�o�b�t�@���̒l���������ޗv�f�̔ԍ�
        INDEX_T_    NewIndex;        ///< �������ޒl
        std::vector< lnU32 >       OrgBoneIndices;      ///< VB[ ToWriteIndex ] �̌��̃{�[���ԍ�
    };

    typedef std::list< NewIndexPair >   NewIndexPairList;

    struct TempAttr
    {
        lnU32               MaterialIndex;
        lnU32               StartIndex;
        lnU32               EndIndex;
        lnU32               EffBoneCount;      ///< ���̃T�u�Z�b�g�ւ̉e���{�[���� (�ő� mEffBoneNum)
        NewIndexPairList    IndexPairList;

        std::vector< lnU32 >    mSelectedBoneFlags;    ///< �I�����ꂽ�{�[���Ԃ̗v�f�ɁA0����n�܂�index���i�[����� (�I�����ꂽ��)                
        lnU32                   mSelectedBoneNum;
        lnU32                   mMaxBoneNum;
        lnU32                   mMaxVertexBoneNum;
        Base::BoolArray*         mIndexUpdatedVertexFlags;  ///< ConvertBlendedMesh::mIndexUpdatedVertexFlags �ւ̎Q��

        /// mSelectedBoneFlags ������
        void resetSelectedBoneFlags( lnU32 max_bone_num_, lnU32 max_vertex_bone_num_, Base::BoolArray* index_updated_vertex_flags_ )
        {
            mMaxBoneNum = max_bone_num_;
            mMaxVertexBoneNum = max_vertex_bone_num_;
            mIndexUpdatedVertexFlags = index_updated_vertex_flags_;
            mSelectedBoneFlags.resize( max_bone_num_ );
            for ( lnU32 i = 0; i < mMaxBoneNum; ++i ) mSelectedBoneFlags[ i ] = 0xffffffff;
            mSelectedBoneNum = 0;
            
        }

        /// 3�̒��_����V�����ǉ������{�[������Ԃ��A�I���ς݃t���O�𗧂Ă�
        lnU32 getNewEffBoneNum( VERTEX_T_* v0_, VERTEX_T_* v1_, VERTEX_T_* v2_ )
        {
            lnU32 bone_count = 0;
            VERTEX_T_* vs[ 3 ] = { v0_, v1_, v2_ };
            VERTEX_T_* v;
            for ( lnU32 i = 0; i < 3; ++i )
            {
                v = vs[ i ];

                for ( lnU32 bone_i = 0; bone_i < mMaxVertexBoneNum; ++bone_i )
                {
                    // �����Ɖe������{�[���ł���ꍇ
                    if ( v->BlendWeights[ bone_i ] > 0.0f )
                    {
                        // �܂��I������Ă��Ȃ��{�[���������ꍇ
                        if ( mSelectedBoneFlags[ v->BlendIndices[ bone_i ] ] == 0xffffffff )
                        {
                            mSelectedBoneFlags[ v->BlendIndices[ bone_i ] ] = mSelectedBoneNum;
                            ++mSelectedBoneNum;
                            ++bone_count;

                            //_p(mSelectedBoneNum);
                        }
                    }
                }
            }
            
            return bone_count;
        }

        /// ���_�Ƀ{�[���ԍ�����������
        void setNewBoneIndex( VERTEX_T_* v_, lnU32 v_index_ )
        {
            // �������_�ɕ�����{�[���C���f�b�N�X���ݒ肳��Ă��܂��̂�h�����߁A
            if ( !mIndexUpdatedVertexFlags->get( v_index_ ) )
            {
                for ( lnU32 bone_i = 0; bone_i < mMaxVertexBoneNum; ++bone_i )
                {
                    // �e���{�[���ő� 255 �͂����̃L���X�g�������B
                    // �x���o�Ă��\��Ȃ��Ȃ�O���ƁA
                    // v_->BlendIndices[ bone_i ] �̌^�̍ő�l���ő吔�ɂȂ�B
                    if ( mSelectedBoneFlags[ v_->BlendIndices[ bone_i ] ] < 26 )//!= 0xffffffff )
                    {
                        v_->BlendIndices[ bone_i ] = static_cast< lnU8 >( mSelectedBoneFlags[ v_->BlendIndices[ bone_i ] ] );
                    }
                    else if ( v_->BlendWeights[ bone_i ] == 0.0f )
                    {
                        v_->BlendIndices[ bone_i ] = 0;
                    }
                    else
                    {
                        //if ( mSelectedBoneFlags[ v_->BlendIndices[ bone_i ] ] != 0xffffffff )
                        //{
                        //    _p( mSelectedBoneFlags[ v_->BlendIndices[ bone_i ] ] );
                        //}
                    }

                    //_p((int)v_->BlendIndices[ bone_i ]);

                    if ( v_->BlendIndices[ bone_i ] >= 26 )
                    {
                        printf( "�~ %u %d\n",v_index_,  v_->BlendIndices[ bone_i ] );
                    }

                }

                mIndexUpdatedVertexFlags->set( v_index_, true );
            }
        }
    };

    typedef std::list< TempAttr* > TempAttrList;

private:

    IGraphicsDevice*        mGraphicsDevice;
    lnU32                   mOrgVertexNum;
    VERTEX_T_*              mOrgVertices;
    lnU32                   mOrgIndexNum;
    INDEX_T_*               mOrgIndices;
    lnU32                   mMaxBoneNum;
    lnU32                   mMaxVertexBoneNum;
    lnU32                   mEffBoneNum;
    lnU32                   mOrgAttrNum;
    LNMeshAttributeRange*   mOrgAttrArray;

    TempAttrList            mNewAttrList;

    Base::BoolArray         mSelectedVertexFlags;    ///< �����̃T�u�Z�b�g����Q�Ƃ���Ă��邩�̔����
    Base::BoolArray         mSelectedSubsetVertexFlags;     ///< (�T�u�Z�b�g�P�ʂŏ�����)
    Base::BoolArray         mIndexUpdatedVertexFlags;
    Base::BoolArray         mIndexUpdatedIndexFlags;
    lnU32                   mNewIncreaseVertexCount;    ///< �V�������₷���_��
    IVertexBuffer*          mNewVertexBuffer;
    lnU32                   mNewAttrArraySize;
    LNMeshAttributeRange*   mNewAttrArray;


};

#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================