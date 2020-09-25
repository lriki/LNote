//=============================================================================
//�y ModelIK �z
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
// �� ModelIK �N���X
//=============================================================================

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
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
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
    ModelIK::~ModelIK()
    {
        _release();
    }

    //---------------------------------------------------------------------
	// �� ������
    //---------------------------------------------------------------------
    void ModelIK::initializePMD( ModelCore* model_core_, const PMD_IK* ik_data_ )
    {
        _release();

        mModelCore = model_core_;

        // IK�^�[�Q�b�g�{�[��
	    mTargetBoneIdx = ik_data_->nTargetNo;

	    // IK��[�{�[��
	    mEffBoneIdx = ik_data_->nEffNo;

	    mCalcCount  = ik_data_->unCount;
	    mFact       = ik_data_->fFact * 3.1415926f;
	    mSortVal    = ik_data_->punLinkNo[0];

	    // IK�����N�z��̍쐬
	    mLinkNum = ik_data_->cbNumLink;
	    mBoneIndexList = LN_NEW lnU32[ mLinkNum ];
	    for( unsigned char i = 0 ; i < mLinkNum ; ++i )
	    {
            // �{�[���ԍ��͍~���Ŋi�[����Ă���
		    mBoneIndexList[ i ] = ik_data_->punLinkNo[ i ];

            // PMD �̎d�l�ŁA�G��IK�͎����ݒ肷�邱�ƂɂȂ��Ă���
            ModelFrame* bone = &( mModelCore->getModelFrames()[ mBoneIndexList[ i ] ] );
            if ( bone->getName() == _T( "���Ђ�" ) || bone->getName() == _T( "�E�Ђ�" ) )
            {
                bone->setIKLimitEnable( true );
            }
	    }
    }

    //---------------------------------------------------------------------
	// �� �K�p
    //---------------------------------------------------------------------
    void ModelIK::update( ModelFrame* frames_ )
    {
        ModelFrame* target_bone = &( frames_[ mTargetBoneIdx ] );
        ModelFrame* eff_bone    = &( frames_[ mEffBoneIdx ] );

        //_p(target_bone->getName().c_str());
        

        //if ( eff_bone->getName() == "�����V" )
        //{
        //    _p(eff_bone->getName().c_str());
        //}

        //LVector3 org_target_pos = target_bone->getWorldMatrix().getPosition();  // MMDX �� WorldMatrix ���炾����
        LVector3 eff_pos;
        LVector3 joint_pos;
        LVector3 target_pos;
        //LMatrix bone_inv;
        LMatrix inv_coord;

        LVector3 local_eff_pos;
        LVector3 local_target_pos;


        //�^�[�Q�b�g�ʒu�̎擾
        target_pos = target_bone->getWorldMatrix().getPosition();

        for ( lnU16 it = 0; it < mCalcCount; ++it )
        {
            for( lnU8 node_idx = 0 ; node_idx < mLinkNum ; ++node_idx )
		    {
                lnU32 bone_idx = mBoneIndexList[ node_idx ];
                ModelFrame* node = &frames_[ bone_idx ];

                

                // �G�t�F�N�^�̈ʒu
                eff_pos = eff_bone->getWorldMatrix().getPosition();

                // ���ڃm�[�h�̈ʒu�̎擾
                joint_pos = node->getWorldMatrix().getPosition();

                // ���[���h���W�n���璍�ڃm�[�h�̋Ǐ����W�n�ւ̕ϊ�
                LMatrix::inverse( &inv_coord, node->getWorldMatrix() );

                // �e�x�N�g���̍��W�ϊ����s���A�������̃{�[��i��̍��W�n�ɂ���
                // (1) ���ڃm�[�h���G�t�F�N�^�ʒu�ւ̃x�N�g��(a)(���ڃm�[�h)
                LVector3::transform( &local_eff_pos, eff_pos, inv_coord );
                // (2) ��֐�i���ڕW�ʒu�ւ̃x�N�g��(b)(�{�[��i����W�n)
                LVector3::transform( &local_target_pos, target_pos, inv_coord );

                

                // (1) ��֐߁��G�t�F�N�^�ʒu�ւ̕����x�N�g��
                local_eff_pos.normalize();
			    // (2) ��֐߁��ڕW�ʒu�ւ̕����x�N�g��
                local_target_pos.normalize();

                // �\���߂���ΏI��
                lnfloat dot = LVector3::dot( local_eff_pos, local_target_pos );
                if ( dot > 0.9999995f ) return;

                // ��]�p
			    float rot_angle = acosf( dot );
            
                if ( 0.0000001f < fabsf( rot_angle ) )
			    {
                    //��]�ʐ�����������
                    if ( rot_angle > LMath::PI * mFact * (node_idx + 1))
                         rot_angle = LMath::PI * mFact * (node_idx + 1);
                    if ( rot_angle < -LMath::PI * mFact * (node_idx + 1))
                         rot_angle = -LMath::PI * mFact * (node_idx + 1);

                    // ��]��
                    LVector3 rot_axis;
                    LVector3::cross( &rot_axis, local_eff_pos, local_target_pos );
                    // ������(�f�t�H�ł�) �����A�E���̐������K�v���ۂ��B
                    // MMDX �ł� rot_axis.y = 0 �ɂ��Ă銴���B
                    rot_axis.normalize();

                
                    // �֐߉�]�ʂ̕␳
				    LQuaternion rot_quat( rot_axis, rot_angle );
                    if ( frames_[ bone_idx ].isIKLimit() )	_limitAngle( &rot_quat, rot_quat );
                    
                    rot_quat.normalize();
                    LQuaternion::multiply( &node->mLocalTransform.Rotation, node->mLocalTransform.Rotation, rot_quat );
                    
                    if ( node->mLocalTransform.Rotation.isZero() )
                    {
                        printf("node->mLocalTransform.Rotation.isZero()\n");
                    }
                    node->mLocalTransform.Rotation.normalize();

                    // IK �\���{�[�����X�V����
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

                // �G�t�F�N�^�̈ʒu
                eff_pos = eff_bone->getWorldMatrix().getPosition();

                // ���ڃm�[�h�̈ʒu�̎擾







                // ���[���h���W�n���璍�ڃm�[�h�̋Ǐ�(���[�J��)���W�n�ւ̕ϊ�
                LMatrix::inverse( &bone_inv, frames_[ bone_idx ].getWorldMatrix() );

                // �G�t�F�N�^�C���B�ڕW�̃��[�J���ʒu
                eff_pos.transform( bone_inv );
                LVector3::transform( &target_pos, org_target_pos, bone_inv );

			    // �\���߂���ΏI��
                if ( LVector3::dot( eff_pos, target_pos ) < 0.0000001f ) return;

			    // (1) ��֐߁��G�t�F�N�^�ʒu�ւ̕����x�N�g��
                eff_pos.normalize();

			    // (2) ��֐߁��ڕW�ʒu�ւ̕����x�N�g��
                target_pos.normalize();
			    
			    // �x�N�g�� (1) �� (2) �Ɉ�v�����邽�߂̍ŒZ��]�ʁiAxis-Angle�j

			    // ��]�p
			    float r = acosf( LVector3::dot( eff_pos, target_pos ) );

                if ( 0.00000001f < fabsf( r ) )
			    {
				    if( r < -mFact )	    r = -mFact;
				    else if( mFact < r )	r =  mFact;

				    // ��]��
                    LVector3 rot_axis;
                    LVector3::cross( &rot_axis, eff_pos, target_pos );
                    _p(LVector3::dot( rot_axis, rot_axis ));
                    //if ( LVector3::dot( rot_axis, rot_axis ) < 0.0000001f ) continue;

                    rot_axis.normalize();

				    // �֐߉�]�ʂ̕␳
				    LQuaternion rot_quat( rot_axis, r );
				    
                    if ( frames_[ bone_idx ].isIKLimit() )	_limitAngle( &rot_quat, rot_quat );

                    rot_quat.normalize();

                    LQuaternion::multiply( &frames_[ bone_idx ].mRotation, frames_[ bone_idx ].mRotation, rot_quat );
				    
                    frames_[ bone_idx ].mRotation.normalize();

                    // IK �\���{�[�����X�V����
                    for ( short j = link_idx ; j >= 0 ; --j ) { frames_[ mBoneIndexList[ j ] ].updateMatrixInverse(); }
                    eff_bone->updateMatrixInverse();
			    }
            }
        }
        */
    }

    //---------------------------------------------------------------------
	// �� ���
    //---------------------------------------------------------------------
    void ModelIK::_release()
    {
        SAFE_DELETE_ARRAY( mBoneIndexList );
        mLinkNum = 0;
    }

    //---------------------------------------------------------------------
	// �� �{�[���̉�]�p�x�𐧌�����
    //---------------------------------------------------------------------
    void ModelIK::_limitAngle( LQuaternion* pvec_out_, const LQuaternion& pvec_src_ )
    {
        LVector3 angle;

	    // XYZ����]�̎擾
        LQuaternion::toEuler( &angle, pvec_src_ );

	    // �p�x����
	    if ( angle.x < -3.14159f )	angle.x = -3.14159f;
	    if ( -0.002f < angle.x )    angle.x = -0.002f;
	    angle.y = 0.0f;
	    angle.z = 0.0f;

	    // XYZ����]����N�H�[�^�j�I����
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