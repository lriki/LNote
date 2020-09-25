//=============================================================================
//【 FadeValue 】
//-----------------------------------------------------------------------------
///**
//  @file       FadeValue.h
//  @brief      FadeValue
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
// ■ FadeValue クラス
//-----------------------------------------------------------------------------
///**
//  @brief      フェードイン・アウトする値のクラス
//*/
//=============================================================================
template < typename TYPE_, typename TIME_TYPE_ = double > class FadeValue
{
public:

    //---------------------------------------------------------------------
    ///**
    //  @brief      コンストラクタ
    //*/
    //---------------------------------------------------------------------
	FadeValue()
		: mStartValue( 0 )
		, mTargetValue( 0 )
		, mValue( 0 )
		, mTime( 0.0 )
		, mFadeTime( 0.0 )
	{}

    //---------------------------------------------------------------------
    ///**
    //  @brief      フェード開始
    // 
    //  @param[in]  start_value_  : 開始時の値
    //  @param[in]  target_value_ : 終了時の値
    //  @param[in]  time_         : 変更にかける時間 ( 秒 )
    // 
    //  @par
    //              time_ 秒後に target_value_ の値になります。<br>
    //              time_ が 0 の場合、getValue() は常に target_value_ を返します。<br>
    //              <br>
    //              また、setTime() で設定されていた時間は 0.0 にリセットされます。<br>
    //*/
    //---------------------------------------------------------------------
	void start( TYPE_ start_value_, TYPE_ target_value_, TIME_TYPE_ time_ )
	{
		mStartValue = start_value_;
		mTargetValue = target_value_;
		mFadeTime = time_;
		mTime = 0.0;
        setTime( mTime );
	}

    //---------------------------------------------------------------------
    ///**
    //  @brief      値の取得
    // 
    //  @par
    //              経過時間が 0 以前の場合は 開始時の値を、
    //              経過時間が 変更にかける時間以上 の場合は 終了時の値 を返します。
    //*/
    //---------------------------------------------------------------------
	const TYPE_& getValue() const { return mValue; }

	/// 開始時の値の取得
	TYPE_ getStartValue() const { return mStartValue; }

	/// 終了時の値の取得
	TYPE_ getTargetValue() const { return mTargetValue; }

	/// 現在の時間の取得
	TIME_TYPE_ getTime() const { return mTime; }


    //---------------------------------------------------------------------
    ///**
    //  @brief      時間を設定する
    // 
    //  @param[in]  time_ : 設定する時間
    //*/
    //---------------------------------------------------------------------
	void setTime( TIME_TYPE_ time_ )
	{
		mTime = time_;
		_calcValue();
	}

    //---------------------------------------------------------------------
    ///**
    //  @brief      時間を進める
    // 
    //  @param[in]  delta_time_ : 進める時間
    //*/
    //---------------------------------------------------------------------
	void advanceTime( TIME_TYPE_ delta_time_ )
	{
		mTime += delta_time_;
		_calcValue();
	}

    //---------------------------------------------------------------------
    ///**
    //  @brief      終了を判定する
    //*/
    //---------------------------------------------------------------------
	bool isFinished() const
	{
		return ( mTime >= mFadeTime );
	}

	
private:

	/// 実際に値を計算する
	void _calcValue()
	{
        if ( mTime >= mFadeTime ) 
		{
			mValue = mTargetValue;
		}
		else if ( mTime <= 0.0 )
		{
			mValue = mStartValue;
		}
		else
		{
			mValue = mStartValue + (TYPE_)( ( mTargetValue - mStartValue ) * ( mTime / mFadeTime ) );	// 線形補間
            //mValue = mStartValue + static_cast< TYPE_ >( static_cast< double >( mTargetValue - mStartValue ) * ( mTime / mFadeTime ) );	// 線形補間
		    // 警告メモ:単純に double にキャストすると、Vector4 が変換でいないとか警告出る
        }

	}

private:

	TYPE_		mStartValue;
	TYPE_		mTargetValue;
	TYPE_		mValue;

	TIME_TYPE_  mTime;
	TIME_TYPE_  mFadeTime;
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