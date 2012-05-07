//
// MeshUniformsBufferManager.hpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Apr 13 17:10:24 2012 gael jochaud-du-plessix
// Last update Mon May  7 15:50:52 2012 gael jochaud-du-plessix
//

#ifndef _MESHUNIFORMS_BUFFER_MANAGER_HPP_
# define _MESHUNIFORMS_BUFFER_MANAGER_HPP_

# include <BufferManager.hpp>

namespace gle {
  class MeshUniformsBufferManager : public BufferManager<MeshUniformsBufferManager, GLfloat>
  {
    friend class Singleton<MeshUniformsBufferManager>;

  private:
    MeshUniformsBufferManager()
      : BufferManager()
    {
      setStorageBuffer(new fBuffer(fBuffer::UniformArray,
                                   fBuffer::StaticDraw));
    }
    ~MeshUniformsBufferManager()
    {
    }

  public:
  };
}

# endif
