//=============================================================================
//�y SpriteRenderer �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include <algorithm>
#include "../../Resource/LNResource.h"
#include "../Manager.h"
#include "SpriteRenderer.h"

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
// �� SpriteRenderer �N���X
//=============================================================================

    /*
        �\�[�g����v�f�́A
            �E�v���C�I���e�B
            �EZ���W or �r���[�s��̈ʒu��������̋���
            �E�e�N�X�`��


        �s��v�Z�̃e�X�g�c

            ���_���Ƀ��[���h�s����o���Ă����� (Vector3 �� Right Up Front Position ��4x3)�A
            �V�F�[�_���Ōv�Z����̂ƁACPU ����4���_�ɑ΂��� transform() ����̂Ƃǂ����������̂��e�X�g�B

            ���@�c
                drawRequest() �� 6400 ����s�B
                ���̃��[�v�ƁA�V�[���S�̂̕`�揈���̎��Ԃ��v������B

                Intel(R) Core(TM)2 Duo CPU     E8200  @ 2.66GHz (2 CPUs), ~2.7GHz
                NVIDIA Quadro FX 1700

            ���ʁc

                �P�DCPU ���ō��W�ϊ�

                    drawRequest : �� 6500ns
                    Render      : �� 3500ns

                �Q�D�V�F�[�_���Ōv�Z

                    drawRequest : �� 11000ns
                    Render      : �� 5000ns

                �R�D�Q�̊��ŁA�s��̑����؂��Ă݂�

                    drawRequest : �� 5000ns
                    Render      : �� 5000ns

                ���܂�.
                    �V�F�[�_���� mul �񐔂� 1 �񂾂��ɂ��Ă݂��B(���͍̂s��쐬��mul��ViewProj mul ��2��)

                    drawRequest : �� 5000ns
                    Render      : �� 5000ns

                    ����܂�ς��Ȃ��B

            �l�@�c

                �s��̌v�Z�����A�l�R�s�[�~4 (4���_��) �̂ق����͂邩�ɏd���B
                �V�F�[�_�̕��͓n�����_�f�[�^�̃T�C�Y����ԉe���傫�������݂����B
                
    */

    BatchSpriteData* SpriteRenderer::sSpriteRequests = NULL;

    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    SpriteRenderer::SpriteRenderer( Manager* manager_ )
        : mManager                  ( manager_ )
        , mGraphicsDevice           ( NULL )
        , mVertexBuffer             ( NULL )
        , mIndexBuffer              ( NULL )
        , mMaxSprites               ( 0 )
        , mSpriteRequests           ( NULL )
        , mNextIndex                ( 0 )
        , mLastSpriteNum            ( 0 )
        , mSpriteIndexArray         ( NULL )
        , mSpriteIndexArraySource   ( NULL )
        , mComFunc                  ( NULL )
        , mSpriteSortMode           ( LN_SPRITESORT_NONE )
        , mEnableViewPosDepth       ( false )
        , mIs3DSystem               ( false )
    {
    }

    //---------------------------------------------------------------------
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    SpriteRenderer::~SpriteRenderer()
    {
        LN_SAFE_RELEASE( mShaderParam.Shader );
        LN_SAFE_RELEASE( mGraphicsDevice );
        LN_SAFE_RELEASE( mVertexBuffer );
        LN_SAFE_RELEASE( mIndexBuffer );
        SAFE_DELETE_ARRAY( mSpriteRequests );
        SAFE_DELETE_ARRAY( mSpriteIndexArray );
        SAFE_DELETE_ARRAY( mSpriteIndexArraySource );
    }

    //---------------------------------------------------------------------
    // �� ������
    //---------------------------------------------------------------------
    LNRESULT SpriteRenderer::initialize( IGraphicsDevice* device_, int max_sprites_, bool is_3d_ )
    {
        LNRESULT lr;

        mGraphicsDevice = device_;
        mMaxSprites     = max_sprites_;
        mIs3DSystem     = is_3d_;
        LN_SAFE_ADDREF( mGraphicsDevice );


        //-----------------------------------------------------
        // ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@

        LN_CALL_R( mGraphicsDevice->createVertexBuffer(
            &mVertexBuffer, BatchSpriteVertex::elements(), mMaxSprites * 4, NULL, true ) );

        LN_CALL_R( mGraphicsDevice->createIndexBuffer(
            &mIndexBuffer, mMaxSprites * 6, NULL, true, true ) );

        // �C���f�b�N�X�o�b�t�@�̒l�͍ŏ�����Œ��OK�Ȃ̂ł����ŏ�������
        u16* ib = (lnU16*)mIndexBuffer->lock();
        int idx = 0;
        int i2 = 0;
        for ( int i = 0; i < mMaxSprites; ++i )
        {
            i2  = i * 6;
            idx = i * 4;
            ib[ i2 + 0 ] = idx;
            ib[ i2 + 1 ] = idx + 1;
            ib[ i2 + 2 ] = idx + 2;
            ib[ i2 + 3 ] = idx + 2;
            ib[ i2 + 4 ] = idx + 1;
            ib[ i2 + 5 ] = idx + 3;
        }
        mIndexBuffer->unlock();


        //-----------------------------------------------------
        // �V�F�[�_
        
        // ���ߍ��݃��\�[�X����ǂݍ���
        mGraphicsDevice->createShader(
            &mShaderParam.Shader,
            _T( "LNResource\\Shader\\SpriteRenderer.lnfx" ) );

        mShaderParam.varTexture         = mShaderParam.Shader->getVariableByName( "gMaterialTexture" );
        mShaderParam.varViewProjMatrix  = mShaderParam.Shader->getVariableByName( "gViewProjMatrix" );
        mShaderParam.varTexture         = mShaderParam.Shader->getVariableByName( "gMaterialTexture" );
        mShaderParam.techMainDraw       = mShaderParam.Shader->getTechniqueByName( "MainDraw" );


        //-----------------------------------------------------
        // �X�v���C�g�ƃ\�[�g�p�C���f�b�N�X�z��

        mSpriteRequests = LN_NEW BatchSpriteData[ mMaxSprites ];
        memset( mSpriteRequests, 0, sizeof( BatchSpriteData ) * mMaxSprites );

        mSpriteIndexArray = LN_NEW u16[ mMaxSprites ];
        for ( int i = 0; i < mMaxSprites; ++i )
        {
            mSpriteIndexArray[ i ] = i;
        }
        mSpriteIndexArraySource = LN_NEW u16[ mMaxSprites ];
        memcpy( mSpriteIndexArraySource, mSpriteIndexArray, sizeof( *mSpriteIndexArraySource ) * mMaxSprites );

        mNextIndex = 0;
        mLastSpriteNum = 0;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� ���W�ϊ��s��̐ݒ�
    //---------------------------------------------------------------------
    void SpriteRenderer::setTransform( const LMatrix& matrix_ )
    {
        mTransformMatrix = matrix_;
    }

    //---------------------------------------------------------------------
    // �� �r���[�A�v���W�F�N�V�����s��̐ݒ�
    //---------------------------------------------------------------------
    void SpriteRenderer::setViewProjMatrix( const LMatrix& view_, const LMatrix& proj_ )
    {
        mViewDirection.set( view_.m02, view_.m12, view_.m22 );

        LMatrix::inverse( &mViewInverseMatrix, view_ );

        mViewPosition = mViewInverseMatrix.getPosition();
        //mViewInverseMatrix.transpose();
        //LMath::MatrixTranspose( &mViewProjMatrix, ( view_ * proj_ ) );

        mViewProjMatrix = ( view_ * proj_ );
    }

    //---------------------------------------------------------------------
    // �� �\�[�g���@�̐ݒ�
    //---------------------------------------------------------------------
    void SpriteRenderer::setSortMode( u32 flags_, bool enable_view_pos_depth_ )
    {
        mSpriteSortMode = flags_;
        mEnableViewPosDepth = enable_view_pos_depth_;
        mComFunc = NULL;

        if ( mSpriteSortMode & LN_SPRITESORT_TEXTURE )
        {
            if ( mSpriteSortMode & LN_SPRITESORT_DEPTH_BACK_TO_FRONT )
            {
                mComFunc = _cmpTexDepthBackToFront;
            }
            else if ( mSpriteSortMode & LN_SPRITESORT_DEPTH_FRONT_TO_BACK )
            {
                mComFunc = _cmpTexDepthFrontToBack;
            }
        }
        else
        {
            if ( mSpriteSortMode & LN_SPRITESORT_DEPTH_BACK_TO_FRONT )
            {
                mComFunc = _cmpDepthBackToFront;
            }
            else if ( mSpriteSortMode & LN_SPRITESORT_DEPTH_FRONT_TO_BACK )
            {
                mComFunc = _cmpDepthFrontToBack;
            }
        }
    }


    //---------------------------------------------------------------------
    // �� �`�惊�N�G�X�g
    //---------------------------------------------------------------------
    LNRESULT SpriteRenderer::drawRequest2D(
        const LVector3& position_,
        const LVector3& center_,
        ITexture* texture_,
        const Geometry::Rect& src_rect_,
        u32 color_ )
    {
        u32 color_table[ 4 ] = { color_, color_, color_, color_ };
        return drawRequest3D( position_, center_, LVector2( static_cast< float >( src_rect_.w ), static_cast< float >( src_rect_.h ) ), texture_, src_rect_, color_table, LN_AADIR_RZ );
    }

    //---------------------------------------------------------------------
    // �� �`�惊�N�G�X�g
    //---------------------------------------------------------------------
    LNRESULT SpriteRenderer::drawRequest3D(
        const LVector3& position_,
        const LVector3& center_,
        const LVector2& size_,
        ITexture* texture_,
        const Geometry::Rect& src_rect_,
        const u32* color_table_,
        LNAxisAlignedDir front_ )
    {
        BatchSpriteData& sprite = mSpriteRequests[ mNextIndex ];

        // 3D �̏ꍇ�̒��_���W
        if ( mIs3DSystem )
        {
            /*
            LVector3 origin(
                position_.x - center_.x,
                position_.y - center_.y,
                position_.z - center_.z );
                */

            LVector3 origin( -center_.x, -center_.y, -center_.z );

            LVector2 harf_size( size_.x * 0.5f, size_.y * 0.5f );

            lnfloat l, t, r, b;
            r =  harf_size.x;
            b = -harf_size.y;
            l = -r;
            t = -b;

#define LN_WRITE_V3( x_, y_, z_ ) origin.x + x_, origin.y + y_, origin.z + z_

            switch ( front_ )
            {
                case LN_AADIR_X:
                    sprite.Vertices[ 0 ].Position.set( LN_WRITE_V3( 0, t, l ) );     // ����
                    sprite.Vertices[ 1 ].Position.set( LN_WRITE_V3( 0, t, r ) );     // �E��
                    sprite.Vertices[ 2 ].Position.set( LN_WRITE_V3( 0, b, l ) );     // ����
                    sprite.Vertices[ 3 ].Position.set( LN_WRITE_V3( 0, b, r ) );     // �E��
                    break;
                case LN_AADIR_Y:
                    sprite.Vertices[ 0 ].Position.set( LN_WRITE_V3( l, 0, t ) );
                    sprite.Vertices[ 1 ].Position.set( LN_WRITE_V3( r, 0, t ) );
                    sprite.Vertices[ 2 ].Position.set( LN_WRITE_V3( l, 0, b ) );
                    sprite.Vertices[ 3 ].Position.set( LN_WRITE_V3( r, 0, b ) );
                    break;
                case LN_AADIR_Z:
                    /*
                    sprite.Vertices[ 0 ].Position.set( LN_WRITE_V3( r, t, 0 ) );
                    sprite.Vertices[ 1 ].Position.set( LN_WRITE_V3( l, t, 0 ) );
                    sprite.Vertices[ 2 ].Position.set( LN_WRITE_V3( r, b, 0 ) );
                    sprite.Vertices[ 3 ].Position.set( LN_WRITE_V3( l, b, 0 ) );
                    */
                    sprite.Vertices[ 0 ].Position.set( LN_WRITE_V3( r, t, 0 ) );
                    sprite.Vertices[ 1 ].Position.set( LN_WRITE_V3( l, t, 0 ) );
                    sprite.Vertices[ 2 ].Position.set( LN_WRITE_V3( r, b, 0 ) );
                    sprite.Vertices[ 3 ].Position.set( LN_WRITE_V3( l, b, 0 ) );
                    break;
                case LN_AADIR_RX:
                    sprite.Vertices[ 0 ].Position.set( LN_WRITE_V3( 0, t, r ) );
                    sprite.Vertices[ 1 ].Position.set( LN_WRITE_V3( 0, t, l ) );
                    sprite.Vertices[ 2 ].Position.set( LN_WRITE_V3( 0, b, r ) );
                    sprite.Vertices[ 3 ].Position.set( LN_WRITE_V3( 0, b, l ) );
                    break;
	            case LN_AADIR_RY:
                    sprite.Vertices[ 0 ].Position.set( LN_WRITE_V3( r, 0, t ) );
                    sprite.Vertices[ 1 ].Position.set( LN_WRITE_V3( l, 0, t ) );
                    sprite.Vertices[ 2 ].Position.set( LN_WRITE_V3( r, 0, b ) );
                    sprite.Vertices[ 3 ].Position.set( LN_WRITE_V3( l, 0, b ) );
                    break;
                case LN_AADIR_RZ:
                    sprite.Vertices[ 0 ].Position.set( LN_WRITE_V3( l, t, 0 ) );
                    sprite.Vertices[ 1 ].Position.set( LN_WRITE_V3( r, t, 0 ) );
                    sprite.Vertices[ 2 ].Position.set( LN_WRITE_V3( l, b, 0 ) );
                    sprite.Vertices[ 3 ].Position.set( LN_WRITE_V3( r, b, 0 ) );
                    /* �E��p
                    sprite.Vertices[ 0 ].Position.set( LN_WRITE_V3( l, t, 0 ) );
                    sprite.Vertices[ 1 ].Position.set( LN_WRITE_V3( l, b, 0 ) );
                    sprite.Vertices[ 2 ].Position.set( LN_WRITE_V3( r, t, 0 ) );
                    sprite.Vertices[ 3 ].Position.set( LN_WRITE_V3( r, b, 0 ) );
                    */
                    break;
            }
#undef LN_WRITE_V3
        }
        // 2D �̏ꍇ�̒��_���W
        else
        {
            /*
            LVector3 origin(
                position_.x - center_.x,
                position_.y - center_.y,
                position_.z - center_.z );
                */
            LVector3 origin( -center_.x, -center_.y, -center_.z );

            sprite.Vertices[ 0 ].Position.set( origin.x,           origin.y,           origin.z );
            sprite.Vertices[ 1 ].Position.set( origin.x + size_.x, origin.y,           origin.z );
            sprite.Vertices[ 2 ].Position.set( origin.x,           origin.y + size_.y, origin.z );
            sprite.Vertices[ 3 ].Position.set( origin.x + size_.x, origin.y + size_.y, origin.z );
        }

        //mTransformMatrix.rotationZ( 0.0001f );

        LMatrix mat;
        mat.setRotateMatrix( mTransformMatrix );


        // �r���{�[�h (Scene ����g���ꍇ�� SceneNode ���ʓ|���Ă�̂ŁAScene �ȊO�ŕK�v�ɂȂ�܂ŕۗ��c)
        if ( 0 )
        {
            mat.setMul3x3( mViewInverseMatrix );
        }
        // �r���{�[�h�EY ���݂̂ɓK�p
        else if ( 0 )
        {
            if ( mViewDirection.x > 0.0f )
		    {
                mat.rotationY( -atanf( mViewDirection.z / mViewDirection.x ) + LMath::PI / 2 );
		    }   
		    else if ( mViewDirection.x == 0.0f )
		    {
			    //D3DXMatrixIdentity(&matWorld); // 0���Z��h������
		    }
		    else
		    {
                mat.rotationY( -atanf( mViewDirection.z / mViewDirection.x ) - LMath::PI / 2 );
		    }
            
        }
        // �r���{�[�h�ł͂Ȃ�
        else
        {
        }

        mat.translation( position_ );
        mat.translation( mTransformMatrix.getPosition() );

        // ���W�ϊ�
        sprite.Vertices[ 0 ].Position.transform( mat );
        sprite.Vertices[ 1 ].Position.transform( mat );
        sprite.Vertices[ 2 ].Position.transform( mat );
        sprite.Vertices[ 3 ].Position.transform( mat );

        //sprite.Vertices[ 0 ].Position.transform( mViewProjMatrix );
        //sprite.Vertices[ 0 ].Position.cdump();
        /*
        sprite.Vertices[ 0 ].Position.transform( mTransformMatrix );
        sprite.Vertices[ 1 ].Position.transform( mTransformMatrix );
        sprite.Vertices[ 2 ].Position.transform( mTransformMatrix );
        sprite.Vertices[ 3 ].Position.transform( mTransformMatrix );
        */

        // �F
        if ( color_table_ )
        {
            sprite.Vertices[ 0 ].Color = color_table_[ 0 ];
            sprite.Vertices[ 1 ].Color = color_table_[ 1 ];
            sprite.Vertices[ 2 ].Color = color_table_[ 2 ];
            sprite.Vertices[ 3 ].Color = color_table_[ 3 ];
        }
        else
        {
            sprite.Vertices[ 0 ].Color =
            sprite.Vertices[ 1 ].Color =
            sprite.Vertices[ 2 ].Color =
            sprite.Vertices[ 3 ].Color = 0xffffffff;
        }
        /*
        sprite.Vertices[ 0 ].Color.setFrom32Bit( color_table_[ 0 ] );
        sprite.Vertices[ 1 ].Color.setFrom32Bit( color_table_[ 1 ] );
        sprite.Vertices[ 2 ].Color.setFrom32Bit( color_table_[ 2 ] );
        sprite.Vertices[ 3 ].Color.setFrom32Bit( color_table_[ 3 ] );
        */

        if ( texture_ )
        {
            // �e�N�X�`�����W
            LVector2 texsize_inv = texture_->getRealSize( true );
            Geometry::RectF sr;//( src_rect_ );
            sr.x = static_cast< lnfloat >( src_rect_.x );
            sr.y = static_cast< lnfloat >( src_rect_.y );
            sr.w = static_cast< lnfloat >( src_rect_.w );
            sr.h = static_cast< lnfloat >( src_rect_.h );
            lnfloat l = sr.x * texsize_inv.x;
            lnfloat t = sr.y * texsize_inv.y;
            lnfloat r = (sr.x + sr.w) * texsize_inv.x;
            lnfloat b = (sr.y + sr.h) * texsize_inv.y;
            sprite.Vertices[ 0 ].TexUV.x = l;
            sprite.Vertices[ 0 ].TexUV.y = t;
            sprite.Vertices[ 1 ].TexUV.x = r;
            sprite.Vertices[ 1 ].TexUV.y = t;
            sprite.Vertices[ 2 ].TexUV.x = l;
            sprite.Vertices[ 2 ].TexUV.y = b;
            sprite.Vertices[ 3 ].TexUV.x = r;
            sprite.Vertices[ 3 ].TexUV.y = b;
          
            // �e�N�X�`��
            sprite.Texture = texture_;
        }
        else
        {
            sprite.Vertices[ 0 ].TexUV.x = 0;
            sprite.Vertices[ 0 ].TexUV.y = 0;
            sprite.Vertices[ 1 ].TexUV.x = 1;
            sprite.Vertices[ 1 ].TexUV.y = 0;
            sprite.Vertices[ 2 ].TexUV.x = 0;
            sprite.Vertices[ 2 ].TexUV.y = 1;
            sprite.Vertices[ 3 ].TexUV.x = 1;
            sprite.Vertices[ 3 ].TexUV.y = 1;
            sprite.Texture = mManager->getDummyTexture();
        }

        // �J��������̋������\�[�g�pZ�l�ɂ���ꍇ
        if ( mEnableViewPosDepth )
        {
            sprite.Depth = ( mViewPosition - position_ ).getSquareLength();
        }
        else
        {
            sprite.Depth = position_.z;
        }

        sprite.Visible = true;
        sprite.Assigned = false;
        sprite.Priority = 0;

        ++mNextIndex;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �o�b�`��������Ă���X�v���C�g�̕`��
    //---------------------------------------------------------------------
    LNRESULT SpriteRenderer::flash()
    {
        int sprite_num = mNextIndex;            // �`�悷��X�v���C�g�̐�
        if ( sprite_num == 0 ) return LN_OK;    // 0 �Ȃ�Ȃɂ����Ȃ�

        memcpy( mSpriteIndexArray, mSpriteIndexArraySource, sizeof( *mSpriteIndexArray ) * mLastSpriteNum );

        // �C���f�b�N�X�̕��ёւ�
        if ( mComFunc )
        {
            sSpriteRequests = mSpriteRequests;
            std::stable_sort( mSpriteIndexArray, mSpriteIndexArray + sprite_num, mComFunc );
        }


        //printf( "flash()\n" );
        //for ( int i = 0; i < sprite_num; ++i )
        //{
        //    printf( "%4d: no:%4d depth:%4f tex:%4p\n", i, mSpriteIndexArray[ i ], mSpriteRequests[ mSpriteIndexArray[ i ] ].Depth, mSpriteRequests[ mSpriteIndexArray[ i ] ].Texture );
        //}


        int si = 0;     // Sprite Index (mSpriteIndexArray)
        int vi = 0;     // Vertex Index
        int ri = 0;     // Request Index (mSpriteRequests)
        int start_idx = 0;
        int prim_num = 0;
        ITexture* current_tex = mSpriteRequests[ mSpriteIndexArray[ 0 ] ].Texture;

        //-----------------------------------------------------
        // �������X�g�����

        mAttributeList.clear();
        while ( true )
        {
            while ( true )
            {
                ++si;
                ++prim_num;

                // ���̃X�v���C�g�̃e�N�X�`�����A�������̃e�N�X�`���ƈقȂ�ꍇ�͎��̑����쐬�Ɉڂ�
                if ( si >= sprite_num || mSpriteRequests[ mSpriteIndexArray[ si ] ].Texture != current_tex )
                {
                    break;
                }
            }

            Attribute attr;
            attr.StartIndex = start_idx;
            attr.PrimitiveNum = prim_num * 2;
            attr.Texture = current_tex;
            mAttributeList.push_back( attr );

            if ( si >= sprite_num )
            {
                break;
            }

            start_idx = si * 6;
            prim_num = 0;
            current_tex = mSpriteRequests[ mSpriteIndexArray[ si ] ].Texture;
        }

        //-----------------------------------------------------
        // ���_�f�[�^���R�s�[


        BatchSpriteVertex* vb = static_cast< BatchSpriteVertex* >( mVertexBuffer->lock() );
        si = 0;
        vi = 0;
        for ( ; si < sprite_num; ++si )
        {
            ri = mSpriteIndexArray[ si ];
            memcpy( &vb[ vi ], mSpriteRequests[ ri ].Vertices, sizeof( mSpriteRequests[ 0 ].Vertices ) );


            
                //printf( "%x\n", vb[ vi ].Color );
                //printf( "%x\n", vb[ vi + 1 ].Color );
                //printf( "%x\n", vb[ vi + 2 ].Color );
                //printf( "%x\n", vb[ vi + 3 ].Color );
            vi += 4;
        }
        mVertexBuffer->unlock();

        //-----------------------------------------------------
        // �`��

        IRenderer* r = mGraphicsDevice->getRenderer();
        r->setVertexBuffer( mVertexBuffer, true );
        r->setIndexBuffer( mIndexBuffer );
        mShaderParam.varViewProjMatrix->setMatrix( mViewProjMatrix );

        IShaderPass* pass = mShaderParam.techMainDraw->getPassByIndex( 0 );
       
        std::list< Attribute >::iterator itr = mAttributeList.begin();
        std::list< Attribute >::iterator end = mAttributeList.end();
        for ( ; itr != end; ++itr )
        {
            mShaderParam.varTexture->setTexture( itr->Texture );
            pass->begin();
            r->drawPrimitiveIndexed( LN_PRIM_TRIANGLELIST, itr->StartIndex, itr->PrimitiveNum );
            pass->end();
        }


        mLastSpriteNum = sprite_num;
        mNextIndex = 0;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� Z �l�̑傫�������珬�������փ\�[�g�����r�֐�
    //---------------------------------------------------------------------
    bool SpriteRenderer::_cmpDepthBackToFront( const u16& l_, const u16& r_ )
    {
        if ( sSpriteRequests[ l_ ].Priority == sSpriteRequests[ r_ ].Priority )
        {
            if ( sSpriteRequests[ l_ ].Depth > sSpriteRequests[ r_ ].Depth )
            {
                return true;
            }
            if ( sSpriteRequests[ l_ ].Depth == sSpriteRequests[ r_ ].Depth )
            {
                return sSpriteRequests[ l_ ].Texture < sSpriteRequests[ r_ ].Texture;
            }
            return false;
        }
        return sSpriteRequests[ l_ ].Priority < sSpriteRequests[ r_ ].Priority;
    }

    //---------------------------------------------------------------------
    // �� Z �l�̏�����������傫�����փ\�[�g�����r�֐�
    //---------------------------------------------------------------------
    bool SpriteRenderer::_cmpDepthFrontToBack( const u16& l_, const u16& r_ )
    {
        if ( sSpriteRequests[ l_ ].Priority == sSpriteRequests[ r_ ].Priority )
        {
            if ( sSpriteRequests[ l_ ].Depth < sSpriteRequests[ r_ ].Depth )
            {
                return true;
            }
            if ( sSpriteRequests[ l_ ].Depth == sSpriteRequests[ r_ ].Depth )
            {
                return sSpriteRequests[ l_ ].Texture < sSpriteRequests[ r_ ].Texture;
            }
            return false;
        }
        return sSpriteRequests[ l_ ].Priority < sSpriteRequests[ r_ ].Priority;
    }

    //---------------------------------------------------------------------
    // �� Z �l�̑傫�������珬�������փ\�[�g�����r�֐� (�e�N�X�`���D��)
    //---------------------------------------------------------------------
    bool SpriteRenderer::_cmpTexDepthBackToFront( const u16& l_, const u16& r_ )
    {
        if ( sSpriteRequests[ l_ ].Priority == sSpriteRequests[ r_ ].Priority )
        {
            if ( sSpriteRequests[ l_ ].Texture < sSpriteRequests[ r_ ].Texture )
            {
                return true;
            }
            if ( sSpriteRequests[ l_ ].Texture == sSpriteRequests[ r_ ].Texture )
            {
                return sSpriteRequests[ l_ ].Depth > sSpriteRequests[ r_ ].Depth;
            }
            return false;
        }
        return sSpriteRequests[ l_ ].Priority < sSpriteRequests[ r_ ].Priority;
    }

    //---------------------------------------------------------------------
    // �� Z �l�̏�����������傫�����փ\�[�g�����r�֐� (�e�N�X�`���D��)
    //---------------------------------------------------------------------
    bool SpriteRenderer::_cmpTexDepthFrontToBack( const u16& l_, const u16& r_ )
    {
        if ( sSpriteRequests[ l_ ].Priority == sSpriteRequests[ r_ ].Priority )
        {
            if ( sSpriteRequests[ l_ ].Texture < sSpriteRequests[ r_ ].Texture )
            {
                return true;
            }
            if ( sSpriteRequests[ l_ ].Texture == sSpriteRequests[ r_ ].Texture )
            {
                return sSpriteRequests[ l_ ].Depth < sSpriteRequests[ r_ ].Depth;
            }
            return false;
        }
        return sSpriteRequests[ l_ ].Priority < sSpriteRequests[ r_ ].Priority;
    }

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

//---------------------------------------------------------------------
//// �� �X�v���C�g�`��N���X�̍쐬 
////---------------------------------------------------------------------
//LNRESULT createSpriteRenderer( ISpriteRenderer** sprite_renderer_, IGraphicsDevice* device_, u32 max_sprites_, bool is_3d_ )
//{
//    LNRESULT lr;
//    *sprite_renderer_ = NULL;
//
//    SpriteRenderer* sr = LN_NEW SpriteRenderer();
//    LN_CALL_R( sr->initialize( device_, max_sprites_, is_3d_ ) );
//
//    *sprite_renderer_ = sr;
//    return LN_OK;
//}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================