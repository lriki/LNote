//=============================================================================
//【 EffectNodeCore 】
//-----------------------------------------------------------------------------
///**
//  @file       EffectNodeCore.h
//  @brief      EffectNodeCore
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include "../../Math/Random.h"
#include "../../Geometry/Matrix.h"
#include "../../Graphics/Common/RenderState.h"
#include "../../Base/HierarchyNode.h"
#include "EffectMasterAnimationTrack.h"
#include "EffectAnimationTrackGroup.h"
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

//struct EffectNodeCommonParameter
//{
//    
//
//    // 初期値
//    EffectNodeCommonParameter()
//    {
//        Visible = true;
//        Name = _T( "Node" );
//        GenerateStartFramePos = 0;
//        GenerateInterval = 1;
//    }
//};

//=============================================================================
// ■ EffectNodeCore クラス
//-----------------------------------------------------------------------------
///**
//  @brief		エフェクトノード
//*/
//=============================================================================
class EffectNodeCore
    : public Base::HierarchyNode< EffectNodeCore >
{
public:

    
public:

    EffectNodeCore();

    virtual ~EffectNodeCore();

public:

    /// 初期化
    void initialize();

    /// 拡張トラックも含めたトラックグループの数
    int getAnimationTrackGroupNum();

    /// トラックグループ
    EffectAnimationTrackGroup* getAnimationTrackGroup( int idx_ );

    /// エミッタの設定(エディタから切り替える用)
    void setEmitterCore( EmitterCoreBase* emitter_ );

    EmitterCoreBase* getEmitterCore() { return mEmitter; }

    /// 共通パラメータの取得
    //EffectNodeCommonParameter* getCommonParameter() { return &mCommonParameter; }

    void        setName( LRefTString name_ ) { mName = name_; mModified = true; }

    LRefTString getName() const { return mName; }

    void        setMaxGenerateCount( int count_ ) { mMaxGenerateCount = count_; mModified = true; }

    int         getMaxGenerateCount() const { return mMaxGenerateCount; }

    void        setGenerateStartFramePos( lnTime interval_ ) { mGenerateStartFramePos = interval_; mModified = true; }

    lnTime      getGenerateStartFramePos() const { return mGenerateStartFramePos; }

    void        setGenerateInterval( lnTime interval_ ) { mGenerateInterval = interval_; mModified = true; }

    lnTime      getGenerateInterval() const { return mGenerateInterval; }

    bool        isModified() const { return mModified; }

    /// このノード以下の Modified フラグを設定する
    void setModifiedFlag( bool flag_ );

public:

    /// 子ノードを追加する
    virtual void addChild( NodeType* child_ );

    /// 子ノードを外す
    virtual void removeChild( NodeType* child_ );

    /// XML に保存 (作成された要素を返す)
    tinyxml2::XMLElement* saveToXML( tinyxml2::XMLDocument* doc_ );

    /// XML 要素から作成 (initialize() とは一緒に呼ばないこと)
    void loadFromXML( tinyxml2::XMLElement* elm_ );

    /// XML テキストにシリアライズ (コピペ用)
    LRefTString serializeXML();

    /// XML テキストからデシリアライズ (コピペ用) (initialize() とは一緒に呼ばないこと)
    void deserializeXML( LRefTString str );

private:

    LRenderState                    mRenderState;

    EffectAnimationTrackGroup       mCommonTrackGroupArray[ LN_EFFECT_COMMONTRACK_GROUPE_NUM ];
  
    EmitterCoreBase*                mEmitter;

    //EffectNodeCommonParameter       mCommonParameter;

    bool        mVisible;
    LRefTString mName;

    int         mMaxGenerateCount;
    lnTime      mGenerateStartFramePos;  ///< 生成開始フレーム位置 (親ノードのGenerateStartFramePosからのオフセット)
    lnTime      mGenerateInterval;       ///< 生成間隔 (1以上であること。生成数が 2 以上の場合に使用)

    //Animation::Scalar   mCommonAnimationTracks

    bool        mModified;
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