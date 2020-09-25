//=============================================================================
//【 LGraphicsModule 】
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
// ■ LGraphics クラス
//=============================================================================

    // インスタンスの取得
    LGraphics LGraphics::getInstance()
    {
        LGraphics o;
        o._impl = gFramework->getGraphicsManager();
        return o;
    }

    // ゲームスクリーンのサイズを取得する
    LVector2 LGraphics::getScreenSize() const
    {
        const LSize& s = _impl->getGraphicsDevice()->getDefaultCanvas()->getSize();
        LVector2 size((lnfloat)s.w, (lnfloat)s.h);
        return size;
    }

    // 背景色を設定する
	void LGraphics::setBGColor( const LColor& color_ )
    {
		gFramework->getSceneManager()->getDefaultSceneGraph()->getDefaultPane()->setBackgroundColor( color_ );
    }

    // 背景色を設定する
	void LGraphics::setBGColor( lnfloat r_, lnfloat g_, lnfloat b_ )
    {
        setBGColor( LColor( r_, g_, b_, 1.0f ) );
        //_impl->getDefaultCanvas()->setBGColor( LColor( r_, g_, b_, 1.0f ) );
    }

//=============================================================================
// ■ LCamera クラス
//=============================================================================

    // デフォルトカメラの取得
    LCamera LCamera::getDefaultCamera()
    {
        return LCamera( gFramework->getSceneManager()->getDefaultSceneGraph()->getDefaultCamera() );
    }

    // 位置の取得
    const LVector3& LCamera::getPosition() const
    {
        return _impl->getPosition();
    }

	// 位置の設定
    void LCamera::setPosition( const LVector3& position_ )
    {
        _impl->setPosition( position_ );
    }

	// 注視点の取得
    const LVector3& LCamera::getLookAt() const
    {
        return _impl->getLookAt();
    }

	// 注視点の設定
    void LCamera::setLookAt( const LVector3& at_ )
    {
        _impl->setLookAt( at_ );
    }

	// 上方向ベクトルの取得
    const LVector3& LCamera::getUpDirection() const
    {
        return _impl->getUpDirection();
    }

	// 上方向ベクトルの設定
    void LCamera::setUpDirection( const LVector3& up_ )
    {
        _impl->setUpDirection( up_ );
    }

    // Y 方向視野角の取得
    lnfloat LCamera::getFovY() const
    {
        return _impl->getFovY();
    }

	// Y 方向視野角の設定
    void LCamera::setFovY( lnfloat fov_y_ )
    {
        _impl->setFovY( fov_y_ );
    }

    // 最も近いビュープレーン位置の取得
    lnfloat LCamera::getNearClip() const
    {
        return _impl->getNearClip();
    }

	// 最も近いビュープレーン位置の設定
    void LCamera::setNearClip( lnfloat near_ )
    {
        return _impl->setNearClip( near_ );
    }

    // 最も遠いビュープレーン位置の取得
    lnfloat LCamera::getFarClip() const
    {
        return _impl->getFarClip();
    }

	// 最も遠いビュープレーン位置の設定
    void LCamera::setFarClip( lnfloat far_ )
    {
        _impl->setFarClip( far_ );
    }

    // カメラタイプの設定
    void LCamera::setType( LNCameraType type_ )
    {
        _impl->setType( type_ );
    }

    // カメラから見た 3D 座標を 2D 座標に変換する
    void LCamera::convertWorldPosToScreenPos( LVector3* out_, const LVector3& world_pos_ )
    {
        LN_PCALL( _impl->convertWorldPosToScreenPos( out_, world_pos_ ) );
    }

    // ビュー行列の取得
    const LMatrix& LCamera::getViewMatrix()
    {
        return _impl->getViewMatrix();
    }

    // プロジェクション行列の取得
    const LMatrix& LCamera::getProjectionMatrix()
    {
        return _impl->getProjectionMatrix();
    }

    // ビュー行列とプロジェクション行列の積の取得
    const LMatrix& LCamera::getViewProjectionMatrix()
    {
        return _impl->getViewProjectionMatrix();
    }

    // ビュー行列の逆行列の取得
    const LMatrix& LCamera::getViewMatrixInverse()
    {
        return _impl->getViewMatrixInverse();
    }

    // 正射影行列の取得
    //const LMatrix& LCamera::getOrthoMatrix()
    //{
    //    return _impl->getOrthoMatrix();
    //}


