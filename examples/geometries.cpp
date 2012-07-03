//
// flycam.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar  2 17:27:21 2012 gael jochaud-du-plessix
// Last update Tue Jul  3 02:10:02 2012 loick michard
//

#include <iostream>

#include "TextureFrameBuffer.hpp"
#include "Example.hpp"
#include "CubeMap.hpp"

#define W_WIDTH 1280
#define W_HEIGHT 720

#define W_FRAMERATE 60

int accelerometerId = 0;

class App : public Example {
public:
  App(int ac, char** av) :
    Example(ac, av, W_WIDTH, W_HEIGHT, W_FRAMERATE, "glEngine : Geometries"),
    _light(), _plane()
  {
    //printGPUMemInfo();
    //_cameraType = Trackball;
    //_recordVideo = true;
  }

  void initScene()
  {
    _scene = new gle::Scene();
    //_scene->generateTree();
    //_scene->enableFrustumCulling();
    _camera = new gle::PerspectiveCamera(gle::Vector3f(-20, 10, 0),
					 gle::Vector3f(0, 0, 0),
					 45, (GLfloat)_winWidth/_winHeight, 1, 10000);
    _renderer = new gle::Renderer();
    _renderer->setDebugMode(gle::Renderer::Camera);
    //_scene->setBackgroundColor(gle::Colorf(1, 1, 1));
    _scene->setFogColor(gle::Colorf(0, 0, 0));
    _scene->setFogDensity(0.005);

    gle::Material* materialLight = new gle::Material();

    materialLight->setDiffuseLightEnabled(true);
    materialLight->setSpecularLightEnabled(true);
    materialLight->setDiffuseColor(gle::Colorf(0.0, 0.0, 1.0));
    
    gle::Material* material = new gle::Material();
    material->setDiffuseLightEnabled(true);
    material->setSpecularLightEnabled(true);
    material->setDiffuseColor(gle::Colorf(1.0, 0.0, 0.0));
    //material->setColorMap(new gle::Texture("./examples/earth.jpg"));

    gle::Material* material2 = new gle::Material();
    material2->setDiffuseLightEnabled(true);
    material2->setSpecularLightEnabled(true);
    material2->setDiffuseColor(gle::Colorf(0.0, 1.0, 0.0));

    gle::Mesh* sphere = gle::Geometries::Sphere(material, 5, 30, 30, true);
    sphere->setPosition(gle::Vector3f(8, 5, 8));
    
    _cube = gle::Geometries::Cube(material2, 10, true);
    _cube->setPosition(gle::Vector3f(8, 0, -8));
    _cube->setRotation(gle::Vector3f(0, 1, 0), 45);
 
    gle::Mesh* pointCube = gle::Geometries::Cube(material2, 10);
    pointCube->setPosition(gle::Vector3f(-24, 0, -8));
    pointCube->setPrimitiveType(gle::Mesh::Points);
    pointCube->setPointSize(5);
    gle::Mesh* pointSphere = gle::Geometries::Sphere(material2, 5, 100, 100);
    pointSphere->setPosition(gle::Vector3f(-24, 0, 8));
    pointSphere->setPrimitiveType(gle::Mesh::Points);

    _plane = gle::Geometries::Plane(materialLight, 80, 80, 1, 1);
    _plane->setPosition(gle::Vector3f(0, -5, 0));
    gle::Mesh* pointPlane =
      gle::Geometries::Plane(material2, 80, 80, 100, 100);
    pointPlane->setPosition(gle::Vector3f(0, -5, 80));
    pointPlane->setPrimitiveType(gle::Mesh::Points);
    pointPlane->setPointSize(2);
    
    _wiredSphere = new gle::Mesh(*sphere);
    _wiredSphere->setPosition(gle::Vector3f(-8, 0, 8));
    _wiredSphere->setRasterizationMode(gle::Mesh::Line);
    _wiredSphere->setMaterial(material2);
    _wiredSphere->setDynamic(true);

    gle::Mesh* wiredCube = new gle::Mesh(*_cube);
    wiredCube->setPosition(gle::Vector3f(-8, 0, -8));
    wiredCube->setRasterizationMode(gle::Mesh::Line);
    wiredCube->setMaterial(material2);

    gle::Mesh* wiredPlane = new gle::Mesh(*_plane);
    wiredPlane->setPosition(gle::Vector3f(0, -5, -80));
    wiredPlane->setRasterizationMode(gle::Mesh::Line);
    wiredPlane->setMaterial(material2);

    *_scene << sphere << _wiredSphere << pointSphere;
    *_scene << _cube << wiredCube << pointCube;
    *_scene << _plane << wiredPlane << pointPlane;

    gle::DirectionalLight* ambientLight = new gle::DirectionalLight(gle::Vector3f(0.2, 1, 0),
    								    gle::Colorf(0.2, 0.2, 0.2));
    *_scene << ambientLight;

    _light = new gle::SpotLight(gle::Vector3f(-53.2, 59, -51),
				gle::Colorf(1.0, 1.0, 1.0), 1.0);
    _light->setTarget(gle::Vector3f(0, 0, 0));
    _lightCamera = _light->getShadowMapCamera();

    *_scene << _lightCamera;

    gle::Mesh* sp = gle::Geometries::Sphere(materialLight, 10, 50, 50);
    sp->setPosition(gle::Vector3f(0, 0, 0));
    
    gle::Material* screenMaterial = new gle::Material();
    screenMaterial->setSpecularLightEnabled(false);
    screenMaterial->setDiffuseLightEnabled(false);
    gle::Mesh* screenPlane = gle::Geometries::Plane(screenMaterial, 16*2, 9*2);

    screenPlane->setPosition(gle::Vector3f(0, 20, -50));
    screenPlane->rotate(gle::Vector3f(1, 0, 0), 90);

    _framebuffer = new gle::TextureFrameBuffer(500, 500);
    //screenMaterial->setColorMap(_framebuffer->getRenderTexture());
    screenMaterial->setColorMap(_light->getShadowMap());
    screenMaterial->setAmbientColor(gle::Color<GLfloat>(1.0, 1.0, 1.0));

    *_scene << screenPlane;

    *_scene << _camera << _light;// << _light2;// << sp;
    
    _scene->update();

    //printGPUMemInfo();
  }

