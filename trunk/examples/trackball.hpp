//
// trackball.hpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar 16 10:38:56 2012 gael jochaud-du-plessix
// Last update Fri May 25 13:49:07 2012 gael jochaud-du-plessix
//

#ifndef _TRACKBALL_HPP_
# define _TRACKBALL_HPP_

# include <SFML/System/Clock.hpp>

namespace trackball {
  extern sf::Clock	timer;
  extern sf::Clock	mouseTimer;
  extern GLfloat	rotationX;
  extern GLfloat	rotationY;
  extern GLfloat	zoom;
  extern GLfloat	rotationSpeedX;
  extern GLfloat	rotationSpeedY;
  extern GLfloat	zoomSpeed;
  extern GLfloat	rotationXDamping;
  extern GLfloat	rotationYDamping;
  extern GLfloat	zoomDamping;
  extern GLfloat	camSpeed;
  extern GLfloat	mouseX;
  extern GLfloat	mouseY;
  extern GLfloat	mouseSpeedX;
  extern GLfloat	mouseSpeedY;
  extern bool		dragging;
  extern GLfloat	startDraggingX;
  extern GLfloat	startDraggingY;
  extern GLfloat	startDraggingRotationX;
  extern GLfloat	startDraggingRotationY;
  extern GLfloat	startDraggingZoom;
  extern bool		draggingZoom;

  void event(sf::Event &event, sf::Window & window);
  void trackball(gle::Camera* camera);
};

#endif