//=============================================================================
// ■ LLayer クラス
//=============================================================================

    // 背景レイヤーの取得
    LLayer LLayer::getBGLayer()
    {
        return LLayer( gFramework->getSceneManager()->getDefaultSceneGraph()->getDefaultPane() );
    }

    // 色調の設定
    void LLayer::setTone( const LTone& tone_, double duration_ )
    {
		gFramework->getSceneManager()->getDefaultSceneGraph()->getDefaultPane()->setTone( tone_, duration_ );
    }

    // ブラーの適用
    void LLayer::blur( double duration_, lnfloat power_, lnfloat scale_, const LVector3& center_, bool center_is_3d_ )
    {
        _impl->blur( duration_, power_, scale_, center_, center_is_3d_ );
    }

    // トランジションの準備として現在の画面を固定する
	void LLayer::freeze()
    {
        _impl->freeze();
    }

    // トランジション実行
    void LLayer::transition( double duration_, LTexture mask_texture_, u32 vague_ )
    {
        _impl->transition( duration_, mask_texture_->getInterface(), vague_ );
    }

    // エフェクトが適用中かを判定する
	bool LLayer::isEffect() const
    {
        return _impl->isBlurEffect();
    }


//=============================================================================
// ■ LTexture クラス
//=============================================================================

    // ファイルから作成
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

        // ファイルを開く
        FileIO::InFile* file;
        lr = file_manager->createInFile( &file, filename_ );
        if ( LN_FAILED( lr ) )
        {
            LN_PCALLF( lr, "テクスチャの作成に失敗しました。\n\n%s", filename_ );
            return o;
        }

        // テクスチャを作る
        lr = graph_manager->getGraphicsDevice()->getResourceManager()->createTexture( &o._impl, file, color_key_, 1, LN_FMT_A8R8G8B8, LSharingKey( filename_ ) );
        if ( LN_FAILED( lr ) )
        {
            LN_SAFE_RELEASE( file );
            LN_PCALLF( lr, "テクスチャの作成に失敗しました。\n\n%s", filename_ );
            return o;
        }

        SAFE_RELEASE( file );
#endif
        return o;
    }

    // テクスチャを作成する
    LTexture LTexture::create( u32 width_, u32 height_, u32 clear_color_ )
    {
        LNRESULT lr;
        LTexture o;

        Core::Graphics::Manager*  graph_manager = gFramework->getGraphicsManager();

        // テクスチャを作る
        lr = graph_manager->getGraphicsDevice()->createTexture( &o._impl, width_, height_, 1, LN_FMT_A8R8G8B8 );
        if ( LN_FAILED( lr ) )
        {
            //LN_SETERR_HEADER( "テクスチャの作成に失敗しました。\n\nwidth:%d height:%d", width_, height_  );
            //LN_ERRPROC_S();
            return o;
        }

        o._impl->clear( LColor( clear_color_, graph_manager->getGraphicsAPI() ) );

        return o;
    }

    // サイズの取得
    const LVector2& LTexture::getSize() const
    {
        return _impl->getSize();
    }

    // 実際のサイズの取得
    const LVector2& LTexture::getRealSize() const
    {
        return _impl->getRealSize();
    }

    // テクスチャをクリアする
    void LTexture::clear( const LColor& color_ )
    {
        _impl->clear( color_ );
    }

    // drawText() で使用するフォントの設定     
    void LTexture::setFont( LFont font_ )
    {
        _impl->setFont( font_->getInterface() );
    }

    // 設定されているフォントの取得     
    LFont LTexture::getFont()
    {
        return LFont( _impl->getFont() );
    }

    // 文字列を描画する
    void LTexture::drawText( const lnChar* text_, int x_, int y_, int len_ )
    {
        const LVector2 size = getRealSize();
        LN_PCALL( _impl->drawText( text_, LRect( x_, y_, static_cast< int >( size.x ), static_cast< int >( size.y ) ), LN_TEXTALIGN_LEFT, len_ ) );
    }

    // 文字列を描画する
    void LTexture::drawText( const lnChar* text_, const LRect& rect_, LNTextAlign align_, int len_ )
    {
        LN_PCALL( _impl->drawText( text_, rect_, align_, len_ ) );
    }

    // テクスチャのブロック転送
    void LTexture::blt( int x_, int y_, LTexture src_texture_, const LRect& src_rect_ )
    {
        LN_PCALL( _impl->blt( x_, y_, src_texture_.getInterface(), src_rect_ ) );
    }


