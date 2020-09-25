//=============================================================================
//�y PointParticle �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "PointParticle.h"

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
// �� PointParticle �N���X
//=============================================================================

    /// �|�C���g�X�v���C�g���g�����p�[�e�B�N���p�̒��_
    struct PointParticleVertex
    {
        LVector3	Position;	///< �ʒu
	    LVector3	Velocity;	///< �����x (�b��)
	    LVector3	Accel;		///< �����x
	    LVector3	Axis;		///< ��]��
	    LVector3	AngleData;	///< ��]��� [ �����p�x, �p���x, �p�����x ]
	    LVector3	SizeData;	///< �傫��   [ ����, ���x, �����x ]
        LVector4    TimeData;   ///< [ �J�n����, ���ݎ���, �t�F�[�h�C������, �t�F�[�h�A�E�g���� ] ���S�ăt���[�����B���_�V�F�[�_���ŏ����l����������ŁB
	    LVector4	FadeData;	///< [ �c���p�A���t�@, �c���p���ԃI�t�Z�b�g ]

	    /// ���_���C�A�E�g
	    static LNVertexElemenst* elements()
	    {
		    static LNVertexElemenst elements[] = {
			    { 0, LN_DECL_FLOAT3, LN_USAGE_POSITION, 0 },
			    { 0, LN_DECL_FLOAT3, LN_USAGE_TEXCOORD, 0 },
			    { 0, LN_DECL_FLOAT3, LN_USAGE_TEXCOORD, 1 },
			    { 0, LN_DECL_FLOAT3, LN_USAGE_TEXCOORD, 2 },
			    { 0, LN_DECL_FLOAT3, LN_USAGE_TEXCOORD, 3 },
			    { 0, LN_DECL_FLOAT3, LN_USAGE_TEXCOORD, 4 },
			    { 0, LN_DECL_FLOAT4, LN_USAGE_TEXCOORD, 5 },
			    { 0, LN_DECL_FLOAT4, LN_USAGE_TEXCOORD, 6 },
			      LN_DECL_END() };
		    return elements;
	    }
    };

    //---------------------------------------------------------------------
	// �� �R���X�g���N�^
    //---------------------------------------------------------------------
    PointParticle::PointParticle()
        : mRenderer             ( NULL )
        , mVertexBuffer         ( NULL )
        , mOriginalPositions    ( NULL )
        , mCreatedPositions     ( NULL )
        , mParticleNum          ( 0 )
        , mAfterImageLevel      ( 0 )
        , mOneLoopFrameNum      ( 0 )
        , mTimeTick             ( 60 )
        , mTime                 ( 0 )
        , mLastTime             ( 0 )
        , mTexture              ( NULL )
        , mIsDynamic            ( false )
    {

    }

    //---------------------------------------------------------------------
	// �� �f�X�g���N�^
    //---------------------------------------------------------------------
    PointParticle::~PointParticle()
    {
        LN_SAFE_RELEASE( mTexture );
        LN_SAFE_RELEASE( mVertexBuffer );
        LN_SAFE_RELEASE( mRenderer );
        SAFE_DELETE_ARRAY( mOriginalPositions );
        SAFE_DELETE_ARRAY( mCreatedPositions );
    }

    //---------------------------------------------------------------------
	// �� ������
    //---------------------------------------------------------------------
    void PointParticle::initialize( Graphics::IGraphicsDevice* device_, const PointParticleParameter& data_ )
    {
        mParticleNum     = data_.ParticleNum;
        mAfterImageLevel = data_.AfterImageNum;
        mIsDynamic       = data_.Dynamic;

        mRenderer = device_->getRenderer();
        LN_SAFE_ADDREF( mRenderer );

        // �c���̐����l���������_�����v�Z����
        u32 vert_num = data_.ParticleNum + ( data_.ParticleNum * data_.AfterImageNum );

        // ���_�o�b�t�@�쐬
        device_->createVertexBuffer(
            &mVertexBuffer, PointParticleVertex::elements(), vert_num, NULL, mIsDynamic );
        _createVertexData( data_ );

        mGlobalAccel = LVector4( data_.GlobalAccel, 0 );
        mGravityPower = data_.GravityPower;
        mIsSequential = ( data_.GenerationInterval == 0 );
        mIsParticleLoop = data_.Loop;
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void PointParticle::setTexture( ITexture* texture_ )
    {
        LN_SAFE_ADDREF( texture_ );
        LN_SAFE_RELEASE( mTexture );
        mTexture = texture_;
    }

    //---------------------------------------------------------------------
	// �� ���Ԃ�i�߂�
    //---------------------------------------------------------------------
    void PointParticle::advanceTime( lnfloat delta_time_ )
    {
        mTime += delta_time_;
    }

    //---------------------------------------------------------------------
	// �� �Đ����Ԃ̐ݒ� (���Ԃ𒼐ڎw�肷��B�c�[���ȂǂŎg��)
    //---------------------------------------------------------------------
    void PointParticle::setTime( lnfloat time_ )
    {
        mTime = time_;
    }

    //---------------------------------------------------------------------
	// �� ���݂̎��Ԃ��擾����
    //---------------------------------------------------------------------
    lnfloat PointParticle::getTime()
    {
        return mTime;
    }

    //---------------------------------------------------------------------
	// �� ���_���X�V���� (advanceTime()�AsetTime() �̌�ɂ�����Ă�)
    //---------------------------------------------------------------------
    LNRESULT PointParticle::update( const LMatrix& world_matrix_ )
    {
        if ( mIsDynamic )
        {
			PointParticleVertex* vb = (PointParticleVertex*)mVertexBuffer->lock();

            u32 v_num = mVertexBuffer->getVertexNum();
            lnfloat t;
            lnfloat frame = mTime * mTimeTick;
            lnfloat last_frame = mLastTime * mTimeTick;
            u32 i;

            for ( i = 0; i < v_num; ++i )
            {
                t = fmod( frame - vb[ i ].TimeData.x, vb[ i ].TimeData.y );
                
                if ( abs( t ) < frame - last_frame )
                {
                    // �{�̗̂��q�̏ꍇ
					if ( i < mParticleNum )
					{
                        vb[ i ].Position = mOriginalPositions[ i ];
                        vb[ i ].Position.transform( world_matrix_ );
                        mCreatedPositions[ i ] = vb[ i ].Position;
                    }
                    // �c���̏ꍇ
                    else
                    {
                        vb[ i ].Position = mCreatedPositions[ ( ( i - mParticleNum ) / mAfterImageLevel ) ];
                    }
                }
            }

            mVertexBuffer->unlock();

            mLastTime = mTime;
        }

        return LN_OK;
    }

    //---------------------------------------------------------------------
	// �� �`��
    //---------------------------------------------------------------------
    LNRESULT PointParticle::draw()
    {
        LNRESULT lr;

        LN_CALL_R( mRenderer->setVertexBuffer( mVertexBuffer ) );

        LN_CALL_R( mRenderer->drawPrimitive( LN_PRIM_POINTLIST, 0, mVertexBuffer->getVertexNum() ) );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� ���_�o�b�t�@�𖄂߂�
    //---------------------------------------------------------------------
    void PointParticle::_createVertexData( const PointParticleParameter& data_ )
    {
        LRandom random;
        random.setSeed( ( data_.RandSeed ) ? data_.RandSeed : LRandom::getDefaultSeed() );

        // ���I�����̏ꍇ�̓I���W�i���̍��W���o���Ă����z������
        if ( mIsDynamic )
        {
            mOriginalPositions = LN_NEW LVector3[ mParticleNum ];
            mCreatedPositions  = LN_NEW LVector3[ mParticleNum ];
        }
        
        mOneLoopFrameNum = 0;

        PointParticleVertex* vertices = (PointParticleVertex*)mVertexBuffer->lock();

        lnfloat begin_frame = 0;
        
		for ( u32 i = 0; i < mParticleNum; ++i )
		{
            lnfloat rand_link_rate = random.getFloat( 0.0f, 1.0f );

            vertices[ i ].Position.set(
                random.getFloatEx( data_.Position.x, data_.PositionRand.x, LN_RAND_RANGE, rand_link_rate, (LNExRandRateType)data_.PosRandTypeX ),
                random.getFloatEx( data_.Position.y, data_.PositionRand.y, LN_RAND_RANGE, rand_link_rate, (LNExRandRateType)data_.PosRandTypeY ),
                random.getFloatEx( data_.Position.z, data_.PositionRand.z, LN_RAND_RANGE, rand_link_rate, (LNExRandRateType)data_.PosRandTypeZ ) );

            vertices[ i ].Velocity.set(
                random.getFloatEx( data_.Velocity.x, data_.VelocityRand.x, LN_RAND_RANGE, rand_link_rate, (LNExRandRateType)data_.VelRandTypeX ),
                random.getFloatEx( data_.Velocity.y, data_.VelocityRand.y, LN_RAND_RANGE, rand_link_rate, (LNExRandRateType)data_.VelRandTypeY ),
                random.getFloatEx( data_.Velocity.z, data_.VelocityRand.z, LN_RAND_RANGE, rand_link_rate, (LNExRandRateType)data_.VelRandTypeZ ) );

            vertices[ i ].Accel.set(
                random.getFloatEx( data_.Accel.x, data_.AccelRand.x, LN_RAND_RANGE, rand_link_rate, (LNExRandRateType)data_.AccelRandTypeX ),
                random.getFloatEx( data_.Accel.y, data_.AccelRand.y, LN_RAND_RANGE, rand_link_rate, (LNExRandRateType)data_.AccelRandTypeY ),
                random.getFloatEx( data_.Accel.z, data_.AccelRand.z, LN_RAND_RANGE, rand_link_rate, (LNExRandRateType)data_.AccelRandTypeZ ) );

            vertices[ i ].Axis.set(
                random.getFloatEx( data_.Axis.x, data_.AxisRand.x, LN_RAND_RANGE, rand_link_rate, (LNExRandRateType)data_.AxisRandTypeX ),
                random.getFloatEx( data_.Axis.y, data_.AxisRand.y, LN_RAND_RANGE, rand_link_rate, (LNExRandRateType)data_.AxisRandTypeY ),
                random.getFloatEx( data_.Axis.z, data_.AxisRand.z, LN_RAND_RANGE, rand_link_rate, (LNExRandRateType)data_.AxisRandTypeZ ) );

            vertices[ i ].AngleData.set(
                random.getFloatEx( data_.Angle,         data_.AngleRand,         LN_RAND_RANGE, rand_link_rate, (LNExRandRateType)data_.AngleRandType ),
                random.getFloatEx( data_.AngleVelocity, data_.AngleVelocityRand, LN_RAND_RANGE, rand_link_rate, (LNExRandRateType)data_.AngVelRandType ),
                random.getFloatEx( data_.AngleAccel,    data_.AngleAccelRand,    LN_RAND_RANGE, rand_link_rate, (LNExRandRateType)data_.AngAccelRandType ) );

            vertices[ i ].SizeData.set(
                random.getFloatEx( data_.Size,         data_.SizeRand,         LN_RAND_RANGE, rand_link_rate, (LNExRandRateType)data_.SizeRandType ),
                random.getFloatEx( data_.SizeVelocity, data_.SizeVelocityRand, LN_RAND_RANGE, rand_link_rate, (LNExRandRateType)data_.SizeVelRandType ),
                random.getFloatEx( data_.SizeAccel,    data_.SizeAccelRand,    LN_RAND_RANGE, rand_link_rate, (LNExRandRateType)data_.SizeAccelRandType ) );
            
            vertices[ i ].TimeData.set(
                begin_frame,
                random.getFloatEx( data_.Life,    data_.LifeRand,    LN_RAND_RANGE, rand_link_rate, (LNExRandRateType)data_.LifeRandType ),
                random.getFloatEx( data_.FadeIn,  data_.FadeInRand,  LN_RAND_RANGE, rand_link_rate, (LNExRandRateType)data_.FadeInRandType ),
                random.getFloatEx( data_.FadeOut, data_.FadeOutRand, LN_RAND_RANGE, rand_link_rate, (LNExRandRateType)data_.FadeOutRandType ) );
            
            vertices[ i ].FadeData.set( 1.0f, 0, 0, 0 );

            // ���͐��K�����K�v
            if ( vertices[ i ].Axis.isZero() )
            {
                vertices[ i ].Axis.set( 0, 1, 0 );
            }
            else
            {
                vertices[ i ].Axis.normalize();
            }

            // �t�F�[�h�C���E�A�E�g�̎��Ԃ��A�������ԓ��Ɏ��߂�Əd�Ȃ��Ă��܂��ꍇ�͐������Ԃ����΂�
			if ( vertices[ i ].TimeData.z + vertices[ i ].TimeData.w > vertices[ i ].TimeData.y )
			{
				vertices[ i ].TimeData.y = vertices[ i ].TimeData.z + vertices[ i ].TimeData.w;
			}
           

            if ( mOriginalPositions )
            {
                mOriginalPositions[ i ] = vertices[ i ].Position;
            }

            begin_frame += data_.GenerationInterval;
        }

        // �c��
        u32 idx;
        lnfloat ai_level_inv = 1.0f / mAfterImageLevel;

        for ( u32 i = 0; i < mAfterImageLevel; ++i )
        {
            for ( u32 j = 0; j < mParticleNum; ++j )
            {
                idx = ( i + 1 ) * mParticleNum + j;
                vertices[ idx ] = vertices[ j ];

                vertices[ idx ].TimeData.x += 1.0f * ( i + 1 );

                vertices[ idx ].FadeData.x = ai_level_inv * ( mAfterImageLevel - i );
            }
        };
        
        mVertexBuffer->unlock();
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================