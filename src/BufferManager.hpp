//
// BufferManager.hpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Apr 13 12:43:29 2012 gael jochaud-du-plessix
// Last update Wed May 30 16:19:50 2012 gael jochaud-du-plessix
//

#ifndef _GLE_BUFFER_MANAGER_HPP_
# define _GLE_BUFFER_MANAGER_HPP_

# include <algorithm>
# include <list>
# include <Singleton.hpp>
# include <Buffer.hpp>

namespace gle {
  
  //! Class for managing a lot of data in one buffer
  /*!
    This class allows to store data in chunks of memory allocated
    on the gpu.
    It allows to store data, to resize a chunk and to free its memory
    in order to reuse it later.
   */
  template<typename UnderClass, typename T>
  class BufferManager : public Singleton<UnderClass>
  {
  protected:
    BufferManager() : _buffer(), _chunks(), _freeChunks()
    {
    }
    ~BufferManager()
    {
      if (0)
      for (Chunk* &chunk : _chunks)
	{
	  std::cout << " - " << chunk->getOffset() << ": " << chunk->getSize()
		    << ", free: " << chunk->isFree() << "\n";
	}
      drain();
      if (_buffer)
	delete _buffer;
    }

    //! Set the internal OpenGL buffer used to store the data

    void setStorageBuffer(gle::Buffer<T> *buffer)
    {
      _buffer = buffer;
    }

    //! Return the internal OpenGL buffer used to store the data

    gle::Buffer<T>* getStorageBuffer() const
    {
      return (_buffer);
    }

  public:
    
    //! Size of pages for allocations on the gpu

    static const GLsizeiptr PageSize = 4096;

    //! Class representing a chunk of memory in a BufferManager
    /*!
      Chunks represent space allocated in the gpu memory and managed
      by a BufferManager.
      They are used internally by the BufferManager to partition its allocated
      space and are returned when data is stored.
     */

    class Chunk
    {
    public:

      //! Minimum size that has to be allocated for the data of a chunk

      static const GLsizeiptr MinSize = 4;

      //! Constructs a chunk
      /*!
	Constructs the representation of a new chunk. This does not allocate
	memory and may not be used directly. Instead, call the store() function
	of BufferManager.
	\param size Size of the chunk
	\param offset Offset in the BufferManager memory
	\param Indicated wether the chunk represents a free or used memory
       */

      Chunk(GLsizeiptr size=0, GLintptr offset=0, bool isFree=true) :
	_offset(offset), _size(size), _isFree(isFree), _refCount(1)
      {
      }

      //! Destructs a chunk
      /*!
	The data is not freed.
	Chunks may not be destructed directly, instead use the free()
	functions from the associated BufferManager.
       */

      ~Chunk()
      {
      }


      //! Returns the offset of the chunk in memory

      GLintptr getOffset() const
      {
	return (_offset);
      }

      //! Returns the size of the chunk

      GLsizeiptr getSize() const
      {
	return (_size);
      }

      //! Set the offset of the chunk in memory

      void setOffset(GLintptr offset)
      {
	_offset = offset;
      }
      
      //! Set the size of the chunk

      void setSize(GLsizeiptr size)
      {
	_size = size;
      }

      //! Indicates wether the chunk represents a free or used memory
      
      bool isFree() const
      {
	return (_isFree);
      }

      //! Set wether the chunk is free or not

      void setFree(bool free)
      {
	_isFree = free;
      }

      //! Set the data of the chunk
      /*!
	Save data in the memory allocated for the chunk.
	\param data A pointer to the data to save in the chunk,
	must be at least as big as the size of the chunk.
       */

      void setData(const void* data)
      {
	gle::Buffer<T>* buffer = UnderClass::getInstance().getStorageBuffer();

	buffer->setData(reinterpret_cast<const T*>(data), _offset, _size);
      }

      //! Map the chunk memory

