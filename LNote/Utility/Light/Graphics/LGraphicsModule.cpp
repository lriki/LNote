//=============================================================================
//�y LGraphicsModule �z
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "LGraphics.h"
#include "LCamera.h"
#include "LLayer.h"
#include "LTexture.h"
#include "LShader.h"
#include "LPanel.h"
#include "LSprite.h"
#include "LBatchPanel.h"
#include "LParticle.h"
#include "LFont.h"
#include "LDebugFont.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{

//=============================================================================
// �� LGraphics �N���X
//=============================================================================

    // �C���X�^���X�̎擾
    LGraphics LGraphics::getInstance()
    {
        LGraphics o;
        o._impl = gFramework->getGraphicsManager();
        return o;
    }

    // �Q�[���X�N���[���̃T�C�Y���擾����
    LVector2 LGraphics::getScreenSize() const
    {
        const LSize& s = _impl->getGraphicsDevice()->getDefaultCanvas()->getSize();
        LVector2 size((lnfloat)s.w, (lnfloat)s.h);
        return size;
    }

    // �w�i�F��ݒ肷��
	void LGraphics::setBGColor( const LColor& color_ )
    {
		gFramework->getSceneManager()->getDefaultSceneGraph()->getDefaultPane()->setBackgroundColor( color_ );
    }

    // �w�i�F��ݒ肷��
	void LGraphics::setBGColor( lnfloat r_, lnfloat g_, lnfloat b_ )
    {
        setBGColor( LColor( r_, g_, b_, 1.0f ) );
        //_impl->getDefaultCanvas()->setBGColor( LColor( r_, g_, b_, 1.0f ) );
    }

//=============================================================================
// �� LCamera �N���X
//=============================================================================

    // �f�t�H���g�J�����̎擾
    LCamera LCamera::getDefaultCamera()
    {
        return LCamera( gFramework->getSceneManager()->getDefaultSceneGraph()->getDefaultCamera() );
    }

    // �ʒu�̎擾
    const LVector3& LCamera::getPosition() const
    {
        return _impl->getPosition();
    }

	// �ʒu�̐ݒ�
    void LCamera::setPosition( const LVector3& position_ )
    {
        _impl->setPosition( position_ );
    }

	// �����_�̎擾
    const LVector3& LCamera::getLookAt() const
    {
        return _impl->getLookAt();
    }

	// �����_�̐ݒ�
    void LCamera::setLookAt( const LVector3& at_ )
    {
        _impl->setLookAt( at_ );
    }

	// ������x�N�g���̎擾
    const LVector3& LCamera::getUpDirection() const
    {
        return _impl->getUpDirection();
    }

	// ������x�N�g���̐ݒ�
    void LCamera::setUpDirection( const LVector3& up_ )
    {
        _impl->setUpDirection( up_ );
    }

    // Y ��������p�̎擾
    lnfloat LCamera::getFovY() const
    {
        return _impl->getFovY();
    }

	// Y ��������p�̐ݒ�
    void LCamera::setFovY( lnfloat fov_y_ )
    {
        _impl->setFovY( fov_y_ );
    }

    // �ł��߂��r���[�v���[���ʒu�̎擾
    lnfloat LCamera::getNearClip() const
    {
        return _impl->getNearClip();
    }

	// �ł��߂��r���[�v���[���ʒu�̐ݒ�
    void LCamera::setNearClip( lnfloat near_ )
    {
        return _impl->setNearClip( near_ );
    }

    // �ł������r���[�v���[���ʒu�̎擾
    lnfloat LCamera::getFarClip() const
    {
        return _impl->getFarClip();
    }

	// �ł������r���[�v���[���ʒu�̐ݒ�
    void LCamera::setFarClip( lnfloat far_ )
    {
        _impl->setFarClip( far_ );
    }

    // �J�����^�C�v�̐ݒ�
    void LCamera::setType( LNCameraType type_ )
    {
        _impl->setType( type_ );
    }

    // �J�������猩�� 3D ���W�� 2D ���W�ɕϊ�����
    void LCamera::convertWorldPosToScreenPos( LVector3* out_, const LVector3& world_pos_ )
    {
        LN_PCALL( _impl->convertWorldPosToScreenPos( out_, world_pos_ ) );
    }

    // �r���[�s��̎擾
    const LMatrix& LCamera::getViewMatrix()
    {
        return _impl->getViewMatrix();
    }

    // �v���W�F�N�V�����s��̎擾
    const LMatrix& LCamera::getProjectionMatrix()
    {
        return _impl->getProjectionMatrix();
    }

    // �r���[�s��ƃv���W�F�N�V�����s��̐ς̎擾
    const LMatrix& LCamera::getViewProjectionMatrix()
    {
        return _impl->getViewProjectionMatrix();
    }

    // �r���[�s��̋t�s��̎擾
    const LMatrix& LCamera::getViewMatrixInverse()
    {
        return _impl->getViewMatrixInverse();
    }

    // ���ˉe�s��̎擾
    //const LMatrix& LCamera::getOrthoMatrix()
    //{
    //    return _impl->getOrthoMatrix();
    //}


