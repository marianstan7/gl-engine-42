//
// Singleton.hpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Apr 13 14:35:11 2012 gael jochaud-du-plessix
// Last update Fri Apr 13 14:36:58 2012 gael jochaud-du-plessix
//

#ifndef _GLE_SINGLETON_HPP_
# define _GLE_SINGLETON_HPP_

namespace gle {

  template<typename T> class Singleton
  {
  public:
    static T& getInstance()
    {
      static T instance;
      return instance;
    }
  };

}

#endif
