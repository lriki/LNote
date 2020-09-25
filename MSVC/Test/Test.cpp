//=============================================================================
// ■ Test
//=============================================================================

#include "stdafx.h"
#include "TestFW.h"

#define TEST03_GUI
//#define TEST02_GameScene
//#define TEST01_Model
//---------------------------------------------------------------------
#ifdef TEST03_GUI
#define EXEC_CLASS Test03
class Test03
    : public TestFW
{
public:

 

    class TestSene
        : public Game::GameScene
    {
    public:

        

    public:

        /// シーン開始
        virtual void onStart()
        {
 
            Game::GameScene::onStart();



            

        }

        /// フレーム更新
        virtual void onUpdate()
        {
            Game::GameScene::onUpdate();
        }
    };
    

public:

    void main()
    {
#if 0
        LRefPtr<GUI::GUIButton> button1;

        GUI::Manager* gui_manager = fw->getGUIManager();

        GUI::GUIDesktop* desktop = gui_manager->getDesktop();

        //LRefPtr<GUI::GUITextBlock> text_block( LN_NEW GUI::GUITextBlock( gui_manager ) );
        ////GUI::GUITextBlock* text_block = LN_NEW GUI::GUITextBlock( gui_manager );
        //text_block->initialize();
        //text_block->setText( _T("lnote") );
        ////desktop->addControl( text_block );

        //LRefPtr<GUI::GUIButton> button1( LN_NEW GUI::GUIButton( gui_manager ) );
        //GUI::GUIButton* button1 = LN_NEW GUI::GUIButton( gui_manager );
        button1.attach( LN_NEW GUI::GUIButton( gui_manager ) );
        button1->initialize();
        desktop->addControl( button1 );
        

        //LN_SAFE_RELEASE( button1 );
        //LN_SAFE_RELEASE(text_block);

#endif

        fw->getGameManager()->run( LN_NEW TestSene() );
    }
};
#endif
//---------------------------------------------------------------------
#ifdef TEST02_GameScene
#define EXEC_CLASS Test02
class Test02
    : public TestFW
{
public:

    class Player
        : public Game::GameObject
    {
    public:

        /// フレーム更新
        virtual void onUpdate()
        {
            printf("void onUpdate()\n");
        }
    };

    class TestSene
        : public Game::GameScene
    {
    public:



    public:

        /// シーン開始
        virtual void onStart()
        {
            Game::GameScene::onStart();


            

            Game::GameObject* obj = LN_NEW Player();
            Game::ModelComponent* model = LN_NEW Game::ModelComponent( mManager );
            model->setFileName( "../../../../MMD/モデル/Lat式ミクVer2.3/Lat式ミクVer2.3_White.pmd" );

            obj->addComponent( model );
            model->release();


            this->addGameObject( obj );
            obj->release();

        }

        /// フレーム更新
        virtual void onUpdate()
        {
            Game::GameScene::onUpdate();
        }
    };
    

public:

    void main()
    {
        fw->getGameManager()->run( LN_NEW TestSene() );
    }
};
#endif
//---------------------------------------------------------------------
#ifdef TEST01_Model

class Test01_Model
    : public TestFW
{
public:

    

public:

    Base::Delegate01<int>  de;
    Base::Delegate08<int, int, int, int, int, int, int, int> de8;

    void Func(int a) {}

    void Func8(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8){ printf("%d\n", a1 + a2);}

    void main()
    {
        
        de = Base::createDelegate(this, &Test01_Model::Func);
        de8 = Base::createDelegate(this, &Test01_Model::Func8);
        de8.call(1, 2, 3, 4, 5, 6, 7, 8);

        //LRefPtr< Scene::Model > model1( LN_NEW Scene::Model( scene ) );
        //model1->initialize( _T( "../../../../MMD/モデル/Lat式ミクVer2.3/Lat式ミクVer2.3_White.pmd" ) );

        //LRefPtr< Scene::Model > model2( LN_NEW Scene::Model( scene ) );
        //model2->initialize( _T( "../../../../MMD/モデル/AliceV033/アリスV033.pmd" ) );

        LRefPtr< Scene::Model > map_model1( LN_NEW Scene::Model( scene ) );
        map_model1->initialize( _T( "../../../../MMD/ステージ/anomarokarintou/packaged用ステージ2・泉のステージ/P・S_2_泉のステージ.x" ), LN_MODELFLAG_COLLISION );
        //map_model1->initialize( _T( "../../../../MMD/エフェクト/lobelia/GodRay_old/森アクセサリ/wood.x" ) );
        //map_model1->initialize( _T( "../../TestMat/MapMesh_1.x" ), LN_MODELFLAG_COLLISION );
        
        std::vector
        LRefPtr< Physics::Box > box1;
        box1.attach( fw->getPhysicsManager()->createBox( LVector3( 2, 2, 2 ), LVector3( 0, 30, 0 ), 2.0f ) );

        const Graphics::CollisionMesh& col_mesh = map_model1->getModel()->getCollisionMesh();
        LNTriangleMeshBodyInfo mesh1_info;
        mesh1_info.VertexCount = col_mesh.VertexCount;
        mesh1_info.VertexTable = col_mesh.CollisionVertexBuffer;
        mesh1_info.IndexCount = col_mesh.IndexCount;
        mesh1_info.IndexStride = col_mesh.IndexStride;
        mesh1_info.IndexTable = col_mesh.CollisionIndexBuffer;

        LRefPtr< Physics::TriMesh > mesh1;
        mesh1.attach( fw->getPhysicsManager()->createStaticTriangleMesh( mesh1_info ) );
        mesh1->setRestitution( 2.0f );
        mesh1->setFriction( 1.0f );


#if 1
        const int box_num = 1000;
        LRefPtr< Physics::Box > boxes[box_num];
        for ( int x = 0 ; x < 10; ++x )
            for ( int y = 0 ; y < 10; ++y )
                for ( int z = 0 ; z < 10; ++z )
        {
            boxes[x * 100 + y * 10 + z].attach(
                fw->getPhysicsManager()->createBox(
                    LVector3( 1, 1, 1 ),
                    LVector3( x - 5, 30 + y, z - 5 ), 1.0f ) );

        }
#endif

        do
        {

#if 1
            LVector3 pos, normal;

            if (fw->getPhysicsManager()->rayCast(
                camera->getPosition(),
                camera->getDirection()*1000,
                &pos, &normal, NULL ))
            {
                Graphics::DebugText& dout = *fw->getGraphicsManager()->getDebugText();
                dout.printf(
                    //"camera(%f %f %f)\n(%f %f %f)\npos (%f %f %f)\nnor (%f %f %f)\n",
                    //camera->getPosition().x, camera->getPosition().y, camera->getPosition().z,
                    //camera->getDirection().x, camera->getDirection().y, camera->getDirection().z,
                    "pos (%f %f %f)\nnor (%f %f %f)\n",
                    pos.x, pos.y, pos.z,
                    normal.x, normal.y, normal.z);
            }
#endif

        } while ( fw->update() );
    }
};
#define EXEC_CLASS Test01_Model

#endif
//---------------------------------------------------------------------
void main()
{
    Base::ExceptionManager em;
    TestFW* f = LN_NEW EXEC_CLASS();
    try
    {
        f->onInit();
        f->main();
    }
    catch ( Base::Exception& e )
    {
        em.procException( e );
    }
    f->onExit();
    SAFE_DELETE( f );
}
//---------------------------------------------------------------------













//#ifdef STATIC_TEST

//#pragma comment(lib, "user32.lib")
//#pragma comment(lib, "gdi32.lib")
//#pragma comment(lib, "vfw32.lib")
//#pragma comment(lib, "glew32d.lib")
//#pragma comment( lib, "glew32s.lib" )
//#pragma comment( lib, "D:/Programing/OpenGL/glew/glew-1.9.0/lib/glew32s.lib" )
//#pragma comment( lib, "D:/Programing/OpenGL/glew/glew-1.9.0/lib/glew32mxs.lib" )
//

