//
// BufferManager.hpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Apr 13 12:43:29 2012 gael jochaud-du-plessix
// Last update Mon Apr 30 21:16:58 2012 gael jochaud-du-plessix
//

#ifndef _GLE_BUFFER_MANAGER_HPP_
# define _GLE_BUFFER_MANAGER_HPP_

# include <algorithm>
# include <list>
# include <Singleton.hpp>
# include <Buffer.hpp>

namespace gle {
  
  template<typename UnderClass, typename T>
  class BufferManager : public Singleton<UnderClass>
  {
  protected:
    BufferManager() : _buffer(), _chunks(), _freeChunks()
    {
    }
    ~BufferManager()
    {
      drain();
      if (_buffer)
	delete _buffer;
    }

    void setStorageBuffer(gle::Buffer<T> *buffer)
    {
      _buffer = buffer;
    }

    gle::Buffer<T>* getStorageBuffer() const
    {
      return (_buffer);
    }

  public:
    static const GLsizeiptr PageSize = 4;

    class Chunk
    {      
    public:
      static const GLsizeiptr MinSize = 4;

      Chunk(GLsizeiptr size=0, GLintptr offset=0, bool isFree=true) :
	_offset(offset), _size(size), _isFree(free), _refCount(1)
      {
      }
      ~Chunk() {}

      GLintptr getOffset() const
      {
	return (_offset);
      }

      GLsizeiptr getSize() const
      {
	return (_size);
      }

      void setOffset(GLintptr offset)
      {
	_offset = offset;
      }

      void setSize(GLsizeiptr size)
      {
	_size = size;
      }
      
      bool isFree() const
      {
	return (_isFree);
      }

      void setFree(bool free)
      {
	_isFree = free;
      }

      void retain()
      {
	_refCount = _refCount + 1;
      }

      void release()
      {
	_refCount = _refCount - 1;
      }

    private:
      GLintptr		_offset;
      GLsizeiptr	_size;
      bool		_isFree;
      int		_refCount;
    };
    
    void drain()
    {
      for (Chunk* &chunk : _chunks)
	delete chunk;
    }

    Chunk* store(void* data, GLsizeiptr size)
    {
      Chunk* bestChunk = NULL;
      typename std::list<Chunk*>::iterator bestChunkIt;

      for (typename std::list<Chunk*>::iterator it = _freeChunks.begin(),
	     end = _freeChunks.end(); it != end; ++it)
	{
	  Chunk* chunk = *it;
	  if (chunk->getSize() <= size
	      && (!bestChunk || bestChunk->getSize() > chunk->getSize()))
	    {
	      bestChunk = chunk;
	      bestChunkIt = it;
	    }
	}
      if (!bestChunk)
	{
	  Chunk* newChunk = new Chunk(size);
	  GLsizeiptr lastElemSize = 0;
	  if (_chunks.size() > 0)
	    {
	      typename std::list<Chunk*>::iterator last = --(_chunks.end());
	      if ((*last)->isFree())
		lastElemSize = (*last)->getSize();
	    }
	  gle::Buffer<T>* newBuffer = new gle::Buffer<T>(*_buffer, size - lastElemSize);
	}
      bestChunkIt = _freeChunks.erase(bestChunkIt);
      bestChunk->setFree(false);
      if (bestChunk->getSize() > (size + BufferManager::Chunk::MinSize))
	{
	  Chunk* newChunk = new Chunk(bestChunk->getSize() - size,
				      bestChunk->getOffset() + size);
	  _freeChunks.insert(bestChunkIt, newChunk);
	  bestChunk->setSize(size);
	  bestChunkIt = std::find(_chunks.begin(), _chunks.end(), bestChunk);
	  _chunks.insert(++bestChunkIt, newChunk);
	}
      return (bestChunk);
    }

  private:
    Buffer<T>*		_buffer;
    std::list<Chunk*>	_chunks;
    std::list<Chunk*>	_freeChunks;
  };  
}
  
# endif
