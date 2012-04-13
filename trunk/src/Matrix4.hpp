//
// Matrix4.hpp for gle in /home/michar_l//Projects/OpenGL
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Mon Feb 13 20:57:51 2012 loick michard
// Last update Thu Apr 12 22:13:40 2012 loick michard
//

#ifndef _GLE_MATRIX4_HPP_
# define _GLE_MATRIX4_HPP_

# include <iostream>
# include <cmath>
# include <Vector3.hpp>

namespace gle {

  //! Class for manipulate 4x4 matrices;
  /*!
    This class encapsulates all 4x4 matrices operations.
    \tparam T Type of matrix values
  */
  template <typename T>
  class Matrix4 {

  public:

    // ! Construct and initialise an identity 4x4 matrix
    /*!
      Create an identity matrix:
      \f[ \begin{bmatrix} 1 & 0 & 0 & 0\\
      0 & 1 & 0 & 0\\
      0 & 0 & 1 & 0\\
      0 & 0 & 0 & 1 \end{bmatrix}
      \f]
     */

    Matrix4()
    {
      identity();
    }

    // ! Construct and initialise a 4x4 matrix
    /*!
      Create a matrix:
      \f[ \begin{bmatrix} value & value & value & value\\
      value & value & value & value\\
      value & value & value & value\\
      value & value & value & value \end{bmatrix}
      \f]
      \param value initialisation value
     */

    Matrix4(T const &value)
    {
      _matrix[0] = _matrix[4] = _matrix[8] = _matrix[12] =
	_matrix[1] = _matrix[5] = _matrix[9] = _matrix[13] =
	_matrix[2] = _matrix[6] = _matrix[10] = _matrix[14] =
	_matrix[3] = _matrix[7]= _matrix[11] = _matrix[15] = value;
    }
    
    // ! Construct and initialise a 4x4 matrix with values
    /*!
      Create a matrix:
      \f[ \begin{bmatrix} a & b & c & d\\
      e & f & g & h\\
      i & j & k & l\\
      m & n & o & p \end{bmatrix}
      \f]
     */

    Matrix4(T const& a, T const& b, T const& c, T const& d,
	    T const& e, T const& f, T const& g, T const& h,
	    T const& i, T const& j, T const& k, T const& l,
	    T const& m, T const& n, T const& o, T const& p)
    {
      _matrix[0] = a;
      _matrix[4] = b;
      _matrix[8] = c;
      _matrix[12] = d;
      _matrix[1] = e;
      _matrix[5] = f;
      _matrix[9] = g;
      _matrix[13] = h;
      _matrix[2] = i;
      _matrix[6] = j;
      _matrix[10] = k;
      _matrix[14] = l;
      _matrix[3] = m;
      _matrix[7] = n;
      _matrix[11] = o;
      _matrix[15] = p;
    }
    
    //! Access to nth element of the matrix
    /*!
      \return The element at index n
    */

    T const & operator[](int n) const
    {
      return (_matrix[n]);
    }

    //! Add a matrix to the current
    /*!
      \param other Matrix to add.
    */

    Matrix4& operator+=(Matrix4 const & other)
    {
      _matrix[0] += other._matrix[0];
      _matrix[4] += other._matrix[4];
      _matrix[8] += other._matrix[8];
      _matrix[12] += other._matrix[12];
      _matrix[1] += other._matrix[1];
      _matrix[5] += other._matrix[5];
      _matrix[9] += other._matrix[9];
      _matrix[13] += other._matrix[13];
      _matrix[2] += other._matrix[2];
      _matrix[6] += other._matrix[6];
      _matrix[10] += other._matrix[10];
      _matrix[14] += other._matrix[14];
      _matrix[3] += other._matrix[3];
      _matrix[7] += other._matrix[7];
      _matrix[11] += other._matrix[11];
      _matrix[15] += other._matrix[15];
      return (*this);
    }

    //! Multiply a matrix by a number
    /*!
      \param value Number to multiply.
    */