//=============================================================================
// �� LLayer �N���X
//=============================================================================

    // �w�i���C���[�̎擾
    LLayer LLayer::getBGLayer()
    {
        return LLayer( gFramework->getSceneManager()->getDefaultSceneGraph()->getDefaultPane() );
    }

    // �F���̐ݒ�
    void LLayer::setTone( const LTone& tone_, double duration_ )
    {
		gFramework->getSceneManager()->getDefaultSceneGraph()->getDefaultPane()->setTone( tone_, duration_ );
    }

    // �u���[�̓K�p
    void LLayer::blur( double duration_, lnfloat power_, lnfloat scale_, const LVector3& center_, bool center_is_3d_ )
    {
        _impl->blur( duration_, power_, scale_, center_, center_is_3d_ );
    }

    // �g�����W�V�����̏����Ƃ��Č��݂̉�ʂ��Œ肷��
	void LLayer::freeze()
    {
        _impl->freeze();
    }

    // �g�����W�V�������s
    void LLayer::transition( double duration_, LTexture mask_texture_, u32 vague_ )
    {
        _impl->transition( duration_, mask_texture_->getInterface(), vague_ );
    }

    // �G�t�F�N�g���K�p�����𔻒肷��
	bool LLayer::isEffect() const
    {
        return _impl->isBlurEffect();
    }


//=============================================================================
// �� LTexture �N���X
//=============================================================================

    // �t�@�C������쐬
    LTexture LTexture::create( const char* filename_, u32 color_key_ )
    {
        LTexture o;
        gFramework->getGraphicsManager()->getGraphicsDevice()->createTexture(
            &o._impl, filename_, color_key_, 1, LN_FMT_A8R8G8B8, LSharingKey( filename_ ) );
#if 0
        LNRESULT lr;
        LTexture o;

        File::Manager*    file_manager = gFramework->getFileManager();
        Graphics::Manager*  graph_manager = gFramework->getGraphicsManager();

        errno = 0;

        // �t�@�C�����J��
        FileIO::InFile* file;
        lr = file_manager->createInFile( &file, filename_ );
        if ( LN_FAILED( lr ) )
        {
            LN_PCALLF( lr, "�e�N�X�`���̍쐬�Ɏ��s���܂����B\n\n%s", filename_ );
            return o;
        }

        // �e�N�X�`�������
        lr = graph_manager->getGraphicsDevice()->getResourceManager()->createTexture( &o._impl, file, color_key_, 1, LN_FMT_A8R8G8B8, LSharingKey( filename_ ) );
        if ( LN_FAILED( lr ) )
        {
            LN_SAFE_RELEASE( file );
            LN_PCALLF( lr, "�e�N�X�`���̍쐬�Ɏ��s���܂����B\n\n%s", filename_ );
            return o;
        }

        SAFE_RELEASE( file );
#endif
        return o;
    }

    // �e�N�X�`�����쐬����
    LTexture LTexture::create( u32 width_, u32 height_, u32 clear_color_ )
    {
        LNRESULT lr;
        LTexture o;

        Core::Graphics::Manager*  graph_manager = gFramework->getGraphicsManager();

        // �e�N�X�`�������
        lr = graph_manager->getGraphicsDevice()->createTexture( &o._impl, width_, height_, 1, LN_FMT_A8R8G8B8 );
        if ( LN_FAILED( lr ) )
        {
            //LN_SETERR_HEADER( "�e�N�X�`���̍쐬�Ɏ��s���܂����B\n\nwidth:%d height:%d", width_, height_  );
            //LN_ERRPROC_S();
            return o;
        }

        o._impl->clear( LColor( clear_color_, graph_manager->getGraphicsAPI() ) );

        return o;
    }

    // �T�C�Y�̎擾
    const LVector2& LTexture::getSize() const
    {
        return _impl->getSize();
    }

    // ���ۂ̃T�C�Y�̎擾
    const LVector2& LTexture::getRealSize() const
    {
        return _impl->getRealSize();
    }

    // �e�N�X�`�����N���A����
    void LTexture::clear( const LColor& color_ )
    {
        _impl->clear( color_ );
    }

    // drawText() �Ŏg�p����t�H���g�̐ݒ�     
    void LTexture::setFont( LFont font_ )
    {
        _impl->setFont( font_->getInterface() );
    }

    // �ݒ肳��Ă���t�H���g�̎擾     
    LFont LTexture::getFont()
    {
        return LFont( _impl->getFont() );
    }

    // �������`�悷��
    void LTexture::drawText( const lnChar* text_, int x_, int y_, int len_ )
    {
        const LVector2 size = getRealSize();
        LN_PCALL( _impl->drawText( text_, LRect( x_, y_, static_cast< int >( size.x ), static_cast< int >( size.y ) ), LN_TEXTALIGN_LEFT, len_ ) );
    }

    // �������`�悷��
    void LTexture::drawText( const lnChar* text_, const LRect& rect_, LNTextAlign align_, int len_ )
    {
        LN_PCALL( _impl->drawText( text_, rect_, align_, len_ ) );
    }

    // �e�N�X�`���̃u���b�N�]��
    void LTexture::blt( int x_, int y_, LTexture src_texture_, const LRect& src_rect_ )
    {
        LN_PCALL( _impl->blt( x_, y_, src_texture_.getInterface(), src_rect_ ) );
    }


