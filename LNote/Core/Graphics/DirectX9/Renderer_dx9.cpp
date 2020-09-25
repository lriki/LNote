//=============================================================================
//�y DX9Renderer �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "GraphicsDevice.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Renderer.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{
namespace DirectX9
{

//=============================================================================
// �� Renderer �N���X
//=============================================================================

    //---------------------------------------------------------------------
    // �� �R���X�g���N�^
    //---------------------------------------------------------------------
    Renderer::Renderer()
        : mLogFile              ( NULL )
        , mGraphicsDevice       ( NULL )
        , mDxDevice             ( NULL )
        , mDxCaps               ( NULL )
        , mCurrentDepthBuffer   ( NULL )
        , mCurrentVertexBuffer  ( NULL )
        , mCurrentIndexBuffer   ( NULL )
        , mEnableScissorTest    ( false )
        , mSceneBegan           ( false )
    {
    }

    //---------------------------------------------------------------------
    // �� �f�X�g���N�^
    //---------------------------------------------------------------------
    Renderer::~Renderer()
    {
    }

    //---------------------------------------------------------------------
    // �� ������
    //---------------------------------------------------------------------
    LNRESULT Renderer::initialize( const InitData& init_data_ )
    {
        mGraphicsDevice = init_data_.GraphicsDevice;
        mDxDevice       = mGraphicsDevice->getDX9Device();
        mDxCaps         = init_data_.DxCaps;

        mCurrentRenderState.BlendMode   = LN_BLEND_ALPHA;
        mCurrentRenderState.Culling     = LN_CULL_FRONT;
        mCurrentRenderState.FillMode    = LN_FILL_SOLID;
        mCurrentRenderState.AlphaTest   = true;
        mCurrentRenderState.DepthTest   = true;
        mCurrentRenderState.DepthWrite  = true;
        //mCurrentRenderState.ScissorTest = false;

        D3DVIEWPORT9 vp;
        mDxDevice->GetViewport( &vp );

        setViewport( Geometry::Rect( 0, 0, vp.Width, vp.Height ) );

        return onResetDevice();
    }

    //---------------------------------------------------------------------
    // �� �����_�����O�X�e�[�g�̐ݒ�
    //---------------------------------------------------------------------
    LNRESULT Renderer::setRenderState( const LRenderState& state_ )
    {
        mRequestRenderState = state_;
        return _updateRenderState();
    }

    //---------------------------------------------------------------------
    // �� �����_�����O�^�[�Q�b�g�̐ݒ�
    //---------------------------------------------------------------------
    LNRESULT Renderer::setRenderTarget( u32 index_, ITexture* texture_, bool reset_ )
    {
        LN_SETERR_R_S_( ( index_ < mDxCaps->NumSimultaneousRTs ), LN_ERR_FATAL,
            "< Renderer::setRenderTarget >\nindex_ = %d ( max = %d )", index_, mDxCaps->NumSimultaneousRTs );

        if ( mCurrentRenderTargets[ index_ ] != texture_ || reset_ )
        {
            LVector2 viewsize;

            // 0 �Ԃ� NULL ���Z�b�g����Ƃ��̓f�t�H���g�̃o�b�N�o�b�t�@�ɂ���
            if ( index_ == 0 && !texture_ )
            {
                texture_ = mGraphicsDevice->getDefaultCanvas()->getBackbuffer();
                mDxDevice->SetRenderTarget(
                    0,
                    ( (DX9TextureBase*)texture_ )->getDxSurface() );
                viewsize = texture_->getSize();
                mCurrentRenderTargets[ 0 ] = texture_;
            }
            else
            {
                if ( texture_ )
                {
                    mDxDevice->SetRenderTarget( index_, ( (DX9TextureBase*)texture_ )->getDxSurface() );
                }
                else
                {
                    mDxDevice->SetRenderTarget( index_, NULL );
                }

                mCurrentRenderTargets[ index_ ] = texture_;
                viewsize = mCurrentRenderTargets[ 0 ]->getRealSize();
            }

            // 0 �Ԃ܂��̓o�b�N�o�b�t�@�̃T�C�Y���r���[�|�[�g�Ƃ��Đݒ�
            Geometry::Rect rc(
                0, 0,
                static_cast< int >( viewsize.x ),
                static_cast< int >( viewsize.y ) );

            setViewport( rc );
        }

        return LN_OK;


        // index_ �� 0 �ȊO�ɐݒ肳�ꂽ�ꍇ�A0 �Ԃɒ肷����Ă���
        // �e�N�X�`���̉𑜓x�Ńr���[�|�[�g���Đݒ肷��̂��g�B

        // �����ł� mCurrentRenderTargets[ 0 ] �� NULL �̂Ƃ��̓o�b�N�o�b�t�@������Ƃ��Ĉ����̂�
        // �e�N�X�`���̎Q�ƃJ�E���g�𑝂₷�K�v�̓i�V�BDxTexture �Ƃ��� DxDevice �ɊǗ�������
    }

