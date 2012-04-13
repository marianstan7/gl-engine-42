//
// Array.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Tue Feb 28 11:10:10 2012 gael jochaud-du-plessix
// Last update Fri Apr 13 14:49:46 2012 gael jochaud-du-plessix
//

#ifndef _GLE_ARRAY_HPP_
# define _GLE_ARRAY_HPP_

# include <vector>
# include <Vector3.hpp>
# include <Color.hpp>

namespace gle {

  //! An array of objects
  /*!
    This class is an abstraction for an array object.
    It inherits from std::vector and add the ability to cast it to
    a T*, and to push several values in one time.
    \tparam T The type of objects to store
    \tparam Allocator The allocator object used by std::vector
   */

  template <typename T, typename Allocator=std::allocator<T> >
  class Array : public std::vector<T, Allocator> {
  public:
    
    //! Construct an array and reserve the specified capacity

    Array(size_t capacity=0) : std::vector<T, Allocator>()
    {
      this->reserve(capacity);
    }
    
    //! Return a pointer to the objects as a T*

    operator T*()
    {
      if (this->size() > 0)
	return (&((*this)[0]));
      return (NULL);
    }

    //! Return a pointer to the objects as a T*    

    operator T const *() const
    {
      if (this->size() > 0)
	return (&((*this)[0]));
      return (NULL);
    }

    //! Push one value into the array

    Array& push(T const & v)
    {
      this->push_back(v);
      return (*this);
    }

    //! Push two values into the array

    Array& push(T const & v1, T const & v2)
    {
      this->push_back(v1);
      this->push_back(v2);
      return (*this);
    }

    //! Push three values into the array

    Array& push(T const & v1, T const & v2, T const & v3)
    {
      this->push_back(v1);
      this->push_back(v2);
      this->push_back(v3);
      return (*this);
    }

    //! Push four values into the array

    Array& push(T const & v1, T const & v2, T const & v3, T const & v4)
    {
      this->push_back(v1);
      this->push_back(v2);
      this->push_back(v3);
      this->push_back(v4);
      return (*this);
    }

    //! Push three values into the array, through a Vector3

    Array& push(gle::Vector3<T> const & v)
    {
      this->push_back(v.x);
      this->push_back(v.y);
      this->push_back(v.z);
      return (*this);
    }

    //! Push four values into the array, through a rgba Color

    Array& push(gle::Color<T> const & c)
    {
      this->push_back(c.r);
      this->push_back(c.g);
      this->push_back(c.b);
      this->push_back(c.a);
      return (*this);
    }
    
  };

}

#endif
