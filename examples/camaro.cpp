//
// flycam.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar  2 17:27:21 2012 gael jochaud-du-plessix
// Last update Tue Jul 10 14:47:45 2012 loick michard
//

#include <iostream>

#include "TextureFrameBuffer.hpp"
#include "Example.hpp"
#include "CubeMap.hpp"
#include "ObjLoader.hpp"
#include "UniversalLoader.hpp"
#include "Mesh.hpp"
#include "Geometries.hpp"
#include "trackball.hpp"

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
      _bodyMaterial->setDiffuseColor(gle::Color<GLfloat>(2.0, 1.0, 0.25));
      _bodyMaterial->setReflectionIntensity(0.75);
      //_bodyMaterial->setEnvMap(_cubeMap);
      body->setMaterial(_bodyMaterial);
    }
    {
      gle::Mesh* mirror = dynamic_cast<gle::Mesh*>(model->getChildByName("mirror"));
      gle::Material* bodyMaterial = new gle::Material();
      bodyMaterial->setDiffuseColor(gle::Color<GLfloat>(0.1, 0.1, 0.1));
      bodyMaterial->setReflectionIntensity(1.0);
      //bodyMaterial->setEnvMap(_cubeMap);
      mirror->setMaterial(bodyMaterial);
    }
    {
      gle::Mesh* black = dynamic_cast<gle::Mesh*>(model->getChildByName("black"));
      gle::Material* bodyMaterial = new gle::Material();
      bodyMaterial->setDiffuseColor(gle::Color<GLfloat>(0.0, 0.0, 0.0));
      bodyMaterial->setSpecularIntensity(0.1);
      bodyMaterial->setReflectionIntensity(0.4);
      //bodyMaterial->setEnvMap(_cubeMap);
      black->setMaterial(bodyMaterial);
    }
    {
      gle::Mesh* mizo = dynamic_cast<gle::Mesh*>(model->getChildByName("mizo"));
      gle::Material* bodyMaterial = new gle::Material();
      bodyMaterial->setDiffuseColor(gle::Color<GLfloat>(0.0, 0.0, 0.0));
      //bodyMaterial->setEnvMap(_cubeMap);
      mizo->setMaterial(bodyMaterial);
    }
    {
      gle::Mesh* glass = dynamic_cast<gle::Mesh*>(model->getChildByName("glass"));
      gle::Material* bodyMaterial = new gle::Material();
      bodyMaterial->setDiffuseColor(gle::Color<GLfloat>(0.0, 0.0, 0.0));
      //bodyMaterial->setEnvMap(_cubeMap);
      bodyMaterial->setReflectionIntensity(0.5);
      glass->setMaterial(bodyMaterial);
    }
    {
      gle::Mesh* tire = dynamic_cast<gle::Mesh*>(model->getChildByName("tire"));
      gle::Material* bodyMaterial = new gle::Material();
      bodyMaterial->setDiffuseColor(gle::Color<GLfloat>(0.14, 0.14, 0.14));
      bodyMaterial->setSpecularLightEnabled(false);
      //bodyMaterial->setEnvMap(_cubeMap);
      tire->setMaterial(bodyMaterial);
    }
    {
      gle::Mesh* tireling = dynamic_cast<gle::Mesh*>(model->getChildByName("tireling"));
      gle::Material* bodyMaterial = new gle::Material();
      bodyMaterial->setDiffuseColor(gle::Color<GLfloat>(0.1, 0.1, 0.1));
      bodyMaterial->setSpecularLightEnabled(false);
      //bodyMaterial->setEnvMap(_cubeMap);
      tireling->setMaterial(bodyMaterial);
    }
    {
      gle::Mesh* black2 = dynamic_cast<gle::Mesh*>(model->getChildByName("black2"));
      gle::Material* bodyMaterial = new gle::Material();
      bodyMaterial->setDiffuseColor(gle::Color<GLfloat>(0.2, 0.2, 0.2));
      bodyMaterial->setSpecularLightEnabled(false);
      //bodyMaterial->setEnvMap(_cubeMap);
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
    _light = new gle::SpotLight(gle::Vector3f(60, 60, 0),
				gle::Colorf(0.8, 0.6, 0.6), 15);
    _light->setTarget(gle::Vector3f(0, 0, 0));
    _light->setInnerCutOff(13);
    gle::SpotLight* light2 = new gle::SpotLight(gle::Vector3f(0, 70, 50),
						gle::Colorf(0.5, 0.5, 0.5), 15);
    light2->setTarget(gle::Vector3f(0, 0, 0));
    light2->setInnerCutOff(13);
    gle::Material *planeMaterial = new gle::Material();
    planeMaterial->setColorMap(new gle::Texture("./examples/textures/soil.jpg"));
    planeMaterial->setNormalMap(new gle::Texture("./examples/textures/soilnormal.jpg"));
    gle::Mesh* plane = gle::Geometries::Plane(planeMaterial, 200, 200, 50, 50, 10, 10);

    gle::DirectionalLight* light3 = new gle::DirectionalLight(gle::Vector3f(-60, 60, 0),
							      gle::Colorf(0.2, 0.2, 0.2));
    *_scene << _camera << plane << _light << light2 << light3;
    _loadCamaro();
    _loader = new gle::UniversalLoader();
    trackball::zoom = 14.0;
  }

  void animate()
  {
    trackball::rotationY += 0.025 / 10;
    trackball::rotationX = 1.168;
    //trackball::zoom = 48.46;
    trackball::zoom += 0.12 / 10;
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
  gle::SpotLight*		_light;
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
