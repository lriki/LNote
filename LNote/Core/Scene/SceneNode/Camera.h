//=============================================================================
//【 Camera 】
//-----------------------------------------------------------------------------
///**
//  @file       Camera.h
//  @brief      Camera
//  @author     Riki
//*/
//=============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Graphics/Common/Camera.h"
#include "../Common.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{

//=============================================================================
// ■ CameraContext
//-----------------------------------------------------------------------------
///**
//  @brief
//*/
//=============================================================================
class CameraContext
{
public:

    CameraContext();
    virtual ~CameraContext();
    
public:

    void initialize( Camera* camera_ );

    virtual void updateContext();

public:

	const LVector4& getPosition() const { return mPosition; }

    const LVector3& getDirection() const { return mDirection; }

	lnfloat getFovY() const { return mFovY; }

	lnfloat getNearClip() const { return mNearClip; }

	lnfloat getFarClip() const { return mFarClip; }

    const LMatrix& getViewMatrix() const { return mViewMatrix; }

    // (ビルボード行列の計算で使用。setProjectionMatrix() 後でないと使えないので注意)
    const LMatrix& getViewMatrixInverse() const { return mCameraSceneParam.Mat_CameraView_Inverse; }    

    const LNCameraSceneParam& getCameraSceneParam() const { return mCameraSceneParam; }

    LNZSortFunc getZSortType() const { return mZSortFunc; }


    /// SceneGraphContext::OnSetActivePaneContext() から設定される
    void setProjectionMatrix( const LMatrix& proj_ );

    const LMatrix& getProjectionMatrix() const { return mProjectionMatrix; }

    /// WORLDVIEWPROJECTION セマンティクス等のために使う
    const LMatrix& getViewProjectionMatrix() const { return mViewProjMatrix; }

private:

    Camera*             mCamera;

    LVector4	        mPosition;
    LVector3            mDirection;

    // fovY 等は、Pane によるプロジェクション行列作成のために必要
    lnfloat             mFovY;
    lnfloat             mNearClip;
    lnfloat             mFarClip;
    
    LMatrix             mViewMatrix;        ///< ビュー行列
    LMatrix             mProjectionMatrix;  ///< プロジェクション行列
    LMatrix             mViewProjMatrix;    ///< ビュー行列とプロジェクション行列の積

    LNZSortFunc         mZSortFunc;
    LNCameraSceneParam  mCameraSceneParam;  ///< カメラ単位でシェーダに送るデータ

    bool                mProjection2D;
    bool                mOrthographic;
};


