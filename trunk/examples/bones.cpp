//
// flycam.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar  2 17:27:21 2012 gael jochaud-du-plessix
// Last update Tue Jun 26 21:01:11 2012 loick michard
//

#include <iostream>
#include <Bone.hpp>
#include <Skeleton.hpp>
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
    Example(ac, av, W_WIDTH, W_HEIGHT, W_FRAMERATE, "glEngine : Bones")
  {
    //printGPUMemInfo();
    _cameraType = Trackball;
    //_recordVideo = true;
  }

  void initScene()
  {
    _scene = new gle::Scene();
    //_scene->setBackgroundColor(gle::Colorf(1.0, 1.0, 1.0));
    _camera = new gle::PerspectiveCamera(gle::Vector3f(-20, 10, 0),
					 gle::Vector3f(0, 0, 0),
					 45, (GLfloat)_winWidth/_winHeight, 1, 1000);
    _renderer = new gle::Renderer();
        _renderer->setDebugMode(gle::Renderer::Bone);

    gle::Material* material = new gle::Material();
    material->setDiffuseColor(gle::Colorf(1.0, 0.0, 0.0));

    gle::Mesh* sphere = gle::Geometries::Sphere(material, 5, 30, 30);
    gle::Mesh* sphere2 = gle::Geometries::Sphere(material, 5, 30, 30);
    sphere->setPosition(gle::Vector3f(-20, 0, 0));
    sphere2->setPosition(gle::Vector3f(20, 0, 0));
    *_scene << _camera << sphere << sphere2;
    
    _light = new gle::PointLight(gle::Vector3f(0, 0, 0),
				 gle::Colorf(1.0, 1.0, 1.0));

    _bone = new gle::Bone(10.0);
    _bone2 = new gle::Bone(6.0);
    _bone2->setPosition(gle::Vector3f(0.0, 10.0, 0.0));
    _bone->addChild(_bone2);
    _skeleton = new gle::Skeleton();
    _skeleton->addChild(_bone);

    gle::Mesh* bras = gle::Geometries::Cuboid(material, 2, 20, 2);
    bras->setSkeleton(_skeleton);
    //bras->translate(gle::Vector3f(0.0, 5.0, 0.0));
    GLfloat bones[] = {0, 1, -1, -1, 0, 1, -1, -1, 0, 1, -1, -1, 0, 1, -1, -1,
		       0, 1, -1, -1, 0, 1, -1, -1, 0, 1, -1, -1, 0, 1, -1, -1,
		       0, 1, -1, -1, 0, 1, -1, -1, 0, 1, -1, -1, 0, 1, -1, -1,
		       0, 1, -1, -1, 0, 1, -1, -1, 0, 1, -1, -1, 0, 1, -1, -1,
		       0, 1, -1, -1, 0, 1, -1, -1, 0, 1, -1, -1, 0, 1, -1, -1,
		       0, 1, -1, -1, 0, 1, -1, -1, 0, 1, -1, -1, 0, 1, -1, -1};
    bras->setBones(bones, sizeof(bones) / sizeof(GLfloat));

    gle::Mesh* bras2 = gle::Geometries::Cuboid(material, 2, 12, 2);
    bras2->setSkeleton(_skeleton);
    GLfloat bones2[] = {1, 1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1,
		       1, 1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1,
		       1, 1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1,
		       1, 1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1,
		       1, 1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1,
		       1, 1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1};
    //bras->translate(gle::Vector3f(0.0, 5.0, 0.0));
    bras2->setBones(bones2, sizeof(bones2) / sizeof(GLfloat));

    *_scene << _skeleton << bras << bras2;

    _camera->addChild(_light);
    _scene->update();

    //printGPUMemInfo();
  }

  void animate()
  {
    //_scene->updateLights();
    _scene->updateSkeletons();
  }

  void render()
  {
    _renderer->render(_scene, gle::Rectf(0, 0, _winWidth, _winHeight));
  }

  void  catchEvent(sf::Event& event)
  {
    static GLfloat angle = 0.0;
    static GLfloat angle2 = 0.0;
    if (event.type == sf::Event::KeyPressed
        && event.key.code == sf::Keyboard::W)
      {
	_bone->setRotation(gle::Vector3f(0.0, 0.0, 1), angle);
	angle += 0.5;
      }
    if (event.type == sf::Event::KeyPressed
        && event.key.code == sf::Keyboard::S)
      {
	_bone->setRotation(gle::Vector3f(0.0, 0.0, 1), angle);
	angle -= 0.5;
      }
    if (event.type == sf::Event::KeyPressed
        && event.key.code == sf::Keyboard::A)
      {
	_bone2->setRotation(gle::Vector3f(0.0, 0.0, 1), angle2);
	angle2 += 0.5;
      }
    if (event.type == sf::Event::KeyPressed
        && event.key.code == sf::Keyboard::D)
      {
	_bone2->setRotation(gle::Vector3f(0.0, 0.0, 1), angle2);
	angle2 -= 0.5;
      }
  }

private	:
  gle::PointLight*		_light;
  gle::Bone*			_bone;
  gle::Bone*			_bone2;
  gle::Skeleton*		_skeleton;
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