      T* map(typename gle::Buffer<T>::MapAccess access=gle::Buffer<T>::ReadWrite)
      {
	gle::Buffer<T>* buffer = UnderClass::getInstance().getStorageBuffer();

	return (buffer->map(_offset, _size, access));
      }

      //! Unmap the chunk memory

      void unmap()
      {
	gle::Buffer<T>* buffer = UnderClass::getInstance().getStorageBuffer();

        buffer->unmap();
      }

      //! Set the reference count of the chunk

      void setRefCount(int count)
      {
	_refCount = count;
      }

      //! Increments the references counter of the chunk

      void retain()
      {
	_refCount = _refCount + 1;
      }

      //! Decrement the references counter of the chunk

      void release()
      {
	_refCount = _refCount - 1;
	if (_refCount == 0)
	  UnderClass::getInstance().free(this);
      }

    private:
      GLintptr		_offset;
      GLsizeiptr	_size;
      bool		_isFree;
      int		_refCount;
    };

    //! Print informations about the BufferManager on standart output

    void print()
    {
      for (Chunk* &chunk : _chunks)
	{
	  std::cout << chunk << " - " << chunk->getOffset() << ": " << chunk->getSize()
		    << ", free: " << chunk->isFree() << "\n";
	}
    }

    //! Bind the internal storage buffer of the BufferManager

    void bind()
    {
      if (_buffer)
	_buffer->bind();
    }

    //! Delete all the chunks of the BufferManager
    
    void drain()
    {
      for (Chunk* &chunk : _chunks)
	delete chunk;
    }

    //! Resize a chunk of memory
    /*!
      If the new size if greater than the precedent, the BufferManager
      creates a new chunk and copy the data from the old to the new one.
      If the new size is smaller than the precedent, the chunk is resized
      and the leaving memory can be reused by the BufferManager.
      \param chunk The chunk to resize
      \param size New size for the chunk
      \param data New data to store in the chunk
      \return A new chunk representing the resized memory
     */

    Chunk* resize(Chunk *chunk, GLsizeiptr size, const void* data=NULL)
    {
      typename decltype(_chunks)::iterator it = find(_chunks.begin(), _chunks.end(), chunk);
      if (it == _chunks.end())
	return (NULL);
      typename decltype(_chunks)::iterator next = (++it);
      --it;
      if (size <= chunk->getSize())
	return (chunk);
      if (next != _chunks.end() && (*next)->isFree() &&
	  (*it)->getSize() + (*next)->getSize() >= size)
	{
	  (*next)->setSize((*next)->getSize() - size + (*it)->getSize());
	  (*it)->setSize(size);
	  if ((*next)->getSize() == 0)
	    {
	      Chunk* tmp = *next;
	      _freeChunks.remove(tmp);
	      _chunks.erase(next);
	      delete tmp;
	    }
	  if (data)
	    (*it)->setData(data);
	  return (*it);
	}
      if (data)
	{
	  free(chunk);
	  return (store(data, size));
	}
      Chunk* newChunk = store(NULL, size);
      glBindBuffer(GL_COPY_WRITE_BUFFER, _buffer->getId());
      glBindBuffer(GL_COPY_READ_BUFFER, _buffer->getId());
      glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER,
			  chunk->getOffset(), newChunk->getOffset(),
			  chunk->getSize());
      free(chunk);
      return (newChunk);
    }

    //! Store data in the buffer manager
    /*!
      Returns a new chunk of memory allocated by the buffer manager
      \param data The data to store in the chunk. Can be NULL to just allocate the memory
      \param size The size of the data to store
      \return The new allocated chunk of memory
    */

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
	  if (data)
	    newChunk->setData(data);
	  return (newChunk);
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
      if (data)
	bestChunk->setData(data);
      return (bestChunk);
    }

    //! Free a memory chunk
    /*!
      Deallocate a chunk of memory. This function delete the chunk
      if necessary.
      \param chunk The chunk to free
     */

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
      newFreeNode->setRefCount(1);
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
