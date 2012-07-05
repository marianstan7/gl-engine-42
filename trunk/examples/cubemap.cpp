//
// flycam.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar  2 17:27:21 2012 gael jochaud-du-plessix
// Last update Wed Jul  4 22:22:15 2012 loick michard
//

#include <iostream>

#include "TextureFrameBuffer.hpp"
#include "Example.hpp"
#include "CubeMap.hpp"
#include "ObjLoader.hpp"
#include "UniversalLoader.hpp"
#include "Mesh.hpp"
#include "Geometries.hpp"

#define W_WIDTH 1280
#define W_HEIGHT 720

#define W_FRAMERATE 60

int accelerometerId = 0;

class App : public Example {
public:
  App(int ac, char** av) :
    Example(ac, av, W_WIDTH, W_HEIGHT, W_FRAMERATE, "glEngine : Camaro CubeMap"),
    _light(), _plane()
  {
    _cameraType = Trackball;
    _recordVideo = true;
  }

  void _loadCamaro()
  {
    gle::ObjLoader loader;
    gle::Scene::Node* model = loader.load("./models/Camaro.obj", new gle::Material());
    {
      gle::Mesh* body = dynamic_cast<gle::Mesh*>(model->getChildByName("Body"));
      _bodyMaterial = new gle::Material();
      _bodyMaterial->setDiffuseColor(gle::Color<GLfloat>(4.0, 2.0, 0.5));
      _bodyMaterial->setReflectionIntensity(0.75);
      _bodyMaterial->setEnvMap(_cubeMap);
      body->setMaterial(_bodyMaterial);
    }
    {
      gle::Mesh* mirror = dynamic_cast<gle::Mesh*>(model->getChildByName("mirror"));
      gle::Material* bodyMaterial = new gle::Material();
      bodyMaterial->setReflectionIntensity(1.0);
      bodyMaterial->setEnvMap(_cubeMap);
      mirror->setMaterial(bodyMaterial);
    }
    {
      gle::Mesh* black = dynamic_cast<gle::Mesh*>(model->getChildByName("black"));
      gle::Material* bodyMaterial = new gle::Material();
      bodyMaterial->setDiffuseColor(gle::Color<GLfloat>(0.0, 0.0, 0.0));
      bodyMaterial->setSpecularIntensity(0.1);
      bodyMaterial->setReflectionIntensity(0.4);
      bodyMaterial->setEnvMap(_cubeMap);
      black->setMaterial(bodyMaterial);
    }
    {
      gle::Mesh* mizo = dynamic_cast<gle::Mesh*>(model->getChildByName("mizo"));
      gle::Material* bodyMaterial = new gle::Material();
      bodyMaterial->setDiffuseColor(gle::Color<GLfloat>(0.0, 0.0, 0.0));
      bodyMaterial->setEnvMap(_cubeMap);
      mizo->setMaterial(bodyMaterial);
    }
    {
      gle::Mesh* glass = dynamic_cast<gle::Mesh*>(model->getChildByName("glass"));
      gle::Material* bodyMaterial = new gle::Material();
      bodyMaterial->setDiffuseColor(gle::Color<GLfloat>(0.0, 0.0, 0.0));
      bodyMaterial->setEnvMap(_cubeMap);
      bodyMaterial->setReflectionIntensity(0.5);
      glass->setMaterial(bodyMaterial);
    }
    {
      gle::Mesh* tire = dynamic_cast<gle::Mesh*>(model->getChildByName("tire"));
      gle::Material* bodyMaterial = new gle::Material();
      bodyMaterial->setDiffuseColor(gle::Color<GLfloat>(0.14, 0.14, 0.14));
      bodyMaterial->setSpecularLightEnabled(false);
      bodyMaterial->setEnvMap(_cubeMap);
      tire->setMaterial(bodyMaterial);
    }
    {
      gle::Mesh* tireling = dynamic_cast<gle::Mesh*>(model->getChildByName("tireling"));
      gle::Material* bodyMaterial = new gle::Material();
      bodyMaterial->setDiffuseColor(gle::Color<GLfloat>(0.1, 0.1, 0.1));
      bodyMaterial->setSpecularLightEnabled(false);
      bodyMaterial->setEnvMap(_cubeMap);
      tireling->setMaterial(bodyMaterial);
    }
    {
      gle::Mesh* black2 = dynamic_cast<gle::Mesh*>(model->getChildByName("black2"));
      gle::Material* bodyMaterial = new gle::Material();
      bodyMaterial->setDiffuseColor(gle::Color<GLfloat>(0.2, 0.2, 0.2));
      bodyMaterial->setSpecularLightEnabled(false);
      bodyMaterial->setEnvMap(_cubeMap);
      black2->setMaterial(bodyMaterial);
    }
    *_scene << model;
  }

