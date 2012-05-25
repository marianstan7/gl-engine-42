//
// fps.hpp for  in /home/jochau_g//dev/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Thu May 24 23:09:31 2012 gael jochaud-du-plessix
// Last update Fri May 25 00:15:53 2012 gael jochaud-du-plessix
//

#ifndef _FPS_HPP_
# define _FPS_HPP_

# include <SFML/System.hpp>
# include <opengl.h>

namespace fps {
  extern sf::Clock	limitTimer;
  extern sf::Clock	timer;
  extern int		calcInterval;
  extern GLuint		framecount;

  void limit(GLuint max);
  void print();
};

#endif
