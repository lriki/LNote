//=============================================================================
//【 ConvertBlendedMesh 】
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
// ■ ConvertBlendedMesh クラス
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
        IVertexBuffer* vb_,         // 一時バッファとしていろいろ使われるので、使用後は開放推奨
        IIndexBuffer* ib_,          // 一部書き換えられます
        lnU32 attr_num_,
        LNMeshAttribute* attrs_,
        lnU32 max_bone_num_,                // 全ボーン数
        lnU32 max_vertex_bone_num_,              // 頂点1つあたりに影響する最大ボーン数 (最大 255)
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

        // 一時属性リスト (mNewAttrList) の作成
        step1_CreateTempAttr();

        // ボーン番号の再設定
        step2_SetBoneIndices();

        // 新しい頂点バッファの作成・インデックスバッファの書き換え
        step3_CreateBuffer( vb_->isDynamic() );

        // 新しい属性の作成
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
    // 一時属性リスト (mNewAttrList) の作成
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
                // 3 つ分の頂点(三角形)を調べるため、3ループ
                for ( lnU32 j = 0; j < 3; ++j )
                {
                    lnU32 idx = i + j;                      // インデックスバッファの番号
                    INDEX_T_ v_idx = mOrgIndices[ idx ];    // 頂点番号

                    // 頂点がどのサブセットからも参照されていない場合
                    if ( !mSelectedVertexFlags.get( v_idx ) )
                    {
                        mSelectedVertexFlags.set( v_idx, true );
                        mSelectedSubsetVertexFlags.set( v_idx, true );
                    }
                    // 頂点がすでに他のサブセットに参照されている場合
                    else
                    {
                        // 現在のサブセット(attr_i)からはまだ参照されていない場合
                        if ( !mSelectedSubsetVertexFlags.get( v_idx ) )
                        {
                            NewIndexPair pr;
                            pr.ToWriteIndex = idx;
                            pr.NewIndex = mOrgVertexNum + mNewIncreaseVertexCount;
                            pr.OrgBoneIndices.push_back( mOrgVertices[ v_idx ].BlendIndices[ 0 ] );  // 一時用二つだけ
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

                // 今回の数を追加すると、サブセットの最大影響ボーン数を超えてしまう場合
                if ( total_bone_count + bone_count > mEffBoneNum )
                {
                    // 新しい属性としてまとめる
                    tmp_attr->EndIndex = i - 1;
                    tmp_attr->EffBoneCount = total_bone_count;
                    mNewAttrList.push_back( tmp_attr );

                    // 次のサブセット用の準備
                    total_bone_count = 0;
                    last_start_index = i;
                    

                    tmp_attr                = LN_NEW TempAttr();
                    tmp_attr->MaterialIndex = ln_attr.MaterialIndex;
                    tmp_attr->StartIndex    = last_start_index;
                    tmp_attr->EndIndex      = end_index;
                    tmp_attr->resetSelectedBoneFlags( mMaxBoneNum, mMaxVertexBoneNum, &mIndexUpdatedVertexFlags );

                    mSelectedSubsetVertexFlags.clear();
                    
                    // 今回のループで調べた面は次のサブセットに入れるので、i は増やさずに続ける
                    continue;
                }

                total_bone_count += bone_count;
                i += 3;
            }

            // 新しい属性としてまとめる
            tmp_attr->EffBoneCount = total_bone_count;
            mNewAttrList.push_back( tmp_attr );
        }

        // 結果チェック
        printf( "---- step1_CreateNewAttr() Result ----\n" );
        ln_foreach( TempAttr* i, mNewAttrList )
        {
            printf( "material:%d start_i:%u end_i:%u bone_count:%u\n",
                i->MaterialIndex, i->StartIndex, i->EndIndex, i->EffBoneCount );
        }   
        printf( "追加するべき頂点数:%d\n", mNewIncreaseVertexCount );
        
    }

    //---------------------------------------------------------------------
    // ボーン番号の再設定
    void step2_SetBoneIndices()
    {
        // 処理前の更新済み頂点数確認
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


                // 新しい影響ボーン番号をセットする
                attr->setNewBoneIndex( &mOrgVertices[ mOrgIndices[ i + 0 ] ], mOrgIndices[ i + 0 ] );
                attr->setNewBoneIndex( &mOrgVertices[ mOrgIndices[ i + 1 ] ], mOrgIndices[ i + 1 ] );
                attr->setNewBoneIndex( &mOrgVertices[ mOrgIndices[ i + 2 ] ], mOrgIndices[ i + 2 ] );
            }
        }


        // 処理後の更新済み頂点数確認
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
    // 新しい頂点バッファの作成・インデックスバッファの書き換え
    LNRESULT step3_CreateBuffer( bool is_dynamic_ )
    {
        LNRESULT lr;

        LN_CALL_R( mGraphicsDevice->createVertexBuffer(
            &mNewVertexBuffer, VERTEX_T_::elements(), mOrgVertexNum + mNewIncreaseVertexCount, NULL, is_dynamic_ ) );

        VERTEX_T_* new_vertices = (VERTEX_T_*)mNewVertexBuffer->lock();

        // まずは全部コピー
        memcpy( new_vertices, mOrgVertices, sizeof( VERTEX_T_ ) * mOrgVertexNum );

        // 新しく作成した頂点へのインデックスを設定
        ln_foreach( TempAttr* attr, mNewAttrList )
        {
            ln_foreach( NewIndexPair& pr, attr->IndexPairList )
            {
                //new_vertices[ pr.NewIndex ] = mOrgVertices[ mOrgIndices[ pr.ToWriteIndex ] ];
                lnU32 idxx = mOrgIndices[ pr.ToWriteIndex ];
                memcpy( &new_vertices[ pr.NewIndex ], &mOrgVertices[ idxx ], sizeof( VERTEX_T_ ) );


                // 新しい頂点のボーンインデックスをつなぎかえる
                lnU32 cnt = 0;//mMaxVertexBoneNumまで
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


        // 結果チェック
        printf( "---- step3_CreateBuffer() Result ----\n" );
        //for ( lnU32 i = 0; i < mOrgIndexNum; ++i )
        for ( lnU32 i = 0; i < mOrgVertexNum + mNewIncreaseVertexCount; ++i )
        {
            VERTEX_T_& v = new_vertices[ i ];//new_vertices[ mOrgIndices[ i ] ];

            // ブレンド率が 0 ではないか
            if ( v.BlendWeights[ 0 ] == 0.0f &&
                 v.BlendWeights[ 1 ] == 0.0f )
            {
                printf( "◆ invalid vertex %d\n", mOrgIndices[ i ] );
            }

            // ボーン番号が影響最大数を超えていないか
            if ( ( v.BlendIndices[ 0 ] >= mEffBoneNum ) ||
                 ( v.BlendIndices[ 1 ] >= mEffBoneNum ) )
            {
                //printf( "◆ invalid vertex %d  BlendIndices[0]:%u [1]:%u\n",
                //    mOrgIndices[ i ],
                //    v.BlendIndices[ 0 ],
                //    v.BlendIndices[ 1 ] );
            }
        }

        mNewVertexBuffer->unlock();
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // 新しい属性の作成
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

            // ボーン番号を詰めていく
            //      step1 でひとつのサブセットの最大影響ボーン数に合わせてサブセットを分割するとき、
            //      getNewEffBoneNum() → 最大数を超えていないか という手順で判定している。
            //      そのため、処理中の次のサブセットに属するべきボーン番号が、
            //      処理中のサブセットのものとして格納されている。
            //      (最大ボーン数が 26 である場合、attr->mSelectedBoneNum は 29 とか)
            //      実際にスキニングするときに影響はないけど、無駄な領域ができてしまうので、
            //      扱っているボーン数が影響最大数(attr->EffBoneCount)を超えてしまう場合は
            //      強制的にループを止める方向で対応している。
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

            // チェック
            VERTEX_T_* new_vertices = (VERTEX_T_*)mNewVertexBuffer->lock();
            for ( lnU32 i = attr->StartIndex; i < attr->EndIndex; ++i )
            {
                lnU32 idx = mOrgIndices[ i ];
                VERTEX_T_& v = new_vertices[ idx ];

                // ボーン番号がサブセットごとの影響最大数を超えていないか
                if ( ( v.BlendIndices[ 0 ] >= attr->EffBoneCount ) ||
                     ( v.BlendIndices[ 1 ] >= attr->EffBoneCount ) )
                {
                    printf( "◆ invalid vertex %d  BlendIndices[0]:%u [1]:%u  / %u  attr:%d\n",
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
        INDEX_T_    ToWriteIndex;    ///< インデックスバッファ内の値を書き込む要素の番号
        INDEX_T_    NewIndex;        ///< 書き込む値
        std::vector< lnU32 >       OrgBoneIndices;      ///< VB[ ToWriteIndex ] の元のボーン番号
    };

    typedef std::list< NewIndexPair >   NewIndexPairList;

    struct TempAttr
    {
        lnU32               MaterialIndex;
        lnU32               StartIndex;
        lnU32               EndIndex;
        lnU32               EffBoneCount;      ///< このサブセットへの影響ボーン数 (最大 mEffBoneNum)
        NewIndexPairList    IndexPairList;

        std::vector< lnU32 >    mSelectedBoneFlags;    ///< 選択されたボーン番の要素に、0から始まるindexが格納される (選択された順)                
        lnU32                   mSelectedBoneNum;
        lnU32                   mMaxBoneNum;
        lnU32                   mMaxVertexBoneNum;
        Base::BoolArray*         mIndexUpdatedVertexFlags;  ///< ConvertBlendedMesh::mIndexUpdatedVertexFlags への参照

        /// mSelectedBoneFlags 初期化
        void resetSelectedBoneFlags( lnU32 max_bone_num_, lnU32 max_vertex_bone_num_, Base::BoolArray* index_updated_vertex_flags_ )
        {
            mMaxBoneNum = max_bone_num_;
            mMaxVertexBoneNum = max_vertex_bone_num_;
            mIndexUpdatedVertexFlags = index_updated_vertex_flags_;
            mSelectedBoneFlags.resize( max_bone_num_ );
            for ( lnU32 i = 0; i < mMaxBoneNum; ++i ) mSelectedBoneFlags[ i ] = 0xffffffff;
            mSelectedBoneNum = 0;
            
        }

        /// 3つの頂点から新しく追加されるボーン数を返し、選択済みフラグを立てる
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
                    // ちゃんと影響するボーンである場合
                    if ( v->BlendWeights[ bone_i ] > 0.0f )
                    {
                        // まだ選択されていないボーンだった場合
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

        /// 頂点にボーン番号を書き込む
        void setNewBoneIndex( VERTEX_T_* v_, lnU32 v_index_ )
        {
            // 同じ頂点に複数回ボーンインデックスが設定されてしまうのを防ぐため、
            if ( !mIndexUpdatedVertexFlags->get( v_index_ ) )
            {
                for ( lnU32 bone_i = 0; bone_i < mMaxVertexBoneNum; ++bone_i )
                {
                    // 影響ボーン最大 255 はここのキャストが原因。
                    // 警告出ても構わないなら外すと、
                    // v_->BlendIndices[ bone_i ] の型の最大値が最大数になる。
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
                        printf( "× %u %d\n",v_index_,  v_->BlendIndices[ bone_i ] );
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

    Base::BoolArray         mSelectedVertexFlags;    ///< 複数のサブセットから参照されているかの判定に
    Base::BoolArray         mSelectedSubsetVertexFlags;     ///< (サブセット単位で初期化)
    Base::BoolArray         mIndexUpdatedVertexFlags;
    Base::BoolArray         mIndexUpdatedIndexFlags;
    lnU32                   mNewIncreaseVertexCount;    ///< 新しく増やす頂点数
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