//
// IndexBufferManager.hpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Apr 13 17:10:24 2012 gael jochaud-du-plessix
// Last update Wed May  2 15:18:29 2012 gael jochaud-du-plessix
//

#ifndef _INDEX_BUFFER_MANAGER_HPP_
# define _INDEX_BUFFER_MANAGER_HPP_

# include <BufferManager.hpp>

namespace gle {
  class IndexBufferManager : public BufferManager<IndexBufferManager, GLfloat>
  {
    friend class Singleton<IndexBufferManager>;

  private:
    IndexBufferManager()
      : BufferManager()
    {
      setStorageBuffer(new fBuffer(fBuffer::ElementArray,
                                   fBuffer::StaticDraw));
    }
    ~IndexBufferManager()
    {
    }

  public:
  };
}

# endif
