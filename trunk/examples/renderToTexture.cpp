//
// renderToTexture.cpp for  in /home/jochau_g//dev/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Thu May 31 19:08:59 2012 gael jochaud-du-plessix
// Last update Wed Jul  4 22:21:29 2012 loick michard
//

#include <iostream>

#include "Example.hpp"

#include "ObjLoader.hpp"
#include "FrameBuffer.hpp"
#include "TextureFrameBuffer.hpp"

#define W_WIDTH 1280
#define W_HEIGHT 720

#define W_FRAMERATE 60

class App : public Example {
public:
  App(int ac, char** av) :
    Example(ac, av, W_WIDTH, W_HEIGHT, W_FRAMERATE, "glEngine : Render To Texture"),
    _textureScene(), _textureFB(), _textureMesh()
  {
    _cameraType = Trackball;
  }

  void initScene()
  {
    _renderer = new gle::Renderer();

    // Build scene
    _scene = new gle::Scene();
    _camera = new gle::PerspectiveCamera(gle::Vector3f(-20, 0, 0),
    					 gle::Vector3f(0, 0, 0),
    					 45, (GLfloat)_winWidth/_winHeight, 1, 10000);

    *_scene << _camera;

    gle::PointLight* light = new gle::PointLight(gle::Vector3f(0, 0, 0),
    						 gle::Colorf(1, 1, 1));

    _camera->addChild(light);

    gle::Material* cubeMaterial = new gle::Material();

    gle::Mesh* cube = gle::Geometries::Cube(cubeMaterial, 5);

    *_scene << cube;

    // Build texture scene
    _textureScene = new gle::Scene();
    _textureScene->setBackgroundColor(gle::Colorf(1, 1, 1));
    gle::Camera* camera2 = new gle::PerspectiveCamera(gle::Vector3f(-20, 10, 10),
    						     gle::Vector3f(0, 2, 0),
    						     45, (GLfloat)500/500, 1, 10000);
    
    *_textureScene << camera2;

    gle::PointLight* light2 = new gle::PointLight(gle::Vector3f(-20, 0, 10),
    						  gle::Colorf(0.9, 0.9, 0.9));

    camera2->addChild(light2);

    gle::Material* redMaterial = new gle::Material();
    redMaterial->setDiffuseColor(gle::Colorf(1, 0, 0));

    gle::ObjLoader* loader = new gle::ObjLoader();
    _textureMesh = loader->load("./models/Camaro.obj", redMaterial);

    if (!_textureMesh)
      throw new gle::Exception::Exception("Cannot load model");

    *_textureScene << _textureMesh;


    // Build the framebuffer and attach it to the cube texture
    _textureFB = new gle::TextureFrameBuffer(500, 500);

    cubeMaterial->setColorMap(_textureFB->getRenderTexture());
  }

  void render()
  {
    _textureMesh->setRotation(gle::Vector3f(0, 1, 0), _time.getElapsedTime().asMilliseconds() / 10);
    _renderer->render(_textureScene, _textureFB->getRenderTexture()->getSize(), _textureFB);
    _renderer->render(_scene, gle::Rectf(0, 0, _winWidth, _winHeight));
  }

private:
  gle::Scene*				_textureScene;  
  gle::TextureFrameBuffer*		_textureFB;
  gle::Scene::Node*			_textureMesh;
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
