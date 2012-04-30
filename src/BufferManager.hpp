//
// BufferManager.hpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Apr 13 12:43:29 2012 gael jochaud-du-plessix
// Last update Tue May  1 01:22:27 2012 gael jochaud-du-plessix
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
      for (Chunk* &chunk : _chunks)
	{
	  std::cout << " - " << chunk->getOffset() << ": " << chunk->getSize()
		    << ", free: " << chunk->isFree() << "\n";
	}
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
    static const GLsizeiptr PageSize = 4096;

    class Chunk
    {      
    public:
      static const GLsizeiptr MinSize = 4;

      Chunk(GLsizeiptr size=0, GLintptr offset=0, bool isFree=true) :
	_offset(offset), _size(size), _isFree(isFree), _refCount(1)
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
	if (_refCount < 0)
	  delete this;
      }

      void setData(const void* data)
      {
	gle::Buffer<T>* buffer = UnderClass::getInstance().getStorageBuffer();

	buffer->setData(reinterpret_cast<const T*>(data), _offset, _size);
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

    Chunk* store(const void* data, GLsizeiptr size)
    {
      Chunk* bestChunk = NULL;
      typename std::list<Chunk*>::iterator bestChunkIt;

      for (typename std::list<Chunk*>::iterator it = _freeChunks.begin(),
	     end = _freeChunks.end(); it != end; ++it)
	{
	  Chunk* chunk = *it;
	  if (chunk->getSize() >= size
	      && (!bestChunk || bestChunk->getSize() > chunk->getSize()))
	    {
	      bestChunk = chunk;
	      bestChunkIt = it;
	    }
	}
      if (!bestChunk)
	{
	  Chunk* newChunk = new Chunk(0, 0);
	  if (_chunks.size() > 0)
	    {
	      typename std::list<Chunk*>::iterator last = --(_chunks.end());
	      if ((*last)->isFree())
		{
		  delete newChunk;
		  _freeChunks.remove(*last);
		  newChunk = *last;
		}
	      else
		newChunk->setOffset((*last)->getOffset() + (*last)->getSize());
	    }
	  GLsizeiptr allocationSize = (((size - newChunk->getSize()) + BufferManager::PageSize - 1) /
				       BufferManager::PageSize) * BufferManager::PageSize;
	  gle::Buffer<T>* newBuffer = new gle::Buffer<T>(*_buffer, _buffer->getSize() + allocationSize);
	  delete _buffer;
	  _buffer = newBuffer;
	  if (newChunk->getSize() == 0)
	    _chunks.insert(_chunks.end(), newChunk);
	  if (allocationSize + newChunk->getSize() > size)
	    {
	      Chunk* newFreeNode = new Chunk(allocationSize + newChunk->getSize() - size,
					     newChunk->getOffset() + size);
	      _chunks.insert(_chunks.end(), newFreeNode);
	      _freeChunks.insert(_freeChunks.end(), newFreeNode);
	    }
	  newChunk->setFree(false);
	  newChunk->setSize(size);
	  newChunk->setData(data);
	  return (newChunk);
	}
      std::cout << "--> " << bestChunk->getSize() << "\n";
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
      bestChunk->setData(data);
      return (bestChunk);
    }

    void free(Chunk* chunk)
    {
      Chunk* newFreeNode = NULL;
      typename decltype(_chunks)::iterator it = find(_chunks.begin(), _chunks.end(), chunk);

      if (it == _chunks.end())
	return;
      newFreeNode = *it;
      typename decltype(_chunks)::iterator next = ++it;
      --it;
      if (next != _chunks.end() && (*next)->isFree())
	{
	  Chunk* tmp = *next;
	  (*it)->setSize((*it)->getSize() + (*next)->getSize());
	  _freeChunks.remove(*next);
	  _chunks.erase(next);
	  delete (tmp);
	}
      if (it != _chunks.begin())
	{
	  typename decltype(_chunks)::iterator prev = --it;
	  ++it;
	  if ((*prev)->isFree())
	    {
	      Chunk* tmp = *it;
	      newFreeNode = *prev;
	      (*prev)->setSize((*prev)->getSize() + (*it)->getSize());
	      _chunks.erase(it);
	      delete (tmp);
	      return;
	    }
	}
      newFreeNode->setFree(true);
      for (typename decltype(_chunks)::iterator it = _freeChunks.begin();
	   it != _freeChunks.end(); ++it)
	{
	  if ((*it)->getOffset() > newFreeNode->getOffset())
	    {
	      _freeChunks.insert(it, newFreeNode);
	      return;
	    }
	}
      _freeChunks.insert(_freeChunks.end(), newFreeNode);
    }

  private:
    Buffer<T>*		_buffer;
    std::list<Chunk*>	_chunks;
    std::list<Chunk*>	_freeChunks;
  };  
}
  
# endif