//#define TEST22_Effect_Ring
//#define TEST21_Effect
//#define TEST20_GUI
//#define TEST19_UtilityLib
//#define TEST18_Tilemap
//#define TEST17_Particle
//#define TEST16_Scene
//#define TEST15_DeviceReset
//#define TEST14_Effect
//#define TEST13_Input
//#define TEST12_GeometryRenderer
//#define TEST11_Scene
//#define TEST10_Render
//#define TEST9_VertexAndShader
//#define TEST8_ArchiveSound
//#define TEST6_Finally
//#define TEST5_Scene
//#define TEST4_Exception
//#define TEST3_Model
//#define TEST2_Sprite3D
//#define TEST1
//---------------------------------------------------------------------
#ifdef TEST22_Effect_Ring
void main()
{
    try
    {
        ConfigData data;
        //data.GraphicsAPI = LN_GRAPHICSAPI_OPENGL;
		data.EnableMakeLogFile = true;
        Framework* fw = createFramework();
        fw->initialize(data);

        Scene::Manager* sm = fw->getSceneManager();
        Scene::SceneGraph* sg = sm->getDefaultSceneGraph();
        sg->getDefaultCamera()->setType( LN_CAMERA_MOUSE_MOVE );

        // グリッド
        Scene::Grid* grid = LN_NEW Scene::Grid( sg );
        grid->initialize();

        // エフェクト
        Effect::Manager* em = fw->getEffectManager();


        


      
        {
            // コア
            LRefPtr< Effect::EffectCore > effect_core( LN_NEW Effect::EffectCore( em ) );
            effect_core->initialize( NULL );

            LRefPtr< Effect::EffectNodeCore > effect_node1( LN_NEW Effect::EffectNodeCore() );
            effect_node1->initialize();
            effect_core->getRootNode()->addChild( effect_node1 );

            // Ring Emitter
            LRefPtr< Effect::RingEmitterCore > ring_emitter( LN_NEW Effect::RingEmitterCore( em ) );
            ring_emitter->initialize();
            ring_emitter->setDivisionNum( 64 );
            effect_node1->setEmitterCore( ring_emitter );

            // アニメーション

            // 色
            {
                Effect::AnimationKey key0;
                key0.FramePos = 0;
                key0.Value = 1;
                key0.InterMode = LN_ANIM_INTER_CATMULLROM;

                Effect::AnimationKey key1;
                key1.FramePos = 30;
                key1.Value = 0;
                key1.InterMode = LN_ANIM_INTER_CATMULLROM;

                Effect::EffectAnimationTrackGroup* track_group =
                    ring_emitter->getAnimationTrackGroup( Effect::RingEmitterCore::TRACKGROUP_CENTER_COLOR );

                Effect::ValueAnimationTrack* track = (Effect::ValueAnimationTrack*)track_group->getTrack( 0 );
                track->addEffectAnimationKey( key0 );
                track->addEffectAnimationKey( key1 );
                track->refreshKeysNotRand();
            }

            // 外輪
            {
                Effect::AnimationKey key0;
                key0.FramePos = 0;
                key0.Value = 0;
                key0.InterMode = LN_ANIM_INTER_CATMULLROM;

                Effect::AnimationKey key1;
                key1.FramePos = 100;
                key1.Value = 5;
                key1.InterMode = LN_ANIM_INTER_CATMULLROM;

                Effect::EffectAnimationTrackGroup* track_group =
                    ring_emitter->getAnimationTrackGroup( Effect::RingEmitterCore::TRACKGROUP_OUTER_LOCATION );

                Effect::ValueAnimationTrack* track = (Effect::ValueAnimationTrack*)track_group->getTrack( 0 );
                track->addEffectAnimationKey( key0 );
                track->addEffectAnimationKey( key1 );
                track->refreshKeysNotRand();
            }

            // 内輪
            {
                Effect::AnimationKey key0;
                key0.FramePos = 0;
                key0.Value = 0;
                key0.InterMode = LN_ANIM_INTER_CATMULLROM;

                Effect::AnimationKey key1;
                key1.FramePos = 30;
                key1.Value = 0;
                key1.InterMode = LN_ANIM_INTER_CATMULLROM;

                Effect::AnimationKey key2;
                key2.FramePos = 130;
                key2.Value = 5;
                key2.InterMode = LN_ANIM_INTER_LINEAR;

                Effect::EffectAnimationTrackGroup* track_group =
                    ring_emitter->getAnimationTrackGroup( Effect::RingEmitterCore::TRACKGROUP_INNER_LOCATION );

                Effect::ValueAnimationTrack* track = (Effect::ValueAnimationTrack*)track_group->getTrack( 0 );
                track->addEffectAnimationKey( key0 );
                track->addEffectAnimationKey( key1 );
                track->addEffectAnimationKey( key2 );
                track->refreshKeysNotRand();
            }



            // インスタンス
            LRefPtr< Scene::Effect > scene_effect( LN_NEW Scene::Effect( sg ) );
            scene_effect->initialize( effect_core );

            fw->resetDelayTime();

            while (fw->update())
            {
                if ( fw->getInputManager()->getGameController( 0 )->isOnTrigger( LN_BUTTON_A ) )
                {
                    scene_effect.release();
                    scene_effect = LRefPtr< Scene::Effect >( LN_NEW Scene::Effect( sg ) );
                    scene_effect->initialize( effect_core );
                }
            }
        }


        LN_SAFE_RELEASE( grid );

        fw->finalize();
        LN_SAFE_RELEASE(fw);
    }
    catch ( Base::Exception e )
    {
        e.process();
    }
}
#endif
//---------------------------------------------------------------------
#ifdef TEST21_Effect
void main()
{
    try
    {
        ConfigData data;
        //data.GraphicsAPI = LN_GRAPHICSAPI_OPENGL;
		data.EnableMakeLogFile = true;
        Framework* fw = createFramework();
        fw->initialize(data);

        Scene::Manager* sm = fw->getSceneManager();
        Scene::SceneGraph* sg = sm->getDefaultSceneGraph();
        sg->getDefaultCamera()->setType( LN_CAMERA_MOUSE_MOVE );

        // グリッド
        Scene::Grid* grid = LN_NEW Scene::Grid( sg );
        grid->initialize();


        // エフェクト
        Effect::Manager* em = fw->getEffectManager();

        // コア
        Effect::EffectCore* ef_core = LN_NEW Effect::EffectCore( em );//em->createEmptyEffectCore();
        ef_core->initialize( NULL );

        // 子ノード 1
        Effect::EffectNodeCore* node1 = LN_NEW Effect::EffectNodeCore();
        node1->initialize();
        ef_core->getRootNode()->addChild( node1 );

        // Ring
        Effect::RingEmitterCore* ring_em = LN_NEW Effect::RingEmitterCore( em );
        ring_em->initialize();
        //ef_core->getRootNode()->setEmitterCore( ring_em );
        node1->setEmitterCore( ring_em );
        LN_SAFE_RELEASE( ring_em );

#if 0
        Effect::Sprite3DEmitterCore* sp3d_emitter = LN_NEW Effect::Sprite3DEmitterCore( em );
        node1->setEmitterCore( sp3d_emitter );
        ef_core->getRootNode()->addChild( node1 );
#endif
        // アニメーション
        Effect::ValueAnimationTrack* track;
        //track = (Effect::ValueAnimationTrack*)ef_core->getRootNode()->getAnimationTrackGroup( Effect::COMMONTRACKGROUP_SCALING )->getTrack( 0 );
        track = (Effect::ValueAnimationTrack*)node1->getAnimationTrackGroup( Effect::COMMONTRACKGROUP_SCALING )->getTrack( 0 );

        Effect::AnimationKey key;
        key.FramePos = 0;
        key.Value = 0;
        key.InterMode = LN_ANIM_INTER_CATMULLROM;
        track->addEffectAnimationKey( key );

        key.FramePos = 200;
        key.Value = 100;
        key.InterMode = LN_ANIM_INTER_CATMULLROM;
        track->addEffectAnimationKey( key );

        track->refreshKeysNotRand();

        //Animation::Scalar::Key key;

        //key.FramePos = 0;
        //key.Value = 0;
        //key.InterMode = LN_ANIM_INTER_CATMULLROM;
        //track->addKeyFrame( key );

        //key.FramePos = 200;
        //key.Value = 10;
        //key.InterMode = LN_ANIM_INTER_CATMULLROM;
        //track->addKeyFrame( key );



        // インスタンス化
        Scene::Effect* sef = LN_NEW Scene::Effect(sg);
        sef->initialize(ef_core);






        

        fw->resetDelayTime();
       
        while (fw->update())
        {
            
        }

        LN_SAFE_RELEASE( sef );

        //LN_SAFE_RELEASE( sp3d_emitter );
        LN_SAFE_RELEASE( node1 );
        LN_SAFE_RELEASE( ef_core );

        LN_SAFE_RELEASE( grid );

        fw->finalize();
        LN_SAFE_RELEASE(fw);
    }
    catch ( Base::Exception e )
    {
        e.process();
    }
}
#endif
//---------------------------------------------------------------------
#ifdef TEST20_GUI
void main()
{
    try
    {
        ConfigData data;
        //data.GraphicsAPI = LN_GRAPHICSAPI_OPENGL;
        Framework* fw = createFramework();
        fw->initialize(data);

        {
            GUI::Manager* gui_manager = fw->getGUIManager();

            GUI::GUIDesktop* desktop = gui_manager->getDesktop();

            //LRefPtr<GUI::GUITextBlock> text_block( LN_NEW GUI::GUITextBlock( gui_manager ) );
            ////GUI::GUITextBlock* text_block = LN_NEW GUI::GUITextBlock( gui_manager );
            //text_block->initialize();
            //text_block->setText( _T("lnote") );
            ////desktop->addControl( text_block );

            //LRefPtr<GUI::GUIButton> button1( LN_NEW GUI::GUIButton( gui_manager ) );
            GUI::GUIButton* button1 = LN_NEW GUI::GUIButton( gui_manager );
            button1->initialize();
            desktop->addControl( button1 );
            

            while (fw->update())
            {
                if (fw->getInputManager()->getGameController(0)->isOnTrigger(LN_BUTTON_A))
                {
                    button1->setText( "button!" );
                }
            }

            LN_SAFE_RELEASE( button1 );
            //LN_SAFE_RELEASE(text_block);
        }

        fw->finalize();
        LN_SAFE_RELEASE(fw);
    }
    catch ( Base::Exception e )
    {
        e.process();
    }
}
#endif
//---------------------------------------------------------------------
#ifdef TEST19_UtilityLib

void lnConfig( LConfig config )
{
	//Core::Base::Error::setProcessMode(LN_ERROR_PROC_IMMABORT);
	//
    //config.setEnableConsole( true );
    //config.Framework.setProcessMode( LN_SYSPROCMODE_ALWAYS );
    config.setEnableFPUPreserve( true );

    config.setWindowTitle( _T( "Last Valfirle Ver1.0.1" ) );

	config.setDirectMusicInitMode( LN_DMINITMODE_THREAD_WAIT );
}

int lnMain()
{
    _p(Core::System::getPhysicalPixelSize());

	//Accessor::Audio.playBGM("D:/LNoteProject/LNotePackage/TestMat/zigg-zagg.mp3");
	////Accessor::Audio.playSE("D:/LNoteProject/LNotePackage/TestMat/cursor21.wav");

	//Accessor::Framework.getInterface()->getAudioManager()->getGameAudio()->
	//	playSE("D:/LNoteProject/LNotePackage/TestMat/cursor21.wav");

	// グリッド
    //Scene::Grid* grid1 = LN_NEW Scene::Grid( scene );
    //grid1->initialize();

	//Graphics.setBGColor(LColor::GRAY);
#if 0
	LParticle       mParticle;
    LParticle       mFirstParticle;

	Camera.setPosition(LVector3(0, 0, -2000));
	Camera.setFarClip(10000);

	LNParticleCreateData data;
    data.ParticleNum = 128;
    data.Size = 20;
    data.SizeRand = 16;
    data.FadeIn = 60;
    data.Life = 180;
    data.AfterImageNum = 10;
	data.GenerationInterval = 5;
    data.Loop = true;

    data.Velocity.set    ( 0, 12.0f, 0 );
    data.VelocityRand.set( 0, 8.0f, 0 );
    data.Axis.set    ( 0, 0, 1 );
    data.AngleRand = LMath::PI * 2;

    data.SizeRandType = LN_RANDRATE_MIN;    // サイズが小さいものほど、
    data.VelRandTypeY = LN_RANDRATE_MAX;    // 速度が大きい


    mParticle = LParticle::create( LTexture::create( "D:/GameProjects/Ota-GamePGs/Volkoff/Volkoff_121223/Volkoff/Data/Graphics/Effect/Particle_1.png" ), data );
    //mParticle.setPosition( LVector3::ZERO );
    mParticle.setPriority( LN_SCENENODEPRIORITY_PROXY - 1 );
    mParticle.setEnableDepthTest( false );
    mParticle.setEnableDepthWrite( false );
    mParticle.setBlendMode( LN_BLEND_ADD );




	LNParticleCreateData data2;
 //   data2.ParticleNum = 64;
 //   data2.Size = 40;
 //   data2.SizeRand = 20;
 //   data2.FadeIn = 1;
 //   data2.FadeOut = 10;
 //   data2.Life = 80;
 //   //data2.AfterImageNum = 16;
	//data2.GenerationInterval = 0;
 //   data2.Loop = false;
 //   data2.Velocity.set( 0, 600, 0 );
 //   data2.VelocityRand.set( 0, 100, 0 );
 //   data2.Accel.set( 0, -800, 0 );
 //   data2.Axis.set    ( 0, 0, 1 );
 //   data2.AngleRand = LMath::PI * 2;

    //mFirstParticle = LParticle::create( LTexture::create( "D:/GameProjects/Ota-GamePGs/Volkoff/Volkoff_121223/Volkoff/Data/Graphics/Effect/Particle_1.png" ), data2 );
	//mFirstParticle.setPosition( LVector3::ZERO );
 //   mFirstParticle.setPriority( LN_SCENENODEPRIORITY_PROXY - 1 );
 //   mFirstParticle.setEnableDepthTest( false );
 //   mFirstParticle.setEnableDepthWrite( false );
 //   mFirstParticle.setBlendMode( LN_BLEND_SUB );

	LSprite sp1 = LSprite::create(
		LTexture::create( "D:/GameProjects/Ota-GamePGs/Volkoff/Volkoff_121223/Volkoff/Data/Graphics/Effect/Particle_1.png" ));
#endif
	//LTexture tex = LTexture::create( "D:/GameProjects/Ota-GamePGs/Volkoff/Volkoff_121223/Volkoff/Data/Graphics/Effect/Effect_2.png" );
	
	LTexture tex = LTexture::create( "../../TestMat/gray.png");
	
	//LPanel pp= LPanel::create( 1, 1, tex );
	//32 + 0 * 12, 256

    Core::Scene::Sprite3D* sp3d = LN_NEW Core::Scene::Sprite3D(
        Framework.getInterface()->getSceneManager()->getDefaultSceneGraph());
    sp3d->initialize(1, 2);
    sp3d->setTexture(tex.getInterface());

    do
    {
        Core::Graphics::DebugText* dt = Framework.getInterface()->getGraphicsManager()->getDebugText();
        dt->printf("test fps:%.2f\n", Framework.getGameTime().getFPS());
        (*dt) << _T("count:") << Framework.getFrameCount();

    } while ( Accessor::Framework.update() );

    SAFE_RELEASE(sp3d);

    return 0;
}