    Matrix4& operator*=(T const & value)
    {
      _matrix[0] *= value;
      _matrix[4] *= value;
      _matrix[8] *= value;
      _matrix[12] *= value;
      _matrix[1] *= value;
      _matrix[5] *= value;
      _matrix[9] *= value;
      _matrix[13] *= value;
      _matrix[2] *= value;
      _matrix[6] *= value;
      _matrix[10] *= value;
      _matrix[14] *= value;
      _matrix[3] *= value;
      _matrix[7] *= value;
      _matrix[11] *= value;
      _matrix[15] *= value;
      return (*this);
    }

    //! Do a scalar multiplication
    /*!
      \param value Scalar product
    */

    Matrix4& operator*=(Matrix4 const & value)
    {
      Matrix4 tmp = *this;

      _matrix[0] = tmp._matrix[0] * value._matrix[0] +
	tmp._matrix[4] * value._matrix[1] +
	tmp._matrix[8] * value._matrix[2] +
	tmp._matrix[12] * value._matrix[3];
      _matrix[4] = tmp._matrix[0] * value._matrix[4] +
	tmp._matrix[4] * value._matrix[5] +
	tmp._matrix[8] * value._matrix[6] +
	tmp._matrix[12] * value._matrix[7];
      _matrix[8] = tmp._matrix[0] * value._matrix[8] +
	tmp._matrix[4] * value._matrix[9] +
	tmp._matrix[8] * value._matrix[10] +
	tmp._matrix[12] * value._matrix[11];
      _matrix[12] = tmp._matrix[0] * value._matrix[12] +
	tmp._matrix[4] * value._matrix[13] +
	tmp._matrix[8] * value._matrix[14] +
	tmp._matrix[12] * value._matrix[15];
      
      _matrix[1] = tmp._matrix[1] * value._matrix[0] +
        tmp._matrix[5] * value._matrix[1] +
        tmp._matrix[9] * value._matrix[2] +
        tmp._matrix[13] * value._matrix[3];
      _matrix[5] = tmp._matrix[1] * value._matrix[4] +
        tmp._matrix[5] * value._matrix[5] +
        tmp._matrix[9] * value._matrix[6] +
        tmp._matrix[13] * value._matrix[7];
      _matrix[9] = tmp._matrix[1] * value._matrix[8] +
        tmp._matrix[5] * value._matrix[9] +
        tmp._matrix[9] * value._matrix[10] +
        tmp._matrix[13] * value._matrix[11];
      _matrix[13] = tmp._matrix[1] * value._matrix[12] +
        tmp._matrix[5] * value._matrix[13] +
        tmp._matrix[9] * value._matrix[14] +
        tmp._matrix[13] * value._matrix[15];

      _matrix[2] = tmp._matrix[2] * value._matrix[0] +
        tmp._matrix[6] * value._matrix[1] +
        tmp._matrix[10] * value._matrix[2] +
        tmp._matrix[14] * value._matrix[3];
      _matrix[6] = tmp._matrix[2] * value._matrix[4] +
        tmp._matrix[6] * value._matrix[5] +
        tmp._matrix[10] * value._matrix[6] +
        tmp._matrix[14] * value._matrix[7];
      _matrix[10] = tmp._matrix[2] * value._matrix[8] +
	tmp._matrix[6] * value._matrix[9] +
        tmp._matrix[10] * value._matrix[10] +
        tmp._matrix[14] * value._matrix[11];
      _matrix[14] = tmp._matrix[2] * value._matrix[12] +
        tmp._matrix[6] * value._matrix[13] +
        tmp._matrix[10] * value._matrix[14] +
        tmp._matrix[14] * value._matrix[15];

      _matrix[3] = tmp._matrix[3] * value._matrix[0] +
        tmp._matrix[7] * value._matrix[1] +
        tmp._matrix[11] * value._matrix[2] +
        tmp._matrix[15] * value._matrix[3];
      _matrix[7] = tmp._matrix[3] * value._matrix[4] +
        tmp._matrix[7] * value._matrix[5] +
        tmp._matrix[11] * value._matrix[6] +
        tmp._matrix[15] * value._matrix[7];
      _matrix[11] = tmp._matrix[3] * value._matrix[8] +
	tmp._matrix[7] * value._matrix[9] +
        tmp._matrix[11] * value._matrix[10] +
        tmp._matrix[15] * value._matrix[11];
      _matrix[15] = tmp._matrix[3] * value._matrix[12] +
        tmp._matrix[7] * value._matrix[13] +
        tmp._matrix[11] * value._matrix[14] +
        tmp._matrix[15] * value._matrix[15];
      return (*this);
    }

