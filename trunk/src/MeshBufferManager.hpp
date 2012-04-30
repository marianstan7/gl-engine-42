//
// MeshBufferManager.hpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Apr 13 17:10:24 2012 gael jochaud-du-plessix
// Last update Mon Apr 30 20:19:19 2012 gael jochaud-du-plessix
//

#ifndef _MESH_BUFFER_MANAGER_HPP_
# define _MESH_BUFFER_MANAGER_HPP_

# include <BufferManager.hpp>

namespace gle {
  class MeshBufferManager : public BufferManager<MeshBufferManager, GLfloat>
  {
    friend class Singleton<MeshBufferManager>;

  private:
    MeshBufferManager()
      : BufferManager()
    {
      setStorageBuffer(new fBuffer(fBuffer::VertexArray,
                                   fBuffer::StaticDraw));
    }
    ~MeshBufferManager()
    {
    }

  public:
  };
}

# endif