#endif
//---------------------------------------------------------------------
#ifdef TEST18_Tilemap
void main()
{
                            //1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99
int StageMap[100*100] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//49
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//48
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 ,//47
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 ,//46
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 ,//45
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 ,//44
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 ,//43
						  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 ,//42
						  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 ,//41
						  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 ,//40
						  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,//39
						  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,70, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,//38
						  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 ,//37
						  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 ,//36
						  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,90, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,90, 0, 0, 0, 0, 0, 0, 0, 0,57, 0, 0,90, 0, 0, 0,41, 0, 0, 0, 0, 0, 0,22,90, 0, 0, 1 ,//35
						  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 ,//34
						  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,71, 0, 0, 0, 0, 0, 0, 0, 1 ,//33
						  1, 0, 0, 0, 0,54, 0, 0, 0, 0, 0, 0, 0,54, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 ,//32
						  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,54, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,54, 0, 0, 0, 0, 0, 0, 0, 0,30, 0, 0, 0, 0,11, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,11, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,//31
						  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,90, 0, 0, 0, 0, 0, 0, 0, 0,90, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//30
						  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//29
						  1,35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,90, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,62, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//28
						  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0,54, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//27
						  1, 0, 0, 0,73, 0,62, 0, 0,62, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,56, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//26
						  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0,90, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//25
				  /*24*/  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//24
				  /*23*/  1,58, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//23
				  /*22*/  1, 0, 0,90, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,90, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//22
				  /*21*/  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,11, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//21
				  /*20*/  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//20
				  /*19*/  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0,39, 0,90, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//19
				  /*18*/  1, 1, 1, 1, 1, 1, 1, 1,11, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,11, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//18
				  /*17*/  1, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0,72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//17
				  /*16*/  1, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//16
				  /*15*/  1, 0, 0,54, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//15
				  /*14*/  1, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0,90, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,30, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0,58, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//14
				  /*13*/  1, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//13
				  /*12*/  1, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,62, 0,62, 0,62, 0, 0, 0, 0, 0, 1, 1,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10,90, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//12
				  /*11*/  1, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//11
				  /*10*/  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,11, 0, 0, 1, 1, 1,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//10
				  /*09*/  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 1, 1,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//9
				  /*08*/  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 1, 1,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//8
				  /*07*/  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 1, 1,10, 0, 0, 0, 0, 0,90, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//7
				  /*06*/  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 1, 1,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//6
				  /*05*/  1, 0, 0, 0,23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 1, 1,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,90, 0, 0, 0, 0, 0, 0, 0, 0,90, 0,57, 0, 0,90, 0, 0, 0, 0,10, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//5
				  /*04*/  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 1, 1,10, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//4
				  /*03*/  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,56, 0, 0,10, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//3
				  /*02*/  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,//2
				  /*01*/  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };//1
                        //1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99

    try
    {
        ConfigData data;
        //data.GraphicsAPI = LN_GRAPHICSAPI_OPENGL;
        Framework* fw = createFramework();
        fw->initialize(data);

		//fw->getAudioManager()->getGameAudio()->playBGM("D:/LNoteProject/LNotePackage/TestMat/zigg-zagg.mp3");
		//fw->getAudioManager()->getGameAudio()->playSE("D:/LNoteProject/LNotePackage/TestMat/cursor21.wav");
		//fw->getAudioManager()->getGameAudio()->playSE("D:/LNoteProject/LNotePackage/TestMat/cursor22.wav");

        Graphics::IGraphicsDevice* gd = fw->getGraphicsManager()->getGraphicsDevice();
        Scene::SceneGraph* scene = fw->getSceneManager()->getDefaultSceneGraph();

        scene->getDefaultCamera()->setType( LN_CAMERA_MOUSE_MOVE );
        scene->getDefaultCamera()->setFarClip( 5000 );

        Graphics::ITexture* tex1;
        gd->createTexture( &tex1, _T( "../../TestMat/MapChip_1.png" ) );

        // グリッド
        Scene::Grid* grid1 = LN_NEW Scene::Grid( scene );
        grid1->initialize();

        Scene::Tilemap* tm = LN_NEW Scene::Tilemap( scene );
        tm->initialize( 100, 100, LN_TILEMAPDIR_XY );
        tm->setTilesetTexture( tex1 );
        tm->setMapData( StageMap );
        //tm->setPosition( LVector3( 7, 1.5, 0 ) );

        // 3Dスプライト 1
        Scene::Sprite3D* sp3d1 = LN_NEW Scene::Sprite3D( scene );
        sp3d1->initialize(2, 3);
        sp3d1->setTexture( tex1 );
        sp3d1->setPosition( LVector3( 3, 1.5, 0 ) );
        sp3d1->setPriority( 50 );
        sp3d1->setBillboardType( LN_BILLBOARD_FRONT );
        sp3d1->setVisible( false );

		// スプライト 1
        Scene::Sprite* sp2d1 = LN_NEW Scene::Sprite( scene );
        sp2d1->initialize();
        sp2d1->setTexture( tex1 );
        sp2d1->setPosition( LVector3( 0, 0, 100 ) );
        sp2d1->setPriority( 50 );
        sp2d1->setVisible( true );


		Scene::Pane* pane = scene->getDefaultPane();
		pane->setTone( LTone( 1, 0, 0, 1 ), 2.0 );

        while (fw->update())
        {
            
        }

		LN_SAFE_RELEASE( sp2d1 );
        LN_SAFE_RELEASE( tm );
        LN_SAFE_RELEASE( sp3d1 );
        LN_SAFE_RELEASE( tex1 );
        LN_SAFE_RELEASE( grid1 );

        fw->finalize();
        LN_SAFE_RELEASE(fw);
    }
    catch ( Base::Exception e )
    {
        e.process();
    }
}
#endif
//---------------------------------------------------------------------
#ifdef TEST17_Particle
void main()
{
    try
    {
        ConfigData data;
        //data.GraphicsAPI = LN_GRAPHICSAPI_OPENGL;
        Framework* fw = createFramework();
        fw->initialize(data);

        Graphics::IGraphicsDevice* gd = fw->getGraphicsManager()->getGraphicsDevice();
        Scene::SceneGraph* scene = fw->getSceneManager()->getDefaultSceneGraph();

        scene->getDefaultCamera()->setType( LN_CAMERA_MOUSE_MOVE );

        Graphics::ITexture* tex1;
        gd->createTexture( &tex1, _T( "../../TestMat/Particle_1.png" ) );

        // グリッド
        Scene::Grid* grid1 = LN_NEW Scene::Grid( scene );
        grid1->initialize();


        /*
        Graphics::PointParticleParameter param;
        param.GenerationInterval = 2;
        param.Position.set( 0, 0, 2 );
        param.PositionRand.set( 0.2, 0.2, 0.2 );
        param.AngleVelocity = 0.1;
        param.Loop = true;
        param.Dynamic = true;
        */

        Graphics::PointParticleParameter param;
        param.GenerationInterval = 2;
        param.Velocity.set( 0, 0.05, 0 );
        param.VelocityRand.set( 0.02, 0.02, 0.02 );
        param.Loop = true;
        param.Dynamic = true;

        //param.Velocity.set( 0, 0.01, 0 );
        
        Scene::PointParticle* prt1 = LN_NEW Scene::PointParticle( scene );
        prt1->initialize( param );
        prt1->setTexture( tex1 );

        float x = -10;

        while (fw->update())
        {
            x += 0.05;

            prt1->setPosition( LVector3( x, 0, 0 ) );
        }

        LN_SAFE_RELEASE( prt1 );
        

        LN_SAFE_RELEASE( tex1 );
        LN_SAFE_RELEASE( grid1 );

        fw->finalize();
        LN_SAFE_RELEASE(fw);
    }
    catch ( Base::Exception e )
    {
        e.process();
    }
}
#endif
//---------------------------------------------------------------------
#ifdef TEST16_Scene
void main()
{
    try
    {
        ConfigData data;
        //data.GraphicsAPI = LN_GRAPHICSAPI_OPENGL;
        data.addArchive(".", "Data.lna", "pass");
        Framework* fw = createFramework();
        fw->initialize(data);

        Graphics::IGraphicsDevice* gd = fw->getGraphicsManager()->getGraphicsDevice();
        Scene::SceneGraph* scene = fw->getSceneManager()->getDefaultSceneGraph();

        scene->getDefaultCamera()->setType( LN_CAMERA_MOUSE_MOVE );

        Graphics::ITexture* tex1;
        //gd->createTexture( &tex1, _T( "../../TestMat/2206234.png" ) );
        //gd->createTexture( &tex1, _T( "Data/Graphics/no_alpha.png" ) 
        gd->createTexture( &tex1, _T( "Data/Graphics/2206234.png" ) );

        // グリッド
        Scene::Grid* grid1 = LN_NEW Scene::Grid( scene );
        grid1->initialize();

#if 1
        // スプライト 1
        Scene::Sprite* sp2d1 = LN_NEW Scene::Sprite( scene );
        sp2d1->initialize();
        sp2d1->setTexture( tex1 );
        sp2d1->setPosition( LVector3( 0, 0, 100 ) );
        sp2d1->setPriority( 50 );
        sp2d1->setVisible( false );

        // スプライト 2
        Scene::Sprite* sp2d2 = LN_NEW Scene::Sprite( scene );
        sp2d2->initialize();
        sp2d2->setTexture( tex1 );
        sp2d2->setPosition( LVector3( 10, 10, 100 ) );
        sp2d2->setPriority( 500 );
        sp2d2->setVisible( false );

        // 3Dスプライト 1
        Scene::Sprite3D* sp3d1 = LN_NEW Scene::Sprite3D( scene );
        sp3d1->initialize(2, 3);
        sp3d1->setTexture( tex1 );
        sp3d1->setPosition( LVector3( 3, 1.5, 0 ) );
        sp3d1->setPriority( 50 );
        sp3d1->setBillboardType( LN_BILLBOARD_FRONT );
        sp3d1->setVisible( true );

        // 3Dスプライト 2
        Scene::Sprite3D* sp3d2 = LN_NEW Scene::Sprite3D( scene );
        sp3d2->initialize(2, 3);
        sp3d2->setTexture( tex1 );
        sp3d2->setPosition( LVector3( 5, 1.5, 0 ) );
        sp3d2->setPriority( 50 );
        sp3d2->setBillboardType( LN_BILLBOARD_ROT_Y );
        sp3d2->setVisible( true );

        // 3Dバッチスプライト 1
        Scene::BatchSprite3D* bsp3d1 = LN_NEW Scene::BatchSprite3D( scene );
        bsp3d1->initialize( 1, 2 );
        bsp3d1->setTexture( tex1 );
        bsp3d1->setBillboardType( LN_BILLBOARD_FRONT );
        bsp3d1->setVisible( true );

        // 2Dバッチスプライト 1
        Scene::BatchSprite* bsp2d1 = LN_NEW Scene::BatchSprite( scene );
        bsp2d1->initialize();
        bsp2d1->setTexture( tex1 );
        bsp2d1->setVisible( true );
#endif
        tex1->getFont()->setColor( LColor(0, 0, 1));
        tex1->drawText( "Test", LRect(0, 0, 0, 0) );

        Graphics::ITexture* ttex1;
        gd->createTexture( &ttex1, _T("../../TestMat/transition2.png") );

        Scene::Pane* root_pane = scene->getDefaultPane();
        root_pane->freeze();
        root_pane->transition( 1.0, ttex1 );

        Input::IGameController* ctrl = fw->getInputManager()->getGameController( 0 );

        while (fw->update())
        {
            if ( ctrl->isOnTrigger( LN_BUTTON_A ) )
            {
                root_pane->freeze();
                root_pane->transition( 1.0, ttex1 );
            }
            if ( ctrl->isOnTrigger( LN_BUTTON_B ) )
            {
                root_pane->blur( 1.0, 0.5f, 1.05f, LVector3( 200, 200, 0 ), false );
            }

            if ( ctrl->isOnTrigger( LN_BUTTON_C ) )
            {
                root_pane->blur( 3.0, 0.7f, 1.02f, LVector3( 10, 0, 0 ), true );
            }

            
        }

        LN_SAFE_RELEASE( ttex1 );
#if 1
        LN_SAFE_RELEASE( bsp2d1 );
        LN_SAFE_RELEASE( bsp3d1 );
        LN_SAFE_RELEASE( sp3d2 );
        LN_SAFE_RELEASE( sp3d1 );
        LN_SAFE_RELEASE( sp2d2 );
        LN_SAFE_RELEASE( sp2d1 );
#endif
        LN_SAFE_RELEASE( tex1 );
        LN_SAFE_RELEASE( grid1 );

        fw->finalize();
        LN_SAFE_RELEASE(fw);
    }
    catch ( Base::Exception e )
    {
        e.process();
    }
}
#endif
//---------------------------------------------------------------------
#ifdef TEST15_DeviceReset
void main()
{
    try
    {
        ConfigData data;
        //data.GraphicsAPI = LN_GRAPHICSAPI_OPENGL;
        //data.ClassInitializeFlag ^= LN_CLASS_SCENE;
		data.EnableMakeLogFile = true;
        Framework* fw = createFramework();
        fw->initialize(data);


        //fw->getGraphicsManager()->getGraphicsDevice()->resetDevice();
        fw->getGraphicsManager()->getGraphicsDevice()->getDefaultCanvas()->resize(320, 240);//(640, 480);//

        while (fw->update())
        {
            
        }




        fw->finalize();
        LN_SAFE_RELEASE(fw);
    }
    catch ( Base::Exception e )
    {
        e.process();
    }
}
#endif
//---------------------------------------------------------------------
#ifdef TEST14_Effect
void main()
{
    try
    {
        ConfigData data;
        //data.GraphicsAPI = LN_GRAPHICSAPI_OPENGL;
		data.EnableMakeLogFile = true;
        Framework* fw = createFramework();
        fw->initialize(data);

        Scene::Manager* sm = fw->getSceneManager();
        Scene::SceneGraph* sg = sm->getDefaultSceneGraph();
        sg->getDefaultCamera()->setType( LN_CAMERA_MOUSE_MOVE );

        // グリッド
        Scene::Grid* grid = LN_NEW Scene::Grid( sg );
        grid->initialize();


        // エフェクト
        Effect::Manager* em = fw->getEffectManager();

        // コア
        Effect::EffectCore* ef_core = LN_NEW Effect::EffectCore( em );//em->createEmptyEffectCore();
        ef_core->initialize( NULL );

        // 子ノード 1 (3Dスプライト)
        Effect::EffectNodeCore* node1 = LN_NEW Effect::EffectNodeCore();
        node1->initialize();
        Effect::Sprite3DEmitterCore* sp3d_emitter = LN_NEW Effect::Sprite3DEmitterCore( em );
        node1->setEmitterCore( sp3d_emitter );
        ef_core->getRootNode()->addChild( node1 );

        // アニメーション
        Effect::ValueAnimationTrack* track;
        track = (Effect::ValueAnimationTrack*)node1->getAnimationTrackGroup( Effect::COMMONTRACKGROUP_SCALING )->getTrack( 0 );

        Animation::Scalar::Key key;

        key.FramePos = 0;
        key.Value = 0;
        key.InterMode = LN_ANIM_INTER_CATMULLROM;
        track->addKeyFrame( key );

        key.FramePos = 200;
        key.Value = 10;
        key.InterMode = LN_ANIM_INTER_CATMULLROM;
        track->addKeyFrame( key );


        fw->getGraphicsManager()->getGraphicsDevice()->resetDevice();

        //fw->getGraphicsManager()->getGraphicsDevice()->getDefaultCanvas()->resize(640, 480);//(320, 240);


        Scene::Effect* sef = LN_NEW Scene::Effect(sg);
        sef->initialize(ef_core);
        

        fw->resetDelayTime();
       
        while (fw->update())
        {
            
        }

        LN_SAFE_RELEASE( sef );

        LN_SAFE_RELEASE( sp3d_emitter );
        LN_SAFE_RELEASE( node1 );
        LN_SAFE_RELEASE( ef_core );

        LN_SAFE_RELEASE( grid );

        fw->finalize();
        LN_SAFE_RELEASE(fw);
    }
    catch ( Base::Exception e )
    {
        e.process();
    }
}
#endif

