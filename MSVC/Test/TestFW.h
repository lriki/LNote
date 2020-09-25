
#pragma once


class TestFW
{
public:

    virtual void main() = 0;

public:

    virtual void onInit()
    {
        ConfigData data;
        //data.GraphicsAPI = LN_GRAPHICSAPI_OPENGL;
        data.EnableShowFPS = true;
		data.EnableMakeLogFile = true;
        fw = createFramework();
        fw->initialize(data);

        Scene::Manager* sm = fw->getSceneManager();
        scene = sm->getDefaultSceneGraph();

        camera = scene->getDefaultCamera();
        //camera->setType( LN_CAMERA_MOUSE_MOVE );
        camera->setCameraOperator( Graphics::CylinderMouseMoveCameraOperator::create() );

        grid = LN_NEW Scene::Grid( scene );
        grid->initialize();
    }

    virtual void onExit()
    {

        Scene::Pane* pane = scene->getDefaultPane();
        pane->addRef();

        LN_SAFE_RELEASE( grid );

        fw->finalize();
        LN_SAFE_RELEASE( fw );


        
        pane->release();
        

    }
public:

    Framework*          fw;

    Scene::SceneGraph*  scene;
    Scene::Camera*      camera;
    Scene::Grid*        grid;
};

