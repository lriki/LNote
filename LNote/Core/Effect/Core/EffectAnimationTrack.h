//=============================================================================
//【 EffectAnimationTrack 】
//-----------------------------------------------------------------------------
///**
//  @file       EffectAnimationTrack.h
//  @brief      EffectAnimationTrack
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../Types.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Effect
{

//=============================================================================
// ■ IAnimationTrack
//-----------------------------------------------------------------------------
///**
//  @brief		
//
//  @note
//              キーの作成、移動などはすべて Managed 側で行う。
//              キーの状態が変更になるたびにすべてのキーを Native に反映させる形。
//*/
//=============================================================================
class IAnimationTrack
    : public Base::ReferenceObject
{
protected:

    virtual ~IAnimationTrack() {}

public:

    /// トラックの種類の取得
    virtual AnimationTrackType getType() const = 0;

    /// フレーム位置を設定して更新 (イベントの場合はひとつ前のフレーム位置からのイベント処理)
    virtual void update( lnTime frame_pos_ ) = 0;

    ///ランダム値を反映する
    virtual void resetRand( int seed_ ) {}

    /// update() で更新された値の取得 (とりあえず ValueAnimationTrack のみ。それ以外は return 0)
    virtual lnfloat getValue() const = 0;

    /// Node 初期値設定用。ValueAnimationTrack のみ有効 (キャストが面倒なので…)
    virtual void setDefaultValue( lnfloat value_ ) {}

    /// XMLElement へ保存
    virtual tinyxml2::XMLElement* serializeXMLElement( tinyxml2::XMLDocument* doc_ ) = 0;

    /// XMLElement から復元
    virtual void deserializeXMLElement( tinyxml2::XMLElement* elm_ ) = 0;

public:

    LRefTString     getElementName() const { return mElementName; }
    const LColor&   getElementColor() const { return mElementColor; }

protected:

    LRefTString     mElementName;
    LColor          mElementColor;
};

//=============================================================================
// ■ ValueAnimationTrack
//-----------------------------------------------------------------------------
///**
//  @brief		値ひとつ分
//*/
//=============================================================================
class ValueAnimationTrack
    : public Animation::Scalar
    , public IAnimationTrack
{
public:

    static const lnChar* XML_ELEMENT_NAME;
    static const lnChar* KEY_XML_ELEMENT_NAME;

public:

    ValueAnimationTrack();
    virtual ~ValueAnimationTrack();

public:

    /// 初期化
    void initialize( const TrackElement& element_ );

    /// トラックの種類の取得
    virtual AnimationTrackType getType() const { return ANIMTRACKTYPE_VALUE; }

    /// フレーム位置を設定して更新
    virtual void update( lnTime frame_pos_ );

    /// ランダム値を反映する
    virtual void resetRand( int seed_ );

    /// update() で更新された値の取得
    virtual lnfloat getValue() const;

    /// Node 初期値設定用。ValueAnimationTrack のみ有効 (キャストが面倒なので…)
    virtual void setDefaultValue( lnfloat value_ ) { Animation::Scalar::setDefaultValue( value_); }

    /// XMLElement へ保存
    virtual tinyxml2::XMLElement* serializeXMLElement( tinyxml2::XMLDocument* doc_ );

    /// XMLElement から復元
    virtual void deserializeXMLElement( tinyxml2::XMLElement* elm_ );

public:

    /// キー追加 (ソースデータ)
    void addEffectAnimationKey( const AnimationKey& key_ ) { mAnimationKeyArray.push_back( key_ ); }

    /// AnimationKey リストのクリア
    void clearEffectAnimationKeyArray() { mAnimationKeyArray.clear(); }

    AnimationKey* getEffectAnimationKey( int idx_ ) { return &mAnimationKeyArray[ idx_ ]; }

    int getEffectAnimationKeyNum() const { return mAnimationKeyArray.size(); }

    /// Animation::Scalar のキーをランダム値なしで再設定
    //      エディタでは基本的にタイムライン構築直前にこの関数で再設定を行う。
    //      ゲームでも、addEffectAnimationKey() や復元などでキーが変化した場合は一度この関数で再構築する
    void refreshKeysNotRand();

private:

    AnimationKeyArray   mAnimationKeyArray;
};

//=============================================================================
// ■ EventAnimationTrack
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class EventAnimationTrack
    : public IAnimationTrack
{
public:

    static const lnChar* XML_ELEMENT_NAME;

public:

    struct Key
    {
        LRefTString     SoundFilename;
        int             Volume;
        int             Pitch;
    };

public:

    EventAnimationTrack();
    virtual ~EventAnimationTrack();

public:

    /// 初期化
    void initialize( const TrackElement& element_ );

    /// トラックの種類の取得
    virtual AnimationTrackType getType() const { return ANIMTRACKTYPE_EVENT; }

    /// フレーム位置を設定して更新
    virtual void update( lnTime frame_pos_ );

    /// update() で更新された値の取得
    virtual lnfloat getValue() const { return 0; }

    /// XMLElement へ保存
    virtual tinyxml2::XMLElement* serializeXMLElement( tinyxml2::XMLDocument* doc_ );

    /// XMLElement から復元
    virtual void deserializeXMLElement( tinyxml2::XMLElement* elm_ );

public:

    /// キーの追加 (ソートは行わない。Managed 側でソート→clear()→ひとつずつaddKeyFrame())
    void addKeyFrame( const Key& key_ );

    /// キーを全て削除する
    void clear();
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Effect
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================