//---------------------------------------------------------------------
#ifdef TEST13_Input
void main()
{
    try
    {
        ConfigData data;
        Framework* fw = createFramework();
        fw->initialize(data);

        Input::IKeyboard* kb = fw->getInputManager()->getKeyboard();

        while (fw->update())
        {
            /*
            if (kb->isPress(LN_KEY_Z))
            {
                //printf("isPress\n");
            }
            if (kb->isOnTrigger(LN_KEY_Z))
            {
                printf("isOnTrigger\n");
            }
            if (kb->isOffTrigger(LN_KEY_Z))
            {
                printf("isOffTrigger\n");
            }
            if (kb->isRepeat(LN_KEY_Z))
            {
                printf("isRepeat\n");
            }
            */

            // バッファリング
            LNVirtualKeyCode key = kb->getKeyBufferState();
            while (key)
            {
                _p(key);

                key = kb->getKeyBufferState();
            }
        }

        fw->finalize();
        LN_SAFE_RELEASE(fw);
    }
    catch ( Base::Exception e )
    {
        e.process();
    }
}
#endif

//---------------------------------------------------------------------
#ifdef TEST12_GeometryRenderer
void main()
{
    try
    {
        ConfigData data;
        data.GraphicsAPI = LN_GRAPHICSAPI_OPENGL;//LN_GRAPHICSAPI_DIRECTX9;//
        data.AllInitialize = false;
        Framework* fw = createFramework();
        fw->initialize( data );
        fw->initializeGraphicsManager();

        

        Graphics::IGraphicsDevice* g = fw->getGraphicsManager()->getGraphicsDevice();
        Graphics::IRenderer* r = g->getRenderer();
        Graphics::GeometryRenderer* gr = fw->getGraphicsManager()->getGeometryRenderer();


        LMatrix view;
        LMatrix::viewTransformLH(
            &view,
            LVector3( 10, 5, -10 ),
            LVector3( 0, 0, 0 ),
            LVector3::UNIT_Y );

        LMatrix proj;
        LMatrix::perspectiveFovLH( &proj, LMath::PI / 4, 640, 480, 1.0, 10000.0 );

        //view.m22 = 10;
        LMatrix vp = view * proj;
        vp.transpose();
        gr->setViewProjMatrix( vp );//setMatrix

        gr->setMatrix( LMatrix() );

        LVector3 o;
        LVector3 v(0,0,0);
        LVector3::transformCoord(&o,v,vp);
        o.cdump();

        LRenderState rs;
        //rs.BlendMode = LN_BLEND_ALPHA;
        rs.BlendMode = LN_BLEND_NORMAL;
        //rs.FillMode = LN_FILL_WIRE;
        rs.Culling = LN_CULL_DOUBLE;
        r->setRenderState( rs );

        do
        {
            r->beginScene();
            r->clear( true, true, 0xffffffff );

            gr->begin();
            //gr->drawSquare(
            //    -1.5f,  1.5f, 0xffffffff, 0.0f, 0.0f,
            //     2.0f,  1.5f, 0xffff0000, 1.0f, 0.0f,
            //    -1.5f, -1.5f, 0xff00ff00, 0.0f, 1.0f,
            //     1.5f, -2.0f, 0xff0000ff, 1.0f, 1.0f );
            gr->drawTransformAxis( 100 );
            gr->end();

            
            r->endScene();

            g->getDefaultCanvas()->present();

        } while ( fw->update() );

        
        fw->finalize();
        LN_SAFE_RELEASE( fw );
    }
    catch ( Base::Exception e )
    {
        e.process();
    }
}
#endif

//---------------------------------------------------------------------
#ifdef TEST11_Scene
void main()
{
    try
    {
        ConfigData data;
        data.GraphicsAPI = LN_GRAPHICSAPI_DIRECTX9;//LN_GRAPHICSAPI_OPENGL;//
        Framework* fw = createFramework();
        fw->initialize( data );

        Graphics::IGraphicsDevice* g = fw->getGraphicsManager()->getGraphicsDevice();
        Graphics::IRenderer* r = g->getRenderer();

        fw->getAudioManager()->getGameAudio()->playBGM("../../TestMat/n32.ogg");

        fw->getSceneManager()->getDefaultSceneGraph()->getDefaultCamera()->setType( LN_CAMERA_MOUSE_MOVE );


        Graphics::ITexture* tex1 = NULL;
        g->createTexture( &tex1, "../../TestMat/2206234.png" );

        

        Scene::Sprite3D* sp3d1 = LN_NEW Scene::Sprite3D( fw->getSceneManager()->getDefaultSceneGraph() );
        sp3d1->initialize( 2, 3 );
        sp3d1->setTexture( tex1 );

        Scene::Sprite* sp1 = LN_NEW Scene::Sprite( fw->getSceneManager()->getDefaultSceneGraph() );
        sp1->initialize();
        sp1->setTexture( tex1 );
        sp1->setPosition(LVector3(10,10,0));

        //Thread::sleep(5000);

        //fw->getAudioManager()->getGameAudio()->stopBGM();

        do
        {
        } while ( fw->update() );

        LN_SAFE_RELEASE( sp1 );
        LN_SAFE_RELEASE( tex1 );
        LN_SAFE_RELEASE( sp3d1 );

        fw->finalize();
        LN_SAFE_RELEASE( fw );
    }
    catch ( Base::Exception e )
    {
        e.process();
    }
}
#endif