//=============================================================================
// �� LShader �N���X
//=============================================================================

    // �V�F�[�_�v���O�������쐬����
    LShader LShader::create( const char* filename_ )
    {
        LShader o;
        gFramework->getSceneManager()->getDefaultSceneGraph()->getSceneShaderManager()->
            createSceneShader( &o._impl, filename_ );
#if 0
        LNRESULT lr;
        LShader o;

        // �t�@�C�����J��
        FileIO::InFile* file;
        LN_PCALL( Framework::getInstance()->getFileIOManager()->createInFile( &file, filename_ ) );

        // �t�@�C���̃f�B���N�g���܂ňړ�
        System::moveCurrentDirectory( filename_ );

        // �V�F�[�_�����
        Scene::ISceneShader* shader;
        lr = Framework::getInstance()->getSceneManager()->getDefaultScene()->createSceneShader( &shader, file, LSharingKey( filename_ ) );

        // ���̃f�B���N�g���ɖ߂�
        System::returnCurrentDirectory();
        
        // ��n��
        SAFE_RELEASE( file );
        LN_PCALL( lr );

        o._impl = shader;
#endif
        return o;
    }
    

//=============================================================================
// �� LPanel �N���X
//=============================================================================

    // �p�l���I�u�W�F�N�g���쐬����
    LPanel LPanel::create( lnfloat w_size_, lnfloat h_size_, LNAxisAlignedDir dir_ )//, LNPanelCenter center_, LNPanelDirection dir_ )
    {
        LPanel o;
        Core::Scene::Sprite3D* panel = LN_NEW Core::Scene::Sprite3D( gFramework->getSceneManager()->getDefaultSceneGraph() );
        LN_PCALL( panel->initialize( w_size_, h_size_, dir_ ) );
        o._lnso = o._impl = panel;
        return o;
    }

    // �p�l���I�u�W�F�N�g���쐬����
    LPanel LPanel::create( lnfloat width_, lnfloat height_, LTexture texture_, LNAxisAlignedDir dir_ )
    {
        LPanel o = create( width_, height_, dir_ );
        o->setTexture( texture_ );
        return o;
    }

    // �傫���̐ݒ�
    void LPanel::setSize( const LVector2& size_ )
    {
        _impl->setSize( size_ );
    }

    // �傫���̐ݒ�
    void LPanel::setSize( lnfloat width_, lnfloat height_ )
    {
        _impl->setSize( LVector2( width_, height_ ) );
    }

    // �傫���̎擾
    const LVector2& LPanel::getSize() const
    {
        return _impl->getSize();
    }

    // �e�N�X�`���̐ݒ�
    void LPanel::setTexture( LTexture texture_ )
    {
        _impl->setTexture( texture_->getInterface() );
    }

    // �e�N�X�`���̎擾
    LTexture LPanel::getTexture() const
    {
        return LTexture( _impl->getTexture() );
    }

    // �e�N�X�`���̓]����`�̎擾
    const LRect& LPanel::getSrcRect() const
    {
        return _impl->getSrcRect();
    }

    // �e�N�X�`���̓]����`�̐ݒ�
    void LPanel::setSrcRect( const LRect& rect_ )
    {
        _impl->setSrcRect( rect_ );
    }

    // �e�N�X�`���̓]����`�̐ݒ�
    void LPanel::setSrcRect( int x_, int y_, int width_, int height_ )
    {
        _impl->setSrcRect( LRect( x_, y_, width_, height_ ) );
    }

    // ���]���@�̐ݒ�
    void LPanel::setFlipFlag( u32 flag_ )
    {
        _impl->setFlipFlag( flag_ );
    }

    // ���]���@�̎擾
    u32 LPanel::getFlipFlag()
    {
        return _impl->getFlipFlag();
    }


