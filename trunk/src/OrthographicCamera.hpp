//
// OrthographicCamera.hpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Sat Feb 18 12:43:33 2012 loick michard
// Last update Wed Jul  4 19:40:43 2012 gael jochaud-du-plessix
//

#ifndef _ORTHOGRAPHIC_CAMERA_HPP_
# define _ORTHOGRAPHIC_CAMERA_HPP_

#include <Camera.hpp>

namespace gle {
  
  //! Class for perspective cameras.

  class OrthographicCamera : public gle::Camera {
  public:

    //! Constructor with position.
    /*!
      \param position Camera position
      \param near Distance to the near clipping plane
      \param far Distance to the far clipping plane
      \param left Distance to the left clipping plane
      \param right Distance to the right clipping plane
      \param bottom Distance to the bottom clipping plane
      \param top Distance to the top clipping plane
     */

    OrthographicCamera(gle::Vector3<GLfloat> const& position,
		       GLfloat near=3.0, GLfloat far=100.0,
		       GLfloat left=-10.0, GLfloat right=10.0,
		       GLfloat bottom=-10.0, GLfloat top=10.0);
    
    //! Constructor with position and target.
    /*!
      \param position Camera position
      \param target Camera target
      \param near Distance to the near clipping plane
      \param far Distance to the far clipping plane
      \param left Distance to the left clipping plane
      \param right Distance to the right clipping plane
      \param bottom Distance to the bottom clipping plane
      \param top Distance to the top clipping plane
     */

    OrthographicCamera(gle::Vector3<GLfloat> const& position,
		       gle::Vector3<GLfloat> const& target,
		       GLfloat near=3.0, GLfloat far=100.0,
		       GLfloat left=-10.0, GLfloat right=10.0,
		       GLfloat bottom=-10.0, GLfloat top=10.0);

    //! Default destructor

    ~OrthographicCamera(){}

    //! Update the projection matrix associated with the camera

    virtual void updateProjectionMatrix();

  private:
    GLfloat _near;
    GLfloat _far;
    GLfloat _left;
    GLfloat _right;
    GLfloat _bottom;
    GLfloat _top;
  };
}

#endif // _ORTHOGRAPHIC_CAMERA_HPP_
