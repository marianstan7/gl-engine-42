//
// flycam.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar  2 17:27:21 2012 gael jochaud-du-plessix
// Last update Sat Mar 10 01:05:42 2012 gael jochaud-du-plessix
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

#define W_WIDTH 1280
#define W_HEIGHT 720

int glEngine(int, char**);

GLfloat teta = M_PI / 2;
GLfloat phi = 0;
std::map<sf::Keyboard::Key, bool> keyState;
GLfloat mouseX = 0.0;
GLfloat mouseY = 0.0;
GLfloat moveUp = 0.0;
GLfloat mouseSensibility = 1;
GLfloat camSpeed = 0.1;

void flycam(gle::Camera* camera)
{
  gle::Vector3<GLfloat> pos = camera->getPosition();
  gle::Vector3<GLfloat> viewVector;
  viewVector.x = sin(teta) * cos(phi);
  viewVector.y = cos(teta);
  viewVector.z = sin(teta) * sin(phi);

  viewVector.normalize();

  viewVector *= camSpeed;

  // Keyboard moves
  if (keyState[sf::Keyboard::W])
    pos += viewVector;
  if (keyState[sf::Keyboard::S])
    pos -= viewVector;

  if (keyState[sf::Keyboard::A])
    {
      gle::Vector3<GLfloat> viewVectorSide =
	viewVector ^ gle::Vector3<GLfloat>(0, 1, 0);
      viewVectorSide.normalize();
      viewVectorSide *= camSpeed;
      pos -= viewVectorSide;
    }
  if (keyState[sf::Keyboard::D])
    {
      gle::Vector3<GLfloat> viewVectorSide =
	viewVector ^ gle::Vector3<GLfloat>(0, 1, 0);
      viewVectorSide.normalize();
      viewVectorSide *= camSpeed;
      pos += viewVectorSide;
    }

  // Mouse moves
  if (mouseY != 0)
    teta += (M_PI / 5) * mouseY * mouseSensibility;
  if (mouseX != 0)
    phi += (M_PI / 5) * mouseX * mouseSensibility;;
  // Mouve up or down with clicks
  if (sf::Mouse::IsButtonPressed(sf::Mouse::Left))
    pos += gle::Vector3<GLfloat>(0, 1 * (mouseSensibility / 10), 0);
  if (sf::Mouse::IsButtonPressed(sf::Mouse::Right))
    pos -= gle::Vector3<GLfloat>(0, 1 * (mouseSensibility / 10), 0);

  // Joystick moves
  GLfloat rightY =
    (GLfloat)sf::Joystick::GetAxisPosition(1, sf::Joystick::V) / 100;
  GLfloat rightX =
    (GLfloat)sf::Joystick::GetAxisPosition(1, sf::Joystick::U) / 100;
  GLfloat leftX =
    (GLfloat)sf::Joystick::GetAxisPosition(1, sf::Joystick::X) / 100;
  GLfloat leftY =
    (GLfloat)sf::Joystick::GetAxisPosition(1, sf::Joystick::Y) / 100;
  GLfloat trigLeft =
    (((GLfloat)sf::Joystick::GetAxisPosition(1, sf::Joystick::Z) / 100)
     + 1) / 2;
  GLfloat trigRight =
    (((GLfloat)sf::Joystick::GetAxisPosition(1, sf::Joystick::R) / 100)
     + 1) / 2;

  if (std::abs(rightY) > 0.1)
    teta += (M_PI / 5) * rightY * 0.01;
  if (std::abs(rightX) > 0.1)
    phi += (M_PI / 5) * rightX * 0.01;

  if (std::abs(leftY) > 0.1)
    pos += viewVector * -leftY;

  if (std::abs(leftX) > 0.1)
    {
      gle::Vector3<GLfloat> viewVectorSide =
	viewVector ^ gle::Vector3<GLfloat>(0, 1, 0);
      viewVectorSide.normalize();
      pos += viewVectorSide * (camSpeed * leftX);
    }

  if (trigRight > 0.1)
    pos += gle::Vector3<GLfloat>(0, trigRight * camSpeed, 0);
  if (trigLeft > 0.1)
    pos -= gle::Vector3<GLfloat>(0, trigLeft * camSpeed, 0);

  teta = teta > (M_PI - 0.001) ? (M_PI - 0.001) : teta;
  teta = teta < 0.001 ? 0.001 : teta;

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

  sf::Window App(sf::VideoMode(W_WIDTH, W_HEIGHT, 32), "glEngine",
		 sf::Style::Default, context);
  
  //! Print OpenGL supported version
  context = App.GetSettings();
  std::cout << context.MajorVersion << '.'
	    << context.MinorVersion << std::endl;

  App.SetActive();

  gle::Scene scene;
  gle::PerspectiveCamera camera(gle::Vector3<GLfloat>(-20, 10, 0),
				gle::Vector3<GLfloat>(0, 0, 0),
				45, (GLfloat)W_WIDTH/W_HEIGHT, 1, 10000);

  gle::ObjLoader loader;
  gle::Material material;
  material.setDiffuseLightEnabled(true);
  material.setSpecularLightEnabled(true);
  material.setColorMap(new gle::Texture("./examples/earth.jpg"));

  gle::Material material2;
  material2.setDiffuseLightEnabled(true);
  material2.setSpecularLightEnabled(true);

  gle::Mesh* cube = gle::Geometries::Cube(&material, 10);
  cube->setPosition(gle::Vector3<GLfloat>(8, 0, -8));
  gle::Mesh* pointCube = gle::Geometries::Cube(&material2, 10);
  pointCube->setPosition(gle::Vector3<GLfloat>(-24, 0, -8));
  pointCube->setType(gle::Mesh::Points);
  pointCube->setPointSize(5);
  gle::Mesh* sphere = gle::Geometries::Sphere(&material, 5);
  sphere->setPosition(gle::Vector3<GLfloat>(8, 0, 8));
  gle::Mesh* pointSphere = gle::Geometries::Sphere(&material2, 5, 100, 100);
  pointSphere->setPosition(gle::Vector3<GLfloat>(-24, 0, 8));
  pointSphere->setType(gle::Mesh::Points);
  gle::Mesh* plane = gle::Geometries::Plane(&material, 80, 80, 20, 20);
  plane->setPosition(gle::Vector3<GLfloat>(0, -5, 0));
  gle::Mesh* pointPlane =
    gle::Geometries::Plane(&material2, 80, 80, 100, 100);
  pointPlane->setPosition(gle::Vector3<GLfloat>(0, -5, 80));
  pointPlane->setType(gle::Mesh::Points);
  pointPlane->setPointSize(2);

  gle::Mesh* wiredSphere = gle::Geometries::WiredMesh(sphere);
  wiredSphere->setMaterial(&material2);
  wiredSphere->setPosition(gle::Vector3<GLfloat>(-8, 0, 8));

  gle::Mesh* wiredCube = gle::Geometries::WiredMesh(cube);
  wiredCube->setMaterial(&material2);
  wiredCube->setPosition(gle::Vector3<GLfloat>(-8, 0, -8));

  gle::Mesh* wiredPlane = gle::Geometries::WiredMesh(plane);
  wiredPlane->setMaterial(&material2);
  wiredPlane->setPosition(gle::Vector3<GLfloat>(0, -5, -80));

  scene << cube << wiredCube << pointCube;
  scene << sphere << wiredSphere << pointSphere;
  scene << plane << wiredPlane << pointPlane;

  srand(time(NULL));

  gle::Material materialLight;

  materialLight.setDiffuseLightEnabled(true);
  materialLight.setSpecularLightEnabled(true);
  gle::PointLight l(gle::Vector3<GLfloat>(0, 2000, 0),
		    gle::Color<GLfloat>(0.8, 0.8, 0.8));
  gle::Mesh* sp = gle::Geometries::Sphere(&materialLight, 10);
  sp->setPosition(gle::Vector3<GLfloat>(0, 200, 0));

  scene << &camera << &materialLight << sp;
  scene << &l;

  gle::Renderer renderer;
  renderer.createPrograms(&scene);

  sf::Clock clock;
  sf::Clock time;

  App.ShowMouseCursor(false);
  
  while (App.IsOpen())
    {      
      sf::Event Event;
      mouseX = 0;
      mouseY = 0;
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
	  else if (Event.Type == sf::Event::MouseMoved)
	    {
	      sf::Vector2i mouse = sf::Mouse::GetPosition(App);
	      mouseX = (GLfloat)(mouse.x - (GLfloat)W_WIDTH / 2)
		/ ((GLfloat)W_WIDTH / 2);
	      mouseY = (GLfloat)(mouse.y - (GLfloat)W_HEIGHT / 2)
		/ ((GLfloat)W_HEIGHT / 2);
	    }
	}
      sf::Mouse::SetPosition(sf::Vector2i(W_WIDTH/2, W_HEIGHT/2), App);
      flycam(&camera);
      l.setPosition(camera.getPosition());
      scene.updateLights();
      renderer.render(&scene);
      App.Display();
      plane->setRotation(gle::Vector3<GLfloat>(1, 0, 0),
			 -sf::Joystick::GetAxisPosition(0, sf::Joystick::X));
      GLfloat elapsed = time.GetElapsedTime().AsMicroseconds();
      if (16666 - elapsed > 0)
	sf::Sleep(sf::Microseconds(16666));
    }
  
  return (0);
}
