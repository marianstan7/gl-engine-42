//
// flycam.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar  2 17:27:21 2012 gael jochaud-du-plessix
// Last update Fri Mar  2 19:01:38 2012 gael jochaud-du-plessix
//

#include <iostream>
#include <map>
#include <SFML/Window.hpp>
#include <Scene.hpp>
#include <PerspectiveCamera.hpp>
#include <Geometries.hpp>
#include <Material.hpp>
#include <Renderer.hpp>
#include <ObjLoader.hpp>
#include <DirectionalLight.hpp>
#include <PointLight.hpp>

int glEngine(int, char**);

GLfloat teta = M_PI / 2;
GLfloat phi = 0;
std::map<sf::Keyboard::Key, bool> keyState;

void flycam(gle::Camera* camera)
{
  gle::Vector3<GLfloat> pos = camera->getPosition();
  gle::Vector3<GLfloat> viewVector;
  viewVector.x = sin(teta) * cos(phi);
  viewVector.y = cos(teta);
  viewVector.z = sin(teta) * sin(phi);

  if (keyState[sf::Keyboard::Up])
    pos += viewVector;
  if (keyState[sf::Keyboard::Down])
    pos -= viewVector;
  if (keyState[sf::Keyboard::Left])
    {
      phi -= M_PI / 2;
      viewVector.x = sin(teta) * cos(phi);
      viewVector.y = cos(teta);
      viewVector.z = sin(teta) * sin(phi);
      pos += viewVector;
      phi += M_PI / 2;
    }
  if (keyState[sf::Keyboard::Right])
    {
      phi -= M_PI / 2;
      viewVector.x = sin(teta) * cos(phi);
      viewVector.y = cos(teta);
      viewVector.z = sin(teta) * sin(phi);
      pos -= viewVector;
      phi += M_PI / 2;
    }
  if (keyState[sf::Keyboard::W])
    teta -= M_PI / 5000;
  if (keyState[sf::Keyboard::S])
    teta += M_PI / 5000;
  if (keyState[sf::Keyboard::A])
    phi -= M_PI / 5000;
  if (keyState[sf::Keyboard::D])
    phi += M_PI / 5000;
  viewVector.x = sin(teta) * cos(phi);
  viewVector.y = cos(teta);
  viewVector.z = sin(teta) * sin(phi);
  gle::Vector3<GLfloat> target = pos + viewVector;
  camera->setTarget(target);
  camera->setPosition(pos);
}

int main(int ac, char **av)
{
  int ret;

  try {
    ret = glEngine(ac, av);
  }
  catch (std::exception *e)
    {
      std::cout << "Error: " << e->what() << std::endl;
    }
  return (ret);
}

int glEngine(int ac, char **av)
{
  (void)ac;
  (void)av;
  sf::ContextSettings context;
  context.DepthBits = 24;
  context.StencilBits = 24;
  context.AntialiasingLevel = 2;
  context.MajorVersion = 3;
  context.MinorVersion = 3;

  sf::Window App(sf::VideoMode(800, 600, 32), "glEngine",
		 sf::Style::Default, context);
  
  //! Print OpenGL supported version
  context = App.GetSettings();
  std::cout << context.MajorVersion << '.'
	    << context.MinorVersion << std::endl;

  App.SetActive();

  gle::Scene scene;
  gle::PerspectiveCamera camera(gle::Vector3<GLfloat>(-100, 0, 0),
				gle::Vector3<GLfloat>(0, 0, 0),
				45, (GLfloat)800/600, 1, 10000);

  gle::ObjLoader loader;
  gle::Mesh* model = loader.load("./models/dustbin/trashbin.obj", NULL);

  if (model)
    scene << model;

  srand(time(NULL));

  gle::Material materialLight;

  materialLight.setDiffuseLightEnabled(true);
  materialLight.setSpecularLightEnabled(true);
  gle::PointLight l(gle::Vector3<GLfloat>(0, 200, 0),
		    gle::Color<GLfloat>(0.8, 0.8, 0.8));
  gle::Mesh* sp = gle::Geometries::Sphere(&materialLight, 10);
  sp->setPosition(gle::Vector3<GLfloat>(0, 200, 0));

  scene << &camera << &materialLight << sp;
  scene << &l;

  gle::Renderer renderer;
  renderer.createPrograms(&scene);

  sf::Clock clock;
  sf::Clock time;
  
  while (App.IsOpen())
    {      
      sf::Event Event;
      while (App.PollEvent(Event))
	{
	  // Close window : exit
	  if (Event.Type == sf::Event::Closed)
	    App.Close();
	  else if (Event.Type == sf::Event::KeyPressed
		   && Event.Key.Code == sf::Keyboard::Escape)
	    App.Close();
	  // Adjust the viewport when the window is resized
	  if (Event.Type == sf::Event::Resized)
	    glViewport(0, 0, Event.Size.Width, Event.Size.Height);
	  if (Event.Type == sf::Event::KeyPressed)
	    keyState[Event.Key.Code] = true;
	  else if (Event.Type == sf::Event::KeyReleased)
	    keyState[Event.Key.Code] = false;
	}
      flycam(&camera);
      scene.updateLights();
      renderer.render(&scene);
      App.Display();
      GLfloat elapsed = time.GetElapsedTime().AsMicroseconds();
      if (16666 - elapsed > 0)
	sf::Sleep(sf::Microseconds(16666));
    }
  
  return (0);
}