//---------------------------------------------------------------------
#ifdef TEST10_Render
void main()
{
    //std::vector<int> ary;
    //ary.push_back(7);
    //ary.push_back(8);
    //ary.push_back(9);
    //ary.push_back(3);

    //typedef std::vector<int> IntAry;


    //ln_foreach3( int i, ary, IntAry )
    //{
    //    _p(i);
    //}



    try
    {
        ConfigData data;
        data.GraphicsAPI = LN_GRAPHICSAPI_OPENGL;//LN_GRAPHICSAPI_DIRECTX9;//
        data.AllInitialize = false;
        Framework* fw = createFramework();
        LN_PCALL( fw->initialize( data ) );
        LN_PCALL( fw->initializeSystemManager() );
        fw->initializeFileManager();
        LN_PCALL( fw->initializeInputManager() );
        LN_PCALL( fw->initializeAudioManager() );
        LN_PCALL( fw->initializePhysicsManager() );
        LN_PCALL( fw->initializeGraphicsManager() );

        Graphics::IGraphicsDevice* g = fw->getGraphicsManager()->getGraphicsDevice();
        Graphics::IRenderer* r = g->getRenderer();
        Graphics::GeometryRenderer* gr = fw->getGraphicsManager()->getGeometryRenderer();

        //r->setViewport( LRect(10, 20, 320, 240 ) );
        //r->setScissor( LRect(10, 20, 320, 240 ) );

        //fw->getFileManager()->moveCurrentPath("model");
        //fw->getFileManager()->moveCurrentPath("../tex");

        Graphics::ITexture* rt1 = NULL;
        g->createRenderTarget( &rt1, 640, 480 );

        Graphics::ITexture* dp1 = NULL;
        g->createDepthBuffer( &dp1, 640, 480, LN_FMT_D24S8 );


        Graphics::ITexture* tex1 = NULL;
        //g->createTexture( &tex1, 256, 256, 1, LN_FMT_A8R8G8B8 );
        g->createTexture( &tex1, "../../TestMat/has_alpha.png" );

        Graphics::ITexture* tex2 = NULL;
        g->createTexture( &tex2, "../../TestMat/no_alpha.png" );

        Graphics::ITexture* tex3 = NULL;
        g->createTexture( &tex3, "../../TestMat/gray.png" );


        //Graphics::ITexture* tex4 = NULL;
        //g->createTexture( &tex4, "../../TestMat/index_color.png" );

        //tex1->drawText( "てきすと！", LRect( 0, 0, 0, 0 ) );

        do
        {
            r->beginScene();
            r->clear( true, true, 0xffffffff );

            //r->setRenderTarget( 0, rt1 );
            //r->setDepthBuffer( dp1 );
            //r->clear( true, true, 0xffeeddff );
            gr->setTexture( tex1 );
            gr->begin();
            gr->drawSquare(
                -0.5f,  0.5f, 0xffffffff, 0.0f, 0.0f,
                 1.0f,  0.5f, 0xffffffff, 1.0f, 0.0f,
                -0.5f, -0.5f, 0xffffffff, 0.0f, 1.0f,
                 0.5f, -1.0f, 0xffffffff, 1.0f, 1.0f );
            gr->end();


            //r->setRenderTarget( 0, NULL );
            //r->setDepthBuffer( NULL );
            //gr->setTexture( rt1 );
            //gr->begin();
            //gr->drawSquare(
            //    -0.5f,  0.5f, 0xffffffff, 0.0f, 0.0f,
            //     0.5f,  0.5f, 0xffffffff, 1.0f, 0.0f,
            //    -0.5f, -0.5f, 0xffffffff, 0.0f, 1.0f,
            //     0.5f, -1.0f, 0xffffffff, 1.0f, 1.0f );
            //gr->end();


            r->endScene();

            g->getDefaultCanvas()->present();

        } while ( fw->update() );

        LN_SAFE_RELEASE( tex3 );
        LN_SAFE_RELEASE( tex2 );
        LN_SAFE_RELEASE( tex1 );
        LN_SAFE_RELEASE( rt1 );
        LN_SAFE_RELEASE( dp1 );

        fw->finalize();
        LN_SAFE_RELEASE( fw );

    }
    catch ( Base::Exception e )
    {
        e.process();
    }

}
#endif
//---------------------------------------------------------------------
#ifdef TEST9_VertexAndShader
void main()
{
    

    try
    {
        ConfigData data;
        data.GraphicsAPI = LN_GRAPHICSAPI_OPENGL;
        Framework* fw = createFramework();
        LN_PCALL( fw->initialize( data ) );
        LN_PCALL( fw->initializeSystemManager() );
        fw->initializeFileManager();
        LN_PCALL( fw->initializeInputManager() );
        LN_PCALL( fw->initializeAudioManager() );
        LN_PCALL( fw->initializePhysicsManager() );
        LN_PCALL( fw->initializeGraphicsManager() );

        Graphics::IGraphicsDevice* g = fw->getGraphicsManager()->getGraphicsDevice();
        Graphics::IRenderer* r = g->getRenderer();

        Graphics::IShader* sh1 = NULL;
        g->createShader( &sh1, "GLSL/GeometryRenderer.lnglfx" );

        /*
        struct TPPosColor
        {
            
            lnU32       Color;      ///< 頂点カラー
            LVector3	Position;   ///< 座標
         
            /// 頂点レイアウト
            static LNVertexElemenst* elements()
            {
	            static LNVertexElemenst elements[] = {
                    { 0, LN_DECL_COLOR4, LN_USAGE_COLOR,    0 },
		            { 0, LN_DECL_FLOAT3, LN_USAGE_POSITION, 0 },
		              LN_DECL_END() };
	            return elements;
            }
        };
        TPPosColor vertices[] =
        {
            { 0xffff00ff, LVector3( -0.5f,  0.5f, 0.2f ) },
            { 0xffff0000, LVector3(  0.5f,  0.5f, 0.2f ) },
            { 0xff00ff00, LVector3( -0.5f, -0.5f, 0.2f ) },
            { 0xff0000ff, LVector3(  0.5f, -0.5f, 0.2f ) },
        };
        */
        /*
        struct TPPosColor
        {
            
            LVector4    Color;      ///< 頂点カラー
            LVector3	Position;   ///< 座標
         
            /// 頂点レイアウト
            static LNVertexElemenst* elements()
            {
	            static LNVertexElemenst elements[] = {
                    { 0, LN_DECL_FLOAT4, LN_USAGE_COLOR,    0 },
		            { 0, LN_DECL_FLOAT3, LN_USAGE_POSITION, 0 },
		              LN_DECL_END() };
	            return elements;
            }
        };
        TPPosColor vertices[] =
        {
            { LVector4( 1, 1, 1, 1 ), LVector3( -0.5f,  0.5f, 0.2f ) },
            { LVector4( 1, 0, 0, 1 ), LVector3(  0.5f,  0.5f, 0.2f ) },
            { LVector4( 0, 1, 0, 1 ), LVector3( -0.5f, -0.5f, 0.2f ) },
            { LVector4( 0, 0, 1, 1 ), LVector3(  0.5f, -0.5f, 0.2f ) },
        };
        */
        /*
        struct TPPosColor
        {
            LVector3	Position;   ///< 座標
            LVector4    Color;      ///< 頂点カラー
            
            /// 頂点レイアウト
            static LNVertexElemenst* elements()
            {
	            static LNVertexElemenst elements[] = {
                    { 0, LN_DECL_FLOAT3, LN_USAGE_POSITION, 0 },
                    { 0, LN_DECL_FLOAT4, LN_USAGE_COLOR,    0 },
		              LN_DECL_END() };
	            return elements;
            }
        };
        TPPosColor vertices[] =
        {
            { LVector3( -0.5f,  0.5f, 0.2f ), LVector4( 1, 1, 1, 1 ) },
            { LVector3(  0.5f,  0.5f, 0.2f ), LVector4( 1, 0, 0, 1 ) },
            { LVector3( -0.5f, -0.5f, 0.2f ), LVector4( 0, 1, 0, 1 ) },
            { LVector3(  0.5f, -0.5f, 0.2f ), LVector4( 0, 0, 1, 1 ) },
        };
        */
        /*
        struct TPPosColor
        {
            LVector3	Position;   ///< 座標
            
            /// 頂点レイアウト
            static LNVertexElemenst* elements()
            {
	            static LNVertexElemenst elements[] = {
                    { 0, LN_DECL_FLOAT3, LN_USAGE_POSITION, 0 },
		              LN_DECL_END() };
	            return elements;
            }
        };
        TPPosColor vertices[] =
        {
            { LVector3( -0.5f,  0.5f, 0.2f ) },
            { LVector3(  0.5f,  0.5f, 0.2f ) },
            { LVector3( -0.5f, -0.5f, 0.2f ) },
            { LVector3(  0.5f, -0.5f, 0.2f ) },
        };
        */
        struct TPPosColorUV
        {
            
            lnU32       Color;      ///< 頂点カラー
            LVector3	Position;   ///< 座標
            LVector2    UV;
         
            /// 頂点レイアウト
            static LNVertexElemenst* elements()
            {
	            static LNVertexElemenst elements[] = {
                    { 0, LN_DECL_COLOR4, LN_USAGE_COLOR,    0 },
		            { 0, LN_DECL_FLOAT3, LN_USAGE_POSITION, 0 },
                    { 0, LN_DECL_FLOAT2, LN_USAGE_TEXCOORD, 0 },
		              LN_DECL_END() };
	            return elements;
            }
        };
        TPPosColorUV vertices[] =
        {
            { 0xffff00ff, LVector3( -0.5f,  0.5f, 0.2f ), LVector2( 0.0f, 0.0f ) },
            { 0xffff0000, LVector3(  0.5f,  0.5f, 0.2f ), LVector2( 1.0f, 0.0f ) },
            { 0xff00ff00, LVector3( -0.5f, -0.5f, 0.2f ), LVector2( 0.0f, 1.0f ) },
            { 0xff0000ff, LVector3(  0.5f, -0.5f, 0.2f ), LVector2( 1.0f, 1.0f ) },
        };
        Graphics::IVertexBuffer* vb1 = NULL;
        g->createVertexBuffer( &vb1, TPPosColorUV::elements(), 4, vertices );

        Graphics::ITexture* tex1 = NULL;
        g->createTexture( &tex1, 32, 32, 1, LN_FMT_A8R8G8B8 );


        LRenderState rs;
        //rs.BlendMode = LN_BLEND_ALPHA;
        rs.BlendMode = LN_BLEND_NORMAL;
        //rs.FillMode = LN_FILL_WIRE;
        rs.Culling = LN_CULL_DOUBLE;
        r->setRenderState( rs );

        



        do
        {
            sh1->getVariableByName( _T( "gWorldMatrix" ) )->setMatrix( LMatrix() );
            sh1->getVariableByName( _T( "gViewProjMatrix" ) )->setMatrix( LMatrix() );
            sh1->getVariableByName( _T( "gMaterialSampler" ) )->setTexture( tex1 );


            r->beginScene();

            r->clear( true, true, 0xff00eeff );

            sh1->getTechniqueByIndex( 0 )->getPassByIndex( 0 )->begin();



            r->setVertexBuffer( vb1 );
            r->drawPrimitive( LN_PRIM_TRIANGLESTRIP, 0, 2 );

            sh1->getTechniqueByIndex( 0 )->getPassByIndex( 0 )->end();


            r->endScene();

            g->getDefaultCanvas()->present();

        } while ( fw->update() );

        LN_SAFE_RELEASE( tex1 );
        LN_SAFE_RELEASE( sh1 );
        LN_SAFE_RELEASE( vb1 );

        fw->finalize();
        LN_SAFE_RELEASE( fw );

    }
    catch ( Base::Exception e )
    {
        e.process();
    }

}
#endif
//---------------------------------------------------------------------
#ifdef TEST8_ArchiveSound
#include "../../LNote/Core/File/ArchiveMaker.h"

void main()
{
    File::ArchiveMaker maker;
    maker.open("D:/LumineProjects/LuminePackage/TestMat/Game.lna", "pass");

    maker.addFile( "D:/LumineProjects/LuminePackage/TestMat/has_alpha.png", "has_alpha.png" );
    maker.addFile( "D:/LumineProjects/LuminePackage/TestMat/no_alpha.png", "no_alpha.png" );
    maker.addFile( "D:/LumineProjects/LuminePackage/TestMat/gray.png", "gray.png" );
    maker.addFile( "D:/LumineProjects/LuminePackage/TestMat/twinkle.ogg", "twinkle.ogg" );

    //maker.addFile( "D:/LumineProjects/LuminePackage/TestMat/twinkle.ogg", "twinkle.ogg" );
    //maker.addFile( "D:/LumineProjects/LuminePackage/TestMat/cursor21.ogg", "cursor21.ogg" );
    //maker.addFile( "D:/LumineProjects/LuminePackage/TestMat/cursor22.wav", "cursor22.wav" );
    //maker.addFile( "D:/LumineProjects/LuminePackage/TestMat/ln28ti.ogg", "ln28ti.ogg" );
    maker.close();

    return;


    LConfig_AddArchive( "D:/LumineProjects/LuminePackage/TestMat", "Game.lna", "pass" );
    LNRESULT lr = Lumine_Init();
    if ( LN_FAILED( lr ) )
    {
        Base::Exception::getLastException().process();
    }

    Framework* fw = Lumine_GetFramework();


    Audio::ISound* sound1 = NULL;
    try
    {
        fw->getAudioManager()->createSound( &sound1, "Game/twinkle.ogg", LN_SOUNDPLAYTYPE_ONMEMORY, false );
        sound1->play();
        sound1->setPitch( 130 );

        fw->getAudioManager()->getGameAudio()->playSE( "Game/cursor22.wav" );
        //fw->getAudioManager()->getGameAudio()->playSE( "Game/cursor21.ogg" );
    }
    catch (Base::Exception e)
    {
        e.process();
    }


    do
    {
    } while ( fw->update() );


    LN_SAFE_RELEASE( sound1 );

    Lumine_End();
}
#endif

//---------------------------------------------------------------------
#ifdef TEST7_Archive
#include "../../LNote/Core/File/ArchiveMaker.h"