    //! Set an identity 4x4 matrix
    /*!
      Set an identity matrix:
      \f[ \begin{bmatrix} 1 & 0 & 0 & 0\\
      0 & 1 & 0 & 0\\
      0 & 0 & 1 & 0\\
      0 & 0 & 0 & 1 \end{bmatrix}
      \f]
     */    

    Matrix4& identity(void)
    {
      _matrix[0] = _matrix[5] = _matrix[10] = _matrix[15] = 1;
      _matrix[4] = _matrix[8] = _matrix[12] = _matrix[1] =
	_matrix[9] = _matrix[13] = _matrix[2] = _matrix[6] =
	_matrix[14] = _matrix[3] = _matrix[7] = _matrix[11] = 0;
      return (*this);
    }

    //! Set the matrix to its inverse matrix

    Matrix4& inverse(void)
    {
      T tmp[16];
      tmp[0] = _matrix[0];
      tmp[1] = _matrix[1];
      tmp[2] = _matrix[2];
      tmp[3] = _matrix[3];
      tmp[4] = _matrix[4];
      tmp[5] = _matrix[5];
      tmp[6] = _matrix[6];
      tmp[7] = _matrix[7];
      tmp[8] = _matrix[8];
      tmp[9] = _matrix[9];
      tmp[10] = _matrix[10];
      tmp[11] = _matrix[11];
      tmp[12] = _matrix[12];
      tmp[13] = _matrix[13];
      tmp[14] = _matrix[14];
      tmp[15] = _matrix[15];
      T tmp2[12];

      tmp2[0] = tmp[0]*tmp[5] - tmp[1]*tmp[4];
      tmp2[1] = tmp[0]*tmp[6] - tmp[2]*tmp[4];
      tmp2[2] = tmp[0]*tmp[7] - tmp[3]*tmp[4];
      tmp2[3] = tmp[1]*tmp[6] - tmp[2]*tmp[5];
      tmp2[4] = tmp[1]*tmp[7] - tmp[3]*tmp[5];
      tmp2[5] = tmp[2]*tmp[7] - tmp[3]*tmp[6];
      tmp2[6] = tmp[8]*tmp[13] - tmp[9]*tmp[12];
      tmp2[7] = tmp[8]*tmp[14] - tmp[10]*tmp[12];
      tmp2[8] = tmp[8]*tmp[15] - tmp[11]*tmp[12];
      tmp2[9] = tmp[9]*tmp[14] - tmp[10]*tmp[13];
      tmp2[10] = tmp[9]*tmp[15] - tmp[11]*tmp[13];
      tmp2[11] = tmp[10]*tmp[15] - tmp[11]*tmp[14];
      
      T invDet = 1.0 / (tmp2[0] * tmp2[11] - tmp2[1] * tmp2[10]
			+ tmp2[2] * tmp2[9] + tmp2[3] * tmp2[8]
			- tmp2[4] * tmp2[7] + tmp2[5] * tmp2[6]); 
      _matrix[0] = (tmp[5] * tmp2[11] - tmp[6] * tmp2[10]
		    + tmp[7] * tmp2[9]) * invDet;
      _matrix[1] = (-tmp[1] * tmp2[11] + tmp[2] * tmp2[10]
		    - tmp[3] * tmp2[9]) * invDet;
      _matrix[2] = (tmp[13] * tmp2[5] - tmp[14] * tmp2[4]
		    + tmp[15] * tmp2[3]) * invDet;
      _matrix[3] = (-tmp[9] * tmp2[5] + tmp[10] * tmp2[4] -
		    tmp[11] * tmp2[3]) * invDet;
      _matrix[4] = (-tmp[4] * tmp2[11] + tmp[6] * tmp2[8] -
		    tmp[7] * tmp2[7]) * invDet;
      _matrix[5] = (tmp[0] * tmp2[11] - tmp[2] * tmp2[8] +
		    tmp[3] * tmp2[7]) * invDet;
      _matrix[6] = (-tmp[12] * tmp2[5] + tmp[14] * tmp2[2] -
		    tmp[15] * tmp2[1]) * invDet;
      _matrix[7] = (tmp[8] * tmp2[5] - tmp[10] * tmp2[2] +
		    tmp[11] * tmp2[1]) * invDet;
      _matrix[8] = (tmp[4] * tmp2[10] - tmp[5] * tmp2[8] +
		    tmp[7] * tmp2[6]) * invDet;
      _matrix[9] = (-tmp[0] * tmp2[10] + tmp[1] * tmp2[8] -
		    tmp[3] * tmp2[6]) * invDet;
      _matrix[10] = (tmp[12] * tmp2[4] - tmp[13] * tmp2[2] +
		     tmp[15] * tmp2[0]) * invDet;
      _matrix[11] = (-tmp[8] * tmp2[4] + tmp[9] * tmp2[2] -
		     tmp[11] * tmp2[0]) * invDet;
      _matrix[12] = (-tmp[4] * tmp2[9] + tmp[5] * tmp2[7] -
		     tmp[6] * tmp2[6]) * invDet;
      _matrix[13] = (tmp[0] * tmp2[9] - tmp[1] * tmp2[7] +
		     tmp[2] * tmp2[6]) * invDet;
      _matrix[14] = (-tmp[12] * tmp2[3] + tmp[13] * tmp2[1] -
		     tmp[14] * tmp2[0]) * invDet;
      _matrix[15] = (tmp[8] * tmp2[3] - tmp[9] * tmp2[1] +
		     tmp[10] * tmp2[0]) * invDet;
      return (*this);
    }

