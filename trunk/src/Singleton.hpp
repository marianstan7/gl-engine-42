//
// Singleton.hpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Apr 13 14:35:11 2012 gael jochaud-du-plessix
// Last update Wed Jul  4 17:16:47 2012 gael jochaud-du-plessix
//

#ifndef _GLE_SINGLETON_HPP_
# define _GLE_SINGLETON_HPP_

namespace gle {

  //! Utility class for creating singletons

  template<typename T> class Singleton
  {
  public:

    //! Returns the unique instance of the singleton
    static T& getInstance()
    {
      static T instance;
      return instance;
    }
  };

}

#endif
