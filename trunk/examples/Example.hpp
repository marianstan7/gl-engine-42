//
// Example.hpp for  in /home/jochau_g//dev/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri May 25 12:25:22 2012 gael jochaud-du-plessix
// Last update Fri May 25 13:00:20 2012 gael jochaud-du-plessix
//

#ifndef _EXAMPLE_HPP_
# define _EXAMPLE_HPP_

# include <string>
# include <SFML/Window.hpp>

#include <Scene.hpp>
#include <PerspectiveCamera.hpp>
#include <Geometries.hpp>
#include <Material.hpp>
#include <Renderer.hpp>
#include <PointLight.hpp>

class Example {
public:
  enum CameraType {
    Flycam,
    Trackball,
    NoCamera
  };

  Example(int winWidth=1280, int winHeight=720, int framerate=60, std::string const & name="glEngine");
  virtual ~Example();
  
  int		run();
  virtual void	initScene() = 0;
  virtual void	animate();

protected:
  int		_winWidth;
  int		_winHeight;
  int		_framerate;
  std::string	_name;

  sf::Window*	_window;

  bool		_showFramerate;
  bool		_limitFramerate;
  CameraType	_cameraType;

  gle::Scene*		_scene;
  gle::Camera*		_camera;
  gle::Renderer*	_renderer;
};

#endif