  void initScene()
  {
    _scene = new gle::Scene();
    _camera = new gle::PerspectiveCamera(gle::Vector3f(-20, 20, 0),
					 gle::Vector3f(0, 0, 0),
					 45, (GLfloat)_winWidth/_winHeight, 1, 10000);
    _renderer = new gle::Renderer();
    _cubeMap = new gle::CubeMap("./examples/cubemaps/bridge", ".jpg");
    //gle::Texture* tex = new gle::Texture("./models/companion_cube.jpg");
    _light = new gle::PointLight(gle::Vector3f(0, 0, 0),
    				 gle::Colorf(0.8, 0.8, 0.8));
    gle::PointLight* light = new gle::PointLight(gle::Vector3f(0, 0, 0),
						 gle::Colorf(0.8, 0.8, 0.8));
    *_scene << _camera;// << _light;

    //gle::Material* mat = new gle::Material();
    //mat->setReflectionIntensity(1.0);
    //mat->setEnvMap(_cubeMap);
    //gle::Mesh* dd = gle::Geometries::Sphere(new gle::Material(), 5.0, 2, 2);
    //*_scene << dd;
    _loadCamaro();
    _loader = new gle::UniversalLoader();
    //_loader->setTexturesPath("./models/mustang/");
    //gle::Scene::Node* model = _loader->load("./models/Camaro.obj", NULL);
    //gle::Scene::Node* model = _loader->load("./models/mustang/Ford Mustang GT Concept.lwo", NULL);
    //gle::Scene::Node* model = _loader->load("./models/textured_cube.obj", new gle::Material());
    //gle::Texture* tex2 = new gle::Texture("./examples/cubemaps/opensea/posy.png");
    //gle::Texture* tex3 = new gle::Texture("./examples/cubemaps/opensea/posz.png");
    //gle::Material* mat = new gle::Material();
    //mat->setReflectionIntensity(0.2);
    //mat->setSpecularIntensity(0.0);
    //mat->setDiffuseIntensity(0.0);
    //mat->setColorMap(tex);
    //mat->setEnvMap(_cubeMap);
    //gle::Mesh* cube = gle::Geometries::Cube(mat, 5.0);
    //*_scene << cube << light;
    //*_scene << model;
    

    _scene->setEnvMap(_cubeMap);
    _camera->addChild(light);
    _camera->addChild(_light);
  }

  void animate()
  {

  }

  void render()
  {
    _renderer->render(_scene, gle::Rectf(0, 0, _winWidth, _winHeight));
  }

  void  catchEvent(sf::Event& event)
  {
    /*    if (event.type == sf::Event::KeyPressed
	&& event.key.code == sf::Keyboard::R)
      _bodyMaterial->setDiffuseColor(gle::Color<GLfloat>(4.0, 0.0, 0.0));
    if (event.type == sf::Event::KeyPressed
        && event.key.code == sf::Keyboard::O)
      _bodyMaterial->setDiffuseColor(gle::Color<GLfloat>(4.0, 2.0, 0.5));
    if (event.type == sf::Event::KeyPressed
        && event.key.code == sf::Keyboard::B)
      _bodyMaterial->setDiffuseColor(gle::Color<GLfloat>(0.0, 0.0, 0.0));
    if (event.type == sf::Event::KeyPressed
        && event.key.code == sf::Keyboard::V)
      _bodyMaterial->setDiffuseColor(gle::Color<GLfloat>(0.0, 1.6, 2.79));
    if (event.type == sf::Event::KeyPressed
        && event.key.code == sf::Keyboard::P)
      _bodyMaterial->setReflectionIntensity(_bodyMaterial->getReflectionIntensity() + 0.1);
    if (event.type == sf::Event::KeyPressed
        && event.key.code == sf::Keyboard::L)
	_bodyMaterial->setReflectionIntensity(_bodyMaterial->getReflectionIntensity() - 0.1);*/
    (void)event;
  }

private	:
  gle::UniversalLoader* _loader;
  gle::CubeMap* _cubeMap;
  gle::PointLight*		_light;
  gle::Mesh*			_plane;
  gle::Mesh*			_cube;
  gle::TextureFrameBuffer*	_framebuffer;
  gle::Material*		_bodyMaterial;
};

int main(int ac, char **av)
{
  try {
    App app(ac, av);
    return (app.run());
  }
  catch (gle::Exception::CompilationError* e)
    {
      std::cout << "Compilation error in " << e->getFilename() << ":\n"
		<< e->what() << std::endl;
    }
  catch (gle::Exception::InvalidOperation* e)
    {
      std::cout << "Invalid Operation: " << e->what() << std::endl;
    }
  catch (std::exception *e)
    {
      std::cout << "Error: " << e->what() << std::endl;
    }
  return (EXIT_FAILURE);
}
