//
// flycam.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar  2 17:27:21 2012 gael jochaud-du-plessix
// Last update Fri Apr  6 23:35:17 2012 gael jochaud-du-plessix
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

#include <btBulletDynamicsCommon.h>

#include "flycam.hpp"
#include "video.hpp"

#define W_WIDTH 1280
#define W_HEIGHT 720
#define W_FRAMERATE 60

#define NB_HOUSES 10

int glEngine(int, char**);

int main(int ac, char **av)
{
  int ret;

  try {
    ret = glEngine(ac, av);
  }
  catch (gle::Exception::ParsingError* e)
    {
      std::cout << e->getLine() << ":" << e->what() << std::endl;
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

  gle::PerspectiveCamera camera(gle::Vector3<GLfloat>(-100, 20, 0),
				gle::Vector3<GLfloat>(0, 0, 0),
				45, (GLfloat)W_WIDTH/W_HEIGHT, 1, 10000);

  gle::Material material;
  material.setDiffuseLightEnabled(true);
  material.setSpecularLightEnabled(true);
  material.setAmbientColor(gle::Color<GLfloat>(0.2, 0.2, 0.2));

  gle::Mesh* plane = gle::Geometries::Plane(&material, 80, 80);

  GLfloat plane_angle = 15.0;

  scene << plane;

  gle::Mesh* cube = gle::Geometries::Cube(&material, 5);
  cube->setPosition(gle::Vector3<GLfloat>(0, 50, 0));

  scene << cube;

  srand(time(NULL));

  gle::Material materialLight;

  materialLight.setDiffuseLightEnabled(true);
  materialLight.setSpecularLightEnabled(true);
  gle::PointLight l(gle::Vector3<GLfloat>(0, 2000, 0),
		    gle::Color<GLfloat>(0.8, 0.8, 0.8));
  gle::Mesh* sp = gle::Geometries::Cube(&materialLight, 10);
  sp->setPosition(gle::Vector3<GLfloat>(0, 200, 0));

  scene << &camera << &materialLight << sp;
  scene << &l;

  gle::Renderer renderer;
  renderer.createPrograms(&scene);

  sf::Clock clock;
  sf::Clock time;

  App.setMouseCursorVisible(false);

  sf::Clock frameTimer;
  int frameCounter = 0;

  btBroadphaseInterface* broadphase = new btDbvtBroadphase();
  btDefaultCollisionConfiguration* collisionConfiguration =
    new btDefaultCollisionConfiguration();
  btCollisionDispatcher* dispatcher =
    new btCollisionDispatcher(collisionConfiguration);
  btSequentialImpulseConstraintSolver* solver =
    new btSequentialImpulseConstraintSolver;
  btDiscreteDynamicsWorld* dynamicsWorld =
    new btDiscreteDynamicsWorld(dispatcher, broadphase, solver,
				collisionConfiguration);

  dynamicsWorld->setGravity(btVector3(0,-10,0));

  btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),0);
  btCollisionShape* fallShape = new btBoxShape(btVector3(2.5, 2.5, 2.5));

  btDefaultMotionState* groundMotionState =
    new btDefaultMotionState(btTransform(btQuaternion(btVector3(1,0,0),
						      plane_angle
						      * M_PI / 180.0),
					 btVector3(0,0,0)));

  btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0,0,0));
  groundRigidBodyCI.m_friction = 1;
  btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

  dynamicsWorld->addRigidBody(groundRigidBody);

  btDefaultMotionState* fallMotionState =
    new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),
					 btVector3(0,50,0)));

  btScalar mass = 1;
  btVector3 fallInertia(0,0,0);
  fallShape->calculateLocalInertia(mass,fallInertia);
  
  btRigidBody::btRigidBodyConstructionInfo
    fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
  fallRigidBodyCI.m_restitution = 0;
  fallRigidBodyCI.m_friction = 1;
  btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
  fallRigidBody->setLinearVelocity(btVector3(0, 10, 5));
  dynamicsWorld->addRigidBody(fallRigidBody);
  
  
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

      plane_angle = sf::Joystick::getAxisPosition(0, sf::Joystick::X);

      plane->setRotation(gle::Vector3<GLfloat>(1, 0, 0), plane_angle);
      btTransform transPlane;
      btMotionState* state = groundRigidBody->getMotionState();
      state->getWorldTransform(transPlane);
      transPlane.setRotation(btQuaternion(btVector3(1,0,0),
					  plane_angle
					  * M_PI / 180.0));
      state->setWorldTransform(transPlane);
      groundRigidBody->setMotionState(state);

      // Physics
      dynamicsWorld->stepSimulation(1/10.0,10);
      btTransform trans;
      fallRigidBody->getMotionState()->getWorldTransform(trans);
 
      cube->setPosition(gle::Vector3<GLfloat>(trans.getOrigin().getX(),
						trans.getOrigin().getY(),
						trans.getOrigin().getZ()));
      gle::Vector3<GLfloat> rotAxis(trans.getRotation().getAxis().getX(),
				    trans.getRotation().getAxis().getY(),
				    trans.getRotation().getAxis().getZ());
      cube->setRotation(rotAxis,
			trans.getRotation().getAngle()
			/ (2.0*M_PI) * 360.0);

      sf::Mouse::setPosition(sf::Vector2i(W_WIDTH/2, W_HEIGHT/2), App);
      flycam::flycam(&camera);
      l.setPosition(camera.getPosition());
      scene.updateLights();
      renderer.render(&scene);
      App.display();
      //video::saveImage(App, W_FRAMERATE);
      GLfloat elapsed = time.getElapsedTime().asMicroseconds();
      if ((1000000.0/W_FRAMERATE) - elapsed > 0)
	sf::sleep(sf::microseconds(1000000.0/W_FRAMERATE - elapsed));
      time.restart();
    }
  
  //video::save(av[0], W_FRAMERATE);

  delete dynamicsWorld;
  delete solver;
  delete dispatcher;
  delete collisionConfiguration;
  delete broadphase;

  delete groundShape;
  delete fallShape;

  return (0);
}
