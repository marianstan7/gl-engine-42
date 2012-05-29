//
// flycam.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar  2 17:27:21 2012 gael jochaud-du-plessix
// Last update Tue May 29 22:09:32 2012 gael jochaud-du-plessix
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

class PhysicMesh : public btMotionState {
public:
  PhysicMesh(const btTransform &initialpos, gle::Mesh *mesh,
	     btCollisionShape* fallShape, btDiscreteDynamicsWorld* dynamicsWorld, btScalar mass) {
    _mesh = mesh;
    mPos1 = initialpos;
    _fallShape = fallShape;
    btVector3 fallInertia(0,0,0);
    if (mass != 0)
      fallShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo
      fallRigidBodyCI(mass,this,fallShape,fallInertia);
    fallRigidBodyCI.m_friction = 1;
    //   fallRigidBodyCI.m_restitution = 0;
    //    fallRigidBodyCI.m_linearDamping = 0.9;
    _rigidBody = new btRigidBody(fallRigidBodyCI);
    //_rigidBody->setLinearVelocity(btVector3(0, 0, 0));
    //    _rigidBody->setCcdMotionThreshold(1);
    // _rigidBody->setCcdSweptSphereRadius(0.9*1);
    //_rigidBody->setDamping(0.1, 0.1);
    dynamicsWorld->addRigidBody(_rigidBody);
  }

  virtual ~PhysicMesh() {
  }

  void setMesh(gle::Mesh *mesh) {
    _mesh = mesh;
  }

  virtual void getWorldTransform(btTransform &worldTrans) const {
    worldTrans = mPos1;
  }

  virtual void setWorldTransform(const btTransform &worldTrans) {
    if(NULL == _mesh)
      return;
    btQuaternion rot = worldTrans.getRotation();
    gle::Vector3<GLfloat> rotAxis(rot.getAxis().getX(),
				  rot.getAxis().getY(),
				  rot.getAxis().getZ());
    _mesh->setRotation(rotAxis, (rot.getAngle() / (M_PI)) * 180.0);
    btVector3 pos = worldTrans.getOrigin();
    _mesh->setPosition(gle::Vector3<GLfloat>(pos.x(), pos.y(), pos.z()));
  }