//=============================================================================
// ■ LShader クラス
//=============================================================================

    // シェーダプログラムを作成する
    LShader LShader::create( const char* filename_ )
    {
        LShader o;
        gFramework->getSceneManager()->getDefaultSceneGraph()->getSceneShaderManager()->
            createSceneShader( &o._impl, filename_ );
#if 0
        LNRESULT lr;
        LShader o;

        // ファイルを開く
        FileIO::InFile* file;
        LN_PCALL( Framework::getInstance()->getFileIOManager()->createInFile( &file, filename_ ) );

        // ファイルのディレクトリまで移動
        System::moveCurrentDirectory( filename_ );

        // シェーダを作る
        Scene::ISceneShader* shader;
        lr = Framework::getInstance()->getSceneManager()->getDefaultScene()->createSceneShader( &shader, file, LSharingKey( filename_ ) );

        // 元のディレクトリに戻る
        System::returnCurrentDirectory();
        
        // 後始末
        SAFE_RELEASE( file );
        LN_PCALL( lr );

        o._impl = shader;
#endif
        return o;
    }
    

//=============================================================================
// ■ LPanel クラス
//=============================================================================

    // パネルオブジェクトを作成する
    LPanel LPanel::create( lnfloat w_size_, lnfloat h_size_, LNAxisAlignedDir dir_ )//, LNPanelCenter center_, LNPanelDirection dir_ )
    {
        LPanel o;
        Core::Scene::Sprite3D* panel = LN_NEW Core::Scene::Sprite3D( gFramework->getSceneManager()->getDefaultSceneGraph() );
        LN_PCALL( panel->initialize( w_size_, h_size_, dir_ ) );
        o._lnso = o._impl = panel;
        return o;
    }

    // パネルオブジェクトを作成する
    LPanel LPanel::create( lnfloat width_, lnfloat height_, LTexture texture_, LNAxisAlignedDir dir_ )
    {
        LPanel o = create( width_, height_, dir_ );
        o->setTexture( texture_ );
        return o;
    }

    // 大きさの設定
    void LPanel::setSize( const LVector2& size_ )
    {
        _impl->setSize( size_ );
    }

    // 大きさの設定
    void LPanel::setSize( lnfloat width_, lnfloat height_ )
    {
        _impl->setSize( LVector2( width_, height_ ) );
    }

    // 大きさの取得
    const LVector2& LPanel::getSize() const
    {
        return _impl->getSize();
    }

    // テクスチャの設定
    void LPanel::setTexture( LTexture texture_ )
    {
        _impl->setTexture( texture_->getInterface() );
    }

    // テクスチャの取得
    LTexture LPanel::getTexture() const
    {
        return LTexture( _impl->getTexture() );
    }

    // テクスチャの転送矩形の取得
    const LRect& LPanel::getSrcRect() const
    {
        return _impl->getSrcRect();
    }

    // テクスチャの転送矩形の設定
    void LPanel::setSrcRect( const LRect& rect_ )
    {
        _impl->setSrcRect( rect_ );
    }

    // テクスチャの転送矩形の設定
    void LPanel::setSrcRect( int x_, int y_, int width_, int height_ )
    {
        _impl->setSrcRect( LRect( x_, y_, width_, height_ ) );
    }

    // 反転方法の設定
    void LPanel::setFlipFlag( u32 flag_ )
    {
        _impl->setFlipFlag( flag_ );
    }

    // 反転方法の取得
    u32 LPanel::getFlipFlag()
    {
        return _impl->getFlipFlag();
    }


