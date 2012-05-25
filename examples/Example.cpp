//
// Example.cpp for  in /home/jochau_g//dev/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri May 25 00:33:16 2012 gael jochaud-du-plessix
// Last update Fri May 25 01:42:16 2012 gael jochaud-du-plessix
//

#include "Example.hpp"

#include "fps.hpp"
#include "flycam.hpp"

Example::Example(int winWidth, int winHeight, int framerate, std::string const & name)
  : _winWidth(winWidth), _winHeight(winHeight), _framerate(framerate),
    _name(name),
    _window(NULL),
    _scene(), _camera(NULL), _renderer(NULL)
{
  
}

Example::~Example()
{

}

int Example::run()
{
  // Init window with opengl context
  sf::ContextSettings context;
  context.depthBits = 24;
  context.stencilBits = 24;
  context.antialiasingLevel = 2;
  context.majorVersion = 3;
  context.minorVersion = 3;

  _window = new sf::Window(sf::VideoMode(_winWidth, _winHeight, 32), _name,
			   sf::Style::Default, context);

  // Print OpenGL supported version
  context = _window->getSettings();
  std::cout << context.majorVersion << '.'
            << context.minorVersion << std::endl;

  _window->setActive();

  initScene();

  _window->setMouseCursorVisible(false);

  while (_window->isOpen())
    {
      fps::print();

      sf::Event event;
      while (_window->pollEvent(event))
        {
          // Close window : exit
          if (event.type == sf::Event::Closed)
            _window->close();
          else if (event.type == sf::Event::KeyPressed
                   && event.key.code == sf::Keyboard::Escape)
            _window->close();
          // Adjust the viewport when the window is resized
          if (event.type == sf::Event::Resized)
            glViewport(0, 0, event.size.width, event.size.height);
	  flycam::event(event, *_window);
        }
      
      sf::Mouse::setPosition(sf::Vector2i(_winWidth/2, _winHeight/2), *_window);
      flycam::flycam(_camera);      
      animate();
      _renderer->render(_scene);
      _window->display();
      fps::limit(_framerate);
    }

  return (0);
}

void Example::animate()
{
  
}