    //! Set the matrix to its transposed matrix

    Matrix4& transpose(void)
    {
      T tmp[16];
      tmp[0] = _matrix[0];
      tmp[1] = _matrix[1];
      tmp[2] = _matrix[2];
      tmp[3] = _matrix[3];
      tmp[4] = _matrix[4];
      tmp[5] = _matrix[5];
      tmp[6] = _matrix[6];
      tmp[7] = _matrix[7];
      tmp[8] = _matrix[8];
      tmp[9] = _matrix[9];
      tmp[10] = _matrix[10];
      tmp[11] = _matrix[11];
      tmp[12] = _matrix[12];
      tmp[13] = _matrix[13];
      tmp[14] = _matrix[14];
      tmp[15] = _matrix[15];
                
      _matrix[1] = tmp[4];
      _matrix[2] = tmp[8];
      _matrix[3] = tmp[12];
      _matrix[4] = tmp[1];
      _matrix[6] = tmp[9];
      _matrix[7] = tmp[13];
      _matrix[8] = tmp[2];
      _matrix[9] = tmp[6];
      _matrix[11] = tmp[14];
      _matrix[12] = tmp[3];
      _matrix[13] = tmp[7];
      _matrix[14] = tmp[11];
      return (*this);
    }

    //! Get a scale matrix
    /*!
      \param x Scale coefficient on x
      \param y Scale coefficient on y
      \param z Scale coefficient on z
    */

    static Matrix4 scale(T const& x, T const& y, T const& z)
    {
      return (Matrix4(x, 0, 0, 0,
		      0, y, 0, 0,
		      0, 0, z, 0,
		      0, 0, 0, 1));
    }
    
    //! Return an initialised frustrum 4x4 Matrix
    /*!
      \param left Left coordinate for vertical clipping plane.
      \param right Right coordinate for vertical clipping plane.
      \param bottom Bottom coordinate for horizontal clipping plane.
      \param top Top coordinate for horizontal clipping plane.
      \param near Distance to the near depth clipping plane.
      \param far Distance to the far depth clipping plane.
      \return An initialised frustrum 4x4 Matrix
    */

    static Matrix4 frustrum(T const & left, T const & right, T const &bottom,
			    T const & top, T const & near, T const &far)
    {
      return (Matrix4((2 * near) / (right - left), 0,
		      (right + left) / (right - left), 0,
		      0, (2 * near) / (top - bottom),
		      (top + bottom) / (top - bottom), 0,
		      0, 0, - (far + near) / (far - near),
		      - (2 * near * far) / (far - near),
		      0, 0, -1, 0));
    }

    //! Return an initialised 4x4 Matrix for perspective projection.
    /*!
    \param fovy Field of view angle in degree
    \param aspect Aspect ratio (width / height)
    \param near near Distance to the near depth clipping plane.
    \param far Distance to the far depth clipping plane.
    \return An initialised 4x4 Matrix for perspective projection.
    */

