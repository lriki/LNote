//=============================================================================
//【 Motion 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <algorithm>
#include "Motion.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace Lumine
{
namespace Graphics
{

//=============================================================================
// ■ VMDBezier クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
    void VMDBezier::initialize( float fPointX1, float fPointY1, float fPointX2, float fPointY2 )
    {
	    if ( fPointX1 == fPointY1 && fPointX2 == fPointY2 )
	    {
		    mIsLinear = true;
	    }
	    else
	    {
		    fPointX1 = (fPointX1 / 127.0f) * 3.0f;
		    fPointY1 = (fPointY1 / 127.0f) * 3.0f;

		    fPointX2 = (fPointX2 / 127.0f) * 3.0f;
		    fPointY2 = (fPointY2 / 127.0f) * 3.0f;

		    // 高速化のためテーブル化しておく
		    mYValue[0] = 0.0f;
		    mYValue[YVAL_DIV_NUM] = 1.0f;

		    float	fAddX = 1.0f / (float)YVAL_DIV_NUM;

		    for( int i = 1 ; i < YVAL_DIV_NUM ; i++ )
		    {
			    mYValue[i] = getYValue( fAddX * i, fPointX1, fPointY1, fPointX2, fPointY2 );
		    }

		    mIsLinear = false;
	    }
    }

    //---------------------------------------------------------------------
	// ● X値に対応するY値を返す
    //---------------------------------------------------------------------
    float VMDBezier::getYValue( float fX, float fX1, float fY1, float fX2, float fY2 )
    {
	    float	fT = fX;
	    float	fInvT = 1.0f - fT;

	    for( int i = 0 ; i < 32 ; i++ )
	    {
		    float	fTempX = fInvT*fInvT*fT*fX1 + fInvT*fT*fT*fX2 + fT*fT*fT;

		    fTempX -= fX;
		    if ( fabsf( fTempX ) < 0.0001f )
		    {
			    break;
		    }
		    else
		    {
			    fT -= fTempX * 0.5f;
			    fInvT = 1.0f - fT;
		    }
	    }

	    return fInvT*fInvT*fT*fY1 + fInvT*fT*fT*fY2 + fT*fT*fT;
    }

    //---------------------------------------------------------------------
	// ● 曲線補間値を返す
    //---------------------------------------------------------------------
    float VMDBezier::getInterValue( float fX )
    {
	    if ( mIsLinear )	return fX;	// 線形補間

	    // テーブルを線形補間する
	    fX *= (float)YVAL_DIV_NUM;

	    int		iIdx = (int)fX;

	    fX -= iIdx;

	    return mYValue[iIdx] * (1.0f - fX) + mYValue[iIdx + 1] * fX;
    }

//=============================================================================
// ■ MotionAnimation
//=============================================================================

    /// ボーンキーフレームソート用比較関数
    static bool boneCompareFunc( const MotionAnimation::KeyFrame& l_, const MotionAnimation::KeyFrame& r_ )
    {
	    return l_.FrameNo < r_.FrameNo;
    }

    //---------------------------------------------------------------------
	// ● キーフレーム追加
    //---------------------------------------------------------------------
    void MotionAnimation::addKeyFrame( const VMD_Motion* vmd_motion_ )
    {
        mKeyFrameList.push_back( KeyFrame() );
        KeyFrame& key = mKeyFrameList.back();

        key.FrameNo  = vmd_motion_->ulFrameNo;
        key.Position = vmd_motion_->vec3Position;
        key.Rotation = vmd_motion_->vec4Rotation;
        key.Rotation.normalize();

        //key.Position.cdump();
        //key.Rotation.cdump();

        key.PosXInterBezier.initialize( vmd_motion_->cInterpolationX[0], vmd_motion_->cInterpolationX[4], vmd_motion_->cInterpolationX[8], vmd_motion_->cInterpolationX[12] );
		key.PosYInterBezier.initialize( vmd_motion_->cInterpolationY[0], vmd_motion_->cInterpolationY[4], vmd_motion_->cInterpolationY[8], vmd_motion_->cInterpolationY[12] );
		key.PosZInterBezier.initialize( vmd_motion_->cInterpolationZ[0], vmd_motion_->cInterpolationZ[4], vmd_motion_->cInterpolationZ[8], vmd_motion_->cInterpolationZ[12] );
		key.RotInterBezier.initialize( vmd_motion_->cInterpolationRot[0], vmd_motion_->cInterpolationRot[4], vmd_motion_->cInterpolationRot[8], vmd_motion_->cInterpolationRot[12] );
    }

    //---------------------------------------------------------------------
	// ● キーフレームのソート
    //---------------------------------------------------------------------
    void MotionAnimation::sortKeyFrame()
    {
        std::sort( mKeyFrameList.begin(), mKeyFrameList.end(), boneCompareFunc );
    }

    class GreaterEqual
    {
        double frame;
    public:
        GreaterEqual( double frame_ ) : frame( frame_ ){}
        bool operator()( const MotionAnimation::KeyFrame& key_ ) const
        {
            return frame <= key_.FrameNo;
        }
    };

    //---------------------------------------------------------------------
	// ● 時間の設定
    //---------------------------------------------------------------------
    void MotionAnimation::setTime( double time_, lnfloat tick_ )
    {
        double frame = time_ * tick_;

      
        // フレーム数 1 個
        if ( mKeyFrameList.size() == 1 )
        {
            mTransform.Rotation = mKeyFrameList.front().Rotation;
            mTransform.Translation = mKeyFrameList.front().Position;
            return;
        }

        // 最初のフレーム以前
        if ( frame <= mKeyFrameList.front().FrameNo )
        {
            mTransform.Rotation = mKeyFrameList.front().Rotation;
            mTransform.Translation = mKeyFrameList.front().Position;
            return;
        }

        // 最後のフレーム以降
        if ( frame >= mKeyFrameList.back().FrameNo )
        {
            mTransform.Rotation = mKeyFrameList.back().Rotation;
            mTransform.Translation = mKeyFrameList.back().Position;
            return;
        }

        // キー検索
        KeyFrameList::iterator itr = std::find_if( mKeyFrameList.begin(), mKeyFrameList.end(), GreaterEqual( frame ) );
        //if ( itr == mKeyFrameList.end() ) // ありえないはず
        //{
        //    mTransform = LSQTTransform::IDENTITY;
        //}

        //lnU32   k0idx = itr - mKeyFrameList.begin();
        //lnU32   k1idx = k0idx + 1;

        lnU32   k1idx = itr - mKeyFrameList.begin();
        lnU32   k0idx = k1idx - 1;
        

        KeyFrame& k0 = mKeyFrameList[ k0idx ];
        KeyFrame& k1 = mKeyFrameList[ k1idx ];

        float	t0 = k0.FrameNo;
	    float	t1 = k1.FrameNo;

        float rate = ( frame - t0 ) / ( t1 - t0 );
        float inter;


        inter = k1.PosXInterBezier.getInterValue( rate );
        mTransform.Translation.x = k0.Position.x * (1.0f - inter) + k1.Position.x * inter;

        inter = k1.PosYInterBezier.getInterValue( rate );
        mTransform.Translation.y = k0.Position.y * (1.0f - inter) + k1.Position.y * inter;

        inter = k1.PosZInterBezier.getInterValue( rate );
        mTransform.Translation.z = k0.Position.z * (1.0f - inter) + k1.Position.z * inter;

        inter = k1.RotInterBezier.getInterValue( rate );
        LQuaternion::slerp( &mTransform.Rotation, k0.Rotation, k1.Rotation, inter );
    }

//=============================================================================
// ■ FaceAnimation
//=============================================================================

    /// 表情キーフレームソート用比較関数
    static bool faceCompareFunc( const FaceAnimation::KeyFrame& l_, const FaceAnimation::KeyFrame& r_ )
    {
	    return l_.FrameNo < r_.FrameNo;
    }

    //---------------------------------------------------------------------
	// ● キーフレーム追加
    //---------------------------------------------------------------------
    void FaceAnimation::addKeyFrame( const VMD_Face* vmd_face_ )
    {
        mKeyFrameList.push_back( KeyFrame() );
        KeyFrame& key = mKeyFrameList.back();

        key.FrameNo  = vmd_face_->ulFrameNo;
        key.Rate     = vmd_face_->fFactor;
    }

    //---------------------------------------------------------------------
	// ● キーフレームのソート
    //---------------------------------------------------------------------
    void FaceAnimation::sortKeyFrame()
    {
        std::sort( mKeyFrameList.begin(), mKeyFrameList.end(), faceCompareFunc );
    }

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
                anim->addKeyFrame( vmd_motion );
                mMotionAnimationList.push_back( anim );
            }
            // すでにあるボーン
            else
            {
                mMotionAnimationList[ (itr->second) ]->addKeyFrame( vmd_motion );
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
                anim->addKeyFrame( vmd_face );
                mFaceAnimationList.push_back( anim );
            }
            // すでにあるボーン
            else
            {
                mFaceAnimationList[ (itr->second) ]->addKeyFrame( vmd_face );
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
} // namespace Lumine

//=============================================================================
//								end of file
//=============================================================================