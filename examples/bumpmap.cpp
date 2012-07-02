//
// flycam.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar  2 17:27:21 2012 gael jochaud-du-plessix
// Last update Fri Jun 29 21:04:07 2012 gael jochaud-du-plessix
//

#include <iostream>

#include "TextureFrameBuffer.hpp"
#include "Example.hpp"
#include "CubeMap.hpp"
#include "ObjLoader.hpp"

#define W_WIDTH 1280
#define W_HEIGHT 720

#define W_FRAMERATE 60

int accelerometerId = 0;

class App : public Example {
public:
  App(int ac, char** av) :
    Example(ac, av, W_WIDTH, W_HEIGHT, W_FRAMERATE, "glEngine : Normal mapping"),
    _light(), _plane()
  {
    //_cameraType = Trackball;
  }

  void initScene()
  {
    _scene = new gle::Scene();
    _camera = new gle::PerspectiveCamera(gle::Vector3f(-20, 20, 0),
					 gle::Vector3f(0, 0, 0),
					 45, (GLfloat)_winWidth/_winHeight, 1, 10000);
    _renderer = new gle::Renderer();
    gle::PointLight* light = new gle::PointLight(gle::Vector3f(0, 0, 0),
						 gle::Colorf(0.8, 0.8, 0.8));
    _light = new gle::PointLight(gle::Vector3f(0, 0.0, 0),
				 gle::Colorf(0.8, 0.8, 0.8));
    _mat = new gle::Material();
    _mat2 = new gle::Material();
    _mat2->setColorMap(new gle::Texture("./examples/textures/color_map.jpg"));
    _mat2->setNormalMap(new gle::Texture("./examples/textures/normal_map.jpg"));
    _mat->setColorMap(new gle::Texture("./examples/earth.png"));
    _mat->setNormalMap(new gle::Texture("./examples/textures/earthNormalMap.png"));
    _cube = gle::Geometries::Sphere(_mat, 5, 200, 200);
    _cube->setPosition(gle::Vector3f(10.0, 0.0, 0.0));
    gle::Mesh* cube = gle::Geometries::Cube(_mat2, 5);
    cube->setPosition(gle::Vector3f(-10.0, -2.5, 0.0));

    _mat3 = new gle::Material();
    //_mat3->setColorMap(new gle::Texture("./examples/textures/soil.jpg"));
    //_mat3->setNormalMap(new gle::Texture("./examples/textures/soilnormal.jpg"));
    //_mat3->setColorMap(new gle::Texture("./examples/textures/terrainwetsand03.png"));
    //_mat3->setNormalMap(new gle::Texture("./examples/textures/terrainwetsand03_n.jpg"));
    _mat3->setColorMap(new gle::Texture("./examples/textures/terrainskstoneground.png"));
    _mat3->setNormalMap(new gle::Texture("./examples/textures/terrainskstoneground_n.jpg"));
    //_mat3->setNormalMapEnabled(false);
    gle::Mesh* plan = gle::Geometries::Plane(_mat3, 200, 200, 50, 50, 10, 10);
    plan->setPosition(gle::Vector3f(0.0, -5.0, 0.0));
    *_scene << _camera << _cube << cube << plan << _light;
    
    _camera->addChild(light);
    gle::Material* ll = new gle::Material();
    ll->setAmbientColor(gle::Color<GLfloat>(1.0, 1.0, 1.0));
    _light->addChild(gle::Geometries::Sphere(ll, 0.2, 50, 50));
    _scene->update();

  }

  void animate()
  {
    //_light->setTarget(_camera->getTarget());
    _scene->updateLights();
  }

  void render()
  {
    _renderer->render(_scene, gle::Rectf(0, 0, _winWidth, _winHeight));
  }

  void  catchEvent(sf::Event& event)
  {
    if (event.type == sf::Event::KeyPressed
	&& event.key.code == sf::Keyboard::N)
      {
	_mat->setNormalMapEnabled(!_mat->isNormalMapEnabled());
	_mat2->setNormalMapEnabled(!_mat2->isNormalMapEnabled());
	_mat3->setNormalMapEnabled(!_mat3->isNormalMapEnabled());
	_scene->update();
      }
  }

private	:
  gle::Material*		_mat;
  gle::Material*		_mat2;
  gle::Material*		_mat3;
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