    static Matrix4 perspective(T const & fovy, T const & aspect,
			       T const & near, T const & far)
    {
      T f = 1.0 / tan(fovy * M_PI / 360.0);

      return (Matrix4(f / aspect, 0, 0, 0,
                      0, f, 0, 0,
                      0, 0, (far + near) / (near - far),
                      (2 * near * far) / (near - far),
                      0, 0, -1, 0));
    }

    //! Return an initisialised 4x4 matrix for viewing transformation
    /*!
      \param eyex, eyey, eyez Position of eye.
      \param centerx, centery, centerz Position of reference.
      \param upx, upy, upz Direction of up vector.
      \return An initisialised 4x4 matrix for viewing transformation
    */

    static Matrix4 lookAt(T const& eyex, T const& eyey, T const& eyez,
			  T const& centerx, T const& centery, T const& centerz,
			  T const& upx, T const& upy, T const& upz)
    {
      T xx, xy, xz, yx, yy, yz, zx, zy, zz;

      zx = eyex - centerx;
      zy = eyey - centery;
      zz = eyez - centerz;
      normalizeVector(zx, zy, zz);
      xx = upy * zz - upz * zy;
      xy = upz * zx - upx * zz;
      xz = upx * zy - upy * zx;
      normalizeVector(xx, xy, xz);
      yx = zy * xz - zz * xy;
      yy = zz * xx - zx * xz;
      yz = zx * xy - zy * xx;
      normalizeVector(yx, yy, yz);

      return (Matrix4(xx, xy, xz, -(xx * eyex + xy * eyey + xz * eyez),
		      yx, yy, yz, -(yx * eyex + yy * eyey + yz * eyez),
		      zx, zy, zz, -(zx * eyex + zy * eyey + zz * eyez),
		      0, 0, 0, 1));
    }

    //! Return an initisialised 4x4 matrix for viewing transformation
    /*!
      \param eye Position of eye.
      \param center Position of reference.
      \param up Direction of up vector.
      \return An initisialised 4x4 matrix for viewing transformation
    */

    static Matrix4 lookAt(Vector3<T> const& eye, Vector3<T> const& center,
			  Vector3<T> const& up)
    {
      return (lookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z,
		     up.x, up.y, up.z));
    }

    //! Translate matrix
    /*!
      Set a translation to matrix
      \param x Value of x translation
      \param y Value of y translation
      \param z Value of z translation
    */

    Matrix4& translate(T const & x, T const & y, T const & z)
    {
      
      _matrix[12] = _matrix[0] * x + _matrix[4] * y + 
	_matrix[8] * z + _matrix[12];
      _matrix[13] = _matrix[1] * x + _matrix[5] * y + 
	_matrix[9] * z + _matrix[13];
      _matrix[14] = _matrix[2] * x + _matrix[6] * y + 
	_matrix[10] * z + _matrix[14];
      _matrix[15] = _matrix[3] * x + _matrix[7] * y +
	_matrix[11] * z + _matrix[15];
      return (*this);
    }

    //! Translate matrix
    /*!
      Set a translation to matrix
      \param vec Vector of translation
    */

    Matrix4& translate(Vector3<T> const& vec)
    {
      return (translate(vec.x, vec.y, vec.z));
    }

    //! Get a value from Matrix
    /*!
      \param n Column index
      \param m Row index
      \return Value at colum and row index
    */

    T const & get(int n, int m) const
    {
      return (_matrix[n * 4 + m]);
    }

    //! Set a value to Matrix
    /*!
      \param n Column index
      \param m Row index
      \param value Value to set
    */

    void set(int n, int m, T& value)
    {
      _matrix[n * 4 + m] = value;
    }

    //! Get a pointer to matrix data.
    /*!
      \return A pointer to the matrix data(T*).
    */

    operator T*()
    {
      return (_matrix);
    }

    operator const T*() const
    {
      return (_matrix);
    }

    //! Destroy the matrix

    ~Matrix4() {}
    
