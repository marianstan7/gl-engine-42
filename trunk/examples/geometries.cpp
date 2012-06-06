//
// flycam.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar  2 17:27:21 2012 gael jochaud-du-plessix
// Last update Wed Jun  6 16:40:35 2012 loick michard
//

#include <iostream>

#include "TextureFrameBuffer.hpp"
#include "Example.hpp"

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
    _camera = new gle::PerspectiveCamera(gle::Vector3f(-20, 10, 0),
					 gle::Vector3f(0, 0, 0),
					 45, (GLfloat)_winWidth/_winHeight, 1, 10000);
    _renderer = new gle::Renderer();
    
    //_scene->setBackgroundColor(gle::Colorf(1, 1, 1));
    _scene->setFogColor(gle::Colorf(0, 0, 0));
    _scene->setFogDensity(0.005);

    gle::Material* materialLight = new gle::Material();

    materialLight->setDiffuseLightEnabled(true);
    materialLight->setSpecularLightEnabled(true);
    
    gle::Material* material = new gle::Material();
    material->setDiffuseLightEnabled(true);
    material->setSpecularLightEnabled(true);
    material->setColorMap(new gle::Texture("./examples/earth.jpg"));

    gle::Material* material2 = new gle::Material();
    material2->setDiffuseLightEnabled(true);
    material2->setSpecularLightEnabled(true);
    
    _cube = gle::Geometries::Cube(material, 10);
    _cube->setPosition(gle::Vector3f(8, 0, -8));
    _cube->setRotation(gle::Vector3f(0, 1, 0), 45);
    
    gle::Mesh* pointCube = gle::Geometries::Cube(material2, 10);
    pointCube->setPosition(gle::Vector3f(-24, 0, -8));
    pointCube->setPrimitiveType(gle::Mesh::Points);
    pointCube->setPointSize(5);
    gle::Mesh* sphere = gle::Geometries::Sphere(material, 5, 30, 30);
    sphere->setPosition(gle::Vector3f(8, 0, 8));
    gle::Mesh* pointSphere = gle::Geometries::Sphere(material2, 5, 100, 100);
    pointSphere->setPosition(gle::Vector3f(-24, 0, 8));
    pointSphere->setPrimitiveType(gle::Mesh::Points);
    pointSphere->setScale(3);
    _plane = gle::Geometries::Plane(materialLight, 80, 80, 1, 1);
    _plane->setPosition(gle::Vector3f(0, -5, 0));
    gle::Mesh* pointPlane =
      gle::Geometries::Plane(material2, 80, 80, 100, 100);
    pointPlane->setPosition(gle::Vector3f(0, -5, 80));
    pointPlane->setPrimitiveType(gle::Mesh::Points);
    pointPlane->setPointSize(2);
    
    gle::Mesh* wiredSphere = new gle::Mesh(*sphere);
    wiredSphere->setPosition(gle::Vector3f(-8, 0, 8));
    wiredSphere->setRasterizationMode(gle::Mesh::Line);
    wiredSphere->setMaterial(material2);

    gle::Mesh* wiredCube = new gle::Mesh(*_cube);
    wiredCube->setPosition(gle::Vector3f(-8, 0, -8));
    wiredCube->setRasterizationMode(gle::Mesh::Line);
    wiredCube->setMaterial(material2);

    gle::Mesh* wiredPlane = new gle::Mesh(*_plane);
    wiredPlane->setPosition(gle::Vector3f(0, -5, -80));
    wiredPlane->setRasterizationMode(gle::Mesh::Line);
    wiredPlane->setMaterial(material2);

    *_scene << _cube << wiredCube << pointCube;
    *_scene << sphere << wiredSphere << pointSphere;
    *_scene << _plane << wiredPlane << pointPlane;

    _light = new gle::PointLight(gle::Vector3f(0, 0, 0),
				 gle::Colorf(1.0, 1.0, 1.0));
    gle::Mesh* sp = gle::Geometries::Sphere(materialLight, 10, 50, 50);
    sp->setPosition(gle::Vector3f(0, 0, 0));
    
    gle::Material* screenMaterial = new gle::Material();
    screenMaterial->setSpecularLightEnabled(false);
    gle::Mesh* screenPlane = gle::Geometries::Plane(screenMaterial, 16*2, 9*2);
    screenPlane->rotate(gle::Vector3f(1, 0, 0), 90);
    screenPlane->setPosition(gle::Vector3f(0, 20, -50));

    _framebuffer = new gle::TextureFrameBuffer(500, 500);
    screenMaterial->setColorMap(_framebuffer->getRenderTexture());
    screenMaterial->setAmbientColor(gle::Color<GLfloat>(1.0, 1.0, 1.0));

    *_scene << screenPlane;

    gle::PointLight* _light2 = new gle::PointLight(gle::Vector3f(0, 200, 0),
						   gle::Colorf(1.0, 0.0, 0.0));

    *_scene << _camera;// << _light2;// << sp;
    _camera->addChild(_light);
    _scene->updateScene();

    //printGPUMemInfo();
  }

  void animate()
  {
    _scene->updateLights();
    //_plane->setRotation(gle::Vector3<GLfloat>(1, 0, 0),
    //			-sf::Joystick::getAxisPosition(accelerometerId,
    //						       sf::Joystick::X));
  }

  void render()
  {
    _renderer->render(_scene, _framebuffer->getRenderTexture()->getSize(), _framebuffer);
    _renderer->render(_scene, gle::Rectf(0, 0, _winWidth, _winHeight));
  }

private:
  gle::PointLight*		_light;
  gle::Mesh*			_plane;
  gle::Mesh*			_cube;
  gle::TextureFrameBuffer*	_framebuffer;
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