//=============================================================================
// �� LSprite �N���X
//=============================================================================

    // �X�v���C�g���쐬����
    LSprite LSprite::create()
    {
        LSprite o;
        Core::Scene::Sprite* sprite = LN_NEW Core::Scene::Sprite( gFramework->getSceneManager()->getDefaultSceneGraph() );
        sprite->initialize();
        o._lnso = o._impl = sprite;
        return o;
    }

    // �X�v���C�g���쐬����
    LSprite LSprite::create( LTexture texture_ )
    {
        LSprite o = create();
        o->setTexture( texture_ );
        return o;
    }

    // �e�N�X�`���̐ݒ�
    void LSprite::setTexture( LTexture texture_ )
    {
        _impl->setTexture( texture_->getInterface() );
    }

    // �e�N�X�`���̎擾
    LTexture LSprite::getTexture() const
    {
        return LTexture( _impl->getTexture() );
    }

    // �e�N�X�`���̓]����`�̎擾
    const LRect& LSprite::getSrcRect() const
    {
        return _impl->getSrcRect();
    }

    // �e�N�X�`���̓]����`�̐ݒ�
    void LSprite::setSrcRect( const LRect& rect_ )
    {
        _impl->setSrcRect( rect_ );
    }

    // �e�N�X�`���̓]����`�̐ݒ�
    void LSprite::setSrcRect( int x_, int y_, int width_, int height_ )
    {
        _impl->setSrcRect( LRect( x_, y_, width_, height_ ) );
    }

    // ���]���@�̐ݒ�
    void LSprite::setFlipFlag( u32 flag_ )
    {
        _impl->setFlipFlag( flag_ );
    }

    // ���]���@�̎擾
    u32 LSprite::getFlipFlag()
    {
        return _impl->getFlipFlag();
    }

