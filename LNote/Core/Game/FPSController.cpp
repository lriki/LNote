//=============================================================================
//【 FPSController 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../System/OS.h"
#include "../Thread/Thread.h"
#include "FPSController.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Game
{

const GameTime GameTime::ZERO_TIME;

//=============================================================================
// ■ FPSController クラス
//=============================================================================

	//---------------------------------------------------------------------
	// ● コンストラクタ
	//---------------------------------------------------------------------
	FPSController::FPSController()
		: mFrameRate		( 0 )
		, mFrameRateRec		( 0.0 )
		, mFPS				( 0.0 )
		, mCurrentTime		( 0.0 )
		, mAverageTime		( 0.0 )
		, mFrameCount		( 0 )
		, mTerm				( 0.0 )
		, mLastTime			( 0.0 )
		, mBaseTime			( 0.0 )
		, mFrameTimes		( NULL )
        , mElapsedGameTime  ( 0.0 )
        , mLastRealTime     ( 0.0 )
        , mElapsedRealTime  ( 0.0 )
        , mStartTime        ( System::getSystemTime() )
        , mTotalGameTime    ( 0 )
        , mTotalRealTime    ( 0 )
        , mCapacityFPS      ( 0.0 )
        , mCapaFPSLastTime  ( 0.0 )
        , mCapaFrameTimes   ( NULL )
        , mCapaAverageTime  ( 0.0 )
        , mEnableFPSTest    ( false )
	{
		setFrameRate( 60 );

        mGameTime._m.FPS             = 0.0f;
        mGameTime._m.ElapsedGameTime = 0.0f;
        mGameTime._m.ElapsedRealTime = 0.0f;
        mGameTime._m.TotalGameTime   = 0.0;
        mGameTime._m.TotalRealTime   = 0.0;
        mGameTime._m.IsRunningSlowly = false;
        mGameTime._m.TotalFrameCount = 0;
	}

	//---------------------------------------------------------------------
	// ● デストラクタ
	//---------------------------------------------------------------------
	FPSController::~FPSController()
	{
		SAFE_DELETE_ARRAY( mFrameTimes );
        SAFE_DELETE_ARRAY( mCapaFrameTimes );
	}

	//---------------------------------------------------------------------
	// ● フレームレートの設定
	//---------------------------------------------------------------------
	void FPSController::setFrameRate( int frame_rate_ )
	{
		mFrameRate = frame_rate_;
		mFrameRateRec = 1.0f / mFrameRate;
		mFrameCount = 0;

		SAFE_DELETE_ARRAY( mFrameTimes );
		mFrameTimes = LN_NEW lnFPSFloat[ mFrameRate ];

        SAFE_DELETE_ARRAY( mCapaFrameTimes );
        mCapaFrameTimes = LN_NEW lnFPSFloat[ mFrameRate ];

		mTerm = 0.0f;
	}

	//---------------------------------------------------------------------
	// ● 遅延をリセットする
	//---------------------------------------------------------------------
	void FPSController::refreshSystemDelay( u32 current_time_ )
	{
		mLastTime = mBaseTime = mCapaFPSLastTime = ( 0.001f * ( current_time_ - mStartTime ) );// + mFrameRateRec;
		mBaseTime -= 1.0f - mFrameRateRec;
		
		// 現在の mFrameCount までのフレーム数から平均時間を計算する
		mAverageTime = 0;
		for( int i = 0; i < mFrameCount; ++i )
		{
			mAverageTime += mFrameTimes[ i ];
		}
		mAverageTime /= mFrameCount;

		mFrameCount = 0;
		mTerm = 0.0f;
	}

	//---------------------------------------------------------------------
	// ● 処理
	//---------------------------------------------------------------------
	void FPSController::process()
	{
		mCurrentTime = 0.001f * ( System::getSystemTime() - mStartTime );


        mElapsedGameTime = mCurrentTime - mLastTime;
        mElapsedRealTime = mCurrentTime - mLastRealTime;

        mTotalGameTime += static_cast< u32 >( 1000.0f * mElapsedGameTime );
        mTotalRealTime += static_cast< u32 >( 1000.0f * mElapsedRealTime );

		// (frame_rate_)フレームの1回目なら
		if ( mFrameCount == 0 )
		{
			// 完全に最初
			if ( mLastTime == 0.0 )
			{
				mTerm = mFrameRateRec;
			}
			// 前回記録した時間を元に計算
			else
			{
				mTerm = mBaseTime + 1.0f - mCurrentTime;
			}
            
            //printf( "f: a:%f bt:%f rc:%f ct:%f lt:%f\n", mTerm, mBaseTime, mFrameRateRec * mFrameCount, mCurrentTime, mCurrentTime - mLastRealTime );
		}
		else
		{
			// 今回待つべき時間 = 現在あるべき時刻 - 現在の時刻
			mTerm = ( mBaseTime + mFrameRateRec * mFrameCount ) - mCurrentTime;

            //if ( mTerm < 0 )
            //{
            //    printf( "c:%f\n", mTerm );
            //}
            //printf( "s: a:%f bt:%f rc:%f ct:%f lt:%f\n", mTerm, mBaseTime, mFrameRateRec * mFrameCount, mCurrentTime, mCurrentTime - mLastRealTime );
		}

        //printf( "frame:%d term:%f etime:%f\n", mFrameCount, mTerm, mElapsedRealTime );
        //printf( "etime:%f term:%f \n", mElapsedRealTime, mTerm );



        
        static int ii = 0;

        ++ii;

        


		// 待つべき時間だけ待つ
		if ( mTerm > 0.0 )
		{
            //printf( "t:%lf c:%d\n", mTerm, mFrameCount );

            
            //if ( ii > 120 && mTerm < 0.016 )
            {
                /*
                ii = 120;

                double bb = 0;
                for ( int i = 0; i < 60; ++i )
                {
                    _p( mFrameTimes[ i ] );
                    bb += mFrameTimes[ i ];
                }
                _p((double)ii / 60.0);

                printf( "a\n" );
                */
            }
            
            //::Sleep( 16 );

            Thread::sleep( static_cast< lnU32 >( mTerm * 1000 ) );
		}
		//// 待ち時間がない場合、次の描画をスキップするフラグを ON
		//else
		//{
		//}

		// 現在の時間
		lnFPSFloat nt = mCurrentTime;

		// mFrameRate フレームに1度基準を作る
		if ( mFrameCount == 0 )
		{
            // + mFrameRateRec で1フレームずらさないと、
            // 60 フレーム中の 1 フレーム目で待ち時間の計算をするときに
            // mCurrentTime の時間が現在あるべき時刻を超えてしまって、
            // 待ち時間が -0.0003 とかになってしまう。
            // 結果、一瞬だけｶｸｯという動作をすることがあった。
			mBaseTime = nt + mFrameRateRec;
		}

		// 今回の分の1周した時間を記録
		mFrameTimes[ mFrameCount ] = nt - mLastTime;

		// 現在の時間を、ひとつ前の時間として記憶
		mLastRealTime = mLastTime = nt;
        

		// mFrameRate の最後のフレーム ( 60 の場合は 59 フレーム目 ) に平均を計算
		if ( mFrameCount == mFrameRate - 1 )
		{
			mAverageTime = 0;
			for( int i = 0; i < mFrameRate; ++i )
			{
				mAverageTime += mFrameTimes[ i ];
			}
			mAverageTime /= mFrameRate;
		}





        if ( mEnableFPSTest )
        {
            // 今回の分の1周した時間を記録
		    mCapaFrameTimes[ mFrameCount ] = mCurrentTime - mCapaFPSLastTime;

            // mFrameRate の最後のフレーム ( 60 の場合は 59 フレーム目 ) に平均を計算
		    if ( mFrameCount == mFrameRate - 1 )
		    {
			    mCapaAverageTime = 0;
			    for ( int i = 0; i < mFrameRate; ++i )
			    {
				    mCapaAverageTime += mCapaFrameTimes[ i ];
			    }

                if ( mCapaAverageTime == 0.0f )
                {
                    mCapaAverageTime = 0.01f;
                }
			    mCapaAverageTime /= mFrameRate;

                mCapacityFPS = 1.0f / mCapaAverageTime;
                if ( mCapacityFPS > 100 * mFrameRate )
                {
                    mCapacityFPS = 100.0f * mFrameRate;
                }
		    }

            mCapaFPSLastTime = 0.001f * ( System::getSystemTime() - mStartTime );
        }

		// mFrameCount を frame で一周するようにする
		mFrameCount = ( ++mFrameCount ) % mFrameRate;

        // GameTime クラス更新
        mGameTime._m.FPS             = getFPS();
        mGameTime._m.ElapsedGameTime = getElapsedGameTime();
        mGameTime._m.ElapsedRealTime = getElapsedRealTime();
        mGameTime._m.TotalGameTime   = getTotalGameTime();
        mGameTime._m.TotalRealTime   = getTotalRealTime();
        mGameTime._m.IsRunningSlowly = isFrameSkipRequest();
        ++mGameTime._m.TotalFrameCount;
	}

    //---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
    void FPSController::processForMeasure()
    {
        mCurrentTime = 0.001f * ( System::getSystemTime() - mStartTime );

        mElapsedGameTime = mCurrentTime - mLastTime;
        mElapsedRealTime = mCurrentTime - mLastRealTime;

        mTotalGameTime += static_cast< u32 >( 1000.0f * mElapsedGameTime );
        mTotalRealTime += static_cast< u32 >( 1000.0f * mElapsedRealTime );

		// (frame_rate_)フレームの1回目なら
		if ( mFrameCount == 0 )
		{
			// 完全に最初
			if ( mLastTime == 0.0 )
			{
				mTerm = mFrameRateRec;
			}
			// 前回記録した時間を元に計算
			else
			{
				mTerm = mBaseTime + 1.0f - mCurrentTime;
			}            
		}
		else
		{
			// 今回待つべき時間 = 現在あるべき時刻 - 現在の時刻
			mTerm = ( mBaseTime + mFrameRateRec * mFrameCount ) - mCurrentTime;
		}

        static int ii = 0;

        ++ii;

		// 待つべき時間だけ待つ
		if ( mTerm > 0.0 )
		{
            //Thread::sleep( static_cast< lnU32 >( mTerm * 1000 ) );
		}
		
		// 現在の時間
		lnFPSFloat nt = mCurrentTime;

		// mFrameRate フレームに1度基準を作る
		if ( mFrameCount == 0 )
		{
            // + mFrameRateRec で1フレームずらさないと、
            // 60 フレーム中の 1 フレーム目で待ち時間の計算をするときに
            // mCurrentTime の時間が現在あるべき時刻を超えてしまって、
            // 待ち時間が -0.0003 とかになってしまう。
            // 結果、一瞬だけｶｸｯという動作をすることがあった。
			mBaseTime = nt + mFrameRateRec;
		}

		// 今回の分の1周した時間を記録
		mFrameTimes[ mFrameCount ] = nt - mLastTime;

		// 現在の時間を、ひとつ前の時間として記憶
		mLastRealTime = mLastTime = nt;
        

		// mFrameRate の最後のフレーム ( 60 の場合は 59 フレーム目 ) に平均を計算
		if ( mFrameCount == mFrameRate - 1 )
		{
			mAverageTime = 0;
			for( int i = 0; i < mFrameRate; ++i )
			{
				mAverageTime += mFrameTimes[ i ];
			}
			mAverageTime /= mFrameRate;
		}

        if ( mEnableFPSTest )
        {
            // 今回の分の1周した時間を記録
		    mCapaFrameTimes[ mFrameCount ] = mCurrentTime - mCapaFPSLastTime;

            // mFrameRate の最後のフレーム ( 60 の場合は 59 フレーム目 ) に平均を計算
		    if ( mFrameCount == mFrameRate - 1 )
		    {
			    mCapaAverageTime = 0;
			    for ( int i = 0; i < mFrameRate; ++i )
			    {
				    mCapaAverageTime += mCapaFrameTimes[ i ];
			    }

                if ( mCapaAverageTime == 0.0f )
                {
                    mCapaAverageTime = 0.01f;
                }
			    mCapaAverageTime /= mFrameRate;

                mCapacityFPS = 1.0f / mCapaAverageTime;
                if ( mCapacityFPS > 100 * mFrameRate )
                {
                    mCapacityFPS = 100.0f * mFrameRate;
                }
		    }

            mCapaFPSLastTime = 0.001f * ( System::getSystemTime() - mStartTime );
        }

		// mFrameCount を frame で一周するようにする
		mFrameCount = ( ++mFrameCount ) % mFrameRate;

        // GameTime クラス更新
        mGameTime._m.FPS             = getFPS();
        mGameTime._m.ElapsedGameTime = getElapsedGameTime();
        mGameTime._m.ElapsedRealTime = getElapsedRealTime();
        mGameTime._m.TotalGameTime   = getTotalGameTime();
        mGameTime._m.TotalRealTime   = getTotalRealTime();
        mGameTime._m.IsRunningSlowly = isFrameSkipRequest();
        ++mGameTime._m.TotalFrameCount;
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Game
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================