    //---------------------------------------------------------------------
	// �� �[�x�o�b�t�@�̐ݒ�
    //---------------------------------------------------------------------
    LNRESULT Renderer::setDepthBuffer( ITexture* texture_ )
    {
        if ( texture_ != mCurrentDepthBuffer )
        {
            if ( !texture_ )
	        {
		        mDxDevice->SetDepthStencilSurface( NULL );
	        }
	        else
	        {
		        mDxDevice->SetDepthStencilSurface( ( (DX9TextureBase*)texture_ )->getDxSurface() );
	        }
            mCurrentDepthBuffer = texture_;
        }
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// �� �r���[�|�[�g��`�̎擾
	//---------------------------------------------------------------------
    const Geometry::Rect& Renderer::getViewport() const
	{
		return mCurrentViewportRect;
	}

    //---------------------------------------------------------------------
	// �� �r���[�|�[�g��`�̐ݒ�
	//---------------------------------------------------------------------
	LNRESULT Renderer::setViewport( const Geometry::Rect& viewport_rect_ )
	{
		mCurrentViewportRect    = viewport_rect_;
        D3DVIEWPORT9 viewport;
	    viewport.X		= static_cast< DWORD >( viewport_rect_.x );
	    viewport.Y      = static_cast< DWORD >( viewport_rect_.y );
	    viewport.Width  = static_cast< DWORD >( viewport_rect_.w );
	    viewport.Height = static_cast< DWORD >( viewport_rect_.h );
		viewport.MinZ   = 0.0f;
		viewport.MaxZ   = 1.0f;
		HRESULT hr;
        
        LN_DXCALL_R( mDxDevice->SetViewport( &viewport ) );
        return LN_OK;
	}

    //---------------------------------------------------------------------
	// �� �V�U�[��`�̎擾
    //---------------------------------------------------------------------
    const Geometry::Rect& Renderer::getScissor() const
    {
        return mCurrentScissorRect;
    }

	//---------------------------------------------------------------------
	// �� �V�U�[��`�̐ݒ�
    //---------------------------------------------------------------------
    LNRESULT Renderer::setScissor( const Geometry::Rect& rect_ )
    {
        bool b;

        if ( rect_ == mCurrentViewportRect )
        {
            b = false;
        }
        else if ( rect_.x == 0 && rect_.y == 0 && rect_.w == 0 && rect_.h == 0 )
        {
            b = false;
        }
        else
        {
            b = true;
        }

        //if ( b )//!= mEnableScissorTest )
	    {
		    const u32 tb[] = { FALSE, TRUE };
		    mDxDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, tb[ b ] );
            if ( b )
            {
                _setScissorRect( rect_ );
            }
            mEnableScissorTest = b;
	    }
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// �� ���_�o�b�t�@�̐ݒ�
	//---------------------------------------------------------------------
	LNRESULT Renderer::setVertexBuffer( IVertexBuffer* vertex_buffer_, bool reset_ )
	{
        VertexBuffer* vb = static_cast< VertexBuffer* >( vertex_buffer_ );
            //dynamic_cast< VertexBuffer* >( vertex_buffer_ );

		if ( vb == mCurrentVertexBuffer && !reset_ ) return LN_OK;

        if ( vb )
        {
		    HRESULT hr;
                
            // ���_�錾�ݒ�
            LN_DXCALL_R(
                mDxDevice->SetVertexDeclaration( vb->getDxVertexDeclaration() ) );

		    // ���_�o�b�t�@��ݒ�
            LN_DXCALL_R(
                mDxDevice->SetStreamSource( 0, vb->getDxVertexBuffer(), 0, vb->getVertexStride() ) );
        }


		mCurrentVertexBuffer = vb;

        return LN_OK;
	}

	//---------------------------------------------------------------------
	// �� �C���f�b�N�X�o�b�t�@�̐ݒ�
	//---------------------------------------------------------------------
	LNRESULT Renderer::setIndexBuffer( IIndexBuffer* index_buffer_, bool reset_ )
	{
        IndexBuffer* ib = static_cast< IndexBuffer* >( index_buffer_ );
            //dynamic_cast< IndexBuffer* >( index_buffer_ );

		if ( ib == mCurrentIndexBuffer && !reset_ ) { 
	return LN_OK; }
		
        if ( ib )
        {
		    HRESULT hr;
            LN_DXCALL_R(
                mDxDevice->SetIndices( ib->getDxIndexBuffer() ) );
        }
		
		mCurrentIndexBuffer = ib;

        return LN_OK;
	}

    //---------------------------------------------------------------------
	// �� �ݒ肳��Ă���J���[�o�b�t�@�A�[�x�o�b�t�@���N���A����
    //---------------------------------------------------------------------
	LNRESULT Renderer::clear( bool target_, bool depth_, u32 color_, lnfloat z_ )
    {
        HRESULT hr;
        u32 flag = 0;
        if ( target_ ) flag |= D3DCLEAR_TARGET;  
        if ( mCurrentDepthBuffer && depth_ ) flag |= ( D3DCLEAR_ZBUFFER );// | D3DCLEAR_STENCIL

        if ( flag == 0 ) return LN_OK;

        //D3DVIEWPORT9 vp;
        //mDxDevice->GetViewport(&vp);
        //vp.Width = 640;
        //vp.Height = 480;
        //mDxDevice->SetViewport(&vp);


        LN_DXCALL_R(
            mDxDevice->Clear( 0, NULL, flag, color_, z_, 0x00000000 ) );
        return LN_OK;
    }

    //---------------------------------------------------------------------
	// �� �v���~�e�B�u�`��
	//---------------------------------------------------------------------
	LNRESULT Renderer::drawPrimitive( LNPrimitiveType primitive_, u32 start_vertex_, u32 primitive_count_ )
	{
        LN_SETERR_R_( ( mCurrentVertexBuffer ), LN_ERR_ASSERT, "< Renderer::drawPrimitive >\n\n���_�o�b�t�@���Z�b�g����Ă��܂���B" );
		
        HRESULT hr;
		D3DPRIMITIVETYPE dx_prim = D3DPT_TRIANGLELIST;
		switch ( primitive_ )
		{
			//case LN_PRIMITIVE_TRIANGLELIST:   dx_prim = D3DPT_TRIANGLELIST; break;
			case LN_PRIM_TRIANGLESTRIP:    
				dx_prim = D3DPT_TRIANGLESTRIP; break;
			case LN_PRIM_LINELIST:         
				dx_prim = D3DPT_LINELIST; break;
            case LN_PRIM_LINESTRIP:        
            	dx_prim = D3DPT_LINESTRIP; break;
			case LN_PRIM_POINTLIST:        
				dx_prim = D3DPT_POINTLIST; break;
		}

		// ���I�Ȓ��_�o�b�t�@�Ƃ��č쐬���Ă���ꍇ�� DrawPrimitiveUP �ŕ`�悷��
		if ( mCurrentVertexBuffer->isDynamic() )
		{
            const char* vertices = (const char*)mCurrentVertexBuffer->lock();
            u32 stride = mCurrentVertexBuffer->getVertexStride();
            vertices += stride * start_vertex_;

			LN_DXCALL_R(
                mDxDevice->DrawPrimitiveUP(
				    dx_prim,
				    primitive_count_,
				    vertices,
                    stride ) );
		}
		else
		{
            LN_DXCALL_R(
                mDxDevice->DrawPrimitive( 
				    dx_prim,
				    start_vertex_,
				    primitive_count_ ) );
		}
        
        return LN_OK;
	}

	//---------------------------------------------------------------------
	// �� �v���~�e�B�u�`�� ( �C���f�b�N�X�t�� )
	//---------------------------------------------------------------------
	LNRESULT Renderer::drawPrimitiveIndexed( LNPrimitiveType prim_, u32 start_index_, u32 primitive_count_ )
	{
		LN_SETERR_R_( ( mCurrentVertexBuffer ), LN_ERR_ASSERT, "< Renderer::drawPrimitiveIndexed >\n\n���_�o�b�t�@���Z�b�g����Ă��܂���B" );
		LN_SETERR_R_( ( mCurrentIndexBuffer ),  LN_ERR_ASSERT, "< Renderer::drawPrimitiveIndexed >\n\n�C���f�b�N�X�o�b�t�@���Z�b�g����Ă��܂���B" );
        LN_SETERR_R_( ( start_index_ < mCurrentIndexBuffer->getIndexNum() ),
            LN_ERR_INVALIDCALL, "< Renderer::drawPrimitiveIndexed >\n\nstart_index_ : %d", start_index_ );

        HRESULT hr;
		D3DPRIMITIVETYPE dx_prim = D3DPT_TRIANGLELIST;
		switch ( prim_ )
		{
			//case LN_PRIMITIVE_TRIANGLELIST:     dx_prim = D3DPT_TRIANGLELIST; break;
			case LN_PRIM_TRIANGLESTRIP:    
				dx_prim = D3DPT_TRIANGLESTRIP; break;
			case LN_PRIM_LINELIST:         
				dx_prim = D3DPT_LINELIST; break;
            case LN_PRIM_LINESTRIP:        
            	dx_prim = D3DPT_LINESTRIP; break;
			case LN_PRIM_POINTLIST:        
				dx_prim = D3DPT_POINTLIST; break;
		}

		bool vd = mCurrentVertexBuffer->isDynamic();
		bool id = mCurrentIndexBuffer->isDynamic();

		// ������ dynamic
		if ( vd && id )
		{
            D3DFORMAT ib_fmt;
            const void* index_data;
            if ( mCurrentIndexBuffer->getIndexStride() == 2 )
            {
                ib_fmt = D3DFMT_INDEX16;
                index_data = &((lnU16*)mCurrentIndexBuffer->lock())[ start_index_ ];
            }
            else
            {
                ib_fmt = D3DFMT_INDEX32;
                index_data = &((lnU32*)mCurrentIndexBuffer->lock())[ start_index_ ];
            }

            //LN_DXCALL_R(
            LN_COMCALL( 
                mDxDevice->DrawIndexedPrimitiveUP(
				    dx_prim,
				    0,
				    mCurrentVertexBuffer->getVertexNum(),
				    primitive_count_,
				    index_data,
				    ib_fmt,
				    mCurrentVertexBuffer->lock(),
				    mCurrentVertexBuffer->getVertexStride() ) );
		}
        // ������ static
		else if ( !vd && !id )
		{
            LN_DXCALL_R(
			    mDxDevice->DrawIndexedPrimitive( 
				    dx_prim,
				    0,
				    0,
				    mCurrentVertexBuffer->getVertexNum(),
				    start_index_,
				    primitive_count_ ) );
		}
        return LN_OK;
	}

    //---------------------------------------------------------------------
    // �� �V�[���J�n
    //---------------------------------------------------------------------
	LNRESULT Renderer::beginScene()
    {
        // �`��J�n�ς݃t���O�� ON �ꍇ�͒��f
		if ( mSceneBegan ) return LN_OK;

		HRESULT hr;
        
        LN_DXCALL_R( mDxDevice->BeginScene() );

        mDxDevice->Clear( 0, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0 );

		mSceneBegan = true;

        return LN_OK;
    }

	//---------------------------------------------------------------------
    // �� �V�[���I��
    //---------------------------------------------------------------------
	LNRESULT Renderer::endScene()
    {
        HRESULT hr;

        // �`��J�n�ς݃t���O�� OFF �ꍇ�͒��f
		if ( !mSceneBegan ) return LN_OK;

        mSceneBegan = false;
 
        LN_DXCALL_R( mDxDevice->EndScene() );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �f�o�C�X���Z�b�g���O GraphicsDevice ����Ă΂��
    //---------------------------------------------------------------------
    LNRESULT Renderer::onLostDevice()
    {
        mCurrentVertexBuffer = NULL;
        mCurrentIndexBuffer  = NULL;
        mCurrentDepthBuffer  = NULL;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� �f�o�C�X���Z�b�g��� GraphicsDevice ����Ă΂��
    //---------------------------------------------------------------------
    LNRESULT Renderer::onResetDevice()
    {
        LNRESULT lr;

        

        LN_CALL_R( _initRenderState() );
        LN_CALL_R( _resetRenderState() );

        memset( mCurrentRenderTargets, 0, sizeof( mCurrentRenderTargets ) );

        LN_CALL_R( setRenderTarget( 0, NULL, true ) );
           
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� ��ŕύX���Ȃ������_�����O�X�e�[�g���f�o�C�X�ɐݒ肷��
    //---------------------------------------------------------------------
    LNRESULT Renderer::_initRenderState()
    {
        HRESULT hr = S_OK;

        mDxDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE );
        //printf("aa\n");
		// �A���t�@�e�X�g�̎��̊�A���t�@�l ( Clear�œh��Ԃ��F�ɓ����F���g���Ȃ��]�X )
		//hr += mDxDevice->SetRenderState( D3DRS_ALPHAREF, 0x00000001 );
		// �A���t�@�e�X�g���̔�r�֐��̎w�� ( D3DCMPFUNC �񋓌^  �f�t�H���g�� D3DCMP_ALWAYS )
		hr += mDxDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
		// �A���t�@�u�����h�L��
		//hr += mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		// �@���̐��K����ݒ肷�� ( ������Ǝ��Ԃ�������ۂ� )
		//hr += mDxDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );

        /*
		// �J�����O���[�h�̐ݒ�
		hr += mDxDevice->SetRenderState( D3DRS_CULLMODE, mCurrentCullMode );
		// Z�o�b�t�@�����O�̗L���ݒ�
		hr += mDxDevice->SetRenderState( D3DRS_ZENABLE, mCurrentDepthTest );

		// Z�o�b�t�@�ւ̏������݋��ݒ�
		hr += mDxDevice->SetRenderState( D3DRS_ZWRITEENABLE, mCurrentDepthWrite );
		// �A���t�@�e�X�g�̗L���ݒ�
		hr += mDxDevice->SetRenderState( D3DRS_ALPHATESTENABLE, mCurrentAlphaTest );
		// �e�N�X�`���k���̕␳
		hr += mDxDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, mCurrentMinFilter );
		// �e�N�X�`���g��̕␳
		hr += mDxDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, mCurrentMagFilter );
		// �~�b�v�t�B���^
		hr += mDxDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, mCurrentMipFilter );
		// �ٕ���
		hr += mDxDevice->SetSamplerState( 0, D3DSAMP_MAXANISOTROPY, mMaxAnisotropy );
        */
		
		// �e�N�X�`���u�����f�B���O�̐ݒ�
		// �e�N�X�`����
		hr += mDxDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		// ���_�F��
		hr += mDxDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		// �F����Z
		hr += mDxDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		// �e�N�X�`����
		hr += mDxDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		// ���_�F��
		hr += mDxDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		// �A���t�@�l����Z
		hr += mDxDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // �� ���ۂɃ����_�����O�X�e�[�g��ݒ肷��
    //---------------------------------------------------------------------
    LNRESULT Renderer::_updateRenderState()
    {
        // �������@
	    if ( mRequestRenderState.BlendMode != mCurrentRenderState.BlendMode )
	    {
		    switch ( mRequestRenderState.BlendMode )
		    {
		        case LN_BLEND_NORMAL:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  127 );
			        break;
		        case LN_BLEND_ALPHA:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  1 );

                    /*
                    mDxDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED );
					mDxDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
					mDxDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
					mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
					
					// �A���t�@�ʃu�����h�L��
					mDxDevice->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
					mDxDevice->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_SRCALPHA );
					mDxDevice->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_DESTALPHA );
			        */
                    break;
		        case LN_BLEND_ADD:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
		        case LN_BLEND_ADD_ALPHADISABLE:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
		        case LN_BLEND_SUB:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_REVSUBTRACT );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
		        case LN_BLEND_SUB_ALPHADISABLE:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_REVSUBTRACT );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
		        case LN_BLEND_MUL:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
		        case LN_BLEND_SCREEN:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_INVDESTCOLOR );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
		    }
	    }

        // �J�����O
	    if ( mRequestRenderState.Culling != mCurrentRenderState.Culling )
	    {
		    const u32 tb[] = { D3DCULL_CCW, D3DCULL_CW, D3DCULL_NONE };
		    mDxDevice->SetRenderState( D3DRS_CULLMODE, tb[ mRequestRenderState.Culling ] );
	    }

        // �h��Ԃ����@
	    if ( mRequestRenderState.FillMode != mCurrentRenderState.FillMode )
	    {
		    const u32 tb[] = { D3DFILL_SOLID, D3DFILL_WIREFRAME, D3DFILL_POINT };
		    mDxDevice->SetRenderState( D3DRS_FILLMODE, tb[ mRequestRenderState.FillMode ] );
	    }
	    
	    // �A���t�@�e�X�g
	    if ( mRequestRenderState.AlphaTest != mCurrentRenderState.AlphaTest )
	    {
		    const u32 tb[] = { FALSE, TRUE };
		    mDxDevice->SetRenderState( D3DRS_ALPHATESTENABLE, tb[ mRequestRenderState.AlphaTest ] );
	    }
	    // �[�x�e�X�g
	    if ( mRequestRenderState.DepthTest != mCurrentRenderState.DepthTest )
	    {
		    const u32 tb[] = { FALSE, TRUE };
		    mDxDevice->SetRenderState( D3DRS_ZENABLE, tb[ mRequestRenderState.DepthTest ] );
	    }
	    // �[�x��������
        if ( mRequestRenderState.DepthWrite != mCurrentRenderState.DepthWrite )
	    {
		    const u32 tb[] = { FALSE, TRUE };
		    mDxDevice->SetRenderState( D3DRS_ZWRITEENABLE, tb[ mRequestRenderState.DepthWrite ] );
	    }
        // �|�C���g�X�v���C�g
        if ( mRequestRenderState.PointSprite != mCurrentRenderState.PointSprite )
	    {
		    const u32 tb[] = { FALSE, TRUE };
            mDxDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, tb[ mRequestRenderState.PointSprite ] );
	    }
	   