//=============================================================================
// �� LBatchPanel �N���X
//=============================================================================

    // �p�l���I�u�W�F�N�g���쐬����
    LBatchPanel LBatchPanel::create( lnfloat width_, lnfloat height_, LNAxisAlignedDir front_ )
    {
        LBatchPanel o;
        Core::Scene::BatchSprite3D* panel = LN_NEW Core::Scene::BatchSprite3D( gFramework->getSceneManager()->getDefaultSceneGraph() );
        panel->initialize( width_, height_, front_ );
        o._lnso = o._impl = panel;
        return o;
    }

    // �p�l���I�u�W�F�N�g���쐬����
    LBatchPanel LBatchPanel::create( lnfloat width_, lnfloat height_, LTexture texture_, LNAxisAlignedDir front_ )
    {
        LBatchPanel o = create( width_, height_, front_ );
        o.setTexture( texture_ );
        return o;
    }

    // �傫���̐ݒ�
    void LBatchPanel::setSize( const LVector2& size_ )
    {
        _impl->setSize( size_ );
    }

    // �傫���̐ݒ�
    void LBatchPanel::setSize( lnfloat width_, lnfloat height_ )
    {
        _impl->setSize( LVector2( width_, height_ ) );
    }

    // �傫���̎擾
    const LVector2& LBatchPanel::getSize() const
    {
        return _impl->getSize();
    }

    // �e�N�X�`���̐ݒ�
    void LBatchPanel::setTexture( LTexture texture_ )
    {
        _impl->setTexture( texture_->getInterface() );
    }

    // �e�N�X�`���̎擾
    LTexture LBatchPanel::getTexture() const
    {
        return LTexture( _impl->getTexture() );
    }

    // �e�N�X�`���̓]����`�̎擾
    const LRect& LBatchPanel::getSrcRect() const
    {
        return _impl->getSrcRect();
    }

    // �e�N�X�`���̓]����`�̐ݒ�
    void LBatchPanel::setSrcRect( const LRect& rect_ )
    {
        _impl->setSrcRect( rect_ );
    }

    // �e�N�X�`���̓]����`�̐ݒ�
    void LBatchPanel::setSrcRect( int x_, int y_, int width_, int height_ )
    {
        _impl->setSrcRect( LRect( x_, y_, width_, height_ ) );
    }

    // ���]���@�̐ݒ�
    void LBatchPanel::setFlipFlag( u32 flag_ )
    {
        _impl->setFlipFlag( (lnU8)flag_ );
    }

    // ���]���@�̎擾
    u32 LBatchPanel::getFlipFlag()
    {
        return _impl->getFlipFlag();
    }


//=============================================================================
// �� LParticle �N���X
//=============================================================================

    // �p�[�e�B�N���I�u�W�F�N�g���쐬����
    LParticle LParticle::create( LTexture texture_, const LNParticleCreateData& data_ )
    {
        LParticle o;
        Core::Scene::PointParticle* particle = LN_NEW Core::Scene::PointParticle(
            gFramework->getSceneManager()->getDefaultSceneGraph() );
        particle->initialize( data_ );
        particle->setTexture( texture_.getInterface() );
        o._lnso = o._impl = particle;
        return o;
    }


    // �e�N�X�`���̐ݒ�
    void LParticle::setTexture( LTexture texture_ )
    {
        _impl->setTexture( texture_.getInterface() );
    }

    // �e�N�X�`���̎擾
    //LTexture LParticle::getTexture() const
    //{
    //    return LTexture( _impl->getTexture() );
    //}

  

//=============================================================================
// �� LFont �N���X
//=============================================================================

    // �f�t�H���g�̐ݒ�Ńt�H���g���쐬����
    LFont LFont::createDefaultFont()
    {
        LFont o;
        LN_PCALL( gFramework->getGraphicsManager()->createFont( &o._impl, NULL ) );
        return o;
    }

    // �f�t�H���g�̃t�H���g���擾����
    LFont LFont::getDefaultFont()
    {
        return LFont( gFramework->getGraphicsManager()->getDefaultFont() );
    }

    // �t�H���g�T�C�Y�̐ݒ�
	void LFont::setSize( int size_ )
    {
        _impl->setSize( size_ );
    }

    // �t�H���g�T�C�Y�̎擾
	int LFont::getSize()
    {
        return _impl->getSize();
    }

    // �t�H���g�J���[�̐ݒ�
	void LFont::setColor( const LColor& color_ )
    {
        _impl->setColor( color_ );
    }

    // �t�H���g�J���[�̐ݒ�
	void LFont::setColor( lnfloat r_, lnfloat g_, lnfloat b_, lnfloat a_ )
    {
        _impl->setColor( LColor( r_, g_, b_, a_ ) );
    }

    // �t�H���g�J���[�̎擾
	const LColor& LFont::getColor()
    {
        return _impl->getColor();
    }

    // �G�b�W�̕��̐ݒ�
	void LFont::setEdgeSize( int size_ )
    {
        _impl->setEdgeSize( size_ );
    }

    // �G�b�W�̕��̎擾
	int LFont::getEdgeSize()
    {
        return _impl->getEdgeSize();
    }

    // �G�b�W�J���[�̐ݒ�
	void LFont::setEdgeColor( lnfloat r_, lnfloat g_, lnfloat b_, lnfloat a_ )
    {
        _impl->setEdgeColor( LColor( r_, g_, b_, a_ ) );
    }

    // �G�b�W�J���[�̐ݒ�
	void LFont::setEdgeColor( const LColor& color_ )
    {
        _impl->setEdgeColor( color_ );
    }

    // �G�b�W�J���[�̎擾
	const LColor& LFont::getEdgeColor()
    {
        return _impl->getEdgeColor();
    }

    // �������̐ݒ�
	void LFont::setBold( bool flag_ )
    {
        _impl->setBold( flag_ );
    }

    // �������̔���
	bool LFont::isBold()
    {
        return _impl->isBold();
    }

    // �C�^���b�N�̂̐ݒ�
	void LFont::setItalic( bool flag_ )
    {
        _impl->setItalic( flag_ );
    }

    // �C�^���b�N�̂̔���
	bool LFont::isItalic()
    {
        return _impl->isItalic();
    }

    // �A���`�G�C���A�X�̗L���ݒ�
	void LFont::setAntiAlias( bool flag_ )
    {
        _impl->setAntiAlias( flag_ );
    }

    // �A���`�G�C���A�X�̗L������
	bool LFont::isAntiAlias()
    {
        return _impl->isAntiAlias();
    }

    // �������`�悵���Ƃ��̃T�C�Y (�s�N�Z���P��) �̎擾
    void LFont::getTextSize( const lnChar* text_, int len_, LRect* rect_ )
    {
        _impl->getTextSize( text_, len_, rect_ );
    }




