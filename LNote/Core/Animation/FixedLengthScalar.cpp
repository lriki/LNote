//=============================================================================
//【 FixedLengthScalar 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <stdlib.h>
#include <cmath>
#include "FixedLengthScalar.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Animation
{

//=============================================================================
// ■ FixedLengthScalar クラス
//=============================================================================

    FixedLengthScalar::Key::Key()
        : FramePos      ( 0.0f )
        , Value         ( 0.0f )
        , Velocity      ( 0.0f )
        , Accel         ( 0.0f )
        , LeftSlope     ( 0.0f )
        , RightSlope    ( 0.0f )
        , InterMode     ( LN_ANIM_INTER_LINEAR )
    {}

    //---------------------------------------------------------------------
    // ● デフォルトコンストラクタ
    //---------------------------------------------------------------------
    FixedLengthScalar::FixedLengthScalar()
        : mCapacity         ( 0 )
        , mKeyFrames        ( NULL )
        , mSize             ( 0 )
        , mTime             ( 0.0 )
        , mTimeTick         ( 60.0f )
        , mCurrentFramePos  ( 0.0f )
        , mLoopBeginPos     ( 0.0f )
        , mLoopEndPos       ( 0.0f )
        , mDefaultValue     ( 0.0f )
        , mValue            ( 0.0f )
        , mEnableLoop       ( false )
    {
    }

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    FixedLengthScalar::FixedLengthScalar( int capacity_ )
        : mCapacity         ( 0 )
        , mKeyFrames        ( NULL )
        , mSize             ( 0 )
        , mTimeTick         ( 60.0f )
        , mCurrentFramePos  ( 0.0f )
        , mLoopBeginPos     ( 0.0f )
        , mLoopEndPos       ( 0.0f )
        , mDefaultValue     ( 0.0f )
        , mValue            ( 0.0f )
        , mEnableLoop       ( false )
    {
        setCapacity( capacity_ );
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    FixedLengthScalar::~FixedLengthScalar()
    {
        SAFE_DELETE_ARRAY( mKeyFrames );
    }

    //---------------------------------------------------------------------
    // ● キーの最大数の設定 (現在のキーは全て破棄される)
    //---------------------------------------------------------------------
    void FixedLengthScalar::setCapacity( int capacity_ )
    {
        clear();
        SAFE_DELETE_ARRAY( mKeyFrames );
        mKeyFrames = LN_NEW Key[ capacity_ ];
        mCapacity = capacity_;
    }

    //---------------------------------------------------------------------
    // ● キーがひとつもないときに返す値の設定
    //---------------------------------------------------------------------
    void FixedLengthScalar::setDefaultValue( lnfloat value_ )
    {
        mDefaultValue = value_;
    }

    //---------------------------------------------------------------------
    // ● キーの追加
    //---------------------------------------------------------------------
    bool FixedLengthScalar::addKeyFrame( lnfloat frame_pos_, lnfloat value_ )
    {
        if ( mSize >= mCapacity ) { return false; }

        mKeyFrames[ mSize ].FramePos = frame_pos_;
        mKeyFrames[ mSize ].Value = value_;
        ++mSize;

        return true;
    }

    //---------------------------------------------------------------------
    // ● 時間の設定
    //---------------------------------------------------------------------
    void FixedLengthScalar::setTime( double time_ )
    {
        mTime = time_;


        time_ *= mTimeTick;

        
        if ( mEnableLoop )
        {
            // ループ位置未設定
            if ( mLoopEndPos == 0.0f && mLoopBeginPos == 0.0f )
            {
                if ( mKeyFrames[ mSize - 1 ].FramePos < time_ )
                {
                    time_ = fmod( time_, static_cast< double >( mKeyFrames[ mSize - 1 ].FramePos ) );
                }
            }
            // ループ位置設定済み
            else 
            {
                // time_ がループ領域を超えていた場合は開始時間から
                if ( mLoopEndPos < time_ )
                {
                    time_ -= mLoopBeginPos;
                    time_ = fmod( time_, ( mLoopEndPos - mLoopBeginPos ) );
                    time_ = mLoopBeginPos + time_;
                }
            }
        }

        mCurrentFramePos = time_;

        Key* key;

        if ( mSize )
        {
            // time_ が最初のフレーム位置より前の場合はデフォルト値
            if ( time_ < mKeyFrames[ 0 ].FramePos )
            {
                mValue = mDefaultValue;
            }
            // キーがひとつだけの場合はその値
            else if ( mSize == 1 )
            {
                mValue = mKeyFrames[ 0 ].Value;
            }
            // time_ が終端以降の場合は終端の値 ( _cmpKey() で終端のひとつ後ろを NULL 参照する対策 )
            else if ( time_ >= mKeyFrames[ mSize - 1 ].FramePos )
            {
                mValue = mKeyFrames[ mSize - 1 ].Value;
            }
            // 以上以外の場合は補間する
            else
            {
                // time_ の型と Key::FramePos の型は同じ主のにしておかないと正常に検索できない
                key = (Key*)bsearch( &time_, mKeyFrames, mSize, sizeof( Key ), _cmpKey );

                lnfloat p0 = key->Value;
		        lnfloat p1 = ( key + 1 )->Value;
                lnfloat t0 = static_cast< lnfloat >( key->FramePos );
		        lnfloat t1 = static_cast< lnfloat >( ( key + 1 )->FramePos );
                lnfloat t  = static_cast< lnfloat >( time_ - t0 ) / ( t1 - t0 );

                switch ( key->InterMode )
		        {
			        // 補間無し
			        case LN_ANIM_INTER_NONE:
                    {
				        mValue = p0;
                        break;
                    }
			        // 線形
			        case LN_ANIM_INTER_LINEAR:
                    {
				        mValue = p0 + ( p1 - p0 ) * t;
                        break;
                    }
			        // 等加速度
			        case LN_ANIM_INTER_ACCEL:
			        {
                        mValue = LMath::accel( p0, key->Velocity, key->Accel, static_cast< lnfloat >( mCurrentFramePos - key->FramePos )  );
                        break;
			        }
			        // 速度カーブ
			        case LN_ANIM_INTER_CUBIC:
			        {
				        mValue = LMath::cubic(
					        p0, p1, 
                            ( key + 1 )->RightSlope,
                            ( key + 1 )->LeftSlope,
					        t );
                        break;
			        }
			        // Catmull-Rom
			        case LN_ANIM_INTER_CATMULLROM:
			        {
                        // ループ再生で time が終端を超えている場合、
                        // この時点でkey の値は ループ開始位置のひとつ前のキーを指している

                        Key* begin = mKeyFrames;
                        Key* end   = &mKeyFrames[ mSize - 1 ];
                        
				        // この補間には、begin のひとつ前と end のひとつ後の値が必要。
				        // それぞれが始点、終点の場合はループするように補間する
				        mValue = LMath::catmullRom(
                            ( ( key == begin ) ? end->Value : ( key - 1 )->Value ),
					        p0,
					        p1,
					        ( ( ( key + 1 ) == end ) ? begin->Value : ( key + 2 )->Value ),
					        t );
                        break;
			        }
		        }
            }
        }
        else
        {
            mValue = mDefaultValue;
        }
    }

    //---------------------------------------------------------------------
    // ● 時間を進める
    //---------------------------------------------------------------------
    void FixedLengthScalar::advanceTime( lnfloat delta_time_ )
    {
        setTime( mTime + static_cast< double >( delta_time_ ) );
    }

    //---------------------------------------------------------------------
    // ● ループ再生の有効設定
    //---------------------------------------------------------------------
    void FixedLengthScalar::setEnableLoop( bool flag_ )
    {
        mEnableLoop = flag_;
    }

    //---------------------------------------------------------------------
    // ● ループ領域の設定
    //---------------------------------------------------------------------
    void FixedLengthScalar::setLoopRange( lnfloat begin_frame_, lnfloat frame_length_ )
    {
        mLoopBeginPos = begin_frame_;
        mLoopEndPos   = ( begin_frame_ + frame_length_ );
    }

    //---------------------------------------------------------------------
    // ● ループ時間の設定
    //---------------------------------------------------------------------
    void FixedLengthScalar::setLoopFrame( lnfloat begin_frame_, lnfloat end_frame_ )
    {
        mLoopBeginPos = begin_frame_;
        mLoopEndPos   = end_frame_;
    }

    //---------------------------------------------------------------------
    // ● 再生が終了しているかを判定する (ループ再生時は常に false)
    //---------------------------------------------------------------------
    bool FixedLengthScalar::isFinished() const
    {
        
        if ( mEnableLoop ) { return false; }
        if ( mSize == 0 ) { return true; }
        
        if ( mCurrentFramePos < mKeyFrames[ mSize - 1 ].FramePos ) { return false; }

        return true;
    }

    //---------------------------------------------------------------------
    // ● キーを全て削除する
    //---------------------------------------------------------------------
    void FixedLengthScalar::clear()
    {
        mSize = 0;
    }

    //---------------------------------------------------------------------
    // ● キーにアクセスする
    //---------------------------------------------------------------------
    FixedLengthScalar::Key& FixedLengthScalar::operator [] ( int idx_ )
    {
        if ( idx_ >= mSize )
        {
            // 警告とか
            *reinterpret_cast< int* >( 0 ) = 0;
        }
        return mKeyFrames[ idx_ ];
    }

    //---------------------------------------------------------------------
    // ● 比較関数
    //---------------------------------------------------------------------
    int FixedLengthScalar::_cmpKey( const void* a_, const void* b_ )
    {
        if ( ((Key*)a_)->FramePos < ((Key*)b_)->FramePos )
            return -1;
        else if ( ((Key*)a_)->FramePos >= ((Key*)b_)->FramePos && ((Key*)a_)->FramePos < (((Key*)b_+1))->FramePos )
            return 0;
        else
            return 1;
    }

    

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Animation
} // namespace Core

} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================