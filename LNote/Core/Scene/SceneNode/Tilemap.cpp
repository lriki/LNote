//=============================================================================
//�y Tilemap �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../SceneShader/SceneShaderManager.h"
#include "../SceneShader/SceneShader.h"
#include "../SceneGraph.h"
#include "Tilemap.h"

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
// �� Tilemap
//=============================================================================

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    Tilemap::Tilemap( SceneGraph* scene_ )
        : SceneNode         ( scene_ )
        , mVertexNum        ( 0 )
        , mVertexBuffer     ( NULL )
        , mIndexBuffer      ( NULL )
        , mTilesetTexture   ( NULL )
        , mXSize            ( 0 )
        , mYSize            ( 0 )
        , mFaceNum          ( 0 )
        , mTileWidth        ( 0.0f )
        , mTileHeight       ( 0.0f )
        , mUVWidth          ( 0.0f )
        , mUVHeight         ( 0.0f )
        , mMapData          ( NULL )
        , mSrcXNum          ( 8 )
    {
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    Tilemap::~Tilemap()
    {
        LN_SAFE_RELEASE( mTilesetTexture );
        LN_SAFE_RELEASE( mVertexBuffer );
        LN_SAFE_RELEASE( mIndexBuffer );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Tilemap::initialize( u32 w_size_, u32 h_size_, LNTilemapDirection dir_ )
    {
        SceneNode::initialize( 1, LN_DRAWCOORD_3D );

        // �^�C���̐�
        mXSize = w_size_;
        mYSize = h_size_;

        // �\������Ƃ��̃^�C���ЂƂ��̑傫��
        mTileWidth = 20.0f;
        mTileHeight = 20.0f;

        // ���݂̏�Ԃ����ɒ��_�o�b�t�@�����
        _makeVertexBuffer( dir_ );

        SceneShader* shader;
        this->mSceneGraph->getSceneShaderManager()->createSceneShader(
            &shader, _T( "LNResource\\Shader\\Tilemap.lnfx" ) );

        setShader( shader );

        LN_SAFE_RELEASE( shader );
    }

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    //void Tilemap::setPosition( int x_, int y_ )
    //{
    //    LN_PRINT_NOT_IMPL_FUNCTION
    //}

    //---------------------------------------------------------------------
	//
    //---------------------------------------------------------------------
    void Tilemap::setTilesetTexture( Graphics::ITexture* texture_ )
    {
        LN_SAFE_ADDREF( texture_ );
        LN_SAFE_RELEASE( mTilesetTexture );
        mTilesetTexture = texture_;

        if ( mTilesetTexture )
        {
            mUVWidth  = static_cast< lnfloat >( mTileWidth  ) / mTilesetTexture->getRealSize().x;
		    mUVHeight = static_cast< lnfloat >( mTileHeight ) / mTilesetTexture->getRealSize().y;
        }

        this->mNodeRenderParam.Subset[ 0 ].Material.setTexture( texture_ );
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    void Tilemap::update( const Game::GameTime& time_ ) 
    {
        _writeVertexUV();
    }

    //---------------------------------------------------------------------
    //
    //---------------------------------------------------------------------
    LNRESULT Tilemap::drawSubset( lnU32 index_ )
    {
        if ( !mTilesetTexture ) { return LN_OK; }

        Graphics::IRenderer* renderer = this->mSceneGraph->getGraphicsManager()->getGraphicsDevice()->getRenderer();

        // TODO:
        renderer->setRenderState(LRenderState::BUFFER_COPY_STATE);

        renderer->setVertexBuffer( mVertexBuffer );
        renderer->setIndexBuffer( mIndexBuffer );

        return renderer->drawPrimitiveIndexed( LN_PRIM_TRIANGLELIST, 0, mFaceNum );
    }

    //---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	LNRESULT Tilemap::_makeVertexBuffer( LNTilemapDirection dir_ )
	{
        LNRESULT lr;

        // ���_�̑���
		mVertexNum = mXSize * mYSize * 4;

        // �O�p�`�̐��͎l�p�`�̐��� 2 �{�ŁA�C���f�b�N�X���͂��� 3 �{
		mFaceNum = mXSize * mYSize * 2;
        int index_num = mFaceNum * 3;

        // ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�쐬
        LN_CALL_R( this->mSceneGraph->getGraphicsManager()->getGraphicsDevice()->createVertexBuffer(
            &mVertexBuffer, Graphics::Vertex::PosUV::elements(), mVertexNum, NULL, true ) );

        LN_CALL_R( this->mSceneGraph->getGraphicsManager()->getGraphicsDevice()->createIndexBuffer(
            &mIndexBuffer, index_num, NULL, true ) );

        
        //-----------------------------------------------------
        // ���_�o�b�t�@�Ɏl�p�`���l�߂Ă���

        Graphics::Vertex::PosUV* vertices = (Graphics::Vertex::PosUV*)mVertexBuffer->lock();
        u32 i, x, y;
        lnfloat dx, dy, dz;
        dz = 0.0f;
        switch ( dir_ )
        {
            // XY ���ʂɑ΂��� Z+ �����ɍ쐬���� (2D �̏ꍇ�͂�����w�肷��)
            default:    // LN_TILEMAPDIR_XYR
            {
                for ( y = 0; y < mYSize; ++y )
                {
                    for ( x = 0; x < mXSize; ++x )
                    {
                        i = ( ( mXSize * y ) + x ) * 4;     // �������݈ʒu
                        dx = mTileWidth * x;
                        dy = mTileHeight * y;

                        // ���ɁA����@�E��@�����@�E��
                        vertices[ i + 0 ].Position.set( dx, dy, dz );
                        vertices[ i + 1 ].Position.set( dx + mTileWidth, dy, dz );
                        vertices[ i + 2 ].Position.set( dx, dy + mTileHeight, dz );
                        vertices[ i + 3 ].Position.set( dx + mTileWidth, dy + mTileHeight, dz );
                    }
                }
                break;
            }
            
            // XY ���ʂɑ΂��āA�}�b�v�̍��������_�ɂȂ�悤�ɍ쐬���� (���ی��ɍ쐬����)
            case LN_TILEMAPDIR_XY:
            {
                lnfloat oy = mTileHeight * mYSize;
                for ( y = 0; y < mYSize; ++y )
                {
                    for ( x = 0; x < mXSize; ++x )
                    {
                        i = ( ( mXSize * y ) + x ) * 4;     // �������݈ʒu
                        dx = mTileWidth * x;
                        dy = -mTileHeight * y + oy;

                        // ���ɁA����@�E��@�����@�E��
                        vertices[ i + 0 ].Position.set( dx, dy, dz );
                        vertices[ i + 1 ].Position.set( dx + mTileWidth, dy, dz );
                        vertices[ i + 2 ].Position.set( dx, dy - mTileHeight, dz );
                        vertices[ i + 3 ].Position.set( dx + mTileWidth, dy - mTileHeight, dz );
                    }
                }
                break;
            }
        }
        mVertexBuffer->unlock();


        //-----------------------------------------------------
        // �C���f�b�N�X���l�߂Ă���

        u16* indices = (u16*)mIndexBuffer->lock();
        u16 vi;
        for ( y = 0; y < mYSize; ++y )
        {
            for ( x = 0; x < mXSize; ++x )
            {
                i  = (u16)( ( mXSize * y ) + x ) * 6;
		        vi = (u16)( ( mXSize * y ) + x ) * 4;
		        indices[ i + 0 ] = vi;
		        indices[ i + 1 ] = vi + 1;
		        indices[ i + 2 ] = vi + 2;
		        indices[ i + 3 ] = vi + 2;
		        indices[ i + 4 ] = vi + 1;
		        indices[ i + 5 ] = vi + 3;
            }
        }
        mIndexBuffer->unlock();

        return LN_OK;
	}

    //---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	void Tilemap::_writeVertexUV()
	{
        Graphics::Vertex::PosUV* vertices = (Graphics::Vertex::PosUV*)mVertexBuffer->lock();
        u32 i, x, y, d;
        for ( y = 0; y < mYSize; ++y )
        {
            for ( x = 0; x < mXSize; ++x )
            {
                i = ( y * mXSize + x ) * 4;
                d = mMapData[ y * mXSize + x ];

		        int idx = ( ( mXSize * y ) + x ) * 4;
		        lnfloat u = mUVWidth * ( d % mSrcXNum );
		        lnfloat v = mUVHeight * ( d / mSrcXNum );
                vertices[ i + 0 ].TexUV.set( u, v );
		        vertices[ i + 1 ].TexUV.set( u + mUVWidth, v );
		        vertices[ i + 2 ].TexUV.set( u, v + mUVHeight );
		        vertices[ i + 3 ].TexUV.set( u + mUVWidth, v + mUVHeight );
            }
        }
        mVertexBuffer->unlock();
	}
    

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================