  btRigidBody* getGroundRigidBody() {
    return (_rigidBody);
  }

protected:
  gle::Mesh *_mesh;
  btTransform mPos1;
  btCollisionShape* _fallShape;
  btRigidBody* _rigidBody;
};

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

  gle::Mesh* plane = gle::Geometries::Plane(&material, 800, 800);

  GLfloat plane_angle = 15.0;

  scene << plane;

  srand(time(NULL));

  gle::Material materialLight;

  materialLight.setDiffuseLightEnabled(true);
  materialLight.setSpecularLightEnabled(true);
  gle::PointLight l(gle::Vector3<GLfloat>(0, 2000, 0),
		    gle::Color<GLfloat>(0.8, 0.8, 0.8));
  gle::Mesh* sp = gle::Geometries::Sphere(&materialLight, 10);
  sp->setPosition(gle::Vector3<GLfloat>(0, 200, 0));

  scene << &camera << sp;
  scene << &l;

  gle::Renderer renderer;

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

  //  dynamicsWorld->getSolverInfo().m_splitImpulse = 0.5;
  dynamicsWorld->setGravity(btVector3(0,-10,0));

  double angle = 0;
  for (double z = 0.0; z < 90.0; z+= 22.7)
    {
  for (double x = -15.0; x < 16.0; x += 30.0)
    {
  for (double y = -15.0; y < 16.0; y += 30.0)
    {
  for (double i = 0.5; i < 22.0; i += 1)
    {
      angle += (double)M_PI/2;
      btQuaternion quat(btVector3(0, 1, 0), angle);
      {
	for(double j = -6; j < 7.0; j+= 6.0)
	{
	  gle::Mesh* cuboid1 = gle::Geometries::Cuboid(&material, 3.0, 1.0, 15.0);
	  PhysicMesh* physicCube1 = new PhysicMesh(btTransform(quat, btVector3(x+j*cos(angle), i+z, y+j*sin(angle))),
						   cuboid1, new btBoxShape(btVector3(1.5, 0.5, 7.5)), dynamicsWorld, 10.0);
	  scene << cuboid1;
	}
      }
    }
    }
    }
   
  btQuaternion quat(btVector3(0, 1, 0), 0);
  gle::Mesh* cuboid1 = gle::Geometries::Cuboid(&material, 50.0, 1.0, 50.0);
  PhysicMesh* physicCube1 = new PhysicMesh(btTransform(quat, btVector3(0, z+22.7, 0)),
					   cuboid1, new btBoxShape(btVector3(25.0, 0.5, 25.0)), dynamicsWorld, 100.0);
  scene << cuboid1;
    }
  
  float boxsize = 400;
  PhysicMesh* physicPlan = new PhysicMesh(btTransform(btQuaternion(btVector3(1,0,0), 0), btVector3(0,-1,0)),
					  plane, new btBoxShape(btVector3(boxsize,1,boxsize)), dynamicsWorld, 0);
  /*PhysicMesh* physicPlan2 = new PhysicMesh(btTransform(btQuaternion(btVector3(1,0,0), 0 * plane_angle * M_PI / 180.0), btVector3(0,0,-(boxsize+1))),
					   NULL, new btBoxShape(btVector3(boxsize,boxsize,0.99)), dynamicsWorld, 0);  
  PhysicMesh* physicPlan3 = new PhysicMesh(btTransform(btQuaternion(btVector3(1,0,0), 0 * plane_angle * M_PI / 180.0), btVector3(0,0,(boxsize+1))),
					   NULL, new btBoxShape(btVector3(boxsize,boxsize,0.99)), dynamicsWorld, 0);  
  PhysicMesh* physicPlan4 = new PhysicMesh(btTransform(btQuaternion(btVector3(1,0,0), 0 * plane_angle * M_PI / 180.0), btVector3(-(boxsize+1),0,0)),
					   NULL, new btBoxShape(btVector3(0.99,boxsize,boxsize)), dynamicsWorld, 0);  
  PhysicMesh* physicPlan5 = new PhysicMesh(btTransform(btQuaternion(btVector3(1,0,0), 0 * plane_angle * M_PI / 180.0), btVector3((boxsize+1),0,0)),
					   NULL, new btBoxShape(btVector3(0.99,boxsize,boxsize)), dynamicsWorld, 0);
*/
  gle::Mesh* sphere = gle::Geometries::Sphere(&material, 2.5);
  PhysicMesh* physicSphere = new PhysicMesh(btTransform(btQuaternion(0, 0, 0, 1),
							btVector3(0, 150, 0)),
					    sphere, new btSphereShape(2.5), dynamicsWorld, 10000);
  scene << sphere;

  scene.updateScene();

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
	  else if (Event.type == sf::Event::MouseButtonPressed && Event.mouseButton.button == 2)
	    {
	      gle::Mesh* sphere = gle::Geometries::Sphere(&material, 2.5);
	      PhysicMesh* physicSphere = new PhysicMesh(btTransform(btQuaternion(0, 0, 0, 1),
								    btVector3(camera.getAbsolutePosition().x, camera.getAbsolutePosition().y, camera.getAbsolutePosition().z)),
						      sphere, new btSphereShape(2.5), dynamicsWorld, 10000);
	      gle::Vector3<GLfloat> velocity = camera.getTarget() - camera.getAbsolutePosition();
	      velocity.normalize();
	      velocity *= 100;
	      physicSphere->getGroundRigidBody()->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
	      scene << sphere;
	      scene.updateScene();
	    }
	  // Adjust the viewport when the window is resized
	  if (Event.type == sf::Event::Resized)
	    glViewport(0, 0, Event.size.width, Event.size.height);
	  flycam::event(Event, App);
	}

      //plane_angle = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
      /*
      plane->setRotation(gle::Vector3<GLfloat>(1, 0, 0), plane_angle);
      btTransform transPlane;
      btMotionState* state = physicPlan->getGroundRigidBody()->getMotionState();
      state->getWorldTransform(transPlane);
      transPlane.setRotation(btQuaternion(btVector3(1,0,0),
					  plane_angle
					  * M_PI / 180.0));
      state->setWorldTransform(transPlane);
      physicPlan->getGroundRigidBody()->setMotionState(state);
      */
      // Physics
      
      sf::Mouse::setPosition(sf::Vector2i(W_WIDTH/2, W_HEIGHT/2), App);
      flycam::flycam(&camera);
      l.setPosition(camera.getAbsolutePosition());
      scene.updateLights();
      renderer.render(&scene);
      App.display();
      //video::saveImage(App, W_FRAMERATE);
      GLfloat elapsed = time.getElapsedTime().asMicroseconds();
      time.restart();      
      dynamicsWorld->stepSimulation(elapsed / 100000.f);
      if ((1000000.0/W_FRAMERATE) - elapsed > 0)
	sf::sleep(sf::microseconds(1000000.0/W_FRAMERATE - elapsed));
    }
  
  //video::save(av[0], W_FRAMERATE);

  delete dynamicsWorld;
  delete solver;
  delete dispatcher;
  delete collisionConfiguration;
  delete broadphase;

  //delete groundShape;
  //delete fallShape;

  return (0);
}