        // �V�U�[�e�X�g
        
     //   if ( mRequestRenderState.ScissorTest != mCurrentRenderState.ScissorTest )
	    //{
		   // const u32 tb[] = { FALSE, TRUE };
		   // mDxDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, tb[ mRequestRenderState.ScissorTest ] );
     //       _setScissorRect( mRequestRenderState.ScissorRect );
	    //}
        

        mCurrentRenderState = mRequestRenderState;

        /*
        switch ( type_ )
			{
				case BLEND_ADD:		// ���Z
					mDxDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED );
					mDxDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
					//hr = mDxDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR );
					mDxDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA ); // �������̕����Ȃ߂炩�����ǁE�E�H
					mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
					mDxDevice->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, FALSE );
					break;

				case BLEND_SUB:		// ���Z
					mDxDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED );
					mDxDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT );
					mDxDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO );
					mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );
					mDxDevice->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, FALSE );
					break;

				case BLEND_REVERSE:	// ���o�[�X
					mDxDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED );
					mDxDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
					mDxDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR );
					mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR ); 
					mDxDevice->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, FALSE );
					break;

				default:	// �ʏ� ( ������ )
					mDxDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED );
					mDxDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
					mDxDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
					mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
					
					// �A���t�@�ʃu�����h�L��
					mDxDevice->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
					mDxDevice->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_SRCALPHA );
					mDxDevice->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_DESTALPHA );
					break;
			}
            */

