//=============================================================================
//【 LRoundTripValue 】
//-----------------------------------------------------------------------------
///**
//  @file       RoundTripValue.h
//  @brief      RoundTripValue
//  @author     Riki
//*/
//=============================================================================

#pragma once

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
// ■ RoundTripValue
//-----------------------------------------------------------------------------
///**
//	@brief		往復する値のクラス
//*/
//=============================================================================
template < typename VALUE_T = lnfloat, typename TIME_T = lnfloat > 
class RoundTripValue
{
public:

	/// コンストラクタ
	//RoundTripValue() {}

	//---------------------------------------------------------------------
    ///**
    //  @brief      アニメーション開始
    // 
    //  @param[in]  begin_value_	: 開始値 ( 時間が 0.0 のときの値 )
    //  @param[in]  end_value_		: 終端値 ( 時間が cycle_time_ のときの値 )
    //  @param[in]  cycle_time_		: 変更にかける時間 (片道)
    //*/
    //---------------------------------------------------------------------
	void start( VALUE_T begin_value_, VALUE_T end_value_, TIME_T cycle_time_ )
	{
		mBeginValue = begin_value_;
		mEndValue = end_value_;
		mValue = 0;
		mCycleTime = cycle_time_;
		mCurrentTime = 0;
	}

	/// 時間を進める
	void advanceTime( TIME_T delta_time_ )
	{
		mCurrentTime += delta_time_;
		_calcValue();
	}

	/// 値の取得
	VALUE_T getValue() const { return mValue; }

private:

	void _calcValue()
	{
		if ( mCurrentTime > mCycleTime )
		{
			mCurrentTime = -mCycleTime;
		}
		TIME_T t = abs( mCurrentTime ) / mCycleTime;
		mValue = static_cast< VALUE_T >( mBeginValue * ( 1.0 - t ) + mEndValue * t );
	}

private:

	VALUE_T	mBeginValue;
	VALUE_T	mEndValue;
	VALUE_T	mValue;
	TIME_T	mCurrentTime;
	TIME_T	mCycleTime;
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Animation
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================