//=============================================================================
// �� LDebugFont �N���X
//=============================================================================

#if 0
    /// �C���X�^���X�̎擾
    LDebugFont LDebugFont::getInstance()
    {
        LDebugFont o;
        Graphics::Manager* manager = Framework::getInstance()->getGraphicsManager();
        if ( manager )
        {
            o._impl = manager->getDebugFont();
        }
        return o;
    }

    // �\���ʒu�̐ݒ�
    void LDebugFont::setPosition( int x_, int y_ )
    {
        _impl->setPosition( x_, y_ );
    }

    // �����F�̐ݒ�
    void LDebugFont::setColor( const LColor& color_ )
    {
        _impl->setColor( color_ );
    }

    // ��������o�͂���
	void LDebugFont::printf( const char* format_, ... )
    {
        char buf[ Graphics::DebugFont::TEMP_BUFFER_SIZE ];
        va_list args;
		va_start( args, format_ );
		vsprintf_s( buf, Graphics::DebugFont::TEMP_BUFFER_SIZE, format_, args );
		va_end( args );

        (*_impl) << buf;
    }

    // printf �̊֐��I�u�W�F�N�g�`��
	void LDebugFont::operator()( const char* format_, ... )
    {
        char buf[ Graphics::DebugFont::TEMP_BUFFER_SIZE ];
        va_list args;
		va_start( args, format_ );
		vsprintf_s( buf, Graphics::DebugFont::TEMP_BUFFER_SIZE, format_, args );
		va_end( args );

        (*_impl) << buf;
    }

    LDebugFont& LDebugFont::operator << ( const char* c_ )
    {
        (*_impl) << c_;
        return *this;
    }

	LDebugFont& LDebugFont::operator << ( unsigned char v_ )
    {
        (*_impl) << v_;
        return *this;
    }

	LDebugFont& LDebugFont::operator << ( short v_ )
    {
        (*_impl) << v_;
        return *this;
    }

	LDebugFont& LDebugFont::operator << ( unsigned short v_ )
    {
        (*_impl) << v_;
        return *this;
    }

	LDebugFont& LDebugFont::operator << ( int v_ )
    {
        (*_impl) << v_;
        return *this;
    }

	LDebugFont& LDebugFont::operator << ( unsigned int v_ )
    {
        (*_impl) << v_;
        return *this;
    }

	LDebugFont& LDebugFont::operator << ( float v_ )
    {
        (*_impl) << v_;
        return *this;
    }

	LDebugFont& LDebugFont::operator << ( double v_ )
    {
        (*_impl) << v_;
        return *this;
    }

	LDebugFont& LDebugFont::operator << ( bool v_ )
    {
        (*_impl) << v_;
        return *this;
    }

	LDebugFont& LDebugFont::operator << ( void* p_ )
    {
        (*_impl) << p_;
        return *this;
    }
#endif

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================