//=============================================================================
//【 Motion 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"

#include "Motion.h"

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
// ■ Motion クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    Motion::Motion()
        : mMaxFrames    ( 0.0 )
    {
    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    Motion::~Motion()
    {
        {
            MotionAnimationList::iterator itr = mMotionAnimationList.begin();
            MotionAnimationList::iterator end = mMotionAnimationList.end();
            for ( ; itr != end; ++itr )
            {
                SAFE_DELETE( *itr );
            }
        }
        {
            FaceAnimationList::iterator itr = mFaceAnimationList.begin();
            FaceAnimationList::iterator end = mFaceAnimationList.end();
            for ( ; itr != end; ++itr )
            {
                SAFE_DELETE( *itr );
            }
        }
    }

    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
    LNRESULT Motion::initializeVMD( const void* data_top_, lnU32 size_ )
    {
        mMotionAnimationList.clear();
        mMaxFrames = 0.0;

        const lnByte* data = static_cast< const lnByte* >( data_top_ );

        // ヘッダのチェック
	    VMD_Header* vmd_header = (VMD_Header *)data;
	    if ( strncmp( vmd_header->szHeader, "Vocaloid Motion Data 0002", 30 ) != 0 )
		    return LN_ERR_FORMAT;	// ファイル形式が違う

        printf("initializeVMD\n");

        data += sizeof( VMD_Header );

        //-----------------------------------------------------
        // ボーンアニメーション

	    // ボーンのキーフレーム数を取得
	    unsigned long	all_bone_key_num = *((unsigned long *)data);
	    data += sizeof( unsigned long );

	    VMD_Motion* vmd_motion = (VMD_Motion *)data;

	    for ( lnU32 i = 0 ; i < all_bone_key_num ; ++i, ++vmd_motion )
	    {
            // 最大フレーム更新
		    if ( mMaxFrames < (float)vmd_motion->ulFrameNo )	mMaxFrames = (float)vmd_motion->ulFrameNo;	

            // 既存アニメーション検索
            AnimationIndexMap::iterator itr = mAnimationIndexMap.find( vmd_motion->szBoneName );

            // 新しいボーン
            if ( itr == mAnimationIndexMap.end() )
            {
                // 名前・インデックスの対応
                mAnimationIndexMap.insert( AnimationIndexPair( LRefTString( vmd_motion->szBoneName ), mMotionAnimationList.size() ) );

                // アニメーション作成、キー追加
                MotionAnimation* anim = LN_NEW MotionAnimation();
                anim->setAnimType( LN_MODELANIM_VMD );
                anim->addVMDKeyFrame(
                    static_cast< double >( vmd_motion->ulFrameNo ),
                    vmd_motion->vec3Position,
                    vmd_motion->vec4Rotation,
                    vmd_motion->cInterpolationX,
                    vmd_motion->cInterpolationY,
                    vmd_motion->cInterpolationZ,
                    vmd_motion->cInterpolationRot );
                mMotionAnimationList.push_back( anim );
            }
            // すでにあるボーン
            else
            {
                mMotionAnimationList[ (itr->second) ]->addVMDKeyFrame(
                    static_cast< double >( vmd_motion->ulFrameNo ),
                    vmd_motion->vec3Position,
                    vmd_motion->vec4Rotation,
                    vmd_motion->cInterpolationX,
                    vmd_motion->cInterpolationY,
                    vmd_motion->cInterpolationZ,
                    vmd_motion->cInterpolationRot );
            }
	    }

        // キーフレーム順にソート
        MotionAnimationList::iterator itr = mMotionAnimationList.begin();
        MotionAnimationList::iterator end = mMotionAnimationList.end();
        for ( ; itr != end; ++itr )
        {
            (*itr)->sortKeyFrame();
        }

        data += sizeof( VMD_Motion ) * all_bone_key_num;

        //-----------------------------------------------------
	    // 表情アニメーション

        unsigned long	all_face_key_num = *((unsigned long *)data);
	    data += sizeof( unsigned long );

	    // モーションデータ中の表情ごとのキーフレーム数をカウント
	    VMD_Face* vmd_face = (VMD_Face*)data;

        for ( lnU32 i = 0 ; i < all_face_key_num ; ++i, ++vmd_face )
	    {
            // 最大フレーム更新
		    if ( mMaxFrames < (float)vmd_face->ulFrameNo )	mMaxFrames = (float)vmd_face->ulFrameNo;	

            // 既存アニメーション検索
            FaceAnimationIndexMap::iterator itr = mFaceAnimationIndexMap.find( vmd_face->szFaceName );

            // 新しいボーン
            if ( itr == mFaceAnimationIndexMap.end() )
            {
                // 名前・インデックスの対応
                mFaceAnimationIndexMap.insert( FaceAnimationIndexPair( LRefTString( vmd_face->szFaceName ), mFaceAnimationList.size() ) );

                // アニメーション作成、キー追加
                FaceAnimation* anim = LN_NEW FaceAnimation();
                anim->addKeyFrame( vmd_face->ulFrameNo, vmd_face->fFactor );
                mFaceAnimationList.push_back( anim );
            }
            // すでにあるボーン
            else
            {
                mFaceAnimationList[ (itr->second) ]->addKeyFrame( vmd_face->ulFrameNo, vmd_face->fFactor );
            }
	    }

        // キーフレーム順にソート
        FaceAnimationList::iterator itr2 = mFaceAnimationList.begin();
        FaceAnimationList::iterator end2 = mFaceAnimationList.end();
        for ( ; itr2 != end2; ++itr2 )
        {
            (*itr2)->sortKeyFrame();
        }

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// ● ボーン名に対応するアニメーションを返す
    //---------------------------------------------------------------------
    MotionAnimation* Motion::findBoneAnimation( const lnChar* name_ )
    {
        lnU32 i = findBoneAnimationIndex( name_ );
        if ( i != LN_INVALID_INDEX )
        {
            return mMotionAnimationList[ i ];
        }
        return NULL;
    }

    //---------------------------------------------------------------------
	// ● 表情名に対応するアニメーションを返す
    //---------------------------------------------------------------------
    FaceAnimation* Motion::findFaceAnimation( const lnChar* name_ )
    {
        lnU32 i = findFaceAnimationIndex( name_ );
        if ( i != LN_INVALID_INDEX )
        {
            return mFaceAnimationList[ i ];
        }
        return NULL;
    }

    //---------------------------------------------------------------------
	// ● ボーン名に対応するアニメーションの番号を返す
    //---------------------------------------------------------------------
    lnU32 Motion::findBoneAnimationIndex( const lnChar* name_ )
    {
        AnimationIndexMap::iterator itr = mAnimationIndexMap.find( name_ );
        if ( itr != mAnimationIndexMap.end() )
        {
            return itr->second;
        }
        return LN_INVALID_INDEX;
    }

    //---------------------------------------------------------------------
	// ● 表情名に対応するアニメーションの番号を返す
    //---------------------------------------------------------------------
    lnU32 Motion::findFaceAnimationIndex( const lnChar* name_ )
    {
        FaceAnimationIndexMap::iterator itr = mFaceAnimationIndexMap.find( name_ );
        if ( itr != mFaceAnimationIndexMap.end() )
        {
            return itr->second;
        }
        return LN_INVALID_INDEX;
    }

    //---------------------------------------------------------------------
	// ● 解放
    //---------------------------------------------------------------------
    //void Motion::_release()
    //{

    //}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================