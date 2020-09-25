//=============================================================================
//【 SceneShaderManager 】
//=============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../SceneGraph.h"
#include "../SceneNode/Light.h"
#include "SceneShader.h"
#include "SceneShaderTechnique.h"
#include "OffscreenScene.h"
#include "SceneShaderManager.h"

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
// ■ SceneShaderManager クラス
//=============================================================================

    struct LNVerNameDesc
    {
        const lnChar* Semantic;
        const lnChar* Variable;   ///< シェーダ固定時に使う変数名
    };

    static LNVerNameDesc gMMESemanticNames[ MME_MAX_SEMANTICS ] =
    {
        { _T( "NONE" ), NULL },  // ダミー
        { _T( "WORLD" ),                                _T( "gMatrix_w" ) },
        { _T( "VIEW" ),                                 _T( "gMatrix_v" ) },
        { _T( "PROJECTION" ),                           _T( "gMatrix_p" ) },
        { _T( "WORLDVIEW" ),                            _T( "gMatrix_wv" ) },
        { _T( "VIEWPROJECTION" ),                       _T( "gMatrix_vp" ) },
        { _T( "WORLDVIEWPROJECTION" ),                  _T( "gMatrix_wvp" ) },      // [ 6 ]

        { _T( "WORLDINVERSE" ),                         _T( "gMatrix_wi" ) },
        { _T( "VIEWINVERSE" ),                          _T( "gMatrix_vi" ) },
        { _T( "PROJECTIONINVERSE" ),                    _T( "gMatrix_pi" ) },
        { _T( "WORLDVIEWINVERSE" ),                     _T( "gMatrix_wvi" ) },
        { _T( "VIEWPROJECTIONINVERSE" ),                _T( "gMatrix_vpi" ) },
        { _T( "WORLDVIEWPROJECTIONINVERSE" ),           _T( "gMatrix_wvpi" ) },     // [ 12 ]

        { _T( "WORLDTRANSPOSE" ),                       _T( "gMatrix_wt" ) },
        { _T( "VIEWTRANSPOSE" ),                        _T( "gMatrix_vt" ) },
        { _T( "PROJECTIONTRANSPOSE" ),                  _T( "gMatrix_pt" ) },
        { _T( "WORLDVIEWTRANSPOSE" ),                   _T( "gMatrix_wvt" ) },
        { _T( "VIEWPROJECTIONTRANSPOSE" ),              _T( "gMatrix_vpt" ) },
        { _T( "WORLDVIEWPROJECTIONTRANSPOSE" ),         _T( "gMatrix_wvpt" ) },     // [ 18 ]
         
        { _T( "WORLDINVERSETRANSPOSE" ),                _T( "gMatrix_wit" ) },
        { _T( "VIEWINVERSETRANSPOSE" ),                 _T( "gMatrix_vit" ) },
        { _T( "PROJECTIONINVERSETRANSPOSE" ),           _T( "gMatrix_pit" ) },
        { _T( "WORLDVIEWINVERSETRANSPOSE" ),            _T( "gMatrix_wvit" ) },
        { _T( "VIEWPROJECTIONINVERSETRANSPOSE" ),       _T( "gMatrix_vpit" ) },
        { _T( "WORLDVIEWPROJECTIONINVERSETRANSPOSE" ),  _T( "gMatrix_wvpit" ) },    // [ 24 ]

        { _T( "DIFFUSE" ),                              _T( "gDiffuse" ) },
        { _T( "AMBIENT" ),                              _T( "gAmbient" ) },
        { _T( "EMISSIVE" ),                             _T( "gEmissive" ) },
        { _T( "SPECULAR" ),                             _T( "gSpecular" ) },
        { _T( "SPECULARPOWER" ),                        _T( "gSpecularPower" ) },
        { _T( "TOONCOLOR" ),                            _T( "gToonColor" ) },
        { _T( "EDGECOLOR" ),                            _T( "gEdgeColor" ) },
        { _T( "POSITION" ),                             _T( "gLightPosition" ) },
        { _T( "DIRECTION" ),                            _T( "gLightDirection" ) },
        { _T( "MATERIALTEXTURE" ),                      _T( "gMaterialTexture" ) },
        { _T( "MATERIALSPHEREMAP" ),        NULL },
        { _T( "VIEWPORTPIXELSIZE" ),        NULL },
        { _T( "TIME" ),                     NULL },
        { _T( "ELAPSEDTIME" ),              NULL },
        { _T( "MOUSEPOSITION" ),            NULL },
        { _T( "LEFTMOUSEDOWN" ),            NULL },
        { _T( "MIDDLEMOUSEDOWN" ),          NULL },
        { _T( "RIGHTMOUSEDOWN" ),           NULL },
        { _T( "CONTROLOBJECT" ),            NULL },
        { _T( "RENDERCOLORTARGET" ),        NULL },
        { _T( "RENDERDEPTHSTENCILTARGET" ), NULL },
        { _T( "ANIMATEDTEXTURE" ),          NULL },
        { _T( "OFFSCREENRENDERTARGET" ),    NULL },
        { _T( "TEXTUREVALUE" ),             NULL },
        { _T( "STANDARDSGLOBAL" ),          NULL },

        // 以下は独自仕様
        //{ _T( "ORTHOGRAPHIC" ),    _T( "gMatrix_wt" ) },
        { _T( "PROJECTION2D" ),     _T( "gMatrix_p2D" ) },
        { _T( "OPACITY" ),          _T( "gOpacity" ) },
        { _T( "COLORSCALE" ),       _T( "gColorScale" ) },
        { _T( "BLENDCOLOR" ),       _T( "gBlendColor" ) },
        { _T( "TONE" ),             _T( "gTone" ) },
        { _T( "UVTRANSFORM" ),      _T( "gUVTransform" ) }
    };

    static const char* gMMEAnnotationNames[ MME_MAX_ANNOTATIONS ] =
    {
        "NONE",
        "OBJECT",
        "SYNCINEDITMODE",

        "NAME",
        "ITEM",

        "RESOURCETYPE",
        "RESOURCENAME",
        "WIDTH",
        "HEIGHT",
        "DEPTH",
        "DIMENSIONS",
        "VIEWPORTRATIO",
        "FORMAT",
        "MIPLEVELS",
        "LEVELS",

        "OFFSET",
        "SPEED",
        "SEEKVARIABLE",

        "CLEARCOLOR",
        "CLEARDEPTH",
        "ANTIALIAS",
        "DESCRIPTION",
        "DEFAULTEFFECT",

        "TEXTURENAME",

        "SCRIPTOUUTPUT",
        "SCRIPTCLASS",
        "SCRIPTORDER",
        
        "SCRIPT"
    };

    //---------------------------------------------------------------------
    // ● コンストラクタ
    //---------------------------------------------------------------------
    SceneShaderManager::SceneShaderManager( SceneGraph* scene_ )
        : mLogFile          ( NULL )
        , mScene            ( scene_ )
        , mFileManager      ( NULL )
        , mGraphicsDevice   ( NULL )
        , mPlayStartTime    ( 0.0f )
    {
        //LN_SAFE_ADDREF( mScene );
    }

    //---------------------------------------------------------------------
    // ● デストラクタ
    //---------------------------------------------------------------------
    SceneShaderManager::~SceneShaderManager()
    {
        //LN_SAFE_RELEASE( mScene );
    }

    //---------------------------------------------------------------------
    // ● 初期化
    //---------------------------------------------------------------------
    LNRESULT SceneShaderManager::initialize( const InitData& init_data_ )
    {
        mFileManager     = init_data_.FileManager;
        mGraphicsManager = init_data_.GraphicsManager;
        mGraphicsDevice  = init_data_.GraphicsManager->getGraphicsDevice();
        mPlayStartTime   = 0.0f;

        // シェーダキャッシュ作成
        SceneShaderCache::initialize( init_data_.ShaderCacheSize );

        // セマンティクス名と識別番号の対応表を作る
        // ※対応表は変数検索の際、総当たりを避けるためのもの
        for ( int i = 0; i < MME_MAX_SEMANTICS; ++i )
        {
            // セマンティクス名を使う
            if ( init_data_.UseSemantics )
            {
                mMMESemanticsMap.insert( MMESemanticsPair( gMMESemanticNames[ i ].Semantic, (MMESemantics)i ) );
            }
            // 内部で定義された変数名を使う
            else
            {
                if ( gMMESemanticNames[ i ].Variable )
                {
                    mMMESemanticsMap.insert( MMESemanticsPair( gMMESemanticNames[ i ].Variable, (MMESemantics)i ) );
                }
            }
        }
        // アノテーション名と識別番号の対応表を作る
        for ( int i = 0; i < MME_MAX_ANNOTATIONS; ++i )
        {
            mMMEAnnotationMap.insert( MMEAnnotationPair( gMMEAnnotationNames[ i ], (MMEAnnotation)i ) );
        }



        printf("MMEShaderVariable::ControlObject の設定を忘れそう！注意\n");
        

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 終了処理
    //---------------------------------------------------------------------
    void SceneShaderManager::finalize()
    {
        ln_foreach( SceneShader* shader, mSceneShaderList )
        {
            shader->release();
        }
        mSceneShaderList.clear();

        SceneShaderCache::finalize();
    }

    //---------------------------------------------------------------------
    // ● シェーダの作成
    //---------------------------------------------------------------------
    //LNRESULT SceneShaderManager::createSceneShader( SceneShader** shader_, const void* data_, u32 size_, const lnChar* name_ )
    //{
    //    return _createSceneShader( shader_, data_, size_, name_ );
    //}

    //---------------------------------------------------------------------
    // ● シェーダの作成 (ファイル名指定)
    //---------------------------------------------------------------------
    LNRESULT SceneShaderManager::createSceneShader( SceneShader** shader_, const lnChar* filename_ )
    {
        // moveCurrentPath() を使用するためにロック
        mFileManager->lock();
  
        //LNRESULT lr;
        *shader_ = NULL;

        File::IInFile* file;
        mFileManager->createInFile( &file, filename_ );

        

        _createSceneShader( shader_, file, filename_ );
        
        LN_SAFE_RELEASE( file );




        mFileManager->moveCurrentPath( File::getDirectoryPath( filename_ ).c_str() );
        

        // 読み込み・ファイル処理が終わった後、必要なリソースを確保する
        if ( (*shader_) )
        {
			(*shader_)->refreshResource( mScene->getDefaultPane()->getViewSize() );
        }

        // 元のパスに戻す
        mFileManager->returnCurrentPath();
        mFileManager->unlock();

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● 解放が必要なシェーダがないかチェックする
    //---------------------------------------------------------------------
    void SceneShaderManager::updateShaderList()
    {
        SceneShaderList::iterator itr = mSceneShaderList.begin();
        SceneShaderList::iterator end = mSceneShaderList.end();
        while ( itr != end )
        {
            if ( (*itr)->getRefCount() == 1 )
            {
                (*itr)->release();
                itr = mSceneShaderList.erase( itr );
				end = mSceneShaderList.end();
                continue;
            }
            ++itr;
        }
    }

    //---------------------------------------------------------------------
    // ● 全シェーダにシーン単位で設定するべきデータを設定する
    //---------------------------------------------------------------------
    void SceneShaderManager::updateAllShaderGlobalParam( const Game::GameTime& game_time_ )
    {
        // とりあえずリアルタイムじゃなくてゲームタイムで計算
        mMMESceneParam.Time         = static_cast< lnfloat >( game_time_.getTotalGameTime() - mPlayStartTime );
        mMMESceneParam.ElapsedTime  = game_time_.getElapsedGameTime();

        //-----------------------------------------------------
        // Scene 単位でシェーダに設定する必要がるデータをまとめる (ここではデフォルトライト)
        // この辺ローカル変数なので描画コマンド実行はこの関数の中で行うこと。

        Light* light = mScene->getDefaultLight();

        mMMESceneParam.Mat_LightView     = &light->getViewMatrix();   
        mMMESceneParam.Mat_LightProj     = &light->getProjectionMatrix();
        mMMESceneParam.Mat_LightViewProj = &light->getViewProjectionMatrix();

        mMMESceneParam.LightPosition     = LVector4( light->getPosition() );
        mMMESceneParam.LightDirection    = LVector4( light->getDirection() );

        mMMESceneParam.LightDiffuse      = (LVector4*)&light->getDiffuseColor();
        mMMESceneParam.LightAmbient      = (LVector4*)&light->getAmbientColor();
        mMMESceneParam.LightSpecular     = (LVector4*)&light->getSpecularColor();

        mMMESceneParam.ViewPixelSize.x   = mScene->getDefaultPane()->getViewSize().x;
        mMMESceneParam.ViewPixelSize.y   = mScene->getDefaultPane()->getViewSize().y;


        mMMESceneParam.calcMatrices();

        // すべてのシェーダにシーン単位の設定項目を送り、
        // ついでに CONTROLOBJECT を要求している変数の値を更新する
        SceneShaderList::iterator itr = mSceneShaderList.begin();
        SceneShaderList::iterator end = mSceneShaderList.end();
        for ( ; itr != end; ++itr )
        {
            (*itr)->updateSceneParam( mMMESceneParam );
            (*itr)->updateControlObjectVariables();
        }
    }

    //---------------------------------------------------------------------
    // ● 全ェーダにカメラ単位で設定するべきデータを設定する
    //---------------------------------------------------------------------
    void SceneShaderManager::updateAllShaderCameraParam( const LNCameraSceneParam& param_ )
    {
        SceneShaderList::iterator itr = mSceneShaderList.begin();
        SceneShaderList::iterator end = mSceneShaderList.end();
        for ( ; itr != end; ++itr )
        {
            (*itr)->updateCameraParam( param_ );
        }
    }

    //---------------------------------------------------------------------
    // ● すべてのオフスクリーンRTの描画コマンドを作成する
    //---------------------------------------------------------------------
    void SceneShaderManager::makeDrawCommand( DrawCommandContainer* container_, MMDPass pass_ )
    {
        ln_foreach( OffscreenScene* ofs, mOffscreenSceneList )
        {
            ofs->makeDrawCommand( container_, pass_ );
        }
    }

    //---------------------------------------------------------------------
    // ● (SceneGraph::addNode() から呼ばれる)
    //---------------------------------------------------------------------
    void SceneShaderManager::addSceneNodeToOffscreenScene( SceneNode* node_ )
    {
        ln_foreach( OffscreenScene* ofs, mOffscreenSceneList )
        {
            ofs->addSceneNode( node_ );
        }
    }

    //---------------------------------------------------------------------
    // ● (SceneGraph::onDeleteNode() から呼ばれる)
    //---------------------------------------------------------------------
    void SceneShaderManager::removeSceneNodeFromOffscreenScene( SceneNode* node_ )
    {
        ln_foreach( OffscreenScene* ofs, mOffscreenSceneList )
        {
            ofs->removeSceneNode( node_ );
        }
    }

    //---------------------------------------------------------------------
    // ● (SceneShader 内から呼ばれる)
    //---------------------------------------------------------------------
    LNRESULT SceneShaderManager::createTexture(
        Graphics::ITexture** texture_,
        MMEVariableRequest req_,
        const lnChar* resource_name_,
        lnU32 width_,
        lnU32 height_,
        lnU32 depth_,
        lnU32 miplevels_,
        LNSurfaceFormat format_)
    {
        LNRESULT lr;

        LN_THROW_InvalidCall( depth_ <= 1, "3D テクスチャは未対応です。" );


        switch ( req_ )
        {
            /////////////////////////////////////// 普通テクスチャ
            case MME_VARREQ_TEXTURE:
            {
                // テクスチャ名がある場合はそれを読む
                if ( resource_name_ )
                {
                    mGraphicsDevice->createTexture( texture_, resource_name_, 0, miplevels_, format_, LSharingKey( resource_name_ ) );
                }
                // ファイル名がない場合はサイズ指定
                else
                {
                    LN_CALL_R( mGraphicsDevice->createTexture( texture_, width_, height_, miplevels_, format_ ) );
                }
                break;
            }
            /////////////////////////////////////// レンダーターゲット
            case MME_VARREQ_RENDERCOLORTARGET:
            {
                LN_CALL_R( mGraphicsDevice->createRenderTarget( texture_, width_, height_, miplevels_, format_ ) );
                break;
            }
            /////////////////////////////////////// 深度バッファ
            case MME_VARREQ_RENDERDEPTHSTENCILTARGET:
            {
                // アノテーションにフォーマットの指定がない場合は D24S8
                if ( format_ == LN_FMT_A8R8G8B8 )
                {
                    format_ = LN_FMT_D24S8;
                }
                LN_CALL_R( mGraphicsDevice->createDepthBuffer( texture_, width_, height_, format_ ) );
                break;
            }
            /////////////////////////////////////// アニメテクスチャ
            case MME_VARREQ_ANIMATEDTEXTURE:
            {
                printf( "ANIMATEDTEXTURE の読み込みは未対応です。\n" );
                break;
            }
            /////////////////////////////////////// オフスクリーン RT
            case MME_VARREQ_OFFSCREENRENDERTARGET:
            {
                LN_CALL_R( mGraphicsDevice->createRenderTarget( texture_, width_, height_, miplevels_, format_ ) );
                break;
            }
        }
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● メッセージ処理 (Manager から呼ばれる)
    //---------------------------------------------------------------------
    LNRESULT SceneShaderManager::onEvent( const System::Event& args_ )
    {
        const System::MouseEvent& ev_mouse = (const System::MouseEvent&)(args_);

        // シェーダに送るマウスデータ
        switch ( args_.Type )
        {
            case LNEV_MOUSE_DOWN:
            {
                switch ( ev_mouse.Button )
                {
                    case LN_MOUSE_LEFT:
                        mMMESceneParam.LeftMouseDown.z = 0.0f;
                        break;
                    case LN_MOUSE_RIGHT:
                        mMMESceneParam.RightMouseDown.z = 0.0f;
                        break;
                    case LN_MOUSE_MIDDLE:
                        mMMESceneParam.MiddleMouseDown.z = 0.0f;
                        break;
                }
                break;
            }
            case LNEV_MOUSE_UP:
            {
                switch ( ev_mouse.Button )
                {
                    case LN_MOUSE_LEFT:
                        mMMESceneParam.LeftMouseDown.set(
                            static_cast< lnfloat >( ev_mouse.X ),
                            static_cast< lnfloat >( ev_mouse.Y ),
                            1.0f,
                            mMMESceneParam.Time );
                        break;
                    case LN_MOUSE_RIGHT:
                        mMMESceneParam.RightMouseDown.set(
                            static_cast< lnfloat >( ev_mouse.X ),
                            static_cast< lnfloat >( ev_mouse.Y ),
                            1.0f,
                            mMMESceneParam.Time );
                        break;
                    case LN_MOUSE_MIDDLE:
                        mMMESceneParam.MiddleMouseDown.set(
                            static_cast< lnfloat >( ev_mouse.X ),
                            static_cast< lnfloat >( ev_mouse.Y ),
                            1.0f,
                            mMMESceneParam.Time );
                        break;
                }
                break;
            }
            case LNEV_MOUSE_MOVE:
            {
                mMMESceneParam.MousePosition.set(
                    static_cast< lnfloat >( ev_mouse.X ),
                    static_cast< lnfloat >( ev_mouse.Y ),
                    0, 0 );
                break;
            }
        }

        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● シェーダの作成
    //---------------------------------------------------------------------
    LNRESULT SceneShaderManager::_createSceneShader( SceneShader** shader_, const void* data_, u32 size_, const lnChar* name_ )
    {
        LNRESULT lr;
        LSharingKey key = LSharingKey( name_ );
        *shader_ = NULL;

        // キャッシュを検索
        *shader_ = SceneShaderCache::findCacheAddRef( key );
        if ( *shader_ )
        {
            return LN_OK;
        }

        // 新しく作る
        SceneShader::InitData data;
        data.ShaderData         = data_;
        data.ShaderDataSize     = size_;
        data.ShaderName         = name_;
        SceneShader* shader = LN_NEW SceneShader( this );
        LN_CALL_R( shader->initialize( data ) );

        if ( shader->isShareable() )
        {
            SceneShaderCache::registerCachedObject( key, shader );
        }

        mSceneShaderList.push_back( shader );
        shader->addRef();

        *shader_ = shader;
        return LN_OK;
    }

    //---------------------------------------------------------------------
    // ● シェーダの作成 (入力ストリームから)
    //---------------------------------------------------------------------
    LNRESULT SceneShaderManager::_createSceneShader( SceneShader** shader_, File::IInStream* stream_, const lnChar* name_ )
    {
        u32 size = stream_->getSize();

        lnByte* buf = mTempBuffer.lock( size );

        // mTempBuffer に全部読み込む
        stream_->read( buf, size );

        LNRESULT lr = _createSceneShader( shader_, buf, size, name_ );

        mTempBuffer.unlock();
        return lr;
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