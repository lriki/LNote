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
namespace LNote
{
namespace Core
{
namespace Graphics
{

//=============================================================================
// �� ModelIKLimitter �N���X
//=============================================================================

    //---------------------------------------------------------------------
	// �� ��]�p�x�����̓K�p
    //---------------------------------------------------------------------
    bool ModelIKLimitter::adjustRotation( ModelFrame* bone_ )
    {
        IKRotationLimits::iterator itr = mIKRotationLimits.find( bone_->getName().c_str() );
        if ( itr == mIKRotationLimits.end() ) return false;

        LQuaternion& bone_rot = bone_->getLocalTransformPtr()->Rotation;

#if 1

        LVector3 euler;
        LQuaternion::toEuler( &euler, bone_rot );

        bool limit = false;
        IKRotationLimit& lim = itr->second;
        limit |= lim.adjust( &euler.x, 0 );
        limit |= lim.adjust( &euler.y, 1 );
        limit |= lim.adjust( &euler.z, 2 );

        LQuaternion::fromEuler( &bone_rot, euler );

        return limit;

#else

        
        //bone_rot.cdump();

        lnfloat xrot, yrot, zrot;
        int factoring_type;

        // �܂���XYZ�ŕ��� (�W���o�����b�N����悤�Ȃ珇����ς��ď������Ă���)
        if ( !LQuaternion::factoringXYZ( &xrot, &yrot, &zrot, bone_rot ) )
        {
            // YZX�ŕ���
            if ( !LQuaternion::factoringYZX( &yrot, &zrot, &xrot, bone_rot ) )
            {
                // ZXY�ŕ���
                LQuaternion::factoringZXY( &zrot, &xrot, &yrot, bone_rot );
                factoring_type = 2;
            }
            else
            {
                factoring_type = 1;
            }
        }
        else
        {
            factoring_type = 0;
        }

        // �����K�p
        bool limit = false;
        IKRotationLimit& lim = itr->second;
        limit |= lim.adjust( &xrot, 0 );
        limit |= lim.adjust( &yrot, 1 );
        limit |= lim.adjust( &zrot, 2 );


        //if ( xrot <= lim.mMinRot[0] )
        //{
        //    printf("min %f %f\n", xrot, lim.mMinRot[0]);
        //}

        //if ( xrot >= lim.mMaxRot[0] )
        //{
        //    printf("max %f %f %f\n", xrot, lim.mMaxRot[0], pre);
        //}


        
        if ( factoring_type == 0 )
        {
            LMatrix mat;
            mat.rotationX( xrot );
            mat.rotationY( yrot );
            mat.rotationZ( zrot );
            LQuaternion::rotationMatrix( &bone_rot, mat );
        }
        else if ( factoring_type == 1 )
        {
            _p(factoring_type);
            LMatrix mat;
            mat.rotationY( yrot );
            mat.rotationZ( zrot );
            mat.rotationX( xrot );
            LQuaternion::rotationMatrix( &bone_rot, mat );
        }
        else
        {
            _p(factoring_type);
            LMatrix mat;
            mat.rotationZ( zrot );
            mat.rotationX( xrot );
            mat.rotationY( yrot );
            LQuaternion::rotationMatrix( &bone_rot, mat );
        }

        return limit;

#endif
    }

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
    void ModelIK::initializePMD( ModelCore* model_core_, const PMD_IK* ik_data_, ModelFrame* frames_ )
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
            ModelFrame* bone = &( frames_[ mBoneIndexList[ i ] ] );
            if ( bone->getName() == _T( "���Ђ�" ) || bone->getName() == _T( "�E�Ђ�" ) )
            {
                printf( "MMDX DefaltIKLimitter.cs �̂悤�Ɏ���������\n" );
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

        /*

        //IK�Ώۂ̃{�[����Global���X�V(�ʂ�IK�e�����̃{�[������IK�`�F�C�����o�Ă���ꍇ������̂�)
        LMatrix local;
        for (int i = mLinkNum - 1; i >= 0; --i)
        {//���Ԃɐe�q�֌W�ɂȂ��Ă���B(Processor�Ń`�F�b�N�����Ă���
            //GlobalTransform�����X�V
            ModelFrame* parentBone = frames_[mBoneIndexList[i]].getParent();


            LMatrix::createFromSQTTransform( &local, *frames_[mBoneIndexList[i]].getLocalTransformPtr());

            LMatrix tmp = *frames_[mBoneIndexList[i]].getWorldMatrixPtr();
            (*frames_[mBoneIndexList[i]].getWorldMatrixPtr()) = local * parentBone->getWorldMatrix();
        }

        ModelFrame* parentBone = eff_bone->getParent();
        LMatrix::createFromSQTTransform( &local, *eff_bone->getLocalTransformPtr());
        //LMatrix tmp = eff_bone->getWorldMatrixPtr();
            (*eff_bone->getWorldMatrixPtr()) = local * parentBone->getWorldMatrix();

        //effector.LocalTransform.CreateMatrix(out local);
        //Matrix.Multiply(ref local, ref BoneManager[effector.SkeletonHierarchy].GlobalTransform, out effector.GlobalTransform);
*/

       

        //LVector3 org_target_pos = target_bone->getWorldMatrix().getPosition();  // MMDX �� WorldMatrix ���炾����
        LVector3 eff_pos;
        LVector3 joint_pos;
        LVector3 target_pos;
        //LMatrix bone_inv;
        LMatrix inv_coord;

        LVector3 local_eff_pos;
        LVector3 local_target_pos;

        for ( short j = mLinkNum - 1 ; j >= 0 ; --j )
        {
            frames_[ mBoneIndexList[ j ] ].updateWorldMatrixFromLocalAndParent();
        }
        eff_bone->updateWorldMatrixFromLocalAndParent();



        //�^�[�Q�b�g�ʒu�̎擾
        target_pos = target_bone->getWorldMatrix().getPosition();

        ///int add_iterations = 0;

        for ( lnU16 it = 0; it < mCalcCount + 10; ++it )
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


                // �\���߂���ΏI�� (���������������}�V�ȋC������c)
                LVector3 diff = local_eff_pos - local_target_pos;
                if ( LVector3::dot( diff, diff ) < 0.0000001f )	return;


                // (1) ��֐߁��G�t�F�N�^�ʒu�ւ̕����x�N�g��
                local_eff_pos.normalize();
			    // (2) ��֐߁��ڕW�ʒu�ւ̕����x�N�g��
                local_target_pos.normalize();

                // �\���߂���ΏI��
                //lnfloat dot = ;
                //if ( dot > 0.9999995f ) return;
                //if ( LVector3::dot( local_eff_pos, local_target_pos ) > 1 - 1.0e-5f ) continue;

                // ��]�p
			    float rot_angle = acosf( LVector3::dot( local_eff_pos, local_target_pos ) );

                if ( 0.00000001f >= fabsf( LVector3::dot( local_eff_pos, local_target_pos ) ) )	continue;
               
                //��]�ʐ�����������
                if ( rot_angle > LMath::PI * mFact * (node_idx + 1))
                     rot_angle = LMath::PI * mFact * (node_idx + 1);
                if ( rot_angle < -LMath::PI * mFact * (node_idx + 1))
                     rot_angle = -LMath::PI * mFact * (node_idx + 1);
    //            if( rot_angle < -mFact )	    rot_angle = -mFact;
				//else if( mFact < rot_angle )	rot_angle = mFact;

                // ��]��
                LVector3 rot_axis;
                LVector3::cross( &rot_axis, local_eff_pos, local_target_pos );
                mModelCore->getIKLimitter()->adjustAxis( node->getName().c_str(), &rot_axis );   // ����
                rot_axis.normalize();

                //if( LVector3::dot( rot_axis, rot_axis ) < 0.0000001f )	continue;

            
                //if ( 0.0000001f < fabsf( rot_angle ) )
                if ( !LMath::isNaN( rot_angle ) && rot_angle > 1.0e-3f && !rot_axis.isNaN() )
			    {
                    // �֐߉�]�ʂ̕␳
                    LQuaternion rot_quat( rot_axis, rot_angle );

                    //if ( frames_[ bone_idx ].isIKLimit() )	_limitAngle( &rot_quat, rot_quat );
                    //rot_quat.normalize();
                    //LQuaternion::multiply( &node->mLocalTransform.Rotation, node->mLocalTransform.Rotation, rot_quat );
                    
                    LQuaternion::multiply( &node->mLocalTransform.Rotation, node->mLocalTransform.Rotation, rot_quat );
                    if ( mModelCore->getIKLimitter()->adjustRotation( node ) )
                    {
                        //add_iterations += 1;
                    }


                    

                    // �֐߉�]�ʂ̕␳
                    //Quaternion subRot = MMDXMath.CreateQuaternionFromAxisAngle(rotationAxis, rotationAngle);
                    //Quaternion.Multiply(ref subRot, ref node.LocalTransform.Rotation, out node.LocalTransform.Rotation);
                    //MMDCore.Instance.IKLimitter.Adjust(node);
                

                
                    // �֐߉�]�ʂ̕␳
                    /*
				    LQuaternion rot_quat( rot_axis, rot_angle );
                    if ( frames_[ bone_idx ].isIKLimit() )	_limitAngle( &rot_quat, rot_quat );
                    
                    rot_quat.normalize();
                    LQuaternion::multiply( &node->mLocalTransform.Rotation, node->mLocalTransform.Rotation, rot_quat );
                    
                    if ( node->mLocalTransform.Rotation.isZero() )
                    {
                        printf("node->mLocalTransform.Rotation.isZero()\n");
                    }
                    node->mLocalTransform.Rotation.normalize();
                    */

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
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================