//=============================================================================
// ■ LSprite クラス
//=============================================================================

    // スプライトを作成する
    LSprite LSprite::create()
    {
        LSprite o;
        Core::Scene::Sprite* sprite = LN_NEW Core::Scene::Sprite( gFramework->getSceneManager()->getDefaultSceneGraph() );
        sprite->initialize();
        o._lnso = o._impl = sprite;
        return o;
    }

    // スプライトを作成する
    LSprite LSprite::create( LTexture texture_ )
    {
        LSprite o = create();
        o->setTexture( texture_ );
        return o;
    }

    // テクスチャの設定
    void LSprite::setTexture( LTexture texture_ )
    {
        _impl->setTexture( texture_->getInterface() );
    }

    // テクスチャの取得
    LTexture LSprite::getTexture() const
    {
        return LTexture( _impl->getTexture() );
    }

    // テクスチャの転送矩形の取得
    const LRect& LSprite::getSrcRect() const
    {
        return _impl->getSrcRect();
    }

    // テクスチャの転送矩形の設定
    void LSprite::setSrcRect( const LRect& rect_ )
    {
        _impl->setSrcRect( rect_ );
    }

    // テクスチャの転送矩形の設定
    void LSprite::setSrcRect( int x_, int y_, int width_, int height_ )
    {
        _impl->setSrcRect( LRect( x_, y_, width_, height_ ) );
    }

    // 反転方法の設定
    void LSprite::setFlipFlag( u32 flag_ )
    {
        _impl->setFlipFlag( flag_ );
    }

    // 反転方法の取得
    u32 LSprite::getFlipFlag()
    {
        return _impl->getFlipFlag();
    }

//=============================================================================
// ■ LBatchPanel クラス
//=============================================================================

    // パネルオブジェクトを作成する
    LBatchPanel LBatchPanel::create( lnfloat width_, lnfloat height_, LNAxisAlignedDir front_ )
    {
        LBatchPanel o;
        Core::Scene::BatchSprite3D* panel = LN_NEW Core::Scene::BatchSprite3D( gFramework->getSceneManager()->getDefaultSceneGraph() );
        panel->initialize( width_, height_, front_ );
        o._lnso = o._impl = panel;
        return o;
    }

    // パネルオブジェクトを作成する
    LBatchPanel LBatchPanel::create( lnfloat width_, lnfloat height_, LTexture texture_, LNAxisAlignedDir front_ )
    {
        LBatchPanel o = create( width_, height_, front_ );
        o.setTexture( texture_ );
        return o;
    }

    // 大きさの設定
    void LBatchPanel::setSize( const LVector2& size_ )
    {
        _impl->setSize( size_ );
    }

    // 大きさの設定
    void LBatchPanel::setSize( lnfloat width_, lnfloat height_ )
    {
        _impl->setSize( LVector2( width_, height_ ) );
    }

    // 大きさの取得
    const LVector2& LBatchPanel::getSize() const
    {
        return _impl->getSize();
    }

    // テクスチャの設定
    void LBatchPanel::setTexture( LTexture texture_ )
    {
        _impl->setTexture( texture_->getInterface() );
    }

    // テクスチャの取得
    LTexture LBatchPanel::getTexture() const
    {
        return LTexture( _impl->getTexture() );
    }

    // テクスチャの転送矩形の取得
    const LRect& LBatchPanel::getSrcRect() const
    {
        return _impl->getSrcRect();
    }

    // テクスチャの転送矩形の設定
    void LBatchPanel::setSrcRect( const LRect& rect_ )
    {
        _impl->setSrcRect( rect_ );
    }

    // テクスチャの転送矩形の設定
    void LBatchPanel::setSrcRect( int x_, int y_, int width_, int height_ )
    {
        _impl->setSrcRect( LRect( x_, y_, width_, height_ ) );
    }

    // 反転方法の設定
    void LBatchPanel::setFlipFlag( u32 flag_ )
    {
        _impl->setFlipFlag( (lnU8)flag_ );
    }

    // 反転方法の取得
    u32 LBatchPanel::getFlipFlag()
    {
        return _impl->getFlipFlag();
    }


