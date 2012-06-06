//
// flycam.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar  2 17:27:21 2012 gael jochaud-du-plessix
// Last update Wed Jun  6 19:54:57 2012 gael jochaud-du-plessix
//

#include <iomanip>
#include <iostream>
#include <sstream>
#include <map>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <Scene.hpp>
#include <PerspectiveCamera.hpp>
#include <Geometries.hpp>
#include <Material.hpp>
#include <Renderer.hpp>
#include <ObjLoader.hpp>
#include <DirectionalLight.hpp>
#include <PointLight.hpp>
#include <MeshBufferManager.hpp>

#include "flycam.hpp"
#include "video.hpp"

#define W_WIDTH 1280
#define W_HEIGHT 720

#define W_FRAMERATE 30

int accelerometerId = 0;

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
  context.depthBits = 24;
  context.stencilBits = 24;
  context.antialiasingLevel = 2;
  context.majorVersion = 3;
  context.minorVersion = 3;

  sf::Window App(sf::VideoMode(W_WIDTH, W_HEIGHT, 32), "glEngine",
		 sf::Style::Default, context);  
  
  //! Print OpenGL supported version
  context = App.getSettings();
  std::cout << context.majorVersion << '.'
	    << context.minorVersion << std::endl;

  App.setActive();

  gle::Scene scene;
  //scene.displayBoundingVolume();
  gle::PerspectiveCamera camera(gle::Vector3<GLfloat>(-20, 10, 0),
				gle::Vector3<GLfloat>(0, 0, 0),
				45, (GLfloat)W_WIDTH/W_HEIGHT, 1, 1000);

  gle::ObjLoader loader;
  gle::Material material;
  material.setDiffuseLightEnabled(true);
  material.setSpecularLightEnabled(true);
  material.setColorMap(new gle::Texture("./examples/earth.jpg"));

  gle::Mesh* cube = gle::Geometries::Sphere(&material, 5, 30, 30);
  for (int i = 0; i < 200; ++i)
    {
      for (int j = 0; j < 200; j++)
	
	{	  
	  gle::Mesh* cubeTmp = new gle::Mesh(*cube);
	  cubeTmp->setPosition(gle::Vector3<GLfloat>(i * 25, (int)random() % 300, j * 25));
	  //cubeTmp->setRotation(gle::Vector3<GLfloat>(0, 1, 0), 90);
	  //cubeTmp->setScale(1, 1, 1);
	  scene << cubeTmp;
	  /*  gle::Mesh* sphere = gle::Geometries::Sphere(&material, 5);
	      sphere->setPosition(gle::Vector3<GLfloat>(8, 0, 8));
	  */	      
	  
	}

	}/*
    gle::Mesh* cube = gle::Geometries::Cube(&material, 10);
    cube->setPosition(gle::Vector3<GLfloat>(0, 0, 0));
    cube->setScale(0.5);
    //cube->setPosition(gle::Vector3<GLfloat>(0, 0, 0));
    scene << cube;*/

  srand(time(NULL));

  gle::PointLight l(gle::Vector3<GLfloat>(0, 2000, 0),
		    gle::Color<GLfloat>(0.8, 0.8, 0.8));
  scene << &camera;
  scene << &l;

  gle::Renderer renderer;

  sf::Clock clock;
  sf::Clock time;
  sf::Clock frameTimer;
  int frameCounter = 0;

  App.setMouseCursorVisible(false);
  //scene.generateTree();
  //scene.displayTree();
  scene.enableFrustumCulling();
  scene.update();
  while (App.isOpen())
    {    
      if (frameTimer.getElapsedTime().asMilliseconds() >= 1000)
        {
	  std::cout << "fps:" << (frameCounter * 1) << std::endl;
          frameCounter = 0;
          frameTimer.restart();
        }
      frameCounter++;
  
      sf::Event Event;
      while (App.pollEvent(Event))
	{
	  // Close window : exit
	  if (Event.type == sf::Event::Closed)
	    App.close();
	  else if (Event.type == sf::Event::KeyPressed
		   && Event.key.code == sf::Keyboard::Escape)
	    App.close();
	  // Adjust the viewport when the window is resized
	  if (Event.type == sf::Event::Resized)
	    glViewport(0, 0, Event.size.width, Event.size.height);
	  flycam::event(Event, App);
	}

      sf::Mouse::setPosition(sf::Vector2i(W_WIDTH/2, W_HEIGHT/2), App);
      flycam::flycam(&camera);
      l.setPosition(camera.getAbsolutePosition());
      scene.updateLights();
      renderer.render(&scene, gle::Rectf(0, 0, W_WIDTH, W_HEIGHT));
      App.display();
      GLfloat elapsed = time.getElapsedTime().asMicroseconds();
      if (1000000/W_FRAMERATE - elapsed > 0)
	{
	  sf::sleep(sf::microseconds(1000000/W_FRAMERATE - elapsed));
	  time.restart();
	}
    }

  return (0);
}
