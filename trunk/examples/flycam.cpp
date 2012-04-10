//
// flycam.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar  2 17:27:21 2012 gael jochaud-du-plessix
// Last update Fri Apr  6 16:16:18 2012 gael jochaud-du-plessix
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

#include "flycam.hpp"

using namespace flycam;

GLfloat flycam::teta = M_PI / 2;
GLfloat flycam::phi = 0;
std::map<sf::Keyboard::Key, bool> flycam::keyState;
GLfloat flycam::mouseX = 0.0;
GLfloat flycam::mouseY = 0.0;
GLfloat flycam::moveUp = 0.0;
GLfloat flycam::mouseSensibility = 1;
GLfloat flycam::camSpeed = 2;

void flycam::flycam(gle::Camera* camera)
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
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    pos += gle::Vector3<GLfloat>(0, 1 * (camSpeed), 0);
  if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    pos -= gle::Vector3<GLfloat>(0, 1 * (camSpeed), 0);

  // Joystick moves
  GLfloat rightY =
    (GLfloat)sf::Joystick::getAxisPosition(1, sf::Joystick::V) / 100;
  GLfloat rightX =
    (GLfloat)sf::Joystick::getAxisPosition(1, sf::Joystick::U) / 100;
  GLfloat leftX =
    (GLfloat)sf::Joystick::getAxisPosition(1, sf::Joystick::X) / 100;
  GLfloat leftY =
    (GLfloat)sf::Joystick::getAxisPosition(1, sf::Joystick::Y) / 100;
  GLfloat trigLeft =
    (((GLfloat)sf::Joystick::getAxisPosition(1, sf::Joystick::Z) / 100)
     + 1) / 2;
  GLfloat trigRight =
    (((GLfloat)sf::Joystick::getAxisPosition(1, sf::Joystick::R) / 100)
     + 1) / 2;

  if (std::abs(rightY) > 0.1)
    teta += (M_PI / 5) * rightY * camSpeed / 30;
  if (std::abs(rightX) > 0.1)
    phi += (M_PI / 5) * rightX * camSpeed / 30;

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

void flycam::event(sf::Event &event, sf::Window & app)
{
  sf::Vector2u size = app.getSize();
  if (event.type == sf::Event::KeyPressed)
    keyState[event.key.code] = true;
  else if (event.type == sf::Event::KeyReleased)
    keyState[event.key.code] = false;
  if (event.type == sf::Event::MouseMoved)
    {
      sf::Vector2i mouse = sf::Mouse::getPosition(app);
      mouseX = (GLfloat)(mouse.x - (GLfloat)size.x / 2)
	/ ((GLfloat)size.x / 2);
      mouseY = (GLfloat)(mouse.y - (GLfloat)size.y / 2)
	/ ((GLfloat)size.y / 2);
    }
}
