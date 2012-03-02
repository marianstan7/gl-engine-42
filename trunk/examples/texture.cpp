//
// texture.cpp for  in /home/jochau_g//dev/opengl/glEngine/examples
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Wed Feb 29 19:44:13 2012 gael jochaud-du-plessix
// Last update Fri Mar  2 16:54:02 2012 gael jochaud-du-plessix
//

#include <iostream>
#include <SFML/Window.hpp>
#include <Scene.hpp>
#include <PerspectiveCamera.hpp>
#include <Geometries.hpp>
#include <Material.hpp>
#include <Renderer.hpp>
#include <ObjLoader.hpp>
#include <DirectionalLight.hpp>
#include <PointLight.hpp>
#include <Texture.hpp>

int glEngine(int, char**);

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

  sf::Window App(sf::VideoMode(500, 500, 32), "glEngine",
		 sf::Style::Default, context);
  
  //! Print OpenGL supported version
  context = App.GetSettings();
  std::cout << context.MajorVersion << '.'
	    << context.MinorVersion << std::endl;

  App.SetActive();

  gle::Scene scene;
  gle::PerspectiveCamera camera(gle::Vector3<GLfloat>(0, 0, 0),
				gle::Vector3<GLfloat>(0, 0, 0),
				45, 1, 1, 10000);
  gle::Material material;

  material.setDiffuseLightEnabled(true);
  material.setSpecularLightEnabled(true);
  material.setShininess(32);
  material.setAmbientColor(gle::Color<GLfloat>(1, 1, 1));

  GLfloat dist = 100;

  srand(time(NULL));

  gle::ObjLoader loader;
  gle::Mesh* model = loader.load("./models/dustbin/trashbin.obj", &material);

  if (model)
    {
      model->setPosition(gle::Vector3<GLfloat>(0, -40, 0));
      model->setScale(0.7);
      scene << model;
    }

  gle::Material materialLight;

  materialLight.setDiffuseLightEnabled(false);
  materialLight.setSpecularLightEnabled(false);
  materialLight.setAmbientColor(gle::Color<GLfloat>(1.0, 1.0, 1.0));

  gle::PointLight l(gle::Vector3<GLfloat>(0, 20, 0), gle::Color<GLfloat>(0.8, 0, 0));
  gle::Mesh* sp = gle::Geometries::Sphere(&materialLight, dist/100);
  sp->setPosition(gle::Vector3<GLfloat>(0, 20, 0));
  gle::PointLight l2(gle::Vector3<GLfloat>(0, 0, -20),
		     gle::Color<GLfloat>(0, 0, 0.8));
  gle::Mesh* sp2 = gle::Geometries::Sphere(&materialLight, dist/100);
  sp2->setPosition(gle::Vector3<GLfloat>(0, 0, -20));
  gle::PointLight l3(gle::Vector3<GLfloat>(40, 0, 0),
		     gle::Color<GLfloat>(0, 0.8, 0));
  gle::Mesh* sp3 = gle::Geometries::Sphere(&materialLight, dist/100);
  sp3->setPosition(gle::Vector3<GLfloat>(40, 0, 0));

  scene << &camera << &material << &materialLight << sp << sp2 << sp3;
  scene << &l << &l2 << &l3;

  gle::Renderer renderer;
  renderer.createPrograms(&scene);

  GLfloat angle = 0;

  sf::Clock clock;
  sf::Clock time;

  size_t frameCounter = 0;
  
  while (App.IsOpen())
    {
      if (time.GetElapsedTime().AsMilliseconds() >= 100)
	{
	  std::cout << "fps:" << (frameCounter * 10) << std::endl;
	  frameCounter = 0;
	  time.Restart();
	}
      frameCounter++;
      sf::Event Event;
      while (App.PollEvent(Event))
	{
	  // Close window : exit
	  if (Event.Type == sf::Event::Closed)
	    App.Close();
	  else if (Event.Type == sf::Event::KeyPressed
		   && Event.Key.Code == sf::Keyboard::Escape)
	    App.Close();
	  // else if (Event.Type == sf::Event::MouseMoved)
	  //   {
	  //     angle = ((GLfloat)Event.MouseMove.X / 500) * 20;
	  //   }
	  // Adjust the viewport when the window is resized
	  if (Event.Type == sf::Event::Resized)
	    glViewport(0, 0, Event.Size.Width, Event.Size.Height);
	}
      angle = (float)clock.GetElapsedTime().AsMilliseconds() / 1000;
      camera.setPosition(gle::Vector3<GLfloat>(cos(angle/3) * -dist,
					       0,//sin(angle/3) * dist/2,
					       sin(angle/3) * -dist));
      float dist2 = dist / 3;
      sp->setPosition(gle::Vector3<GLfloat>(sin(angle/2) * dist2, cos(angle/2) * dist2, 0));
      l.setPosition(gle::Vector3<GLfloat>(sin(angle/2) * dist2, cos(angle/2) * dist2, 0));

      sp2->setPosition(gle::Vector3<GLfloat>(sin(angle/2) * dist2, 0, cos(angle/2) * dist2));
      l2.setPosition(gle::Vector3<GLfloat>(sin(angle/2) * dist2, 0, cos(angle/2) * dist2));

      sp3->setPosition(gle::Vector3<GLfloat>(0, sin(angle/2) * dist2, cos(angle/2) * dist2));
      l3.setPosition(gle::Vector3<GLfloat>(0, sin(angle/2) * dist2, cos(angle/2) * dist2));

      scene.updateLights();

      renderer.render(&scene);
      App.Display();
    }
  
  return (0);
}