//=============================================================================
// ■ Camera クラス
//-----------------------------------------------------------------------------
///**
//  @brief		カメラを扱うクラス
//
//  @note
//              ・Camera = Pane として扱うべき？
//                  
//*/
//=============================================================================
class Camera
    : public Base::ReferenceObject
{
public:
    Camera( SceneGraph* scene_ );
    LN_REFOBJ_METHODS;

protected:
    virtual ~Camera();

public:

    // 初期化
    void initialize( bool projection_2d_ );

    /// カメラタイプの設定
	//void setType( LNCameraType type_ ) { mCoreCamera.setType( type_ ); }

	/// 位置の設定
    void setPosition( const LVector3& pos ) { mCoreCamera.setPosition( pos ); }

    /// 位置の取得
	const LVector3& getPosition() const { return mCoreCamera.getPosition(); }

	/// 注視点の設定
	void setLookAt( const LVector3& at ) { mCoreCamera.setLookAt( at ); }

    /// 注視点の取得
	const LVector3& getLookAt() const { return mCoreCamera.getPosition(); }

	/// 上方向ベクトルの設定
	void setUpDirection( const LVector3& up ) { mCoreCamera.setUpDirection( up ); }

    /// 上方向ベクトルの取得
	const LVector3& getUpDirection() const { return mCoreCamera.getUpDirection(); }

    /// 向きの取得 (正規化済み)
    const LVector3& getDirection() const { return mCoreCamera.getDirection(); }

    /// Y 方向視野角の設定
	void setFovY( lnfloat fov_y ) { mCoreCamera.setFovY( fov_y ); }

    /// Y 方向視野角の取得
	lnfloat getFovY() const { return mCoreCamera.getFovY(); }

	/// 最も近いビュープレーン位置の設定
	void setNearClip( lnfloat near_clip ) { mCoreCamera.setNearClip( near_clip ); }

    /// 最も近いビュープレーン位置の取得
	lnfloat getNearClip() const { return mCoreCamera.getNearClip(); }

	/// 最も遠いビュープレーン位置の設定
	void setFarClip( lnfloat far_clip ) { mCoreCamera.setFarClip( far_clip ); }

    /// 最も遠いビュープレーン位置の取得
	lnfloat getFarClip() const { return mCoreCamera.getFarClip(); }

    /// ビュー行列の取得
    const LMatrix& getViewMatrix() const { return mCoreCamera.getViewMatrix(); }

    /// プロジェクション行列の取得
	const LMatrix& getProjectionMatrix() const { return mCoreCamera.getProjectionMatrix(); }

    /// ビュー行列とプロジェクション行列の積の取得
	const LMatrix& getViewProjectionMatrix() const { return mCoreCamera.getViewProjectionMatrix(); }

    /// ビュー行列の逆行列の取得
	const LMatrix& getViewMatrixInverse() const { return mCoreCamera.getViewMatrixInverse(); }

    /// 視錐台の取得
    const LViewFrustum& getViewFrustum() const { return mCoreCamera.getViewFrustum(); }

public:

    /// カメラ操作クラスの設定 (参照カウントはインクリメントしないが、デストラクト時に release())
    void setCameraOperator( Graphics::CameraOperator* camera_operator ) { mCoreCamera.setCameraOperator( camera_operator ); }
    
    /// Z ソート方法の設定
    void setZSortType( LNZSortFunc type_ ) { mZSortFunc = type_; }

    /// Z ソート方法の取得
    LNZSortFunc getZSortType() const { return mZSortFunc; }

    /// カメラから見た 3D 座標を 2D 座標に変換する
    void convertWorldPosToScreenPos( LVector3* out, const LVector3& world_pos ) const;


    LNDrawCoordSpace getCoordSpace() const { return ( mProjection2D ) ? LN_DRAWCOORD_2D : LN_DRAWCOORD_3D; }



    /// 
    void updateContext() { mContext->updateContext(); }

    CameraContext* getCameraContext() { return mContext; }

public:

	/// (Pane にセットされたときに呼ばれる)
	void setHolderPane( Pane* pane );

    /// カメラ単位でシェーダに送るデータの取得 (行列演算などいろいろやってるので 1 フレーム複数回呼ぶのは避けること)
    LNCameraSceneParam* getCameraSceneParam();

    /// (called from SceneGraph::onEvent)
    LNRESULT onEvent( const System::Event& args );

protected:

    /*/// 回転操作の処理
	void _doMouseMoveR( lnfloat dx_, lnfloat dy_, lnfloat width_, lnfloat height_ );

    /// 平行移動の処理 (dx_ dy_ には移動量を設定する)
	void _doMouseMoveM( lnfloat dx_, lnfloat dy_ );

    /// マウスホイール操作の処理
	void _doMouseWheelEvent( int pos_ );*/

protected:

    ///// マウス操作関係のデータ
    //struct
    //{
    //    Geometry::Point	PrevPos;
    //    bool			RDrag;
    //    bool			MDrag;

    //} mMouseData;

protected:

    SceneGraph*             mScene;
	Pane*					mHolderPane;
    Graphics::Camera        mCoreCamera;

    //LNCameraType	        mType;			    ///< カメラの動作タイプ
	//LVector3	            mPosition;		    ///< カメラ座標
	//LVector3	            mLookAt;		    ///< 注視点
	//LVector3	            mUpDirection;	    ///< 上方向
 //   LVector3                mDirection;

    u32                     mClearFlag;         ///< LNClearFlag の組み合わせ
    /*lnfloat                 mFovY;
    lnfloat                 mNearClip;          ///< カメラから最も近い点
    lnfloat                 mFarClip;           ///< カメラから最も遠い点*/
    bool                    mOrthographic;      ///< 遠近感シミュレートフラグ

    //mutable LMatrix         mViewMatrix;        ///< ビュー行列
    //mutable LMatrix         mProjectionMatrix;  ///< プロジェクション行列
    //mutable LMatrix         mViewProjMatrix;    ///< ビュー行列とプロジェクション行列の積
    //mutable LMatrix         mViewProjMatrixInv; ///< ビュー行列の逆行列
    //mutable LViewFrustum    mViewFrustum;

    LNZSortFunc             mZSortFunc;
    LNCameraSceneParam      mCameraSceneParam;  ///< カメラ単位でシェーダに送るデータ
    CameraContext*          mContext;

    mutable bool            mMemberChanged;
     
    bool                    mProjection2D;

    friend class CameraContext;
};


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================