//=============================================================================
//�y Grid �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "Grid.h"

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
// �� Grid �N���X
//=============================================================================

    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    Grid::Grid()
        : mGraphicsDevice   ( NULL )
        , mRenderer         ( NULL )
        , mGridVertexBuffer ( NULL )
        , mSquareNum        ( 10 )
        , mInterval         ( 1.0f )
        , mVisibleFlags     ( LN_GRID_XZ | LN_GRID_AXIS )//( LN_GRID_XZ | LN_GRID_XY | LN_GRID_YZ | LN_GRID_AXIS )
        , mXPlace           ( LN_GRIDPLACE_PLUS_MINUS )
        , mYPlace           ( LN_GRIDPLACE_PLUS_MINUS )
        , mZPlace           ( LN_GRIDPLACE_PLUS_MINUS )
        , mXZGridIndex      ( 0 )
        , mXYGridIndex      ( 0 )
        , mYZGridIndex      ( 0 )
        , mAxisIndex        ( 0 )
    {
    }

    //---------------------------------------------------------------------
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    Grid::~Grid()
    {
        LN_SAFE_RELEASE( mGraphicsDevice );
        LN_SAFE_RELEASE( mGridVertexBuffer );
    }

    //---------------------------------------------------------------------
    // �� ������
    //---------------------------------------------------------------------
    LNRESULT Grid::initialize( IGraphicsDevice* device_ )
    {
        mGraphicsDevice = device_;
        LN_SAFE_ADDREF( mGraphicsDevice );
        mRenderer = mGraphicsDevice->getRenderer();
        return _updateGridVertexBuffer();
    }

    //---------------------------------------------------------------------
    // �� �i�q���̐ݒ�
    //---------------------------------------------------------------------
    LNRESULT Grid::setLatticeNum( u32 num_ )
    {
        mSquareNum = num_;
        return _updateGridVertexBuffer();
    }

    //---------------------------------------------------------------------
    // �� �Ԋu�̐ݒ�
    //---------------------------------------------------------------------
    LNRESULT Grid::setInterval( lnfloat interval_ )
    {
        mInterval = interval_;
        return _updateGridVertexBuffer();
    }

    //---------------------------------------------------------------------
    // �� �e���ʂ̕\���͈͂̐ݒ�
    //---------------------------------------------------------------------
    LNRESULT Grid::setPlaceRange( LNGridPlaceRange x_, LNGridPlaceRange y_, LNGridPlaceRange z_ )
    {
        mXPlace = x_;
        mYPlace = y_;
        mZPlace = z_;
        return _updateGridVertexBuffer();
    }

    //---------------------------------------------------------------------
    // �� �e���ʂ̕\���͈͂̎擾
    //---------------------------------------------------------------------
    void Grid::getPlaceRange( LNGridPlaceRange* x_, LNGridPlaceRange* y_, LNGridPlaceRange* z_ )
    {
        *x_ = mXPlace;
        *y_ = mYPlace;
        *z_ = mZPlace;
    }

    //---------------------------------------------------------------------
    // �� �T�u�Z�b�g��`�悷��
    //---------------------------------------------------------------------
    LNRESULT Grid::draw()
    {
        LNRESULT lr;

        LN_CALL_R( mRenderer->setVertexBuffer( mGridVertexBuffer ) );

        // XZ ����
        if ( mVisibleFlags & LN_GRID_XZ )
        {
            LN_CALL_R( mRenderer->drawPrimitive( LN_PRIM_LINELIST, mXZGridIndex, ( mXYGridIndex - mXZGridIndex ) / 2 ) );
        }

        // XY ����
        if ( mVisibleFlags & LN_GRID_XY )
        {
            LN_CALL_R( mRenderer->drawPrimitive( LN_PRIM_LINELIST, mXYGridIndex, ( mYZGridIndex - mXYGridIndex ) / 2 ) );
        }

        // YZ ����
        if ( mVisibleFlags & LN_GRID_YZ )
        {
            LN_CALL_R( mRenderer->drawPrimitive( LN_PRIM_LINELIST, mYZGridIndex, ( mAxisIndex - mYZGridIndex ) / 2 ) );
        }

        // ��
        if ( mVisibleFlags & LN_GRID_AXIS )
        {
            LN_CALL_R( mRenderer->drawPrimitive( LN_PRIM_LINELIST, mAxisIndex, 3 ) );
        }

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� ���_�o�b�t�@����蒼��
    //---------------------------------------------------------------------
    LNRESULT Grid::_updateGridVertexBuffer()
    {
        LNRESULT lr;

        //-----------------------------------------------------
        // �K�v�Ȓ��_�̐��𒲂ׂ�
        u32 v_num = 0;

        u32 plnum = ( ( mSquareNum + 1 ) * 2 ); // ���ʂЂƂ�����́A+�܂���-�����Б������ɐ�����ׂ�Ƃ��ɕK�v�Ȓ��_��

        // XZ ���ʂɍ쐬����ꍇ
        //if ( mVisibleFlags & LN_GRID_XZ )    // �ŏ��ɑS�������Ă��܂�
        {
            mXZGridIndex = v_num;
            v_num += plnum;
            v_num += ( mXPlace == LN_GRIDPLACE_PLUS_MINUS ) ? plnum - 2 : 0;
            v_num += plnum;
            v_num += ( mZPlace == LN_GRIDPLACE_PLUS_MINUS ) ? plnum - 2 : 0;
        }
        // XY ���ʂɍ쐬����ꍇ
        //if ( mVisibleFlags & LN_GRID_XY )
        {
            mXYGridIndex = v_num;
            v_num += plnum;
            v_num += ( mXPlace == LN_GRIDPLACE_PLUS_MINUS ) ? plnum - 2 : 0;
            v_num += plnum;
            v_num += ( mYPlace == LN_GRIDPLACE_PLUS_MINUS ) ? plnum - 2 : 0;
        }
        // YZ ���ʂɍ쐬����ꍇ
        //if ( mVisibleFlags & LN_GRID_YZ )
        {
            mYZGridIndex = v_num;
            v_num += plnum;
            v_num += ( mYPlace == LN_GRIDPLACE_PLUS_MINUS ) ? plnum - 2 : 0;
            v_num += plnum;
            v_num += ( mZPlace == LN_GRIDPLACE_PLUS_MINUS ) ? plnum - 2 : 0;
        }

        // ��
        mAxisIndex = v_num;
        v_num += 6;


        //-----------------------------------------------------
        // ���_�o�b�t�@������Ē��g����������ł���
        LN_SAFE_RELEASE( mGridVertexBuffer );
        LN_CALL_R( mGraphicsDevice->createVertexBuffer( &mGridVertexBuffer, Vertex::PosColor::elements(), v_num, NULL ) );
        Vertex::PosColor* v = (Vertex::PosColor*)mGridVertexBuffer->lock();
        
    
        // �e�ʂ̊i�q�̃T�C�Y�����߂�
        lnfloat width = static_cast< lnfloat >( mSquareNum ) * mInterval;
        lnfloat min_x, min_y, min_z, max_x, max_y, max_z;
        min_x = ( mXPlace == LN_GRIDPLACE_PLUS ) ? 0.0f : -width;
        min_y = ( mYPlace == LN_GRIDPLACE_PLUS ) ? 0.0f : -width;
        min_z = ( mZPlace == LN_GRIDPLACE_PLUS ) ? 0.0f : -width;
        max_x = ( mXPlace == LN_GRIDPLACE_MINUS ) ? 0.0f : width;
        max_y = ( mYPlace == LN_GRIDPLACE_MINUS ) ? 0.0f : width;
        max_z = ( mZPlace == LN_GRIDPLACE_MINUS ) ? 0.0f : width;

        u32 loop, vi, i;
        vi = 0;

        // XZ ���ʂɍ쐬����ꍇ
        //if ( mVisibleFlags & LN_GRID_XZ )
        {
            // X����
            loop = ( mXPlace == LN_GRIDPLACE_PLUS_MINUS ) ? ( mSquareNum * 2 + 1 ) : ( mSquareNum + 1 );
		    for ( i = 0; i < loop; i++ )
		    {
			    v[ vi + 0 ].Position.set( min_x + ( mInterval * i ), 0, min_z );
			    v[ vi + 1 ].Position.set( min_x + ( mInterval * i ), 0, max_z );
			    vi += 2;
		    }

            // Z ����
            loop = ( mZPlace == LN_GRIDPLACE_PLUS_MINUS ) ? ( mSquareNum * 2 + 1 ) : ( mSquareNum + 1 );
		    for ( i = 0; i < loop; i++ )
		    {
			    v[ vi + 0 ].Position.set( min_x, 0, min_z + ( mInterval * i ) );
			    v[ vi + 1 ].Position.set( max_x, 0, min_z + ( mInterval * i ) );
			    vi += 2;
		    }
        }
        

        // XY ���ʂɍ쐬����ꍇ
        //if ( mVisibleFlags & LN_GRID_XY )
        {
            // X����
            loop = ( mXPlace == LN_GRIDPLACE_PLUS_MINUS ) ? ( mSquareNum * 2 + 1 ) : ( mSquareNum + 1 );
		    for ( i = 0; i < loop; i++ )
		    {
			    v[ vi + 0 ].Position.set( min_x + ( mInterval * i ), min_y, 0 );
			    v[ vi + 1 ].Position.set( min_x + ( mInterval * i ), max_y, 0 );
			    vi += 2;
		    }

            // Y ����
            loop = ( mYPlace == LN_GRIDPLACE_PLUS_MINUS ) ? ( mSquareNum * 2 + 1 ) : ( mSquareNum + 1 );
		    for ( i = 0; i < loop; i++ )
		    {
			    v[ vi + 0 ].Position.set( min_x, min_y + ( mInterval * i ), 0 );
			    v[ vi + 1 ].Position.set( max_x, min_y + ( mInterval * i ), 0 );
			    vi += 2;
		    }
        }

        // YZ ���ʂɍ쐬����ꍇ
        //if ( mVisibleFlags & LN_GRID_YZ )
        {
            // Y ����
            loop = ( mYPlace == LN_GRIDPLACE_PLUS_MINUS ) ? ( mSquareNum * 2 + 1 ) : ( mSquareNum + 1 );
		    for ( i = 0; i < loop; i++ )
		    {
			    v[ vi + 0 ].Position.set( 0, min_y + ( mInterval * i ), min_z );
			    v[ vi + 1 ].Position.set( 0, min_y + ( mInterval * i ), max_z );
			    vi += 2;
		    }

            // Z ����
            loop = ( mZPlace == LN_GRIDPLACE_PLUS_MINUS ) ? ( mSquareNum * 2 + 1 ) : ( mSquareNum + 1 );
		    for ( i = 0; i < loop; i++ )
		    {
			    v[ vi + 0 ].Position.set( 0, min_y, min_z + ( mInterval * i ) );
			    v[ vi + 1 ].Position.set( 0, max_y, min_z + ( mInterval * i ) );
			    vi += 2;
		    }
        }

        LNGraphicsAPI api = mGraphicsDevice->getGraphicsAPI();

        // �F�͑S���D�F ( -6 �͎��̕� )
        for ( i = 0; i < v_num - 6; ++i )
        {
            v[ i ].Color = LColor::GRAY.to32Bit( api );
        }

        // X��
		v[ vi + 0 ].Position.set( 0, 0, 0 );
		v[ vi + 0 ].Color = LColor::RED.to32Bit( api );
		v[ vi + 1 ].Position.set( width, 0, 0 );
		v[ vi + 1 ].Color = LColor::RED.to32Bit( api );
		// Y��
		v[ vi + 2 ].Position.set( 0, 0, 0 );
        v[ vi + 2 ].Color = LColor::GREEN.to32Bit( api );
		v[ vi + 3 ].Position.set( 0, width, 0 );
		v[ vi + 3 ].Color = LColor::GREEN.to32Bit( api );
		// Z��
		v[ vi + 4 ].Position.set( 0, 0, 0 );
        v[ vi + 4 ].Color = LColor::BLUE.to32Bit( api );
		v[ vi + 5 ].Position.set( 0, 0, width );
		v[ vi + 5 ].Color = LColor::BLUE.to32Bit( api );


        mGridVertexBuffer->unlock();

        return LN_OK;
    };


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================