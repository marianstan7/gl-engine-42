//
// video.hpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar 16 10:54:32 2012 gael jochaud-du-plessix
// Last update Fri Mar 16 11:11:44 2012 gael jochaud-du-plessix
//

#ifndef _VIDEO_HPP_
# define _VIDEO_HPP_

#include <iomanip>
#include <iostream>
#include <sstream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace video {
  extern sf::Clock videoTimer;
  extern int currentVideoFrame;
  extern sf::Texture videoTexture;

  void saveImage(sf::Window &app, int framerate);
  void save(std::string name, int framerate);
};

#endif