void main()
{
    File::ArchiveMaker maker;
    maker.open("D:/LumineProjects/LuminePackage/テスト素材/TestArchive.lna", "pass");
    maker.addFile( "D:/LumineProjects/LuminePackage/テスト素材/TestArchive/text1.txt", "txt/text1.txt" );
    maker.addFile( "D:/LumineProjects/LuminePackage/テスト素材/TestArchive/テキスト2.txt", "img/テキスト2.txt" );
    _p( maker.addFile( "D:/Media/Music/Material/em.mk/twinkle.mp3", "audio/bgm/twinkle.mp3" ) );
    maker.close();

    LConfig_AddArchive( "D:/LumineProjects/LuminePackage/テスト素材", "TestArchive.lna", "pass" );
    LNRESULT lr = Lumine_Init();
    if ( LN_FAILED( lr ) )
    {
        Base::Exception::getLastException().process();
    }

    Framework* fw = Lumine_GetFramework();

    //fw->getFileManager()->setCurrentPath( "TestArchive" );
    fw->getFileManager()->moveCurrentPath( "TestArchive/img" );
    fw->getFileManager()->moveCurrentPath( "../txt" );

    //fw->getFileManager()->moveCurrentPath( "" );

    File::IInFile*  in_file = NULL;

    try
    {
        fw->getFileManager()->createInFile( &in_file, "text1.txt" );
        in_file->load();
        _p((char*)in_file->getData());
    }
    catch (Base::Exception e)
    {
        e.process();
    }

    
    

    LN_SAFE_RELEASE(in_file);

    fw->getFileManager()->returnCurrentPath();
    fw->getFileManager()->returnCurrentPath();

    try
    {
        fw->getAudioManager()->getGameAudio()->playBGM( "TestArchive/audio/bgm/twinkle.mp3", 100, 120 );
    }
    catch (Base::Exception e)
    {
        e.process();
    }


    /*
    File::IInFile*  in_file = NULL;
    File::IOutFile* out_file = NULL;

    fw->getFileManager()->createInFile( &in_file, "/sdcard/test.txt" );
    
    in_file->load();
    
    while ( !in_file->isLoadFinished() ) { }

    LRefString str( (char*)in_file->getData() );
    str += " OutPut_1";

    fw->getFileManager()->createOutFile(
        &out_file,
        "/sdcard/test_out.txt",
        str.c_str(),
        str.size() + 1 );

    out_file->save( true );

    while ( !out_file->isSaveFinished() ) { }
    */


    //

    do
    {
    } while ( fw->update() );


    Lumine_End();
}
#endif
//---------------------------------------------------------------------
#ifdef TEST6_Finally

class TestObj
{
public:

    TestObj()  { printf( "cons\n" ); }
    ~TestObj() { printf( "dest\n" ); }
    void init()
    {
        LN_THROW_InvalidCall( 0 );
    }
};

template < class TYPE_ >
class finally_delete
    : Base::NonCopyable
{
public:

    finally_delete( TYPE_ obj_ )
        : mObject( obj_ )
    { }

    ~finally_delete()
    {
        SAFE_DELETE( mObject );
    }

    TYPE_& operator * ()
    {
        return *mObject;
    }

    TYPE_ operator -> ()
    {
        return mObject;
    }

    TYPE_ returnObject()
    {
        TYPE_ obj = mObject;
        mObject = NULL;
        return obj;
    }

private:

    TYPE_  mObject;
};

TestObj* Func1()
{
    finally_delete< TestObj* > obj = LN_NEW TestObj();

    //obj->init();

    return obj.returnObject();
}

void main()
{
    TestObj* obj = NULL;

    try
    {
        obj = Func1();
    }
    catch ( Base::Exception e )
    {
        _p( e.getOutputMessage() );
    }

    printf("---\n");
    
    SAFE_DELETE( obj );

    return;


    Lumine_Init();
    Framework* fw = Lumine_GetFramework();

    

    do
    {
    } while ( fw->update() );


    Lumine_End();
}
#endif
//---------------------------------------------------------------------
#ifdef TEST5_Scene
int main()
{
    Base::ExceptionManager em;

    Lumine_Init();
    Framework* fw = Lumine_GetFramework();
    fw->getSceneManager()->enableMulticoreOptimization( true );
    fw->getSceneManager()->getDefaultSceneGraph()->getDefaultCamera()->setType( LN_CAMERA_MOUSE_MOVE );

    Scene::SceneGraph* scene = fw->getSceneManager()->getDefaultSceneGraph();
    Scene::SceneShaderManager* sm = scene->getSceneShaderManager();

    Scene::Model* model1 = NULL;
    Scene::Model* godray_model = NULL;
    Scene::SceneShader* shader1 = NULL;

    
    
    try
    {
        model1 = LN_NEW Scene::Model( fw->getSceneManager()->getDefaultSceneGraph() );
        model1->initialize( _T( "D:/MMD/エフェクト/lobelia/GodRay_old/森アクセサリ/wood.x" ) );
        model1->setScale( LVector3( 10, 10, 10 ) );
        //model1->initialize( _T( "../../テスト素材/dxshader/shadow_city.x" ) );
        //model2->initialize( _T( "C:/Users/admin/Documents/Capsule.x" ) );
        //model2->initialize( _T( "G:/Document/Documents/Metasepuoia/Char001/t.x" ) );
        //model2->initialize( _T( "../../Sample/Media/Graphics/VertexColorBox.x" ) );
    
        //god_ray->initialize( _T( "../../テスト素材/MME/GodRay/GodRay.pmd" ) );



        //sm->createSceneShader( &shader1, _T( "../../テスト素材/dxshader/shadowmap.fx" ) );
        //sm->createSceneShader( &shader1, _T( "../../テスト素材/DepthToon/DepthToon.fx" ) );
        //model1->setShader( shader1 );

        //sm->createSceneShader( &shader1, _T( "../../テスト素材/DepthToon/DepthToon.fx" ) );

        //scene->getDefaultNode( LN_DEFAULT_SCENENODE_3D )->addPostEffectShader( shader1, 0 );

        


        godray_model = LN_NEW Scene::Model( fw->getSceneManager()->getDefaultSceneGraph() );
        godray_model->initialize( _T( "../../テスト素材/GodRay/GodRay.pmd" ) );
    }
    catch ( Base::Exception e )
    {
        em.procException( e );
    }


    do
    {
    } while ( fw->update() );

    LN_SAFE_RELEASE( godray_model );
    LN_SAFE_RELEASE( shader1 );
    LN_SAFE_RELEASE( model1 );


    Lumine_End();

    
    return 0;
}
#endif
//---------------------------------------------------------------------
#ifdef TEST4_Exception

class TestC
{
public:

    static void Func()
    {
        LN_THROW_InvalidCall( 0, "もじれつ" );
    }
};

void Test()
{
    printf( "Test()\n" );

    int i;

    

    try
    {
        printf( "Func()\n" );
        TestC::Func();
        printf( "Func()\n" );
    }
    catch( Base::Exception e )
    {
        throw e;
    }
    
    BOOST_SCOPE_EXIT((&i))
    {
        printf( "BOOST_SCOPE_EXIT\n" );
    }
    BOOST_SCOPE_EXIT_END
}

HRESULT Test2() { return E_INVALIDARG; }

void main()
{
    Base::ExceptionManager em;



    try
    {
        ConfigData data;
        data.WindowTitle = _T( "Tales Note" );
        Framework* fw = createFramework();
        LN_PCALL( fw->initialize( data ) );
        LN_PCALL( fw->initializeSystemManager() );
        LN_PCALL( fw->initializeFileManager() );
        LN_PCALL( fw->initializeInputManager() );
        LN_PCALL( fw->initializeAudioManager() );
        LN_PCALL( fw->initializePhysicsManager() );
        LN_PCALL( fw->initializeGraphicsManager() );
        LN_PCALL( fw->initializeSceneManager() );

        do
        {

        } while ( fw->update() );

        fw->finalize();
        LN_SAFE_RELEASE( fw );

        /*
        //LN_THROW( Base::ArgumentException, "えらー %d", 999 );

        //LN_THROW_ArgumentException();
        
        //LN_THROW_ArgumentException( "ひきすうえらー %d", 999 );

        HRESULT hr;

        
        

        Test();
        */

        Test();

        HRESULT hr;
        LN_COMCALL( Test2() );
    }
    catch ( Base::Exception e )
    {
        em.procException( e );
    }
    


    
}
#endif

//---------------------------------------------------------------------
#ifdef TEST3_Model
void main()
{
    Base::ExceptionManager em;
    ConfigData data;
    data.WindowTitle = _T( "Tales Note" );
    Framework* fw = createFramework();
    LN_PCALL( fw->initialize( data ) );
    LN_PCALL( fw->initializeSystemManager() );
    LN_PCALL( fw->initializeFileManager() );
    LN_PCALL( fw->initializeInputManager() );
    LN_PCALL( fw->initializeAudioManager() );
    LN_PCALL( fw->initializePhysicsManager() );
    LN_PCALL( fw->initializeGraphicsManager() );
    LN_PCALL( fw->initializeSceneManager() );


    //_p( File::getFileName( "File.txt" ).c_str() );
    //_p( File::getFileName( "/File.txt" ).c_str() );
    //_p( File::getFileName( "Dir/File.txt" ).c_str() );
    //_p( File::getFileName( "\\File.txt" ).c_str() );
    //_p( File::getFileName( "./Dir\\File.txt" ).c_str() );
    //_p( File::getFileName( "File.txt\\" ).c_str() );
    //_p( File::getFileName( "./Dir\\File.txt/" ).c_str() );



    fw->getSceneManager()->enableMulticoreOptimization( true );
    fw->getSceneManager()->getDefaultSceneGraph()->getDefaultCamera()->setType( LN_CAMERA_MOUSE_MOVE );

    //Scene::Model* model1 = LN_NEW Scene::Model( fw->getSceneManager()->getDefaultSceneGraph() );
    //model1->initialize( _T( "../../../../MMD/モデル/Lat式ミクVer2.3/Lat式ミクVer2.3_White.pmd" ) );

    //model1->setShader( fw->getSceneManager()->getDefaultSceneGraph()->getDefaultShader( LN_DEFSHADER_NOLIGHTING ) );
 


 
    Scene::Model* model2 = NULL;//LN_NEW Scene::Model( fw->getSceneManager()->getDefaultSceneGraph() );
    Scene::Model* god_ray = LN_NEW Scene::Model( fw->getSceneManager()->getDefaultSceneGraph() );





    try
    {
        //model2->initialize( _T( "D:/MMD/エフェクト/lobelia/GodRay_old/森アクセサリ/wood.x" ) );
        //model2->initialize( _T( "C:/Users/admin/Documents/Capsule.x" ) );
        //model2->initialize( _T( "G:/Document/Documents/Metasepuoia/Char001/t.x" ) );
        //model2->initialize( _T( "../../Sample/Media/Graphics/VertexColorBox.x" ) );
    
        god_ray->initialize( _T( "../../テスト素材/MME/GodRay/GodRay.pmd" ) );
    }
    catch ( Base::Exception e )
    {
        em.procException( e );
    }


//#if 0
    Scene::SceneShader* sh1;
    LN_PCALL( fw->getSceneManager()->getDefaultSceneGraph()->getSceneShaderManager()->createSceneShader(
        //&sh1, _T( "../../テスト素材/MMESample/Glass/Glass.fx" ) ) );
        &sh1, _T( "../../テスト素材/MMESample/Wood/Wood.fx" ) ) );


    Scene::SceneShader* sh2;
    LN_PCALL( fw->getSceneManager()->getDefaultSceneGraph()->getSceneShaderManager()->createSceneShader(
        &sh2, _T( "../../テスト素材/MMESample/Gaussian/Gaussian.fx" ) ) );

    //fw->getSceneManager()->getDefaultSceneGraph()->getDefaultNode( LN_DEFAULT_SCENENODE_3D )->addPostprocessShader( sh2, 0 );


    //model1->setShader( sh1 );

    do
    {
        fw->getPhysicsManager()->update( fw->getGameTime().getElapsedGameTime() );

    } while ( fw->update() );


    //fw->getSceneManager()->getDefaultSceneGraph()->getDefaultNode( LN_DEFAULT_SCENENODE_3D )->removePostprocessShader( sh2 );
    
    
    LN_SAFE_RELEASE( god_ray );
    LN_SAFE_RELEASE( sh2 );
    LN_SAFE_RELEASE( sh1 );
    //LN_SAFE_RELEASE( model1 );
//#endif
    LN_SAFE_RELEASE( god_ray );


    



    fw->finalize();
    LN_SAFE_RELEASE( fw );
}
#endif

