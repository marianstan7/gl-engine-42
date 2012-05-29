//
// Example.cpp for  in /home/jochau_g//dev/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri May 25 00:33:16 2012 gael jochaud-du-plessix
// Last update Wed May 30 00:39:45 2012 gael jochaud-du-plessix
//

#include <algorithm>

#include "Example.hpp"

#include "fps.hpp"
#include "flycam.hpp"
#include "trackball.hpp"
#include "video.hpp"

Example::Example(int ac, char**av, int winWidth, int winHeight, int framerate, std::string const & name)
  : _argv(),
    _winWidth(winWidth), _winHeight(winHeight), _framerate(framerate),
    _name(name),
    _window(NULL),
    _showFramerate(true), _limitFramerate(true), _cameraType(Flycam),
    _recordVideo(false),
    _scene(), _camera(NULL), _renderer(NULL)
{
  for (int i = 0; i < ac; ++i)
      _argv.push_back(av[i]);
  for (int i = 0; i < ac; ++i)
    {
      if (_argv[i] == "--width" && i + 1 < ac)
	_winWidth = atoi(_argv[i + 1].c_str());
      else if (_argv[i] == "--height" && i + 1 < ac)
	_winHeight = atoi(_argv[i + 1].c_str());
      else if (_argv[i] == "--framerate" && i + 1 < ac)
	_framerate = atoi(_argv[i + 1].c_str());
      else if (_argv[i] == "--show-framerate")
	_showFramerate = true;
      else if (_argv[i] == "--limit-framerate" && i + 1 < ac)
	_limitFramerate = atoi(_argv[i + 1].c_str());
      else if (_argv[i] == "--record-video")
	_recordVideo = true;
      else if (_argv[i] == "--help")
	{
	  std::cout << "Usage: \n\t" << av[0] << " [OPTIONS]\n"
		    << "Options:\n"
		    << "\t--width WINDOW_WIDTH\n"
		    << "\t--height WINDOW_HEIGHT\n"
		    << "\t--framerate FRAMERATE\n"
		    << "\t--limit-framerate [0-1]\n"
		    << "\t--show-framerate\n"
		    << "\t--record-video\n"
		    << "\t--help\n";
	  exit(EXIT_SUCCESS);
	}
    }
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

  if (_cameraType == Flycam)
    _window->setMouseCursorVisible(false);

  while (_window->isOpen())
    {
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
	  if (_cameraType == Flycam)
	    flycam::event(event, *_window);
	  if (_cameraType == Trackball)
	    trackball::event(event, *_window);
        }

      if (_cameraType == Flycam)
	{
	  sf::Mouse::setPosition(sf::Vector2i(_winWidth/2, _winHeight/2), *_window);
	  flycam::flycam(_camera);
	}
      else if (_cameraType == Trackball)
	trackball::trackball(_camera);

      animate();
      _renderer->render(_scene);
      _window->display();
      if (_recordVideo)
	video::saveImage(*_window, _framerate);

      if (_showFramerate)
	fps::print();
      if (_limitFramerate)
	fps::limit(_framerate);
    }

  if (_recordVideo)
    {
      std::string filename = _name;
      std::replace(filename.begin(), filename.end(), ' ', '_');
      std::replace(filename.begin(), filename.end(), ':', '_');
      std::replace(filename.begin(), filename.end(), '-', '_');
      video::save(filename, _framerate);
    }

  return (0);
}

void Example::animate()
{
  
}