        return LN_OK;
    }


    //---------------------------------------------------------------------
    // �� ���݂̃����_�����O�X�e�[�g���f�o�C�X�ɍĐݒ肷��
    //---------------------------------------------------------------------
    LNRESULT Renderer::_resetRenderState()
    {
        // �������@
	    {
		    switch ( mCurrentRenderState.BlendMode )
		    {
		        case LN_BLEND_NORMAL:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  127 );
			        break;
		        case LN_BLEND_ALPHA:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  1 );
			        break;
		        case LN_BLEND_ADD:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
		        case LN_BLEND_ADD_ALPHADISABLE:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
		        case LN_BLEND_SUB:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_REVSUBTRACT );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
		        case LN_BLEND_SUB_ALPHADISABLE:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_REVSUBTRACT );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
		        case LN_BLEND_MUL:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
		        case LN_BLEND_SCREEN:
			        mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			        mDxDevice->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
			        mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			        mDxDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_INVDESTCOLOR );
			        mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
			        break;
                case LN_BLEND_REVERSE:
                    mDxDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
                    mDxDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
					mDxDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR );
					mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );
                    mDxDevice->SetRenderState( D3DRS_ALPHAREF,  0 );
                    /*
					mDxDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED );
					mDxDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
					mDxDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR );
					mDxDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR ); 
					mDxDevice->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, FALSE );
                    */
					break;
		    }
	    }

        // �J�����O
	    {
		    const u32 tb[] = { D3DCULL_CCW, D3DCULL_CW, D3DCULL_NONE };
		    mDxDevice->SetRenderState( D3DRS_CULLMODE, tb[ mCurrentRenderState.Culling ] );
	    }

        // �h��Ԃ����@
	    {
		    const u32 tb[] = { D3DFILL_SOLID, D3DFILL_WIREFRAME, D3DFILL_POINT };
		    mDxDevice->SetRenderState( D3DRS_FILLMODE, tb[ mCurrentRenderState.FillMode ] );
	    }
	    
	    // �A���t�@�e�X�g
	    {
		    const u32 tb[] = { FALSE, TRUE };
		    mDxDevice->SetRenderState( D3DRS_ALPHATESTENABLE, tb[ mCurrentRenderState.AlphaTest ] );
	    }
	    // �[�x�e�X�g
	    {
		    const u32 tb[] = { FALSE, TRUE };
		    mDxDevice->SetRenderState( D3DRS_ZENABLE, tb[ mCurrentRenderState.DepthTest ] );
	    }
	    // �[�x��������
	    {
		    const u32 tb[] = { FALSE, TRUE };
		    mDxDevice->SetRenderState( D3DRS_ZWRITEENABLE, tb[ mCurrentRenderState.DepthWrite ] );
	    }
        // �|�C���g�X�v���C�g
        if ( mRequestRenderState.PointSprite != mCurrentRenderState.PointSprite )
	    {
		    const u32 tb[] = { FALSE, TRUE };
            mDxDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, tb[ mRequestRenderState.PointSprite ] );
	    }
	   
	   
        // �V�U�[�e�X�g
	    //{
		   // const u32 tb[] = { FALSE, TRUE };
		   // mDxDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, tb[ mEnableScissorTest ] );
     //       _setScissorRect( mScissorRect );
	    //}

        float pt_size = 5.0f;
        mDxDevice->SetRenderState( D3DRS_POINTSIZE, *((DWORD*)&pt_size) );
        
        return LN_OK;
    }


    //---------------------------------------------------------------------
    // �� �V�U�[��`�̐ݒ�
    //---------------------------------------------------------------------
    LNRESULT Renderer::_setScissorRect( const Geometry::Rect& rect_ )
    {
        RECT rect = {
		    rect_.x,
		    rect_.y,
		    rect_.x + rect_.w,
		    rect_.y + rect_.h,
	    };
        HRESULT hr;
        LN_DXCALL_R( mDxDevice->SetScissorRect( &rect ) );

        mCurrentScissorRect = rect_;
        return LN_OK;
    }


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace DirectX9
} // namespace Graphics
} // namespace Core
} // namespace LNote

//=============================================================================
//								end of file
//=============================================================================