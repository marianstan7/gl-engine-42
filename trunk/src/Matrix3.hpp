//
// Matrix3.hpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Feb 23 14:42:48 2012 loick michard
// Last update Thu Apr 12 22:17:58 2012 loick michard
//

#ifndef _MATRIX3_HPP
# define _MATRIX3_HPP

# include <Matrix4.hpp>

namespace gle {

  //! Class for manipulate 3x3 matrices
  /*!
    This class encapsulates all 3x3 matrices operations.
    \tparam T Type of matrix values
  */
  template <typename T>
  class Matrix3 {
  public:

    
    // ! Construct and initialise an identity 3x3 matrix
    /*!
      Create an identity matrix:
      \f[ \begin{bmatrix} 1 & 0 & 0\\
      0 & 1 & 0\\
      0 & 0 & 1 \end{bmatrix}
      \f]
     */

    Matrix3()
    {
      this->identity();
    }

    
    // ! Construct and initialise a 3x3 matrix with values
    /*!
      Create a matrix:
      \f[ \begin{bmatrix} a & b & c\\
      d & e & f\\
      g & h & i \end{bmatrix}
      \f]
     */
    
    Matrix3(T const& a, T const& b, T const& c,
	    T const& d, T const& e, T const& f,
	    T const& g, T const& h, T const& i)
    {
      _matrix[0] = a;
      _matrix[1] = d;
      _matrix[2] = g;
      _matrix[3] = b;
      _matrix[4] = e;
      _matrix[5] = h;
      _matrix[6] = c;
      _matrix[7] = f;
      _matrix[8] = i;
    }
    

    //! Contruct a 3x3 matrix from a 4x4 matrix
    /*!
      \param other A 4x4 matrix
     */
    Matrix3(Matrix4<T> const& other)
    {
      _matrix[0] = other[0];
      _matrix[1] = other[1];
      _matrix[2] = other[2];
      _matrix[3] = other[4];
      _matrix[4] = other[5];
      _matrix[5] = other[6];
      _matrix[6] = other[8];
      _matrix[7] = other[9];
      _matrix[8] = other[10];
    }

    //! Assign values from a 4x4 matrix
    /*!
      \param other A 4x4 matrix
     */
    Matrix3& operator=(Matrix4<T> const& other)
    {
      _matrix[0] = other[0];
      _matrix[1] = other[1];
      _matrix[2] = other[2];
      _matrix[3] = other[4];
      _matrix[4] = other[5];
      _matrix[5] = other[6];
      _matrix[6] = other[8];
      _matrix[7] = other[9];
      _matrix[8] = other[10];
      return (*this);
    }

    //! Destruct a 3x3 matrix
    ~Matrix3(){}
    
    //! Load the identity matrix
    Matrix3& identity()
    {
      _matrix[0] = _matrix[4] = _matrix[8] = 1;
      _matrix[1] = _matrix[2] = _matrix[3] =
	_matrix[5] = _matrix[6] = _matrix[7] = 0;
      return (*this);
    }

    //! Update the matrix to its transpose matrix
    Matrix3& transpose()
    {
      T tmp1 = _matrix[1], tmp2 = _matrix[2], tmp3 = _matrix[5];
      
      _matrix[1] = _matrix[3];
      _matrix[2] = _matrix[6];
      _matrix[3] = tmp1;
      _matrix[5] = _matrix[7];
      _matrix[6] = tmp2;
      _matrix[7] = tmp3;
      return (*this);
    }

    //! Return a pointer to the matrix values
    operator T*()
    {
      return (_matrix);
    }

    operator const T*() const
    {
      return (_matrix);
    }
  private:
    T _matrix[9];
  };
  
}

#endif
  
