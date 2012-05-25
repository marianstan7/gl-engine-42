//
// trackball.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar  2 17:27:21 2012 gael jochaud-du-plessix
// Last update Fri May 25 13:55:49 2012 gael jochaud-du-plessix
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

#include "trackball.hpp"

using namespace trackball;

sf::Clock trackball::timer;
sf::Clock trackball::mouseTimer;
GLfloat trackball::rotationX = M_PI / 2;
GLfloat trackball::rotationY = M_PI / 2;
GLfloat trackball::zoom = 50;
GLfloat trackball::rotationSpeedX = 0;
GLfloat trackball::rotationSpeedY = 0;
GLfloat trackball::zoomSpeed = 0;
GLfloat trackball::rotationXDamping = 0.9;
GLfloat trackball::rotationYDamping = 0.9;
GLfloat trackball::zoomDamping = 0.9;
GLfloat trackball::camSpeed = 5;
GLfloat trackball::mouseX = 0;
GLfloat trackball::mouseY = 0;
GLfloat trackball::mouseSpeedX = 0;
GLfloat trackball::mouseSpeedY = 0;
bool	trackball::dragging = false;
GLfloat	trackball::startDraggingX = 0;
GLfloat	trackball::startDraggingY = 0;
GLfloat	trackball::startDraggingRotationX = 0;
GLfloat	trackball::startDraggingRotationY = 0;
GLfloat	trackball::startDraggingZoom = 0;
bool	trackball::draggingZoom = false;

void trackball::trackball(gle::Camera* camera)
{
  static GLfloat oldMouseX = mouseX;
  static GLfloat oldMouseY = mouseY;

  GLfloat elapsed = timer.getElapsedTime().asMicroseconds();
  GLfloat speed = elapsed * (camSpeed/1000000);

  // Calc mouse speed and applie speed damping every 10ms
  if (mouseTimer.getElapsedTime().asMilliseconds() > 10)
    {
      mouseSpeedX = -1 * (GLfloat)(oldMouseX - mouseX) / elapsed * 1000000;
      mouseSpeedY = -1 * (GLfloat)(oldMouseY - mouseY) / elapsed * 1000000;
      oldMouseX = mouseX;
      oldMouseY = mouseY;

      rotationSpeedX *= rotationXDamping;
      rotationSpeedY *= rotationYDamping;
      zoomSpeed *= zoomDamping;

      mouseTimer.restart();
    }

  if (dragging)
    {
      if (draggingZoom)
	zoom = startDraggingZoom - (camSpeed * 15) * (startDraggingY - mouseY);
      else
	{
	  rotationY = startDraggingRotationY - camSpeed * (startDraggingX - mouseX);
	  rotationX = startDraggingRotationX + camSpeed * (startDraggingY - mouseY);
	}
    }
  else
    {
      rotationX -= speed * rotationSpeedX;
      rotationY += speed * rotationSpeedY;
      zoom += (speed * 15) * zoomSpeed;
    }

  rotationX = rotationX > (M_PI - 0.001) ? (M_PI - 0.001) : rotationX;
  rotationX = rotationX < 0.001 ? 0.001 : rotationX;

  gle::Vector3f center = camera->getTarget();
  camera->setPosition(gle::Vector3f(center.x + sin(rotationX) * cos(rotationY) * zoom,
				    center.y + cos(rotationX) * zoom,
				    center.z + sin(rotationX) * sin(rotationY) * zoom));

  timer.restart();
}

void trackball::event(sf::Event &event, sf::Window & window)
{
  sf::Vector2u size = window.getSize();
  if (event.type == sf::Event::MouseMoved)
    {
      sf::Vector2i mouse = sf::Mouse::getPosition(window);
      mouseX = (GLfloat)(mouse.x - (GLfloat)size.x / 2)
      	/ ((GLfloat)size.x / 2);
      mouseY = (GLfloat)(mouse.y - (GLfloat)size.y / 2)
      	/ ((GLfloat)size.y / 2);
    }
  if (event.type == sf::Event::MouseButtonPressed
      && (event.mouseButton.button == sf::Mouse::Left
	  || event.mouseButton.button == sf::Mouse::Right))
    {
      draggingZoom = event.mouseButton.button == sf::Mouse::Right;
      startDraggingX = mouseX;
      startDraggingY = mouseY;
      startDraggingRotationX = rotationX;
      startDraggingRotationY = rotationY;
      startDraggingZoom = zoom;
      dragging = true;
      rotationSpeedX = 0;
      rotationSpeedY = 0;
      zoomSpeed = 0;
    }
  if (event.type == sf::Event::MouseButtonReleased
      && (event.mouseButton.button == sf::Mouse::Left
	  || event.mouseButton.button == sf::Mouse::Right))
    {
      dragging = false;
      if (draggingZoom)
	zoomSpeed = mouseSpeedY;
      else
	{
	  rotationSpeedX = mouseSpeedY;
	  rotationSpeedY = mouseSpeedX;
	}
      rotationSpeedX = rotationSpeedX > 1 ? 1 : rotationSpeedX < -1 ? -1 : rotationSpeedX;
      rotationSpeedY = rotationSpeedY > 1 ? 1 : rotationSpeedY < -1 ? -1 : rotationSpeedY;
      zoomSpeed = zoomSpeed > 1 ? 1 : zoomSpeed < -1 ? -1 : zoomSpeed;
    }
}
