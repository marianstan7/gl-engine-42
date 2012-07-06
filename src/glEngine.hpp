//
// glEngine.hpp for  in /home/jochau_g//dev/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Wed Jul  4 20:28:54 2012 gael jochaud-du-plessix
//

#include "Renderer.hpp"
#include "Scene.hpp"
#include "PerspectiveCamera.hpp"
#include "Geometries.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"

#include "Exception.hpp"

//! Main namespace of the engine
/*!
  The gle namespace contains all the classes and functions of the glEngine.
 */
namespace gle {}

/*!
  \mainpage glEngine v1.0 (July 2012)

  \section Introduction 
  glEngine is a simple 3D engine using OpenGL. It's main purpose are to be easy to use and to use the latest OpenGL features.
  
  It's interface allows to easily create and manipulate the scene graph, to render to a texture and to configure several effects such as fog, environment map, etc...

  glEngine is multiplatform, it uses ASSIMP for loading assets and SFML for images. Everything else is just pure C++0x.
  The engine also need OpenGL v3.3 minimum, and will certainly need newer versions in the future.

  \section Installation

  To compile the glEngine, you need to have ASSIMP and SFML libraries installed. Then, simply go to the glEngine source directory and execute the following commands:
  \code
  cmake .
  make
  \endcode
  Then, you can use glEngine in your project by including the needed headers and linking with the dynamic library.

  \section Documentation
  To start using the glEngine, first read the \ref GettingStarted.
  When you are ok with the main concepts of the scene graph and the renderer, you can have a look at the API documentation with a full description of all the classes of the engine.
The documentation also contains examples and tips on how to use the classes in order to easily use the most of the engine capabilities.


  \page GettingStarted Getting Started tutorial

  \section Introduction
  This tutorial will show you how to build a simple program using the glEngine to render a simple scene.
  It will introduce the main concepts of the glEngine, such as the Scene graph, the renderer and several kinds of scene elements.

  \section Step_1 Step 1: The window
  First, in order to use the glEngine, we need an OpenGL context. In this tutorial we will use the SFML library to create a window with the context we need.
  \code
  // Include what's necessary to create the window
  #include <SFML/Window.hpp>

  int main(int, char**)
  {
	// First create the window and set it active
	sf::Window window(sf::VideoMode(1280, 720, 32), "glEngine: Getting Started tutorial",
			  sf::Style::Default);  
	window.setActive();

	// Insert code that creates the scene here...
  
	// Set up a timer for animation
	sf::Clock timer;
	
	// Application main loop
	while (window.isOpen())
	{
		// Event handling
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed
			|| (event.type == sf::Event::KeyPressed
			&& event.key.code == sf::Keyboard::Escape))
			{
				window.close();
				return (0);
				}
			}
			
		GLfloat time = (float)timer.getElapsedTime().asMilliseconds();
		// Animate the scene here...

		// Render the scene here...
  
		// Display the window
		window.display();
       }
       return (0);
  }
  \endcode

  \section Step_2 Step 2: The scene
  The first object we need to create to use the glEngine is the scene.\n
  A scene is composed of nodes. The nodes are linked together by a hiearchy that allows to manipulate them relatively to each other.\n
  In this tutorial, we will not manipulate directly the scene graph, but just add the nodes we want to the scene by using the << operator on the scene object.\n
  So to create a scene, simply instantiate a new gle::Scene object.
  \code
  gle::Scene scene;
  \endcode
  
  \section Step_3 Step 3: The camera
  The first node we will add to the scene is the camera. Of course, the scene cannot be rendered without it.\n
  There are several kinds of cameras. The most used is called "PerspectiveCamera", and we can simply create one of these by instianting a gle::PerspectiveCamera object, like this:
  \code
  gle::PerspectiveCamera camera(
	gle::Vector3f(0, 50, 100),      // Position of the camera
        gle::Vector3f(0, 10, 0),        // Target point (where we look at)
	45,                             // Vertical field of view
	1280.0/720.0,                   // Aspect ratio of the viewport
	1, 10000                        // Near and far planes of the camera
  );
 \endcode

 To add the camera to the scene, you have to add it to the root node. The simplest way to do this is to use the << operator of the scene:

 \code
 scene << &camera;
 \endcode

 Once added, the camera will automatically be used for rendering. To select an other camera, use the gle::Scene::setCurrentCamera() function.

 \section Step_4 Step 4: Materials
 We are almost ready to add meshes to our scene. Before, we must create some materials, in order to describe the way they must be drawn by the renderer.\n
 To create a new material, simply instantiate a gle::Material object. You can give a name to your material, in order to retrieve it easily later.
 \code
 gle::Material white("white");
 gle::Material green("green");
 \endcode
 Materials have a lot of properties, affecting the way they will react to light. Here, we set the diffuse and specular colors.
 \code
 white.setDiffuseColor(gle::Colorf(1, 1, 1));
 white.setSpecularColor(gle::Colorf(1, 0.6, 0));
 green.setDiffuseColor(gle::Colorf(0.5, 1.0, 0.2));
 \endcode
 
 \section Step_5 Step 5: A cube on a plane
 Now, we are ready to create some meshes. You can do this by:
	- Loading it from a file
	- Programatically setting the vertex attributes of the mesh
	- Using the base geometries available in the engine

 In this tutorial, we will use the 3rd option, which is the easiest and shortest to use.\n
 So, to create a plane and a cube, we just have to create two gle::Scene::Node* that will hold the two meshes created by the gle::Geometries functions:
 \code
 gle::Scene::Node* plane = gle::Geometries::Plane(&white, 500, 500);   // Material, width, height
 gle::Scene::Node* cube = gle::Geometries::Cube(&green, 30, true);     // Material, size, isDynamic
 \endcode
 Note that we pass \c true as the third argument of gle::Geometries::Cube, because we will animate it.\n
 Once our objects are created, we must set their position in space. To do this, simply use the gle::Scene::Node::setPosition() function.
 \code
 cube->setPosition(gle::Vector3f(0, 15, 0)); 
 \endcode
 The glEngine uses the standard OpenGL axis for its coordinates system. The \c X axis points to the right of the screen, the \c Y axis points to the top of the screen and the \c Z axis points through the screen, like this:
 \image html axis.png
 Once our objects are ready, we can add it to the scene:
 \code
 scene << plane << cube;
 \endcode

 \section Step_6 Step 6: Let there be light !
 Last step with the scene: the lights ! For the tutorial, we will create two lights: a spot and a point light.\n
 We use a spot light because it can project nice shadows. The point light will add more light to the rest of the scene.
 \code
  gle::SpotLight spotLight(gle::Vector3f(-50, 100, -50),        // Position of the light
                           gle::Colorf(0.8, 1.0, 0.5),          // Light color
                           1.0);                                // Spot angle

  spotLight.setTarget(gle::Vector3f(0, 0, 0));                  // Point the light to the center of the scene 

  gle::PointLight pointLight(gle::Vector3f(0, 50, 100),		// Position of the light
                             gle::Colorf(0.9, 0.8, 0.6));	// Color of the light
 \endcode
 Now simply add to the scene, and you are done with it !
 \code
 scene << &spotLight << &pointLight;
 \endcode

 \section Step_7 Step 7: The renderer
 Once we have a ready scene, we can create the renderer.\n
 The renderer is just the glEngine object that will operate all rendering operations. It is very easy to use, as you just have to instantiate a gle::Renderer object:
 \code
 gle::Renderer renderer;
 \endcode
 And to call its gle::Renderer::render() function with the scene to render, and the size of the screen, like this:
 \code
 renderer.render(&scene, gle::Rectf(0, 0, 1280, 720));	// Scene to render, size of the screen 
 \endcode

 \section Recap
 Here is the final code of our first glEngine program. I added some code to animate the cube and the camera, in order to make the scene more dynamic.
 \code
 // Include what's necessary to create the window
 #include <SFML/Window.hpp>
 
 // Include the glEngine header
 #include "glEngine.hpp"
 
 int main(int, char**)
 {
	// First create the window and set it active
	sf::Window window(sf::VideoMode(1280, 720, 32), "glEngine: Getting Started tutorial",
			  sf::Style::Default);  
	window.setActive();

	// Then, we create the scene
	gle::Scene scene;
	
	// To render our scene, we need a camera
	gle::PerspectiveCamera camera(
	                              gle::Vector3f(0, 50, 100),	// Position of the camera
				      gle::Vector3f(0, 10, 0),		// Target point (where we look at)
				      45,				// Vertical field of view
				      1280.0/720.0,			// Aspect ratio of the viewport
				      1, 10000);			// Near and far planes of the camera
				      
	// We have to ways to add our camera to the scene. The simpliest:
	scene << &camera;
	// Is equivalent to
	// scene.getRootNode().addChild(&camera);
	
	// Before we can create the objects of our scene, we need some materials
	gle::Material white("white");
	gle::Material green("green");
	
	white.setDiffuseColor(gle::Colorf(1, 1, 1));
	white.setSpecularColor(gle::Colorf(1, 0.6, 0));
	green.setDiffuseColor(gle::Colorf(0.5, 1.0, 0.2));
	
	// Then, we create the two objects of our scene: a cube and a plane
	gle::Scene::Node* plane = gle::Geometries::Plane(&white, 500, 500);	// Material, width, height
	gle::Scene::Node* cube = gle::Geometries::Cube(&green, 30, true);	// Material, size, isDynamic
	
	cube->setPosition(gle::Vector3f(0, 15, 0));
	
	// And add our objects to the scene
	scene << plane << cube;
	
	// Now we create the light
	gle::SpotLight spotLight(gle::Vector3f(-50, 100, -50),	// Position of the light
	                         gle::Colorf(0.8, 1.0, 0.5),	// Light color
				 1.0);				// Spot angle

	spotLight.setTarget(gle::Vector3f(0, 0, 0));	      	// Point the light to the center of the scene
	
	// Plus a point light
	gle::PointLight pointLight(gle::Vector3f(0, 50, 100),		// Position of the light
	                           gle::Colorf(0.9, 0.8, 0.6));		// Color of the light
				   
	// And add the lights to the scene
	scene << &spotLight << &pointLight;

	// In order to render our scene, we need a renderer
	gle::Renderer renderer;
	
	// Set up a timer for animation
	sf::Clock timer;
	
	// Application main loop
	while (window.isOpen())
	{
		// Event handling
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed
			    || (event.type == sf::Event::KeyPressed
			    && event.key.code == sf::Keyboard::Escape))
			{
				window.close();
				return (0);
			}
		}

	        GLfloat time = (float)timer.getElapsedTime().asMilliseconds();
		cube->setPosition(gle::Vector3f(0, 10 + 5 * sin(time/1000), 0));
		cube->setScale((1.2 + (float)sin(time/1000)) / 2);
		camera.setRotation(gle::Vector3f(0, 1, 0), time / 50); // Axis, angle

		// To render the scene, simply call the render() function of the renderer
		renderer.render(&scene, gle::Rectf(0, 0, 1280, 720));	// Scene to render, size of the screen
		
		// Display the window
		window.display();
        }
	
	return (0);
}
 \endcode

 \section Final_result Final result
 \image html GettingStarted_result.png

 \section Go_further Go further
 Now, you know how to create, set up and render a complete scene with lights, objects and cameras.\n
 glEngine is capable of much more, such as loading models from files, rendering texture, normal maps or environment maps. To use these features, just go through the API documentation. Classes are documented and contains example codes, so you should be ready to do whatever you want !\n
 Here are some classes that should be interesting:
 - gle::Scene
 - gle::Mesh
 - gle::Material
 - gle::Texture
 - gle::UniversalLoader
 - gle::CubeMap
 - etc...
 */