//---------------------------------------------------------------------
#ifdef TEST2_Sprite3D
void main()
{
    ConfigData data;
    //data.UpdateMode = LN_UPDATEMODE_ALWAYS;
    Framework* fw = createFramework();
    LN_PCALL( fw->initialize( data ) );
    LN_PCALL( fw->initializeSystemManager() );
    LN_PCALL( fw->initializeFileManager() );
    LN_PCALL( fw->initializeInputManager() );
    LN_PCALL( fw->initializeAudioManager() );
    LN_PCALL( fw->initializePhysicsManager() );
    LN_PCALL( fw->initializeGraphicsManager() );
    LN_PCALL( fw->initializeSceneManager() );

    fw->getSceneManager()->getDefaultSceneGraph()->getDefaultCamera()->setType( LN_CAMERA_MOUSE_MOVE );

    Scene::SceneShader* shader1 = NULL;
    LN_PCALL( fw->getSceneManager()->getDefaultSceneGraph()->getSceneShaderManager()->createSceneShader(
        &shader1, _T( "../../テスト素材/MMESample/Basic/basic.fx" ) ) );

    Scene::Sprite3D* sprite3d1 = LN_NEW Scene::Sprite3D( fw->getSceneManager()->getDefaultSceneGraph() );
    LN_PCALL( sprite3d1->initialize( 1, 2 ) );

    Graphics::ITexture* tex1;
    fw->getGraphicsManager()->createTextureFromFile( &tex1, "../../Sample/Media/Graphics/Picture_1.png" );

    sprite3d1->setTexture( tex1 );



    do
    {
    } while ( fw->update() );

    LN_SAFE_RELEASE( tex1 );
    LN_SAFE_RELEASE( sprite3d1 );
    LN_SAFE_RELEASE( shader1 );

    fw->finalize();
    LN_SAFE_RELEASE( fw );
}
#endif

