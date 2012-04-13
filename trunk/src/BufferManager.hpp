//
// BufferManager.hpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Apr 13 12:43:29 2012 gael jochaud-du-plessix
// Last update Fri Apr 13 18:05:56 2012 gael jochaud-du-plessix
//

#ifndef _GLE_BUFFER_MANAGER_HPP_
# define _GLE_BUFFER_MANAGER_HPP_

# include <list>
# include <Singleton.hpp>
# include <Buffer.hpp>

namespace gle {
  
  template<typename UnderClass, typename T>
  class BufferManager : public Singleton<UnderClass>
  {
  protected:
    BufferManager() : _buffer(NULL) {}
    ~BufferManager()
    {
      if (_buffer)
	{
	  drain();
	  delete _buffer;
	}
    }

    void setStorageBuffer(Buffer<T> *buffer)
    {
      if (_buffer)
	{
	  drain();
	  delete _buffer;
	}
      _buffer = buffer;
    }

  public:
    class Chunk
    {
    public:
      Chunk() {}
      ~Chunk() {}

      GLintptr getOffset() const
      {
	return (_offset);
      }

      GLsizeiptr getSize() const
      {
	return (_size);
      }

    private:
      GLintptr _offset;
      GLsizeiptr _size;
      int _refCount;
    };
    
    void drain()
    {
      for (Chunk* &chunk : _chunks)
	delete chunk;
    }

    Chunk* store(void* data, GLsizeiptr size)
    {
      Chunk* bestChunk = NULL;

      for (Chunk* &chunk : _freeChunks)
	{
	  if (chunk->getSize() <= size
	      && (!bestChunk || bestChunk->getSize() > chunk->getSize()))
	    bestChunk = chunk;
	}
    }
    
  private:
    Buffer<T>*		_buffer;
    std::list<Chunk*>	_chunks;
    std::list<Chunk*>	_freeChunks;
  };  
}
  
# endif
