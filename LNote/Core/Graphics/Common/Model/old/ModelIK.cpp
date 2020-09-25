//=============================================================================
//【 ModelIK 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "ModelFrame.h"
#include "ModelCore.h"
#include "ModelIK.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace Lumine
{
namespace Graphics
{

//=============================================================================
// ■ ModelIK クラス
//=============================================================================

    //---------------------------------------------------------------------
	// ● コンストラクタ
    //---------------------------------------------------------------------
    ModelIK::ModelIK()
        : mModelCore        ( NULL )
        , mTargetBoneIdx    ( 0 )
        , mEffBoneIdx       ( 0 )
        , mCalcCount        ( 0 )
        , mFact             ( 0 )
        , mSortVal          ( 0 )
        , mLinkNum          ( 0 )
        , mBoneIndexList    ( NULL )
    {
    }

    //---------------------------------------------------------------------
	// ● デストラクタ
    //---------------------------------------------------------------------
    ModelIK::~ModelIK()
    {
        _release();
    }

    //---------------------------------------------------------------------
	// ● 初期化
    //---------------------------------------------------------------------
    void ModelIK::initializePMD( ModelCore* model_core_, const PMD_IK* ik_data_ )
    {
        _release();

        mModelCore = model_core_;

        // IKターゲットボーン
	    mTargetBoneIdx = ik_data_->nTargetNo;

	    // IK先端ボーン
	    mEffBoneIdx = ik_data_->nEffNo;

	    mCalcCount  = ik_data_->unCount;
	    mFact       = ik_data_->fFact * 3.1415926f;
	    mSortVal    = ik_data_->punLinkNo[0];

	    // IKリンク配列の作成
	    mLinkNum = ik_data_->cbNumLink;
	    mBoneIndexList = LN_NEW lnU32[ mLinkNum ];
	    for( unsigned char i = 0 ; i < mLinkNum ; ++i )
	    {
            // ボーン番号は降順で格納されている
		    mBoneIndexList[ i ] = ik_data_->punLinkNo[ i ];

            // PMD の仕様で、膝のIKは自動設定することになっている
            ModelFrame* bone = &( mModelCore->getModelFrames()[ mBoneIndexList[ i ] ] );
            if ( bone->getName() == _T( "左ひざ" ) || bone->getName() == _T( "右ひざ" ) )
            {
                bone->setIKLimitEnable( true );
            }
	    }
    }

    //---------------------------------------------------------------------
	// ● 適用
    //---------------------------------------------------------------------
    void ModelIK::update( ModelFrame* frames_ )
    {
        ModelFrame* target_bone = &( frames_[ mTargetBoneIdx ] );
        ModelFrame* eff_bone    = &( frames_[ mEffBoneIdx ] );

        //_p(target_bone->getName().c_str());
        

        //if ( eff_bone->getName() == "左髪７" )
        //{
        //    _p(eff_bone->getName().c_str());
        //}

        //LVector3 org_target_pos = target_bone->getWorldMatrix().getPosition();  // MMDX で WorldMatrix からだった
        LVector3 eff_pos;
        LVector3 joint_pos;
        LVector3 target_pos;
        //LMatrix bone_inv;
        LMatrix inv_coord;

        LVector3 local_eff_pos;
        LVector3 local_target_pos;


        //ターゲット位置の取得
        target_pos = target_bone->getWorldMatrix().getPosition();

        for ( lnU16 it = 0; it < mCalcCount; ++it )
        {
            for( lnU8 node_idx = 0 ; node_idx < mLinkNum ; ++node_idx )
		    {
                lnU32 bone_idx = mBoneIndexList[ node_idx ];
                ModelFrame* node = &frames_[ bone_idx ];

                

                // エフェクタの位置
                eff_pos = eff_bone->getWorldMatrix().getPosition();

                // 注目ノードの位置の取得
                joint_pos = node->getWorldMatrix().getPosition();

                // ワールド座標系から注目ノードの局所座標系への変換
                LMatrix::inverse( &inv_coord, node->getWorldMatrix() );

                // 各ベクトルの座標変換を行い、検索中のボーンi基準の座標系にする
                // (1) 注目ノード→エフェクタ位置へのベクトル(a)(注目ノード)
                LVector3::transform( &local_eff_pos, eff_pos, inv_coord );
                // (2) 基準関節i→目標位置へのベクトル(b)(ボーンi基準座標系)
                LVector3::transform( &local_target_pos, target_pos, inv_coord );

                

                // (1) 基準関節→エフェクタ位置への方向ベクトル
                local_eff_pos.normalize();
			    // (2) 基準関節→目標位置への方向ベクトル
                local_target_pos.normalize();

                // 十分近ければ終了
                lnfloat dot = LVector3::dot( local_eff_pos, local_target_pos );
                if ( dot > 0.9999995f ) return;

                // 回転角
			    float rot_angle = acosf( dot );
            
                if ( 0.0000001f < fabsf( rot_angle ) )
			    {
                    //回転量制限をかける
                    if ( rot_angle > LMath::PI * mFact * (node_idx + 1))
                         rot_angle = LMath::PI * mFact * (node_idx + 1);
                    if ( rot_angle < -LMath::PI * mFact * (node_idx + 1))
                         rot_angle = -LMath::PI * mFact * (node_idx + 1);

                    // 回転軸
                    LVector3 rot_axis;
                    LVector3::cross( &rot_axis, local_eff_pos, local_target_pos );
                    // ここで(デフォでは) 左足、右足の制限が必要っぽい。
                    // MMDX では rot_axis.y = 0 にしてる感じ。
                    rot_axis.normalize();

                
                    // 関節回転量の補正
				    LQuaternion rot_quat( rot_axis, rot_angle );
                    if ( frames_[ bone_idx ].isIKLimit() )	_limitAngle( &rot_quat, rot_quat );
                    
                    rot_quat.normalize();
                    LQuaternion::multiply( &node->mLocalTransform.Rotation, node->mLocalTransform.Rotation, rot_quat );
                    
                    if ( node->mLocalTransform.Rotation.isZero() )
                    {
                        printf("node->mLocalTransform.Rotation.isZero()\n");
                    }
                    node->mLocalTransform.Rotation.normalize();

                    // IK 構成ボーンを更新する
                    for ( short j = node_idx ; j >= 0 ; --j ) { 
                    	frames_[ mBoneIndexList[ j ] ].updateWorldMatrixFromLocalAndParent(); }
                    eff_bone->updateWorldMatrixFromLocalAndParent();
                }
            }
        }
        /*
        for ( lnU16 it = 0; it < mCalcCount; ++it )
        {
            for( lnU8 link_idx = 0 ; link_idx < mLinkNum ; ++link_idx )
		    {
                lnU32 bone_idx = mBoneIndexList[ link_idx ];

                // エフェクタの位置
                eff_pos = eff_bone->getWorldMatrix().getPosition();

                // 注目ノードの位置の取得







                // ワールド座標系から注目ノードの局所(ローカル)座標系への変換
                LMatrix::inverse( &bone_inv, frames_[ bone_idx ].getWorldMatrix() );

                // エフェクタ，到達目標のローカル位置
                eff_pos.transform( bone_inv );
                LVector3::transform( &target_pos, org_target_pos, bone_inv );

			    // 十分近ければ終了
                if ( LVector3::dot( eff_pos, target_pos ) < 0.0000001f ) return;

			    // (1) 基準関節→エフェクタ位置への方向ベクトル
                eff_pos.normalize();

			    // (2) 基準関節→目標位置への方向ベクトル
                target_pos.normalize();
			    
			    // ベクトル (1) を (2) に一致させるための最短回転量（Axis-Angle）

			    // 回転角
			    float r = acosf( LVector3::dot( eff_pos, target_pos ) );

                if ( 0.00000001f < fabsf( r ) )
			    {
				    if( r < -mFact )	    r = -mFact;
				    else if( mFact < r )	r =  mFact;

				    // 回転軸
                    LVector3 rot_axis;
                    LVector3::cross( &rot_axis, eff_pos, target_pos );
                    _p(LVector3::dot( rot_axis, rot_axis ));
                    //if ( LVector3::dot( rot_axis, rot_axis ) < 0.0000001f ) continue;

                    rot_axis.normalize();

				    // 関節回転量の補正
				    LQuaternion rot_quat( rot_axis, r );
				    
                    if ( frames_[ bone_idx ].isIKLimit() )	_limitAngle( &rot_quat, rot_quat );

                    rot_quat.normalize();

                    LQuaternion::multiply( &frames_[ bone_idx ].mRotation, frames_[ bone_idx ].mRotation, rot_quat );
				    
                    frames_[ bone_idx ].mRotation.normalize();

                    // IK 構成ボーンを更新する
                    for ( short j = link_idx ; j >= 0 ; --j ) { frames_[ mBoneIndexList[ j ] ].updateMatrixInverse(); }
                    eff_bone->updateMatrixInverse();
			    }
            }
        }
        */
    }

    //---------------------------------------------------------------------
	// ● 解放
    //---------------------------------------------------------------------
    void ModelIK::_release()
    {
        SAFE_DELETE_ARRAY( mBoneIndexList );
        mLinkNum = 0;
    }

    //---------------------------------------------------------------------
	// ● ボーンの回転角度を制限する
    //---------------------------------------------------------------------
    void ModelIK::_limitAngle( LQuaternion* pvec_out_, const LQuaternion& pvec_src_ )
    {
        LVector3 angle;

	    // XYZ軸回転の取得
        LQuaternion::toEuler( &angle, pvec_src_ );

	    // 角度制限
	    if ( angle.x < -3.14159f )	angle.x = -3.14159f;
	    if ( -0.002f < angle.x )    angle.x = -0.002f;
	    angle.y = 0.0f;
	    angle.z = 0.0f;

	    // XYZ軸回転からクォータニオンへ
        LQuaternion::fromEuler( pvec_out_, angle );

        //LN_PRINT_NOT_IMPL_FUNCTION;
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Lumine

//=============================================================================
//								end of file
//=============================================================================