//---------------------------------------------------------------------
#ifdef TEST1
void main()
{
    ConfigData data;
    data.FrameRate = 60;
    data.addFontFile( _T( "../../テスト素材/apjfont/APJapanesefont.ttf" ) );
    Framework* fw = createFramework();
    LN_PCALL( fw->initialize( data ) );
    LN_PCALL( fw->initializeSystemManager() );
    LN_PCALL( fw->initializeFileManager() );
    LN_PCALL( fw->initializeInputManager() );
    LN_PCALL( fw->initializeAudioManager() );
    LN_PCALL( fw->initializePhysicsManager() );
    LN_PCALL( fw->initializeGraphicsManager() );


    //LN_PCALL( fw->getAudioManager()->getGameAudio()->playBGM( "D:/MMD/オーディオ/ZIGG-ZAGG.mp3", 100, 130 ) );
    //LN_PCALL( fw->getAudioManager()->getGameAudio()->playBGM( "D:/Documents/CravingExplorer/spring ～君とのメロディ～ _飛蘭　Full.mp3", 100, 105 ) );

    Graphics::IGraphicsDevice* device = fw->getGraphicsManager()->getGraphicsDevice();
    Graphics::IRenderer* r = device->getRenderer();

    Graphics::IVertexBuffer* vb1 = NULL;
    Graphics::Vertex::PosColor v1[3] = 
    {
        { LVector3( 0, 0, 0 ), 0xffff0000 },
        { LVector3( 0, 0.7, 0 ), 0xff00ff00 },
        { LVector3( 0.5, 0, 0 ), 0xff0000ff },
    };
    LN_PCALL( device->createVertexBuffer( &vb1, Graphics::Vertex::PosColor::elements(), 3, v1 ) );

    File::IInFile* file1 = NULL;
    LN_PCALL( fw->getFileManager()->createInFile( &file1, _T( "../../Sample/Media/Shader/PosColor.fx" ) ) );

    Graphics::IShader* shader1 = NULL;
    LN_PCALL( device->createShader( &shader1, file1, LSharingKey( ( "s1key" ) ) ) );
    

    Graphics::SpriteRenderer* sprite_r = LN_NEW Graphics::SpriteRenderer();
    LN_PCALL( sprite_r->initialize( device, 1024, false ) );
    LMatrix proj_2d;
    LMatrix::perspective2DLH( &proj_2d, 640, 480, 0, 10000 );
    sprite_r->setViewProjMatrix( LMatrix::IDENTITY, proj_2d );

    
    File::IInFile* file2 = NULL;
    LN_PCALL( fw->getFileManager()->createInFile( &file2, _T( "../../Sample/Media/Graphics/Picture_1.png" ) ) );
    Graphics::ITexture* tex1 = NULL;
    LN_PCALL( device->createTexture( &tex1, file2, 0, 1, LN_FMT_A8R8G8B8 ) );

    //-----------------------------------------------
    // フォント
    Graphics::IFont* font1 = NULL;
    LN_PCALL( fw->getGraphicsManager()->createFont( &font1, _T( "あんずもじ" ) ) );//LN_DEFAULT_FONT_NAME ) );

    //tex1->clear( LColor( 0, 0, 0, 0 ) );
    tex1->setFont( font1 );
    tex1->drawText( _T( "LNote" ), LRect( 0, 0, 128, 128 ) );

    LRenderState rstate;
    rstate.BlendMode = LN_BLEND_ALPHA;
    r->setRenderState( rstate );

    //-----------------------------------------------
    // GeometryRenderer
    Graphics::GeometryRenderer* gr = fw->getGraphicsManager()->getGeometryRenderer();
    LMatrix view1, proj1;
    //LMatrix::viewTransformRH( &view1, LVector3( 5, 20, -20 ), LVector3( 0, 15, 0 ), LVector3( 0, 1, 0 ) );
    //LMatrix::viewTransformRH( &view1, LVector3( 50, 30, -50 ), LVector3( 0, 10, 0 ), LVector3( 0, 1, 0 ) );
    //LMatrix::viewTransformLH( &view1, LVector3( 170, 150, -170 ), LVector3( 0, 10, 0 ), LVector3( 0, 1, 0 ) );
    LMatrix::viewTransformLH( &view1, LVector3( -10, 30, -50 ), LVector3( -10, 10, 0 ), LVector3( 0, 1, 0 ) );
    LMatrix::perspectiveFovLH( &proj1, LMath::PI * 0.125f, 640, 480, 1, 1000 );
    gr->setViewProjMatrix( view1 * proj1 );

    //-----------------------------------------------
    // Physics
    const int BOX_NUM = 1;
    Physics::Plane* plane = fw->getPhysicsManager()->createFloor();
    Physics::Box* boxes[BOX_NUM] = { NULL };
    for ( int i = 0; i < BOX_NUM; ++i )
    {
        //boxes[ i ] = fw->getPhysicsManager()->createBox( LVector3( 1, 1, 1 ), LVector3( 0.5 * i - BOX_NUM / 4, 1.5 * i + 2.0f, i ), 3.0f );
    }

    //-----------------------------------------------
    // Model
    
    File::IInFile* file3 = NULL;
    LN_PCALL( fw->getFileManager()->createInFile( &file3, _T( "../../LNote/Core/Resource/Shader/MMDBasic.fx" ) ) );
    Graphics::IShader* shader2 = NULL;
    LN_PCALL( device->createShader( &shader2, file3 ) );

    // GPU スキニングシェーダ
    File::IInFile* file5 = NULL;
    LN_PCALL( fw->getFileManager()->createInFile( &file5, _T( "../../LNote/Core/Resource/Shader/MMDBasicGPU.fx" ) ) );
    Graphics::IShader* shader3 = NULL;
    LN_PCALL( device->createShader( &shader3, file5 ) );
    LN_SAFE_RELEASE( file5 );

    Graphics::ITexture* depth1 = NULL;
    LN_PCALL( device->createDepthBuffer( &depth1, 640, 480, LN_FMT_D24S8 ) );
    r->setDepthBuffer( depth1 );

    // ダミーテクスチャ
    Graphics::ITexture* wtex1 = NULL;
    LN_PCALL( device->createTexture( &wtex1, 32, 32, 1, LN_FMT_A8R8G8B8 ));
    wtex1->clear( LColor::WHITE );
    //wtex1->clear( LColor(0, 0, 0, 0.5) );

    LVector3 vec0( 0, 1, 0 );
    LVector3 vec1( 0, 1, 0 );
    _p(LVector3::dot(vec0, vec1));
    //v.transform( view1 * proj1 );
    //v.cdump();

    File::IInFile* file4 = NULL;
    LN_PCALL( fw->getFileManager()->createInFile( &file4, _T( "../../LNote/Core/Resource/Texture/toon04.bmp" ) ) );
    Graphics::ITexture* toon1 = NULL;
    LN_PCALL( device->createTexture( &toon1, file4, 0, 1, LN_FMT_A8R8G8B8 ));
    LN_SAFE_RELEASE( file4 );

    shader2->getVariableByName( "gToonTexture" )->setTexture( toon1 );
    
    // モデル
    Graphics::ModelManager* model_manager = fw->getGraphicsManager()->getModelManager();
    Graphics::Model* model1 = NULL;
    LN_PCALL( model_manager->createModel( &model1, _T( "../../../../MMD/モデル/Lat式ミクVer2.3/Lat式ミクVer2.3_White.pmd" ) ) );
    //LN_PCALL( model_manager->createModel( &model1, _T( "../../../../Documents/GameProjects/8th/model/t.pmd" ), lnNullKey ) );
    //LN_PCALL( model_manager->createModel( &model1, _T( "../../../../MMD/モデル/AliceV033/アリスV033.pmd" ) ) );
    //LN_PCALL( model_manager->createModel( &model1, _T( "../../../../Programing/MMD/MMDX_src/SlimMMDXDemo1/models/Miku.pmd" ) ) );

    // スキニングテクスチャ
    Graphics::ITexture* skinning_tex = NULL;
    //LN_PCALL( device->createTexture( &skinning_tex, 4, 256, 1, LN_FMT_A32B32G32R32F ));

    // スキニングテクスチャ用シェーダ
    //LN_PCALL( fw->getFileManager()->createInFile( &file5, _T( "../../LNote/Core/Resource/Shader/MMDBasicGPUTex.fx" ) ) );
    Graphics::IShader* skinning_tex_shader = NULL;
    //LN_PCALL( device->createShader( &skinning_tex_shader, file5 ) );
    //LN_SAFE_RELEASE( file5 );


    // モーション
    Graphics::Motion* motion1 = NULL;
    LN_PCALL( model_manager->createMotion( &motion1, _T( "../../../../MMD/モーション/Zigg-Zagg/ZZ-MikuV2.vmd" ), lnNullKey ) );
    //LN_PCALL( model_manager->createMotion( &motion1, _T( "../../../../MMD/モーション/test.vmd" ), lnNullKey ) );
    Graphics::MotionController* mc1 = model1->getMotionController();


    mc1->attachMotion( 0, motion1 );
    

    fw->getPhysicsManager()->update(0.1f);

    // 最初のポーズ
    mc1->advanceTime(0.01);
    model1->updateFrameMatrix();
    //model1->mModelCore->correction(1.0f);
    model1->resetRigidBody();


    float YRot11, XRot11, ZRot11;
    LVector3 vvv( 2, 1, 5 );
    vvv.normalize();
    LQuaternion qt55(vvv, 5.0f);
    LQuaternion::factoringZXY(&XRot11,&YRot11,&ZRot11,qt55);
    printf( "%f %f %f %f\n", qt55.x, qt55.y, qt55.z, qt55.w);


    //---------------------------------------------------------
    // メインループ
    fw->getFPSController()->refreshSystemDelay(System::getSystemTime());
    while ( fw->update() )
    {
        r->beginScene();
        r->clear( true, true, 0xffcceeff );

        mc1->advanceTime(0.016/2);

        model1->updateFrameMatrix();

        model1->postUpdate(fw->getGameTime().getElapsedGameTime());

        //model1->updateFrameMatrix();

        //-----------------------------------------------
        // Physics
        fw->getPhysicsManager()->update(fw->getGameTime().getElapsedGameTime());//0.016f);//0.0f);//

        gr->begin();


        LRenderState rs3;
        rs3.DepthTest = false;
        r->setRenderState( rs3 );
        gr->setMatrix( LMatrix() );
        gr->drawTransformAxis( 10.0f );
        fw->getPhysicsManager()->drawDebugShape( gr );
        rs3.DepthTest = true;
        r->setRenderState( rs3 );

        //-----------------------------------------------
        // Model

        

        model1->updateSkinningMatrix();
        model1->skinning();

        shader2->getVariableByName( "gWorldViewProjMatrix" )->setMatrix( view1 * proj1 );
        shader2->getVariableByName( "gWorldViewMatrix" )->setMatrix( view1 );

        lnU32 snum = model1->getSubsetNum();
        for ( lnU32 i = 0; i < snum; ++i )
        {
            LNMeshAttribute* attr = model1->getAttribute( i );
            Graphics::ITexture* t = model1->getMaterial( attr->MaterialIndex )->getTexture();
            t = (t) ? t : wtex1;
            shader2->getVariableByName( "gTexture" )->setTexture( t );

            shader2->getTechniqueByIndex( 0 )->getPassByIndex( 0 )->begin();
            model1->drawSubset( i );
            shader2->getTechniqueByIndex( 0 )->getPassByIndex( 0 )->end();
        }
        
        
        gr->end();




        LRenderState dt_rs;
        dt_rs.DepthTest = false;
        dt_rs.Culling = LN_CULL_DOUBLE;
        r->setRenderState( dt_rs );


        fw->getGraphicsManager()->getDebugText()->printf( "Test!! %d\n", 888 );
        fw->getGraphicsManager()->getDebugText()->printf( "にゅーらいん\n" );

        fw->getGraphicsManager()->getDebugText()->draw();



        r->endScene();
    }
    



#if 0

    mc1->attachMotion( 0, motion1 );
    

    fw->getPhysicsManager()->update(0.1f);

    // 最初のポーズ
    mc1->advanceTime(0.01);
    model1->updateFrameMatrix();
    //model1->mModelCore->correction(1.0f);
    model1->resetRigidBody();


    int fc = 0;

    fw->getFPSController()->refreshSystemDelay(System::getSystemTime());
    while ( fw->update() )
    {
        //continue;

        r->beginScene();

        r->clear( true, true, 0xffcceeff );

        /*
        r->setVertexBuffer( vb1 );

        shader1->getTechniqueByIndex( 0 )->getPassByIndex( 0 )->begin();
        r->drawPrimitive( LN_PRIM_TRIANGLELIST, 0, 1 );
        shader1->getTechniqueByIndex( 0 )->getPassByIndex( 0 )->end();


        sprite_r->drawRequest2D(
            LVector3( 10, 20, 0 ),
            LVector3( 0, 0, 0 ),
            tex1,
            LRect( 0, 0, tex1->getSize().x, tex1->getSize().y ),
            0xffffffff );

        sprite_r->flash();
        */

        if ( fc == 244 )
        {
            LN_PCALL( fw->getAudioManager()->getGameAudio()->playBGM( "D:/MMD/オーディオ/ZIGG-ZAGG.mp3", 100, 100 ) );
            model1->resetRigidBody();
        }

        ++fc;


        //-----------------------------------------------
        // GeometryRenderer
        /*
        gr->begin();

        gr->drawTransformAxis( 10.0f );

        gr->drawLine( LVector3( 0, 0, 5 ), LVector3( 0, 7, 0 ), LColor::RED, LColor::BLUE );

        LRenderState rs;
        rs.FillMode = LN_FILL_WIRE;
        rs.Culling = LN_CULL_DOUBLE;
        r->setRenderState( rs );

        //gr->drawBox( LVector3( -1, -2, -3 ), LVector3( 4, 5, 6 ), LColor( 1.0f, 0, 0, 0.5f ) );

       
       //gr->drawCapsule( 1, 5, LColor( 0.0f, 0, 1.0f, 0.5f ) );
        gr->drawSphere( LVector3( 1, 2, 3 ), 3, LColor( 0.0f, 0, 1.0f, 0.5f ) );

        gr->end();
        */

        LRenderState rs;
        //rs.BlendMode = LN_BLEND_NORMAL;
        ////rs.FillMode = LN_FILL_WIRE;
        //rs.Culling = LN_CULL_DOUBLE;
        //r->setRenderState( rs );


     
        mc1->advanceTime(0.016/2);


        model1->updateFrameMatrix();
                


        model1->mModelCore->correction(1.0f);
        model1->updateSkinningMatrix();

#if 0 // GPU スキニング

        //printf( "-----------\n" );

        shader3->getVariableByName( _T( "mViewProj" ) )->setMatrix( view1 * proj1 );

        LMatrix bones[ 26 ];

        lnU32 snum = model1->mModelCore->getAttrGroupNum();
        for ( lnU32 subset_i = 0; subset_i < snum; ++subset_i )
        {
            LNMeshAttributeRange& attr = model1->mModelCore->getAttributes()[ subset_i ];
            lnU32 i = 0;
            lnU32 bone_i;
            //ln_foreach( bone_i, attr.BoneIndices )
            LNEffectedBoneIndices::iterator itr = attr.BoneIndices.begin();
            LNEffectedBoneIndices::iterator end = attr.BoneIndices.end();
            for ( ; itr != end; ++itr )
            {
                //_p((*itr));
                lnU32 bi = (*itr);
                bones[ i ] = model1->getFrame( bi )->getSkinningMatrix();
                ++i;
            }

            //_p(i);


            
            shader3->getVariableByName( _T( "mWorldMatrixArray" ) )->setMatrixArray( bones, i );

            Graphics::ITexture* t = model1->mModelCore->getMaterials()[ attr.MaterialIndex ].getTexture();
            t = (t) ? t : wtex1;
            //_p(model1->mModelCore->getMaterials()[ i ].getTexture());
            shader3->getVariableByName( "gTexture" )->setTexture( t );

            shader3->getTechniqueByIndex( 0 )->getPassByIndex( 0 )->begin();
            model1->mModelCore->drawSubset( subset_i );
            shader3->getTechniqueByIndex( 0 )->getPassByIndex( 0 )->end();
            



            /*
            shader2->getVariableByName( "gWorldViewProjMatrix" )->setMatrix( view1 * proj1 );
            shader2->getVariableByName( "gWorldViewMatrix" )->setMatrix( view1 );
            lnU32 snum = model1->mModelCore->getAttrGroupNum();
            for ( lnU32 i = 0; i < snum; ++i )
            {
                Graphics::ITexture* t = model1->mModelCore->getMaterials()[ i ].getTexture();
                t = (t) ? t : wtex1;
                //_p(model1->mModelCore->getMaterials()[ i ].getTexture());
                shader2->getVariableByName( "gTexture" )->setTexture( t );

                shader2->getTechniqueByIndex( 0 )->getPassByIndex( 0 )->begin();
                model1->mModelCore->drawSubset( i );
                shader2->getTechniqueByIndex( 0 )->getPassByIndex( 0 )->end();
            }
            */

        }


#elif 1 // スキニングテクスチャ

        //model1->skinning();

        
        lnU32 frame_num = model1->getFrameNum();
        LMatrix* t = (LMatrix*)skinning_tex->lock();
        for ( lnU32 i = 0; i < frame_num; ++i )
        {
            const LMatrix& mat = model1->getFrame( i )->getSkinningMatrix();
            t[ i ] = mat;
        }
        skinning_tex->unlock();




        skinning_tex_shader->getVariableByName( "gWorldViewProjMatrix" )->setMatrix( view1 * proj1 );
        skinning_tex_shader->getVariableByName( "gWorldViewMatrix" )->setMatrix( view1 );

        //skinning_tex_shader->getVariableByName( "gBoneTextureSize" )->setVector();
        skinning_tex_shader->getVariableByName( "gBoneRotationTexture" )->setTexture( skinning_tex );
        
        lnU32 snum = model1->mModelCore->getAttrGroupNum();
        for ( lnU32 i = 0; i < snum; ++i )
        {
            Graphics::ITexture* t = model1->mModelCore->getMaterials()[ i ].getTexture();
            t = (t) ? t : wtex1;
            //_p(model1->mModelCore->getMaterials()[ i ].getTexture());
            skinning_tex_shader->getVariableByName( "gTexture" )->setTexture( t );

            skinning_tex_shader->getTechniqueByIndex( 0 )->getPassByIndex( 0 )->begin();
            model1->mModelCore->drawSubset( i );
            skinning_tex_shader->getTechniqueByIndex( 0 )->getPassByIndex( 0 )->end();
        }


#else // ソフトウェアスキニング
        model1->skinning();
       


        //LRenderState rs2;
        //rs2.Culling = LN_CULL_DOUBLE;
        //r->setRenderState( rs2 );

        shader2->getVariableByName( "gWorldViewProjMatrix" )->setMatrix( view1 * proj1 );
        shader2->getVariableByName( "gWorldViewMatrix" )->setMatrix( view1 );

        lnU32 snum = model1->mModelCore->getAttrGroupNum();
        for ( lnU32 i = 0; i < snum; ++i )
        {
            Graphics::ITexture* t = model1->mModelCore->getMaterials()[ i ].getTexture();
            t = (t) ? t : wtex1;
            //_p(model1->mModelCore->getMaterials()[ i ].getTexture());
            shader2->getVariableByName( "gTexture" )->setTexture( t );

            shader2->getTechniqueByIndex( 0 )->getPassByIndex( 0 )->begin();
            model1->mModelCore->drawSubset( i );
            shader2->getTechniqueByIndex( 0 )->getPassByIndex( 0 )->end();
        }
#endif

        //-----------------------------------------------
        // Physics
        fw->getPhysicsManager()->update(fw->getGameTime().getElapsedGameTime());//0.016f);//0.0f);//

        gr->begin();


        LRenderState rs3;
        rs3.DepthTest = false;
        r->setRenderState( rs3 );

        gr->setMatrix( LMatrix() );
        gr->drawTransformAxis( 10.0f );
        //fw->getPhysicsManager()->drawDebugShape( gr );


        rs.DepthTest = true;
        r->setRenderState( rs );
        
        gr->end();

        
        
        


        r->endScene();
    }

#endif

    LN_SAFE_RELEASE( motion1 );
    LN_SAFE_RELEASE( skinning_tex_shader );
    LN_SAFE_RELEASE( skinning_tex );
    LN_SAFE_RELEASE( model1 );
    LN_SAFE_RELEASE( toon1 );
    LN_SAFE_RELEASE( wtex1 );
    LN_SAFE_RELEASE( depth1 );
    LN_SAFE_RELEASE( shader3 );
    LN_SAFE_RELEASE( shader2 );
    LN_SAFE_RELEASE( file3 );
    

    for ( int i = 0; i < BOX_NUM; ++i )
    {
        LN_SAFE_RELEASE( boxes[i] );
    }
    LN_SAFE_RELEASE( plane );

    LN_SAFE_RELEASE( font1 );
    LN_SAFE_RELEASE( tex1 );
    LN_SAFE_RELEASE( file2 );
    LN_SAFE_RELEASE( sprite_r );
    LN_SAFE_RELEASE( shader1 );
    LN_SAFE_RELEASE( file1 );
    LN_SAFE_RELEASE( vb1 );

    fw->finalize();
    LN_SAFE_RELEASE( fw );
}

#endif
//#endif // STATIC_TEST
//=============================================================================
//								end of file
//=============================================================================