//=============================================================================
// ■ LParticle クラス
//=============================================================================

    // パーティクルオブジェクトを作成する
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


    // テクスチャの設定
    void LParticle::setTexture( LTexture texture_ )
    {
        _impl->setTexture( texture_.getInterface() );
    }

    // テクスチャの取得
    //LTexture LParticle::getTexture() const
    //{
    //    return LTexture( _impl->getTexture() );
    //}

  

//=============================================================================
// ■ LFont クラス
//=============================================================================

    // デフォルトの設定でフォントを作成する
    LFont LFont::createDefaultFont()
    {
        LFont o;
        LN_PCALL( gFramework->getGraphicsManager()->createFont( &o._impl, NULL ) );
        return o;
    }

    // デフォルトのフォントを取得する
    LFont LFont::getDefaultFont()
    {
        return LFont( gFramework->getGraphicsManager()->getDefaultFont() );
    }

    // フォントサイズの設定
	void LFont::setSize( int size_ )
    {
        _impl->setSize( size_ );
    }

    // フォントサイズの取得
	int LFont::getSize()
    {
        return _impl->getSize();
    }

    // フォントカラーの設定
	void LFont::setColor( const LColor& color_ )
    {
        _impl->setColor( color_ );
    }

    // フォントカラーの設定
	void LFont::setColor( lnfloat r_, lnfloat g_, lnfloat b_, lnfloat a_ )
    {
        _impl->setColor( LColor( r_, g_, b_, a_ ) );
    }

    // フォントカラーの取得
	const LColor& LFont::getColor()
    {
        return _impl->getColor();
    }

    // エッジの幅の設定
	void LFont::setEdgeSize( int size_ )
    {
        _impl->setEdgeSize( size_ );
    }

    // エッジの幅の取得
	int LFont::getEdgeSize()
    {
        return _impl->getEdgeSize();
    }

    // エッジカラーの設定
	void LFont::setEdgeColor( lnfloat r_, lnfloat g_, lnfloat b_, lnfloat a_ )
    {
        _impl->setEdgeColor( LColor( r_, g_, b_, a_ ) );
    }

    // エッジカラーの設定
	void LFont::setEdgeColor( const LColor& color_ )
    {
        _impl->setEdgeColor( color_ );
    }

    // エッジカラーの取得
	const LColor& LFont::getEdgeColor()
    {
        return _impl->getEdgeColor();
    }

    // 太文字の設定
	void LFont::setBold( bool flag_ )
    {
        _impl->setBold( flag_ );
    }

    // 太文字の判定
	bool LFont::isBold()
    {
        return _impl->isBold();
    }

    // イタリック体の設定
	void LFont::setItalic( bool flag_ )
    {
        _impl->setItalic( flag_ );
    }

    // イタリック体の判定
	bool LFont::isItalic()
    {
        return _impl->isItalic();
    }

    // アンチエイリアスの有効設定
	void LFont::setAntiAlias( bool flag_ )
    {
        _impl->setAntiAlias( flag_ );
    }

    // アンチエイリアスの有効判定
	bool LFont::isAntiAlias()
    {
        return _impl->isAntiAlias();
    }

    // 文字列を描画したときのサイズ (ピクセル単位) の取得
    void LFont::getTextSize( const lnChar* text_, int len_, LRect* rect_ )
    {
        _impl->getTextSize( text_, len_, rect_ );
    }




//=============================================================================
// ■ LDebugFont クラス
//=============================================================================

#if 0
    /// インスタンスの取得
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

    // 表示位置の設定
    void LDebugFont::setPosition( int x_, int y_ )
    {
        _impl->setPosition( x_, y_ );
    }

    // 文字色の設定
    void LDebugFont::setColor( const LColor& color_ )
    {
        _impl->setColor( color_ );
    }

    // 文字列を出力する
	void LDebugFont::printf( const char* format_, ... )
    {
        char buf[ Graphics::DebugFont::TEMP_BUFFER_SIZE ];
        va_list args;
		va_start( args, format_ );
		vsprintf_s( buf, Graphics::DebugFont::TEMP_BUFFER_SIZE, format_, args );
		va_end( args );

        (*_impl) << buf;
    }

    // printf の関数オブジェクト形式
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