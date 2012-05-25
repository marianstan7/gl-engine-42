//
// fps.cpp for  in /home/jochau_g//dev/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Thu May 24 23:14:25 2012 gael jochaud-du-plessix
// Last update Fri May 25 00:26:56 2012 gael jochaud-du-plessix
//

#include <iostream>

#include "fps.hpp"

using namespace fps;

sf::Clock	fps::timer;
sf::Clock	fps::limitTimer;
int		fps::calcInterval = 1000;
GLuint		fps::framecount = 0;

void fps::limit(GLuint max)
{
  GLfloat elapsed = limitTimer.getElapsedTime().asMicroseconds();
  if (elapsed < 1000000/max)
    sf::sleep(sf::microseconds(1000000/max - elapsed));
  limitTimer.restart();
}

void fps::print()
{
  if (timer.getElapsedTime().asMilliseconds() >= calcInterval)
    {
      std::cout << "fps:" << ((float)(framecount * 1000) / calcInterval) << std::endl;
      framecount = 0;
      timer.restart();
    }
  framecount++;
}