  void animate()
  {
    _light->setPosition(gle::Vector3f(-53.2 * sin(_elapsedTime / 1000.0), 59.0, -51));
    //_wiredSphere->setPosition(gle::Vector3f(-8, 5 + 5 * sin(_elapsedTime / 1000.0), 8));
    _wiredSphere->setRotation(gle::Vector3f(0, 1, 0), _elapsedTime / 50.0);
    _scene->updateLights();
  }

  void render()
  {
    // _scene->setCurrentCamera(_lightCamera);
    // _scene->updateLights();
    // _renderer->render(_scene, _framebuffer->getRenderTexture()->getSize(), _framebuffer);
    _scene->setCurrentCamera(_camera);
    _scene->updateLights();
    _renderer->render(_scene, gle::Rectf(0, 0, _winWidth, _winHeight));
  }

  void  catchEvent(sf::Event& event)
  {
    if (event.type == sf::Event::KeyPressed
	&& event.key.code == sf::Keyboard::O)
      _light->setCosCutOff(_light->getCosCutOff() + 0.02);
    if (event.type == sf::Event::KeyPressed
        && event.key.code == sf::Keyboard::P)
      _light->setCosCutOff(_light->getCosCutOff() - 0.02);
  }

private	:
  gle::SpotLight*		_light;
  gle::Mesh*			_plane;
  gle::Mesh*			_cube;
  gle::Mesh*			_wiredSphere;
  gle::TextureFrameBuffer*	_framebuffer;
  gle::Camera*			_lightCamera;
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
  catch (std::exception *e)
    {
      std::cout << "Error: " << e->what() << std::endl;
    }
  return (EXIT_FAILURE);
}