  private:
    inline static void normalizeVector(T &x, T &y, T &z)
    {
      T len = sqrt(x * x + y * y + z * z);

      if (!len)
	x = y = z = 0;
      else
	{
	  len = 1.0 / len;
	  x *= len;
	  y *= len;
	  z *= len;
	}
    }
    T _matrix[16];
  };

}

template <typename T>
std::ostream& operator<<(std::ostream& os, gle::Matrix4<T> const &matrix)
{
  os << "Matrix 4x4 {" <<std::endl
     << matrix[0] << ",\t" << matrix[4] <<  ",\t" << matrix[8] <<
    ",\t" << matrix[12] << "," << std::endl
     << matrix[1] << ",\t" << matrix[5] <<  ",\t" << matrix[9] <<
    ",\t" << matrix[13] << "," << std::endl
     << matrix[2] << ",\t" << matrix[6] <<  ",\t" << matrix[10] <<
    ",\t" << matrix[14] << "," << std::endl
     << matrix[3] << ",\t" << matrix[7] <<  ",\t" << matrix[11] <<
    ",\t" << matrix[15] << "," <<  std::endl << "}";
  return (os);
}

template <typename T>
gle::Matrix4<T> operator+(gle::Matrix4<T> const &m1, gle::Matrix4<T> const &m2)
{
  return (gle::Matrix4<T>(m1[0] + m2[0], m1[4] + m2[4], m1[8] + m2[8],
			  m1[12] + m2[12], m1[1] + m2[1], m1[5] + m2[5],
			  m1[9] + m2[9], m1[13] + m2[13], m1[2] + m2[2],
			  m1[6] + m2[6], m1[10] + m2[10], m1[14] + m2[14],
			  m1[3] + m2[3], m1[7] + m2[7], m1[11] + m2[11],
			  m1[15] + m2[15]));
}

template <typename T>
gle::Matrix4<T> operator*(gle::Matrix4<T> const &m1, T const &value)
{
  return (gle::Matrix4<T>(m1[0] * value, m1[4] * value, m1[8] * value,
			  m1[12] * value, m1[1] * value, m1[5] * value,
			  m1[9] * value, m1[13] * value, m1[2] * value,
			  m1[6] * value, m1[10] * value, m1[14] * value,
			  m1[3] * value, m1[7] * value, m1[11] * value,
			  m1[15] * value));
}

template <typename T>
gle::Matrix4<T> operator*(T const &value, gle::Matrix4<T> const &m1)
{
  return (gle::Matrix4<T>(m1[0] * value, m1[4] * value, m1[8] * value,
			  m1[12] * value, m1[1] * value, m1[5] * value,
			  m1[9] * value, m1[13] * value, m1[2] * value,
			  m1[6] * value, m1[10] * value, m1[14] * value,
			  m1[3] * value, m1[7] * value, m1[11] * value,
			  m1[15] * value));
}

template <typename T>
gle::Matrix4<T> operator*(gle::Matrix4<T> const &m1, gle::Matrix4<T> const &m2)
{
  return (gle::Matrix4<T>(m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] +
			  m1[12] * m2[3], m1[0] * m2[4] + m1[4] * m2[5] +
			  m1[8] * m2[6] + m1[12] * m2[7], m1[0] * m2[8] +
			  m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11],
			  m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] +
			  m1[12] * m2[15], m1[1] * m2[0] + m1[5] * m2[1] +
			  m1[9] * m2[2] + m1[13] * m2[3], m1[1] * m2[4] +
			  m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7],
			  m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] +
			  m1[13] * m2[11], m1[1] * m2[12] + m1[5] * m2[13] +
			  m1[9] * m2[14] + m1[13] * m2[15], m1[2] * m2[0] +
			  m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3],
			  m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] +
			  m1[14] * m2[7], m1[2] * m2[8] + m1[6] * m2[9] +
			  m1[10] * m2[10] + m1[14] * m2[11], m1[2] * m2[12] +
			  m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15],
			  m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] +
			  m1[15] * m2[3], m1[3] * m2[4] + m1[7] * m2[5] +
			  m1[11] * m2[6] + m1[15] * m2[7], m1[3] * m2[8] +
			  m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11],
			  m1[3] * m2[12] + m1[7] * m2[13] +
			  m1[11] * m2[14] + m1[15] * m2[15]));
}

#endif // _GLE_MATRIX4_HPP_
